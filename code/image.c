/* image.c

   MolScriptSVG v2.1.5

   Image file general output routines.

   This implementation prefers a headless EGL OpenGL context for image
   rendering and falls back to GLUT if EGL setup fails. The original code
   used GLX pixmaps for off-screen rendering; that path no longer works
   reliably on modern Linux/X11 stacks.

   Copyright (C) 1997-1998 Per Kraulis
    11-Sep-1997  split out of jpeg.c
    14-Sep-1997  finished
    12-Mar-1998  fixed number of required buffer bits
    19-Aug-1998  implemented GLX Pbuffer extension
    23-Nov-1998  got rid of GLX Pbuffer extension; fixed visual depth bug
    19-Mar-2026  replaced legacy GLX pixmap path with hidden GLUT window
*/

#include <assert.h>
#include <string.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "image.h"
#include "global.h"
#include "graphics.h"
#include "opengl.h"


/*============================================================*/
enum image_backend_modes {
  IMAGE_BACKEND_NONE,
  IMAGE_BACKEND_EGL,
  IMAGE_BACKEND_GLUT
};

static int image_backend = IMAGE_BACKEND_NONE;
static int image_window = 0;
static EGLDisplay image_egl_display = EGL_NO_DISPLAY;
static EGLContext image_egl_context = EGL_NO_CONTEXT;
static EGLSurface image_egl_surface = EGL_NO_SURFACE;


/*------------------------------------------------------------*/
static int
image_extension_supported (const char *extensions, const char *name)
{
  const char *pos;
  int len;

  assert (name);

  if (extensions == NULL || *extensions == '\0') return FALSE;

  len = strlen (name);
  pos = extensions;
  while ((pos = strstr (pos, name)) != NULL) {
    if ((pos == extensions || pos[-1] == ' ') &&
        (pos[len] == '\0' || pos[len] == ' ')) return TRUE;
    pos += len;
  }
  return FALSE;
}


/*------------------------------------------------------------*/
static void
image_egl_cleanup (void)
{
  if (image_egl_display != EGL_NO_DISPLAY) {
    eglMakeCurrent (image_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                    EGL_NO_CONTEXT);
    if (image_egl_context != EGL_NO_CONTEXT) {
      eglDestroyContext (image_egl_display, image_egl_context);
      image_egl_context = EGL_NO_CONTEXT;
    }
    if (image_egl_surface != EGL_NO_SURFACE) {
      eglDestroySurface (image_egl_display, image_egl_surface);
      image_egl_surface = EGL_NO_SURFACE;
    }
    eglTerminate (image_egl_display);
    image_egl_display = EGL_NO_DISPLAY;
  }
}


/*------------------------------------------------------------*/
static int
image_try_egl (void)
{
  PFNEGLGETPLATFORMDISPLAYEXTPROC get_platform_display = NULL;
  const char *client_extensions;
  EGLConfig config;
  EGLint count;
  EGLint major, minor;
  EGLint config_attribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_DEPTH_SIZE, 16,
    EGL_NONE
  };
  EGLint pbuffer_attribs[] = {
    EGL_WIDTH, 0,
    EGL_HEIGHT, 0,
    EGL_NONE
  };

  pbuffer_attribs[1] = output_width;
  pbuffer_attribs[3] = output_height;

  client_extensions = eglQueryString (EGL_NO_DISPLAY, EGL_EXTENSIONS);
  get_platform_display = (PFNEGLGETPLATFORMDISPLAYEXTPROC)
    eglGetProcAddress ("eglGetPlatformDisplayEXT");

  if (get_platform_display != NULL &&
      image_extension_supported (client_extensions, "EGL_EXT_platform_base") &&
      image_extension_supported (client_extensions, "EGL_MESA_platform_surfaceless")) {
    image_egl_display = get_platform_display (EGL_PLATFORM_SURFACELESS_MESA,
                                              EGL_DEFAULT_DISPLAY, NULL);
  }

  if (image_egl_display == EGL_NO_DISPLAY) {
    image_egl_display = eglGetDisplay (EGL_DEFAULT_DISPLAY);
  }
  if (image_egl_display == EGL_NO_DISPLAY) return FALSE;
  if (! eglInitialize (image_egl_display, &major, &minor)) {
    image_egl_cleanup();
    return FALSE;
  }
  if (! eglBindAPI (EGL_OPENGL_API)) {
    image_egl_cleanup();
    return FALSE;
  }
  if (! eglChooseConfig (image_egl_display, config_attribs, &config, 1, &count) ||
      count < 1) {
    image_egl_cleanup();
    return FALSE;
  }

  image_egl_surface = eglCreatePbufferSurface (image_egl_display, config,
                                               pbuffer_attribs);
  if (image_egl_surface == EGL_NO_SURFACE) {
    image_egl_cleanup();
    return FALSE;
  }

  image_egl_context = eglCreateContext (image_egl_display, config,
                                        EGL_NO_CONTEXT, NULL);
  if (image_egl_context == EGL_NO_CONTEXT) {
    image_egl_cleanup();
    return FALSE;
  }

  if (! eglMakeCurrent (image_egl_display, image_egl_surface,
                        image_egl_surface, image_egl_context)) {
    image_egl_cleanup();
    return FALSE;
  }

  glDrawBuffer (GL_BACK);
  glReadBuffer (GL_BACK);
  image_backend = IMAGE_BACKEND_EGL;
  return TRUE;
}


/*------------------------------------------------------------*/
static void
image_glut_first_plot (void)
{
  GLboolean bparam;

  glutInitWindowPosition (0, 0);
  glutInitWindowSize (output_width, output_height);
  if (ogl_accum() != 0) {
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ACCUM);
  } else {
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  }
  if (! glutGet (GLUT_DISPLAY_MODE_POSSIBLE)) {
    if (ogl_accum() != 0) {
      glutInitDisplayMode (GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH | GLUT_ACCUM);
    } else {
      glutInitDisplayMode (GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    }
  }
  if (! glutGet (GLUT_DISPLAY_MODE_POSSIBLE))
    yyerror ("could not create an OpenGL image window with the required buffers");

  image_window = glutCreateWindow ("MolScript image");
  if (image_window <= 0)
    yyerror ("could not create hidden OpenGL image window");

#ifdef FREEGLUT
  glutHideWindow();
#endif

  glGetBooleanv (GL_DOUBLEBUFFER, &bparam);
  if (bparam == GL_TRUE) {
    glDrawBuffer (GL_FRONT);
    glReadBuffer (GL_FRONT);
  }

  image_backend = IMAGE_BACKEND_GLUT;
}


/*------------------------------------------------------------*/
void
image_first_plot (void)
{
  if (! first_plot)
    yyerror ("only one plot per input file allowed for image file output");

  image_backend = IMAGE_BACKEND_NONE;
  if (! image_try_egl()) {
    image_glut_first_plot();
  }
}


/*------------------------------------------------------------*/
void
image_render (void)
{
  ogl_render_init();
  ogl_render_lights();
  ogl_render_lists();
  glDisable(GL_BLEND);          /* optimize pixel transfer rates */
  glDisable (GL_DEPTH_TEST);
  glDisable (GL_DITHER);
  glDisable (GL_FOG);
  glDisable (GL_LIGHTING);
  glFinish();
}


/*------------------------------------------------------------*/
void
image_close (void)
{
  if (image_backend == IMAGE_BACKEND_EGL) {
    image_egl_cleanup();
  } else if (image_backend == IMAGE_BACKEND_GLUT && image_window > 0) {
    glutDestroyWindow (image_window);
    image_window = 0;
  }
  image_backend = IMAGE_BACKEND_NONE;
}
