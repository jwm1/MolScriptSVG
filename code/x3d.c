/* x3d.c

   MolScript v2.1.2

   X3D Classic

   Copyright (C) 1997-1998 Per Kraulis
     4-Dec-1996  first attempts
    31-Mar-1997  fairly finished
    18-Aug-1997  split out and generalized segment handling
    15-Oct-1997  USE/DEF for helix and strand different surfaces
    25-Jan-1998  prepare some proto's for dynamics
    20-Aug-1998  fixed level-of-detail bug
    28-Aug-1998  split out indent functions
*/

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "clib/angle.h"
#include "clib/extent3d.h"
#include "clib/str_utils.h"
#include "clib/dynstring.h"
#include "clib/vrml.h"

#include "x3d.h"
#include "global.h"
#include "graphics.h"
#include "segment.h"
#include "state.h"

#define x3d_comment vrml_comment
#define x3d_s_newline vrml_s_newline
#define x3d_s_quoted vrml_s_quoted
#define x3d_i vrml_i
#define x3d_f vrml_f
#define x3d_g vrml_g
#define x3d_f2 vrml_f2
#define x3d_g3 vrml_g3
#define x3d_v3 vrml_v3
#define x3d_v3_g vrml_v3_g
#define x3d_v3_g3 vrml_v3_g3
#define x3d_colour vrml_colour
#define x3d_rgb_colour vrml_rgb_colour
#define x3d_tri_indices vrml_tri_indices
#define x3d_quad_indices vrml_quad_indices
#define x3d_begin_node vrml_begin_node
#define x3d_finish_node vrml_finish_node
#define x3d_begin_list vrml_begin_list
#define x3d_finish_list vrml_finish_list
#define x3d_valid_name vrml_valid_name
#define x3d_def vrml_def
#define x3d_proto vrml_proto
#define x3d_node vrml_node
#define x3d_list vrml_list
#define x3d_bbox vrml_bbox
#define x3d_material vrml_material
#define x3d_directionallight vrml_directionallight
#define x3d_pointlight vrml_pointlight
#define x3d_spotlight vrml_spotlight
#define x3d_viewpoint vrml_viewpoint
#define x3d_fog vrml_fog


/*============================================================*/
typedef struct s_viewpoint_node viewpoint_node;

struct s_viewpoint_node {
  vector3 from;
  vector3 ori;
  double rot;
  double fov;
  char *descr;
  viewpoint_node *next;
};

static viewpoint_node *viewpoints = NULL;
static char *viewpoint_str = NULL;
typedef enum {
  X3D_OUTPUT_XML,
  X3D_OUTPUT_CLASSIC,
  X3D_OUTPUT_WEBGL
} x3d_output_format;

static x3d_output_format x3d_output_format_mode = X3D_OUTPUT_XML;
static FILE *x3d_proper_outfile = NULL;
static FILE *x3d_classic_tmpfile = NULL;

static int line_count;
static int point_count;
static int polygon_count;
static int colour_count;	/* segment colours */
static int cylinder_count;
static int sphere_count;
static int label_count;
static int def_count;

#define STICK_SEGMENTS 8

#define MAX_COLOUR_CACHE 16
#define COLOUR_MAXDIFF 0.005
static colour colour_cache[MAX_COLOUR_CACHE];
static colour colour_cache_latest;
static int colour_cache_count;
static int colour_cache_index_count;

static int anchor_parameter_not_started = TRUE;

static double *lod_ranges = NULL;
static int lod_alloc;
static int lod_count;

typedef enum {
  XML_CONTEXT_NODE,
  XML_CONTEXT_PROTO,
  XML_CONTEXT_ATTRIBUTE_LIST,
  XML_CONTEXT_NODE_LIST
} xml_context_type;

typedef enum {
  XML_NODE_REGULAR,
  XML_NODE_PROTO_INSTANCE
} xml_node_type;

typedef struct {
  xml_context_type type;
  char *name;
  char *tag;
  boolean body_open;
  xml_node_type node_type;
  dynstring *is_connects;
  dynstring *list_values;
  dynstring *attrs;
  FILE *body_stream;
} xml_context;

#define XML_MAX_DEPTH 256

static xml_context *x3d_xml_find_current_node (xml_context *stack, int top);


/*------------------------------------------------------------*/
static char *
x3d_strdup (const char *str)
{
  char *copy;

  assert (str);

  copy = malloc (strlen (str) + 1);
  if (copy == NULL) yyerror ("out of memory");
  strcpy (copy, str);
  return copy;
}


/*------------------------------------------------------------*/
static void
x3d_xml_escape (FILE *f, const char *str, boolean attr_mode)
{
  assert (f);
  assert (str);

  for (; *str; str++) {
    switch (*str) {
    case '&': fputs ("&amp;", f); break;
    case '<': fputs ("&lt;", f); break;
    case '>': fputs ("&gt;", f); break;
    case '"':
      if (attr_mode) {
        fputs ("&quot;", f);
      } else {
        fputc ('"', f);
      }
      break;
    default:
      fputc (*str, f);
      break;
    }
  }
}


/*------------------------------------------------------------*/
static void
x3d_xml_indent (FILE *f, int level)
{
  int slot;

  for (slot = 0; slot < level; slot++) fputs ("  ", f);
}


/*------------------------------------------------------------*/
static void
x3d_trim_inplace (char *str)
{
  char *start, *end;

  assert (str);

  start = str;
  while (*start && isspace ((unsigned char) *start)) start++;
  if (start != str) memmove (str, start, strlen (start) + 1);

  end = str + strlen (str);
  while (end > str && isspace ((unsigned char) end[-1])) end--;
  *end = '\0';
}


/*------------------------------------------------------------*/
static boolean
x3d_starts_with (const char *str, const char *prefix)
{
  return strncmp (str, prefix, strlen (prefix)) == 0;
}


/*------------------------------------------------------------*/
static boolean
x3d_is_proto_name (const char *name)
{
  return str_eq (name, "Ball") || str_eq (name, "Cyl") ||
         str_eq (name, "Stick") || str_eq (name, "Label");
}


/*------------------------------------------------------------*/
static boolean
x3d_is_node_list_field (const char *field)
{
  return str_eq (field, "children") || str_eq (field, "level");
}


/*------------------------------------------------------------*/
static boolean
x3d_is_multi_string_field (const char *field)
{
  return str_eq (field, "info") || str_eq (field, "type") ||
         str_eq (field, "url") || str_eq (field, "parameter") ||
         str_eq (field, "string") || str_eq (field, "c");
}


/*------------------------------------------------------------*/
static const char *
x3d_use_field_to_tag (const char *field)
{
  if (str_eq (field, "coord")) return "Coordinate";
  if (str_eq (field, "color")) return "Color";
  if (str_eq (field, "normal")) return "Normal";
  if (str_eq (field, "appearance")) return "Appearance";
  if (str_eq (field, "material")) return "Material";
  return NULL;
}


/*------------------------------------------------------------*/
static void
x3d_normalize_value (dynstring *out, const char *raw, boolean keep_string_quotes)
{
  int slot;
  boolean previous_blank = TRUE;
  char quote;
  int len;

  assert (out);
  assert (raw);

  ds_reset (out);
  len = strlen (raw);
  if (!keep_string_quotes &&
      len >= 2 &&
      ((raw[0] == '"' && raw[len - 1] == '"') ||
       (raw[0] == '\'' && raw[len - 1] == '\''))) {
    ds_subcat (out, raw, 1, len - 2);
    return;
  }

  for (slot = 0; raw[slot] != '\0'; slot++) {
    if (raw[slot] == '[' || raw[slot] == ']') continue;
    if (raw[slot] == ',') {
      if (!previous_blank) ds_add (out, ' ');
      previous_blank = TRUE;
      continue;
    }
    if (isspace ((unsigned char) raw[slot])) {
      if (!previous_blank) ds_add (out, ' ');
      previous_blank = TRUE;
      continue;
    }
    previous_blank = FALSE;
    ds_add (out, raw[slot]);
    if ((raw[slot] == '"' || raw[slot] == '\'') && keep_string_quotes) {
      quote = raw[slot];
      for (slot++; raw[slot] != '\0'; slot++) {
        ds_add (out, raw[slot]);
        if (raw[slot] == quote) break;
      }
    }
  }

  if (out->length > 0 && out->string[out->length - 1] == ' ')
    ds_truncate (out, out->length - 1);
}


/*------------------------------------------------------------*/
static void
x3d_xml_close_pending_is (FILE *xml_out, xml_context *ctx, int indent)
{
  char *copy, *line;

  if (ctx->is_connects == NULL) return;
  if (!ctx->body_open) ctx->body_open = TRUE;
  x3d_xml_indent (xml_out, indent + 1);
  fputs ("<IS>\n", xml_out);
  copy = x3d_strdup (ctx->is_connects->string);
  for (line = strtok (copy, "\n"); line; line = strtok (NULL, "\n")) {
    x3d_xml_indent (xml_out, indent + 2);
    fputs (line, xml_out);
    fputc ('\n', xml_out);
  }
  free (copy);
  x3d_xml_indent (xml_out, indent + 1);
  fputs ("</IS>\n", xml_out);
  ds_delete (ctx->is_connects);
  ctx->is_connects = NULL;
}


/*------------------------------------------------------------*/
static void
x3d_xml_ensure_body (xml_context *ctx)
{
  if (ctx->type != XML_CONTEXT_NODE) return;
  if (ctx->body_open) return;
  if (ctx->body_stream == NULL) {
    ctx->body_stream = tmpfile ();
    if (ctx->body_stream == NULL) yyerror ("could not create temporary XML node stream");
  }
  ctx->body_open = TRUE;
}


/*------------------------------------------------------------*/
static FILE *
x3d_xml_target_stream (FILE *xml_out, xml_context *stack, int top)
{
  xml_context *ctx;

  ctx = x3d_xml_find_current_node (stack, top);
  if (ctx == NULL) return xml_out;
  x3d_xml_ensure_body (ctx);
  return ctx->body_stream;
}


/*------------------------------------------------------------*/
static void
x3d_xml_add_attr_fragment (dynstring *attrs, const char *field, const char *value)
{
  assert (attrs);
  assert (field);
  assert (value);

  ds_cat (attrs, " ");
  ds_cat (attrs, field);
  ds_cat (attrs, "=\"");
  while (*value) {
    switch (*value) {
    case '&': ds_cat (attrs, "&amp;"); break;
    case '<': ds_cat (attrs, "&lt;"); break;
    case '>': ds_cat (attrs, "&gt;"); break;
    case '"': ds_cat (attrs, "&quot;"); break;
    default: ds_add (attrs, *value); break;
    }
    value++;
  }
  ds_add (attrs, '"');
}


/*------------------------------------------------------------*/
static void
x3d_xml_emit_attr (FILE *xml_out, xml_context *ctx, int indent,
                   const char *field, const char *value)
{
  dynstring *normalized;
  boolean keep_quotes;

  assert (ctx);
  assert (field);
  assert (value);

  normalized = ds_create ("");
  keep_quotes = x3d_is_multi_string_field (field);
  x3d_normalize_value (normalized, value, keep_quotes);

  if (ctx->node_type == XML_NODE_PROTO_INSTANCE) {
    x3d_xml_ensure_body (ctx);
    xml_out = ctx->body_stream;
    x3d_xml_indent (xml_out, indent + 1);
    fprintf (xml_out, "<fieldValue name=\"");
    x3d_xml_escape (xml_out, field, TRUE);
    fprintf (xml_out, "\" value=\"");
    x3d_xml_escape (xml_out, normalized->string, TRUE);
    fprintf (xml_out, "\"/>\n");
  } else {
    x3d_xml_add_attr_fragment (ctx->attrs, field, normalized->string);
  }

  ds_delete (normalized);
}


/*------------------------------------------------------------*/
static void
x3d_xml_open_node (FILE *xml_out, xml_context *ctx, int indent)
{
  (void) xml_out;
  (void) indent;
  assert (ctx);

  ctx->attrs = ds_create ("");
  ctx->body_stream = NULL;
  ctx->body_open = FALSE;
}


/*------------------------------------------------------------*/
static void
x3d_xml_close_node (FILE *xml_out, xml_context *ctx, int indent)
{
  int ch;

  assert (ctx);

  if (ctx->is_connects != NULL) {
    x3d_xml_ensure_body (ctx);
    x3d_xml_close_pending_is (ctx->body_stream, ctx, indent);
  }

  x3d_xml_indent (xml_out, indent);
  if (ctx->node_type == XML_NODE_PROTO_INSTANCE) {
    fprintf (xml_out, "<ProtoInstance name=\"");
    x3d_xml_escape (xml_out, ctx->tag, TRUE);
    fprintf (xml_out, "\"");
  } else {
    fprintf (xml_out, "<%s", ctx->tag);
  }
  if (ctx->name) {
    fprintf (xml_out, " DEF=\"");
    x3d_xml_escape (xml_out, ctx->name, TRUE);
    fprintf (xml_out, "\"");
  }
  if (ctx->attrs && ctx->attrs->length > 0) {
    fputs (ctx->attrs->string, xml_out);
  }

  if (ctx->body_stream == NULL) {
    fputs ("/>\n", xml_out);
  } else {
    fputs (">\n", xml_out);
    rewind (ctx->body_stream);
    while ((ch = fgetc (ctx->body_stream)) != EOF) fputc (ch, xml_out);
    x3d_xml_indent (xml_out, indent);
    if (ctx->node_type == XML_NODE_PROTO_INSTANCE) {
      fputs ("</ProtoInstance>\n", xml_out);
    } else {
      fprintf (xml_out, "</%s>\n", ctx->tag);
    }
  }

  if (ctx->is_connects) ds_delete (ctx->is_connects);
  if (ctx->attrs) ds_delete (ctx->attrs);
  if (ctx->body_stream) fclose (ctx->body_stream);
  free (ctx->name);
  free (ctx->tag);
}


static xml_context *
x3d_xml_find_current_node (xml_context *stack, int top)
{
  int slot;

  for (slot = top - 1; slot >= 0; slot--) {
    if (stack[slot].type == XML_CONTEXT_NODE) return stack + slot;
  }
  return NULL;
}


/*------------------------------------------------------------*/
static void
x3d_xml_finalize_list (FILE *xml_out, xml_context *stack, int top, int indent)
{
  xml_context *list_ctx, *node_ctx;

  assert (top > 0);

  list_ctx = stack + top - 1;
  assert (list_ctx->type == XML_CONTEXT_ATTRIBUTE_LIST);

  node_ctx = x3d_xml_find_current_node (stack, top - 1);
  if (node_ctx == NULL) yyerror ("invalid X3D XML conversion state");

  x3d_xml_emit_attr (xml_out, node_ctx, indent - 1,
                     list_ctx->name, list_ctx->list_values->string);
  ds_delete (list_ctx->list_values);
  free (list_ctx->name);
  list_ctx->list_values = NULL;
  list_ctx->name = NULL;
}


/*------------------------------------------------------------*/
static void
x3d_xml_append_list_line (xml_context *ctx, const char *line)
{
  assert (ctx);
  assert (ctx->type == XML_CONTEXT_ATTRIBUTE_LIST);
  assert (ctx->list_values);

  if (ctx->list_values->length > 0) ds_add (ctx->list_values, ' ');
  ds_cat (ctx->list_values, line);
}


/*------------------------------------------------------------*/
static void
x3d_xml_add_is_connect (xml_context *ctx, const char *node_field,
                        const char *proto_field)
{
  assert (ctx);
  assert (node_field);
  assert (proto_field);

  if (ctx->is_connects == NULL) ctx->is_connects = ds_create ("");
  ds_cat (ctx->is_connects, "<connect nodeField=\"");
  ds_cat (ctx->is_connects, node_field);
  ds_cat (ctx->is_connects, "\" protoField=\"");
  ds_cat (ctx->is_connects, proto_field);
  ds_cat (ctx->is_connects, "\"/>");
  ds_add (ctx->is_connects, '\n');
}


/*------------------------------------------------------------*/
static int
x3d_proto_field_arity (const char *field_name)
{
  if (str_eq (field_name, "p")) return 3;
  if (str_eq (field_name, "rad")) return 1;
  if (str_eq (field_name, "dc") || str_eq (field_name, "ec") ||
      str_eq (field_name, "sc")) return 3;
  if (str_eq (field_name, "sh") || str_eq (field_name, "tr") ||
      str_eq (field_name, "sz")) return 1;
  if (str_eq (field_name, "r")) return 4;
  if (str_eq (field_name, "s") || str_eq (field_name, "o")) return 3;
  return 0;
}


/*------------------------------------------------------------*/
static char *
x3d_next_token (char **cursor)
{
  char *start;

  assert (cursor);
  assert (*cursor);

  while (**cursor && isspace ((unsigned char) **cursor)) (*cursor)++;
  if (**cursor == '\0') return NULL;

  start = *cursor;
  if (**cursor == '"' || **cursor == '\'') {
    char quote = **cursor;
    (*cursor)++;
    while (**cursor && **cursor != quote) (*cursor)++;
    if (**cursor == quote) (*cursor)++;
  } else {
    while (**cursor && !isspace ((unsigned char) **cursor)) (*cursor)++;
  }

  if (**cursor) {
    **cursor = '\0';
    (*cursor)++;
  }

  return start;
}


/*------------------------------------------------------------*/
static boolean
x3d_xml_emit_proto_instance_fields (xml_context *ctx, int indent, char *line)
{
  char *cursor, *field_name, *token;
  int arity, count;
  dynstring *value;

  assert (ctx);
  assert (ctx->node_type == XML_NODE_PROTO_INSTANCE);
  assert (line);

  cursor = line;
  while ((field_name = x3d_next_token (&cursor)) != NULL) {
    arity = x3d_proto_field_arity (field_name);
    if (arity == 0) return FALSE;

    value = ds_create ("");
    for (count = 0; count < arity; count++) {
      token = x3d_next_token (&cursor);
      if (token == NULL) {
        ds_delete (value);
        return FALSE;
      }
      if (value->length > 0) ds_add (value, ' ');
      ds_cat (value, token);
    }

    x3d_xml_emit_attr (ctx->body_stream, ctx, indent, field_name, value->string);
    ds_delete (value);
  }

  return TRUE;
}


/*------------------------------------------------------------*/
static void
x3d_xml_parse_node_open (char *line, char **field_name, char **def_name,
                         char **node_name)
{
  char *tokens[8];
  int count = 0;
  char *tok;

  *field_name = NULL;
  *def_name = NULL;
  *node_name = NULL;

  for (tok = strtok (line, " \t"); tok && count < 8; tok = strtok (NULL, " \t"))
    tokens[count++] = tok;

  if (count == 0) return;
  if (count == 1) {
    *node_name = tokens[0];
  } else if (count == 2) {
    *field_name = tokens[0];
    *node_name = tokens[1];
  } else if (count == 3 && str_eq (tokens[0], "DEF")) {
    *def_name = tokens[1];
    *node_name = tokens[2];
  } else if (count == 4 && str_eq (tokens[1], "DEF")) {
    *field_name = tokens[0];
    *def_name = tokens[2];
    *node_name = tokens[3];
  } else {
    yyerror ("unsupported X3D Classic node syntax");
  }
}


/*------------------------------------------------------------*/
static void
x3d_xml_open_parsed_node (FILE *xml_out, xml_context *stack, int *top,
                          int *indent, char *line, boolean self_closing)
{
  xml_context *parent;
  xml_context ctx;
  char *field_name, *def_name, *node_name;
  char *work;

  work = x3d_strdup (line);
  x3d_xml_parse_node_open (work, &field_name, &def_name, &node_name);
  if (node_name == NULL) {
    free (work);
    yyerror ("could not parse X3D node");
  }

  parent = x3d_xml_find_current_node (stack, *top);
  if (parent) x3d_xml_ensure_body (parent);

  memset (&ctx, 0, sizeof (ctx));
  ctx.type = XML_CONTEXT_NODE;
  ctx.name = def_name ? x3d_strdup (def_name) : NULL;
  ctx.tag = x3d_strdup (node_name);
  ctx.node_type = x3d_is_proto_name (node_name) ?
    XML_NODE_PROTO_INSTANCE : XML_NODE_REGULAR;
  x3d_xml_open_node (xml_out, &ctx, *indent);

  if (self_closing) {
    x3d_xml_close_node (x3d_xml_target_stream (xml_out, stack, *top), &ctx, *indent);
  } else {
    if (*top >= XML_MAX_DEPTH) yyerror ("X3D XML nesting too deep");
    stack[(*top)++] = ctx;
    (*indent)++;
  }

  free (work);
}


/*------------------------------------------------------------*/
static void
x3d_convert_classic_to_xml_stream (FILE *xml_out, FILE *classic_infile,
                                   boolean include_xml_declaration)
{
  xml_context stack[XML_MAX_DEPTH];
  char *line = NULL;
  size_t line_capacity = 0;
  ssize_t line_length;
  int top = 0;
  int indent = 1;

  assert (xml_out);
  assert (classic_infile);
  rewind (classic_infile);

  if (include_xml_declaration) {
    fputs ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", xml_out);
  }
  fputs ("<X3D profile=\"Interchange\" version=\"3.3\" "
         "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema-instance\" "
         "xsd:noNamespaceSchemaLocation=\"http://www.web3d.org/specifications/x3d-3.3.xsd\">\n",
         xml_out);
  fputs ("  <Scene>\n", xml_out);

  while ((line_length = getline (&line, &line_capacity, classic_infile)) >= 0) {
    xml_context *ctx;
    char *field, *value, *use_name, *tag_name;
    char *space, *work;

    (void) line_length;
    x3d_trim_inplace (line);
    if (line[0] == '\0' || line[0] == '#') continue;
    if (x3d_starts_with (line, "PROFILE ")) continue;
    if (str_eq (line, "{")) continue;

    if (str_eq (line, "}")) {
      if (top <= 0) yyerror ("invalid X3D Classic nesting");
      ctx = stack + top - 1;
      if (ctx->type == XML_CONTEXT_NODE) {
        indent--;
        x3d_xml_close_node (x3d_xml_target_stream (xml_out, stack, top - 1),
                            ctx, indent);
        top--;
      } else if (ctx->type == XML_CONTEXT_PROTO) {
        if (!ctx->body_open) {
          indent--;
          x3d_xml_indent (xml_out, indent);
          fputs ("</ProtoInterface>\n", xml_out);
          x3d_xml_indent (xml_out, indent);
          fputs ("<ProtoBody>\n", xml_out);
          ctx->body_open = TRUE;
          indent++;
        }
        indent--;
        x3d_xml_indent (xml_out, indent);
        fputs ("</ProtoBody>\n", xml_out);
        indent--;
        x3d_xml_indent (xml_out, indent);
        fputs ("</ProtoDeclare>\n", xml_out);
        free (ctx->name);
        top--;
      } else {
        yyerror ("unexpected X3D Classic block closure");
      }
      continue;
    }

    if (str_eq (line, "]")) {
      if (top <= 0) yyerror ("unexpected X3D Classic list closure");
      ctx = stack + top - 1;
      if (ctx->type == XML_CONTEXT_ATTRIBUTE_LIST) {
        x3d_xml_finalize_list (xml_out, stack, top, indent);
        top--;
      } else if (ctx->type == XML_CONTEXT_NODE_LIST) {
        free (ctx->name);
        top--;
      } else if (ctx->type == XML_CONTEXT_PROTO && !ctx->body_open) {
        indent--;
        x3d_xml_indent (xml_out, indent);
        fputs ("</ProtoInterface>\n", xml_out);
        x3d_xml_indent (xml_out, indent);
        fputs ("<ProtoBody>\n", xml_out);
        ctx->body_open = TRUE;
        indent++;
      } else {
        yyerror ("unexpected X3D Classic list closure");
      }
      continue;
    }

    if (top > 0 && stack[top - 1].type == XML_CONTEXT_ATTRIBUTE_LIST) {
      x3d_xml_append_list_line (stack + top - 1, line);
      continue;
    }

    if (x3d_starts_with (line, "PROTO ")) {
      char *proto_name;
      work = x3d_strdup (line + 6);
      space = strchr (work, ' ');
      if (space == NULL) {
        free (work);
        yyerror ("invalid PROTO declaration");
      }
      *space = '\0';
      proto_name = work;
      if (top >= XML_MAX_DEPTH) yyerror ("X3D XML nesting too deep");
      x3d_xml_indent (xml_out, indent);
      fprintf (xml_out, "<ProtoDeclare name=\"");
      x3d_xml_escape (xml_out, proto_name, TRUE);
      fprintf (xml_out, "\">\n");
      x3d_xml_indent (xml_out, indent + 1);
      fputs ("<ProtoInterface>\n", xml_out);
      memset (stack + top, 0, sizeof (xml_context));
      stack[top].type = XML_CONTEXT_PROTO;
      stack[top].name = x3d_strdup (proto_name);
      stack[top].body_open = FALSE;
      top++;
      indent += 2;
      free (work);
      continue;
    }

    if (top > 0 && stack[top - 1].type == XML_CONTEXT_PROTO && !stack[top - 1].body_open) {
      char access[64], type_name[64], field_name[64];
      char raw_value[1024];
      dynstring *normalized;

      raw_value[0] = '\0';
      if (sscanf (line, "%63s %63s %63s %1023[^\n]", access, type_name, field_name, raw_value) < 3)
        yyerror ("invalid X3D PROTO interface field");
      x3d_xml_indent (xml_out, indent);
      fprintf (xml_out, "<field accessType=\"%s\" type=\"%s\" name=\"%s\"",
               access, type_name, field_name);
      if (raw_value[0] != '\0') {
        normalized = ds_create ("");
        x3d_normalize_value (normalized, raw_value, str_eq (type_name, "MFString"));
        fprintf (xml_out, " value=\"");
        x3d_xml_escape (xml_out, normalized->string, TRUE);
        fprintf (xml_out, "\"");
        ds_delete (normalized);
      }
      fputs ("/>\n", xml_out);
      continue;
    }

    if (line[strlen (line) - 1] == '[') {
      line[strlen (line) - 1] = '\0';
      x3d_trim_inplace (line);
      if (top >= XML_MAX_DEPTH) yyerror ("X3D XML nesting too deep");
      memset (stack + top, 0, sizeof (xml_context));
      if (x3d_is_node_list_field (line)) {
        ctx = x3d_xml_find_current_node (stack, top);
        if (ctx) x3d_xml_ensure_body (ctx);
        stack[top].type = XML_CONTEXT_NODE_LIST;
        stack[top].name = x3d_strdup (line);
      } else {
        stack[top].type = XML_CONTEXT_ATTRIBUTE_LIST;
        stack[top].name = x3d_strdup (line);
        stack[top].list_values = ds_create ("");
      }
      top++;
      continue;
    }

    if (strstr (line, "{ }")) {
      char *brace = strstr (line, "{ }");
      *brace = '\0';
      x3d_trim_inplace (line);
      x3d_xml_open_parsed_node (xml_out, stack, &top, &indent, line, TRUE);
      continue;
    }

    if (line[strlen (line) - 1] == '{') {
      line[strlen (line) - 1] = '\0';
      x3d_trim_inplace (line);
      x3d_xml_open_parsed_node (xml_out, stack, &top, &indent, line, FALSE);
      continue;
    }

    ctx = x3d_xml_find_current_node (stack, top);
    if (ctx == NULL) yyerror ("X3D Classic statement outside node");

    if (ctx->node_type == XML_NODE_PROTO_INSTANCE &&
        x3d_xml_emit_proto_instance_fields (ctx, indent - 1, line)) {
      continue;
    }

    if ((space = strstr (line, " IS ")) != NULL) {
      *space = '\0';
      field = line;
      value = space + 4;
      x3d_trim_inplace (field);
      x3d_trim_inplace (value);
      x3d_xml_add_is_connect (ctx, field, value);
      continue;
    }

    if ((space = strstr (line, " USE ")) != NULL) {
      *space = '\0';
      field = line;
      use_name = space + 5;
      x3d_trim_inplace (field);
      x3d_trim_inplace (use_name);
      tag_name = (char *) x3d_use_field_to_tag (field);
      if (tag_name == NULL) yyerror ("unsupported X3D USE field");
      {
        FILE *target = x3d_xml_target_stream (xml_out, stack, top);
        x3d_xml_indent (target, indent);
        fprintf (target, "<%s USE=\"", tag_name);
        x3d_xml_escape (target, use_name, TRUE);
        fprintf (target, "\"/>\n");
      }
      continue;
    }

    space = strchr (line, ' ');
    if (space == NULL) yyerror ("unsupported X3D Classic statement");
    *space = '\0';
    field = line;
    value = space + 1;
    x3d_trim_inplace (field);
    x3d_trim_inplace (value);
    x3d_xml_emit_attr (xml_out, ctx, indent - 1, field, value);
  }

  free (line);
  fputs ("  </Scene>\n</X3D>\n", xml_out);
}


/*------------------------------------------------------------*/
static void
x3d_convert_classic_to_xml (void)
{
  char classic_path[256];
  char xml_path[256];
  FILE *classic_out;
  FILE *xml_in;
  char command[1024];
  int ch;

  assert (x3d_output_format_mode == X3D_OUTPUT_XML);
  assert (x3d_proper_outfile);
  assert (x3d_classic_tmpfile);

  fflush (outfile);
  outfile = x3d_proper_outfile;
  snprintf (classic_path, sizeof (classic_path),
            "/tmp/molscript_x3d_%ld_%d.x3dv",
            (long) getpid(), rand());
  snprintf (xml_path, sizeof (xml_path),
            "/tmp/molscript_x3d_%ld_%d.x3d",
            (long) getpid(), rand());

  classic_out = fopen (classic_path, "w");
  if (classic_out == NULL) yyerror ("could not create temporary X3D file");
  rewind (x3d_classic_tmpfile);
  while ((ch = fgetc (x3d_classic_tmpfile)) != EOF) fputc (ch, classic_out);
  fclose (classic_out);

  snprintf (command, sizeof (command),
            "HOME=/tmp view3dscene --write --write-encoding=xml \"%s\" > \"%s\" 2>/dev/null",
            classic_path, xml_path);
  if (system (command) != 0) yyerror ("view3dscene failed converting X3D output");

  xml_in = fopen (xml_path, "r");
  if (xml_in == NULL) yyerror ("could not read converted X3D XML file");
  while ((ch = fgetc (xml_in)) != EOF) fputc (ch, outfile);
  fclose (xml_in);
  unlink (classic_path);
  unlink (xml_path);
  fclose (x3d_classic_tmpfile);
  x3d_classic_tmpfile = NULL;
  x3d_proper_outfile = NULL;
}


/*------------------------------------------------------------*/
static void
x3d_write_data_url_encoded_stream (FILE *out, FILE *infile)
{
  int ch;

  assert (out);
  assert (infile);

  rewind (infile);
  while ((ch = fgetc (infile)) != EOF) {
    if ((ch >= 'A' && ch <= 'Z') ||
        (ch >= 'a' && ch <= 'z') ||
        (ch >= '0' && ch <= '9') ||
        ch == '-' || ch == '_' || ch == '.' || ch == '~') {
      fputc (ch, out);
    } else {
      fprintf (out, "%%%02X", (unsigned char) ch);
    }
  }
}


/*------------------------------------------------------------*/
static void
x3d_convert_classic_to_webgl_html (void)
{
  FILE *classic_infile;
  FILE *xml_infile;
  FILE *classic_out;
  const char *page_title;
  char classic_path[256];
  char xml_path[256];
  char command[1024];
  int ch;

  assert (x3d_output_format_mode == X3D_OUTPUT_WEBGL);
  assert (x3d_proper_outfile);
  assert (x3d_classic_tmpfile);

  fflush (outfile);
  outfile = x3d_proper_outfile;
  classic_infile = x3d_classic_tmpfile;
  page_title = title ? title : program_str;
  snprintf (classic_path, sizeof (classic_path),
            "/tmp/molscript_webgl_%ld_%d.x3dv",
            (long) getpid(), rand());
  snprintf (xml_path, sizeof (xml_path),
            "/tmp/molscript_webgl_%ld_%d.x3d",
            (long) getpid(), rand());
  classic_out = fopen (classic_path, "w");
  if (classic_out == NULL) yyerror ("could not create temporary WebGL X3D file");
  rewind (classic_infile);
  while ((ch = fgetc (classic_infile)) != EOF) fputc (ch, classic_out);
  fclose (classic_out);

  snprintf (command, sizeof (command),
            "HOME=/tmp view3dscene --write --write-encoding=xml \"%s\" > \"%s\" 2>/dev/null",
            classic_path, xml_path);
  if (system (command) != 0) yyerror ("view3dscene failed converting WebGL scene");

  xml_infile = fopen (xml_path, "r");
  if (xml_infile == NULL) yyerror ("could not read converted WebGL XML scene");

  fputs ("<!DOCTYPE html>\n"
         "<html lang=\"en\">\n"
         "<head>\n"
         "  <meta charset=\"utf-8\">\n"
         "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
         "  <title>", outfile);
  x3d_xml_escape (outfile, page_title, FALSE);
  fputs ("</title>\n"
         "  <script defer src=\"https://cdn.jsdelivr.net/npm/x_ite@12.1.9/dist/x_ite.min.js\"></script>\n"
         "  <style>\n"
         "    html, body {\n"
         "      margin: 0;\n"
         "      width: 100%;\n"
         "      height: 100%;\n"
         "      background: #ffffff;\n"
         "    }\n"
         "    x3d-canvas {\n"
         "      display: block;\n"
         "      width: 100vw;\n"
         "      height: 100vh;\n"
         "    }\n"
         "  </style>\n"
         "</head>\n"
         "<body>\n"
         "  <x3d-canvas id=\"molscript-scene\" contentScale=\"auto\" update=\"auto\" src=\"data:model/x3d+xml;charset=utf-8,",
         outfile);
  x3d_write_data_url_encoded_stream (outfile, xml_infile);
  fputs ("\"></x3d-canvas>\n"
         "</body>\n"
         "</html>\n",
         outfile);

  fclose (xml_infile);
  unlink (classic_path);
  unlink (xml_path);
  fclose (classic_infile);
  x3d_classic_tmpfile = NULL;
  x3d_proper_outfile = NULL;
}


/*------------------------------------------------------------*/
void
x3d_set_xml_encoding (boolean xml_encoding)
{
  x3d_output_format_mode = xml_encoding ? X3D_OUTPUT_XML : X3D_OUTPUT_CLASSIC;
}


/*------------------------------------------------------------*/
void
x3d_set_webgl_output (boolean webgl_output)
{
  if (webgl_output) {
    x3d_output_format_mode = X3D_OUTPUT_WEBGL;
  } else if (x3d_output_format_mode == X3D_OUTPUT_WEBGL) {
    x3d_output_format_mode = X3D_OUTPUT_XML;
  }
}


/*------------------------------------------------------------*/
static int
x3d_header (void)
{
  assert (indent_valid_state());

  indent_needs_blank = FALSE;
  if (fprintf (indent_file, "#X3D V3.3 utf8\nPROFILE Interchange\n") < 0)
    return -1;
  return 0;
}


/*------------------------------------------------------------*/
static void
x3d_orthoviewpoint (vector3 *pos, vector3 *ori, double rot,
                    double xmin, double ymin, double xmax, double ymax,
                    char *descr)
{
  assert (pos);
  assert (xmin < xmax);
  assert (ymin < ymax);

  indent_newline();
  x3d_node ("OrthoViewpoint");
  indent_string ("position");
  x3d_v3 (pos);
  if (ori) {
    indent_newline();
    indent_string ("orientation");
    x3d_v3_g (ori);
    x3d_g (rot);
  }
  indent_newline();
  x3d_list ("fieldOfView");
  x3d_g (xmin);
  x3d_g (ymin);
  x3d_g (xmax);
  x3d_g (ymax);
  x3d_finish_list();
  if (descr) {
    indent_newline();
    indent_string ("description");
    x3d_s_quoted (descr);
  }
  x3d_finish_node();
}


/*------------------------------------------------------------*/
static void
output_rot (vector3 *axis, double angle)
{
  assert (axis);

  indent_string ("r ");
  x3d_v3_g (axis);
  x3d_g (angle);
}


/*------------------------------------------------------------*/
static void
output_scale (double x, double y, double z)
{
  assert (x > 0.0);
  assert (y > 0.0);
  assert (z > 0.0);

  indent_string ("s ");
  x3d_g (x);
  x3d_g (y);
  x3d_g (z);
}


/*------------------------------------------------------------*/
static void
output_dc (colour *dc)
{
  assert (dc);

  if (colour_unequal (dc, &white_colour)) {
    indent_string ("dc ");
    x3d_colour (dc);
  }
}


/*------------------------------------------------------------*/
static void
output_ec (colour *ec)
{
  assert (ec);

  if (colour_unequal (ec, &black_colour)) {
    indent_string ("ec ");
    x3d_colour (ec);
  }
}


/*------------------------------------------------------------*/
static void
output_sc (colour *sc)
{
  assert (sc);

  if (colour_unequal (sc, &grey02_colour)) {
    indent_string ("sc ");
    x3d_colour (sc);
  }
}


/*------------------------------------------------------------*/
static void
output_sh (double sh)
{
  assert (sh >= 0.0);
  assert (sh <= 1.0);

  if (sh != 0.0) {
    indent_string ("sh");
    x3d_g3 (sh);
  }
}


/*------------------------------------------------------------*/
static void
output_tr (double tr)
{
  assert (tr >= 0.0);
  assert (tr <= 1.0);

  if (tr != 0.0) {
    indent_string ("tr");
    x3d_g3 (tr);
  }
}


/*------------------------------------------------------------*/
static void
x3d_webgl_maybe_two_sided (void)
{
  if (x3d_output_format_mode == X3D_OUTPUT_WEBGL)
    x3d_s_newline ("solid FALSE");
}


/*------------------------------------------------------------*/
static void
output_appearance (int is_line, colour *c)
{
  assert (c);

  indent_newline_conditional();
  x3d_node ("appearance Appearance");
  indent_string ("material");

  if (current_state->colourparts) {
    if (is_line) {
      x3d_material (NULL, NULL, &(current_state->specularcolour),
		     0.2, current_state->shininess,
		     current_state->transparency);
    } else {
      x3d_material (NULL, &(current_state->emissivecolour),
		     &(current_state->specularcolour),
		     0.2, current_state->shininess,
		     current_state->transparency);
    }

  } else {
    if (is_line) {
      x3d_material (NULL, c, &(current_state->specularcolour),
		     0.2, current_state->shininess,
		     current_state->transparency);
    } else {
      x3d_material (c, &(current_state->emissivecolour),
		     &(current_state->specularcolour),
		     0.2, current_state->shininess,
		     current_state->transparency);
    }
  }

  x3d_finish_node();
}


/*------------------------------------------------------------*/
static void
colour_cache_init (void)
{
  colour_cache_count = 0;
}


/*------------------------------------------------------------*/
static void
colour_cache_add (colour *c)
{
  int slot;

  assert (c);

  colour_to_rgb (c);

  for (slot = colour_cache_count - 1; slot >= 0; slot--) {
    if (colour_approx_equal (c, &(colour_cache[slot]), COLOUR_MAXDIFF)) return;
  }

  if (colour_cache_count < MAX_COLOUR_CACHE) {
    colour_cache_index_count = colour_cache_count;
    colour_cache[colour_cache_count++] = *c;
  } else if (colour_approx_equal (c, &colour_cache_latest, COLOUR_MAXDIFF)) {
    return;
  }

  x3d_rgb_colour (c);
  colour_count++;
  colour_cache_latest = *c;
}


/*------------------------------------------------------------*/
static void
colour_cache_index_init (void)
{
  colour_cache_latest.spec = -1;
}


/*------------------------------------------------------------*/
static int
colour_cache_index (colour *c)
{
  int slot;

  assert (c);

  for (slot = colour_cache_count - 1; slot >= 0; slot--) {
    if (colour_approx_equal (c, &(colour_cache[slot]), COLOUR_MAXDIFF)) {
      return slot;
    }
  }

  if (colour_approx_equal (c, &colour_cache_latest, COLOUR_MAXDIFF)) {
    return colour_cache_index_count;
  } else {
    colour_cache_latest = *c;
    return ++colour_cache_index_count;
  }
}


/*------------------------------------------------------------*/
void
x3d_set (void)
{
  output_first_plot = x3d_first_plot;
  output_start_plot = x3d_start_plot;
  output_finish_plot = x3d_finish_plot;
  output_finish_output = x3d_finish_output;

  set_area = x3d_set_area;
  set_background = x3d_set_background;
  anchor_start = x3d_anchor_start;
  anchor_description = x3d_anchor_description;
  anchor_parameter = x3d_anchor_parameter;
  anchor_start_geometry = x3d_anchor_start_geometry;
  anchor_finish = x3d_anchor_finish;
  lod_start = x3d_lod_start;
  lod_finish = x3d_lod_finish;
  lod_start_group = x3d_lod_start_group;
  lod_finish_group = x3d_lod_finish_group;
  viewpoint_start = x3d_viewpoint_start;
  viewpoint_output = x3d_viewpoint_output;
  output_directionallight = x3d_ms_directionallight;
  output_pointlight = x3d_ms_pointlight;
  output_spotlight = x3d_ms_spotlight;
  output_comment = x3d_comment;

  output_coil = x3d_coil;
  output_cylinder = x3d_cylinder;
  output_helix = x3d_helix;
  output_label = x3d_label;
  output_line = x3d_line;
  output_sphere = x3d_sphere;
  output_stick = x3d_stick;
  output_strand = x3d_strand;

  output_start_object = do_nothing;
  output_object = x3d_object;
  output_finish_object = do_nothing;

  output_pickable = NULL;

  constant_colours_to_rgb();

  output_mode = (x3d_output_format_mode == X3D_OUTPUT_WEBGL) ?
    WEBGL_MODE : X3D_MODE;
}


/*------------------------------------------------------------*/
void
x3d_first_plot (void)
{
  int slot;
  double angle;

  if (!first_plot)
    yyerror ("only one plot per input file allowed for X3D");

  set_outfile ("w");
  x3d_proper_outfile = NULL;
  x3d_classic_tmpfile = NULL;
  if (x3d_output_format_mode != X3D_OUTPUT_CLASSIC) {
    x3d_proper_outfile = outfile;
    outfile = tmpfile();
    if (outfile == NULL) yyerror ("could not create the temporary X3D Classic stream");
    x3d_classic_tmpfile = outfile;
  }
  indent_initialize (outfile);
  indent_flag = pretty_format;

  if (x3d_header() < 0) yyerror ("could not write to X3D file");

  x3d_node ("WorldInfo");
  if (title) {
    indent_string ("title ");
    x3d_s_quoted (title);
    indent_newline();
  }
  x3d_list ("info");
  INDENT_FPRINTF (indent_file,
		  "\"Program: %s, %s\"", program_str, copyright_str);
  if (user_str[0] != '\0') {
    indent_newline();
    INDENT_FPRINTF (indent_file, "\"Author: %s\"", user_str);
  }
  x3d_finish_list();
  x3d_finish_node();

  indent_newline();
  x3d_comment ("MolScript: begin proto definitions");
  x3d_proto ("Ball");
  x3d_s_newline ("inputOutput SFVec3f p 0 0 0");
  x3d_s_newline ("initializeOnly SFFloat rad 1.5");
  x3d_s_newline ("initializeOnly SFColor dc 1 1 1");
  x3d_s_newline ("initializeOnly SFColor ec 0 0 0");
  x3d_s_newline ("initializeOnly SFColor sc 0.2 0.2 0.2");
  x3d_s_newline ("initializeOnly SFFloat sh 0");
  indent_string ("initializeOnly SFFloat tr 0");
  x3d_finish_list();
  indent_newline();
  x3d_begin_node();
  x3d_node ("Transform");
  x3d_s_newline ("translation IS p");
  x3d_list ("children");
  x3d_node ("Shape");
  x3d_node ("appearance Appearance");
  x3d_node ("material Material");
  x3d_s_newline ("diffuseColor IS dc");
  x3d_s_newline ("emissiveColor IS ec");
  x3d_s_newline ("specularColor IS sc");
  x3d_s_newline ("shininess IS sh");
  indent_string ("transparency IS tr");
  x3d_finish_node();
  x3d_finish_node();
  indent_newline();
  x3d_node ("geometry Sphere");
  indent_string ("radius IS rad");
  x3d_finish_node();
  x3d_finish_node();
  x3d_finish_list();
  x3d_finish_node();
  x3d_finish_node();

  x3d_proto ("Cyl");
  x3d_s_newline ("inputOutput SFVec3f p 0 0 0");
  x3d_s_newline ("inputOutput SFRotation r 0 0 1 0");
  x3d_s_newline ("inputOutput SFVec3f s 1 1 1");
  x3d_s_newline ("initializeOnly SFColor dc 1 1 1");
  x3d_s_newline ("initializeOnly SFColor ec 0 0 0");
  x3d_s_newline ("initializeOnly SFColor sc 0.2 0.2 0.2");
  x3d_s_newline ("initializeOnly SFFloat sh 0");
  indent_string ("initializeOnly SFFloat tr 0");
  x3d_finish_list();
  indent_newline();
  x3d_begin_node();
  x3d_node ("Transform");
  x3d_s_newline ("translation IS p");
  x3d_s_newline ("rotation IS r");
  x3d_s_newline ("scale IS s");
  x3d_list ("children");
  x3d_node ("Shape");
  x3d_node ("appearance Appearance");
  x3d_node ("material Material");
  x3d_s_newline ("diffuseColor IS dc");
  x3d_s_newline ("emissiveColor IS ec");
  x3d_s_newline ("specularColor IS sc");
  x3d_s_newline ("shininess IS sh");
  indent_string ("transparency IS tr");
  x3d_finish_node();
  x3d_finish_node();
  indent_newline();
  indent_string ("geometry Cylinder { }");
  x3d_finish_node();
  x3d_finish_list();
  x3d_finish_node();
  x3d_finish_node();

  x3d_proto ("Stick");
  x3d_s_newline ("inputOutput SFVec3f p 0 0 0");
  x3d_s_newline ("inputOutput SFRotation r 0 0 1 0");
  x3d_s_newline ("inputOutput SFVec3f s 1 1 1");
  x3d_s_newline ("initializeOnly SFColor dc 1 1 1");
  x3d_s_newline ("initializeOnly SFColor ec 0 0 0");
  x3d_s_newline ("initializeOnly SFColor sc 0.2 0.2 0.2");
  x3d_s_newline ("initializeOnly SFFloat sh 0");
  indent_string ("initializeOnly SFFloat tr 0");
  x3d_finish_list();
  indent_newline();
  x3d_begin_node();
  x3d_node ("Transform");
  x3d_s_newline ("translation IS p");
  x3d_s_newline ("rotation IS r");
  x3d_s_newline ("scale IS s");
  x3d_list ("children");
  x3d_node ("Shape");
  x3d_node ("appearance Appearance");
  x3d_node ("material Material");
  x3d_s_newline ("diffuseColor IS dc");
  x3d_s_newline ("emissiveColor IS ec");
  x3d_s_newline ("specularColor IS sc");
  x3d_s_newline ("shininess IS sh");
  indent_string ("transparency IS tr");
  x3d_finish_node();
  x3d_finish_node();
  indent_newline();
  x3d_node ("geometry Extrusion");
  x3d_s_newline ("spine [0 -1 0, 0 1 0]");
  x3d_list ("crossSection");
  x3d_g (1.0);
  x3d_g (0.0);
  for (slot = STICK_SEGMENTS - 1; slot > 0; slot--) {
    angle = 2.0 * ANGLE_PI * (double) slot / (double) STICK_SEGMENTS;
    x3d_g (cos (angle));
    x3d_g (sin (angle));
  }
  x3d_g (1.0);
  x3d_g (0.0);
  x3d_finish_list();
  indent_newline();
  x3d_s_newline ("beginCap FALSE");
  x3d_s_newline ("endCap FALSE");
  indent_string ("creaseAngle 1.7");
  x3d_finish_node();
  x3d_finish_node();
  x3d_finish_list();
  x3d_finish_node();
  x3d_finish_node();

  x3d_proto ("Label");
  x3d_s_newline ("initializeOnly SFVec3f p 0 0 0");
  x3d_s_newline ("initializeOnly SFFloat sz 1");
  x3d_s_newline ("initializeOnly MFString c []");
  x3d_s_newline ("initializeOnly SFVec3f o 0 0 0");
  x3d_s_newline ("initializeOnly SFColor dc 1 1 1");
  x3d_s_newline ("initializeOnly SFColor ec 0 0 0");
  x3d_s_newline ("initializeOnly SFColor sc 0.2 0.2 0.2");
  x3d_s_newline ("initializeOnly SFFloat sh 0");
  indent_string ("initializeOnly SFFloat tr 0");
  x3d_finish_list();
  indent_newline();
  x3d_begin_node();
  x3d_node ("Transform");
  x3d_s_newline ("translation IS p");
  x3d_list ("children");
  x3d_node ("Billboard");
  x3d_s_newline ("axisOfRotation 0 0 0");
  x3d_list ("children");
  x3d_node ("Transform");
  x3d_s_newline ("translation IS o");
  x3d_list ("children");
  x3d_node ("Shape");
  x3d_node ("appearance Appearance");
  x3d_node ("material Material");
  x3d_s_newline ("diffuseColor IS dc");
  x3d_s_newline ("emissiveColor IS ec");
  x3d_s_newline ("specularColor IS sc");
  x3d_s_newline ("shininess IS sh");
  indent_string ("transparency IS tr");
  x3d_finish_node();
  x3d_finish_node();
  indent_newline();
  x3d_node ("geometry Text");
  x3d_s_newline ("string IS c");
  x3d_node ("fontStyle FontStyle");
  indent_string ("size IS sz");
  x3d_finish_node();
  x3d_finish_node();
  x3d_finish_node();
  x3d_finish_list();
  x3d_finish_node();
  x3d_finish_list();
  x3d_finish_node();
  x3d_finish_list();
  x3d_finish_node();
  x3d_finish_node();
  indent_newline();
  x3d_comment ("MolScript: end proto definitions");
}


/*------------------------------------------------------------*/
void
x3d_finish_output (void)
{
  indent_newline();
  x3d_comment ("MolScript: end of output");
  if (x3d_output_format_mode == X3D_OUTPUT_XML) {
    x3d_convert_classic_to_xml();
  } else if (x3d_output_format_mode == X3D_OUTPUT_WEBGL) {
    x3d_convert_classic_to_webgl_html();
  }
}


/*------------------------------------------------------------*/
void
x3d_start_plot (void)
{
  set_area_values (-1.0, -1.0, 1.0, 1.0);

  line_count = 0;
  point_count = 0;
  polygon_count = 0;
  colour_count = 0;
  cylinder_count = 0;
  sphere_count = 0;
  label_count = 0;
  def_count = 0;

  colour_copy_to_rgb (&background_colour, &black_colour);

  indent_newline();
  x3d_comment ("MolScript: collision detection switched off for all objects");
  x3d_node ("Collision");
  x3d_s_newline ("collide FALSE");
  if (indent_level == 0) {
    x3d_list ("children");
  } else {
    indent_string ("children [");
    indent_increment_level (TRUE);
  }
}


/*------------------------------------------------------------*/
void
x3d_finish_plot (void)
{
  vector3 center, size, pos;
  viewpoint_node *vp, *vp2;

  x3d_finish_list();

  set_extent();

  ext3d_get_center_size (&center, &size);
  size.x += 2.0;		/* add a safety margin to the bounding box */
  size.y += 2.0;
  size.z += 2.0;
  x3d_bbox (&center, &size);

  x3d_finish_node();

  indent_newline();
  x3d_node ("NavigationInfo");
  indent_string ("speed ");
  x3d_g (4.0);
  indent_newline();
  x3d_list ("type");
  x3d_s_quoted ("EXAMINE");
  x3d_s_quoted ("FLY");
  x3d_finish_list();
  if (!headlight) {
    indent_newline();
    indent_string ("headlight");
    indent_string ("FALSE");
  }
  x3d_finish_node();

  if (x3d_output_format_mode == X3D_OUTPUT_WEBGL ||
      colour_unequal (&background_colour, &black_colour)) {
    colour bg;

    if (x3d_output_format_mode == X3D_OUTPUT_WEBGL &&
        !colour_unequal (&background_colour, &black_colour)) {
      bg = white_colour;
    } else {
      bg = background_colour;
    }
    indent_newline();
    x3d_node ("Background");
    x3d_list ("skyColor");
    x3d_colour (&bg);
    x3d_finish_list();
    x3d_finish_node();
  }

  indent_newline();
  pos = center;
  if (size.x > size.y) {
    pos.z += 1.2 * size.x;
  } else {
    pos.z += 1.2 * size.y;
  }
  x3d_viewpoint (&pos, NULL, 0.0, 0.0, "overall default");

  if (fog != 0.0) x3d_fog (FALSE, fog, &background_colour);

  for (vp = viewpoints; vp; vp = vp2) {
    if (vp->rot > 0.0001) {
      x3d_viewpoint (&(vp->from), &(vp->ori), vp->rot, vp->fov, vp->descr);
    } else {
      x3d_viewpoint (&(vp->from), NULL, 0.0, vp->fov, vp->descr);
    }
    vp2 = vp->next;
    free (vp->descr);
    free (vp);
  }
  viewpoints = NULL;

  if (message_mode) {
    fprintf (stderr, "%i line coordinates, %i points, %i polygons, %i segment colours,\n",
	     line_count, point_count, polygon_count, colour_count);
    fprintf (stderr, "%i cylinders, %i spheres and %i labels.\n",
	     cylinder_count, sphere_count, label_count);
  }
}


/*------------------------------------------------------------*/
void
x3d_anchor_start (char *str)
{
  assert (str);
  assert (*str);

  indent_newline();
  x3d_node ("Anchor");
  indent_string ("url ");
  x3d_s_quoted (str);
}


/*------------------------------------------------------------*/
void
x3d_anchor_description (char *str)
{
  assert (str);
  assert (*str);

  indent_newline();
  indent_string ("description ");
  x3d_s_quoted (str);
}


/*------------------------------------------------------------*/
void
x3d_anchor_parameter (char *str)
{
  assert (str);
  assert (*str);

  indent_newline();
  if (anchor_parameter_not_started) {
    x3d_list ("parameter");
    anchor_parameter_not_started = FALSE;
  }
  x3d_s_quoted (str);
}


/*------------------------------------------------------------*/
void
x3d_anchor_start_geometry (void)
{
  if (!anchor_parameter_not_started) {
    x3d_finish_list();
    anchor_parameter_not_started = TRUE;
  }
  indent_newline();
  if (indent_level <= 0) {
    x3d_list ("children");
  } else {
    indent_string ("children [");
    indent_increment_level (TRUE);
  }

  ext3d_push();
  ext3d_initialize();
}


/*------------------------------------------------------------*/
void
x3d_anchor_finish (void)
{
  vector3 center, size;

  x3d_finish_list();

  ext3d_get_center_size (&center, &size);
  ext3d_pop (TRUE);
  if (size.x < 0.0) yyerror ("anchor has no clickable objects");
  size.x += 1.0;		/* add a safety margin to the bounding box */
  size.y += 1.0;
  size.z += 1.0;
  x3d_bbox (&center, &size);

  x3d_finish_node();
}


/*------------------------------------------------------------*/
void
x3d_lod_start (void)
{
  indent_newline();
  x3d_node ("LOD");
  x3d_list ("level");

  ext3d_push();
  ext3d_initialize();

  if (lod_alloc == 0) {
    lod_alloc = 8;
    lod_ranges = malloc (lod_alloc * sizeof (double));
  }
  lod_count = 0;
}


/*------------------------------------------------------------*/
void
x3d_lod_finish (void)
{
  vector3 center, size;
  int slot;

  assert (lod_ranges);

  x3d_finish_list();

  ext3d_get_center_size (&center, &size);
  ext3d_pop (TRUE);

  indent_newline();
  indent_string ("center");
  x3d_f2 (center.x);
  x3d_f2 (center.y);
  x3d_f2 (center.z);

  indent_newline();
  x3d_list ("range");
  for (slot = 0; slot < lod_count; slot++) x3d_f2 (lod_ranges[slot]);
  x3d_finish_list();

  x3d_finish_node();
}


/*------------------------------------------------------------*/
void
x3d_lod_start_group (void)
{
  assert (lod_ranges);
  assert ((dstack_size == 1) || (dstack_size == 0));

  if (dstack_size == 1) {
    if (lod_count + 1 >= lod_alloc) {
      lod_alloc *= 2;
      lod_ranges = realloc (lod_ranges, lod_alloc * sizeof (double));
    }

    lod_ranges[lod_count++] = dstack[0];
    clear_dstack();

    if (lod_count > 1 &&
	lod_ranges[lod_count - 1] <= lod_ranges[lod_count - 2])
      yyerror ("level-of-detail range not in ascending order");
  }

  indent_newline();
  x3d_node ("Group");
  x3d_list ("children");

  assert (dstack_size == 0);
}


/*------------------------------------------------------------*/
void
x3d_lod_finish_group (void)
{
  assert (lod_ranges);

  x3d_finish_list();
  x3d_finish_node();
}


/*------------------------------------------------------------*/
void
x3d_viewpoint_start (char *str)
{
  assert (str);
  assert (*str);

  viewpoint_str = str_clone (str);
}


/*------------------------------------------------------------*/
void
x3d_viewpoint_output (void)
{
  viewpoint_node *vp;

  vector3 towards, from, dir;
  vector3 zdir = {0.0, 0.0, -1.0};
  double angle;

  assert (viewpoint_str);
  assert ((dstack_size == 4) ||
	  (dstack_size == 6) ||
	  (dstack_size == 7));

  vp = malloc (sizeof (viewpoint_node));
  vp->next = NULL;

  if (dstack_size == 4) {	/* towards origin */
    towards.x = dstack[0];
    towards.y = dstack[1];
    towards.z = dstack[2];
    dir = towards;
    v3_normalize (&dir);
    v3_sum_scaled (&from, &towards, dstack[3], &dir);

  } else {			/* to, from specified points */
    from.x = dstack[0];
    from.y = dstack[1];
    from.z = dstack[2];
    towards.x = dstack[3];
    towards.y = dstack[4];
    towards.z = dstack[5];
    if (dstack_size == 7) {	/* additional distance */
      v3_difference (&dir, &from, &towards);
      v3_normalize (&dir);
      v3_add_scaled (&from, dstack[6], &dir);
    }
  }
  clear_dstack();

  v3_difference (&dir, &towards, &from);
  angle = v3_angle (&dir, &zdir);

  vp->from = from;
  vp->rot = angle;
  vp->fov = 0.0;
  if (angle > 0.0001) {
    v3_cross_product (&(vp->ori), &zdir, &dir);
    v3_normalize (&(vp->ori));
  }
  vp->descr = viewpoint_str;
  viewpoint_str = NULL;		/* do not delete the string */

  if (viewpoints == NULL) {
    viewpoints = vp;
  } else {
    viewpoint_node *vp2;
    for (vp2 = viewpoints; vp2->next; vp2 = vp2->next) ;
    vp2->next = vp;
  }

  assert (dstack_size == 0);
}


/*------------------------------------------------------------*/
void
x3d_ms_directionallight (void)
{
  vector3 dir;

  assert ((dstack_size == 3) || (dstack_size == 6));

  if (dstack_size == 3) {
    dir.x = dstack[0];
    dir.y = dstack[1];
    dir.z = dstack[2];
  } else {
    dir.x = dstack[3] - dstack[0];
    dir.y = dstack[4] - dstack[1];
    dir.z = dstack[5] - dstack[2];
  }
  v3_normalize (&dir);
  clear_dstack();

  indent_newline();
  x3d_directionallight (current_state->lightintensity,
			 current_state->lightambientintensity,
			 &(current_state->lightcolour),
			 &dir);
}


/*------------------------------------------------------------*/
void
x3d_ms_pointlight (void)
{
  vector3 pos;

  assert (dstack_size == 3);

  pos.x = dstack[0];
  pos.y = dstack[1];
  pos.z = dstack[2];
  clear_dstack();

  indent_newline();
  x3d_pointlight (current_state->lightintensity,
		   current_state->lightambientintensity,
		   &(current_state->lightcolour),
		   &pos,
		   current_state->lightradius,
		   &(current_state->lightattenuation));

  assert (dstack_size == 0);
}


/*------------------------------------------------------------*/
void
x3d_ms_spotlight (void)
{
  vector3 pos, dir;
  double angle;

  assert ((dstack_size == 7) || (dstack_size == 10));

  pos.x = dstack[0];
  pos.y = dstack[1];
  pos.z = dstack[2];

  if (dstack_size == 7) {
    dir.x = dstack[3];
    dir.y = dstack[4];
    dir.z = dstack[5];
    angle = dstack[6];
  } else {
    dir.x = dstack[6] - dstack[3];
    dir.y = dstack[7] - dstack[4];
    dir.z = dstack[8] - dstack[5];
    angle = dstack[9];
  }
  v3_normalize (&dir);
  clear_dstack();

  indent_newline();
  x3d_spotlight (current_state->lightintensity,
		  current_state->lightambientintensity,
		  &(current_state->lightcolour),
		  &pos, &dir,
		  to_radians (angle), to_radians (angle) / 2.0,
		  current_state->lightradius,
		  &(current_state->lightattenuation));

  assert (dstack_size == 0);
}


/*------------------------------------------------------------*/
void
x3d_set_area (void)
{
  if (message_mode) fprintf (stderr, "ignoring 'area' for X3D output\n");
  clear_dstack();
}


/*------------------------------------------------------------*/
void
x3d_set_background (void)
{
  colour_copy_to_rgb (&background_colour, &given_colour);
}


/*------------------------------------------------------------*/
void
x3d_coil (void)
{
  int slot, base;
  coil_segment *cs;

  indent_newline();
  x3d_comment ("MolScript: coil or turn");
  x3d_node ("Shape");
  output_appearance (FALSE, &(current_state->planecolour));
  indent_newline();
  x3d_node ("geometry IndexedFaceSet");
  x3d_s_newline ("creaseAngle 2.5");
  x3d_webgl_maybe_two_sided();

  x3d_node ("coord Coordinate");
  x3d_list ("point");
  for (slot = 0; slot < coil_segment_count; slot++) {
    cs = coil_segments + slot;
    x3d_v3 (&(cs->p1));
    x3d_v3 (&(cs->p2));
    x3d_v3 (&(cs->p3));
    x3d_v3 (&(cs->p4));
  }
  x3d_finish_list();
  x3d_finish_node();

  indent_newline();
  x3d_list ("coordIndex");
  for (slot = 0; slot < coil_segment_count - 1; slot++) {
    base = 4 * slot;
    x3d_tri_indices (base, 0, 1, 5);
    x3d_tri_indices (base, 0, 5, 4);
    x3d_tri_indices (base, 3, 0, 4);
    x3d_tri_indices (base, 3, 4, 7);
    x3d_tri_indices (base, 2, 3, 7);
    x3d_tri_indices (base, 2, 7, 6);
    x3d_tri_indices (base, 1, 2, 5);
    x3d_tri_indices (base, 2, 6, 5);
  }
  polygon_count += 8 * coil_segment_count;
  base = 0;
  x3d_quad_indices (base, 3, 2, 1, 0);
  base = 4 * (coil_segment_count - 1);
  x3d_quad_indices (base, 3, 2, 1, 0);
  polygon_count += 2;
  x3d_finish_list();

  if (current_state->colourparts) {
    colour rgb;
    int index;

    indent_newline();
    x3d_s_newline ("colorPerVertex FALSE");
    x3d_node ("color Color");
    x3d_list ("color");

    colour_to_rgb (&(coil_segments[0].c));
    rgb = coil_segments[0].c;
    x3d_rgb_colour (&rgb);

    for (slot = 1; slot < coil_segment_count; slot++) {
      cs = coil_segments + slot;

      colour_to_rgb (&(cs->c));
      if (colour_unequal (&rgb, &(cs->c))) {
	rgb = cs->c;
	x3d_rgb_colour (&rgb);
	colour_count++;
      }
    }

    x3d_finish_list();
    x3d_finish_node();

    indent_newline();
    x3d_list ("colorIndex");

    rgb = coil_segments[0].c;
    index = 0;
    for (slot = 0; slot < coil_segment_count; slot++) {
      cs = coil_segments + slot;

      if (colour_unequal (&rgb, &(cs->c))) {
	rgb = cs->c;
	index++;
      }
      x3d_i (index);
      x3d_i (index);
      x3d_i (index);
      x3d_i (index);
      x3d_i (index);
      x3d_i (index);
      x3d_i (index);
      x3d_i (index);
    }
    x3d_i (0);
    x3d_i (index);

    x3d_finish_list();
  }

  x3d_finish_node();
  x3d_finish_node();
}


/*------------------------------------------------------------*/
void
x3d_cylinder (vector3 *v1, vector3 *v2)
{
  vector3 dir, rot, middle;
  double angle;

  assert (v1);
  assert (v2);
  assert (v3_distance (v1, v2) > 0.0);

  v3_difference (&dir, v1, v2);
  angle = v3_angle (&dir, &yaxis);
  if (angle > 0.0) {
    v3_cross_product (&rot, &yaxis, &dir);
    v3_normalize (&rot);
  } else {
    rot = yaxis;
  }
  v3_middle (&middle, v1, v2);

  indent_newline();
  x3d_node ("Cyl");
  indent_string ("p ");
  x3d_v3 (&middle);
  output_rot (&rot, angle);
  output_scale (current_state->cylinderradius,
		v3_length (&dir) / 2.0,
		current_state->cylinderradius);
  output_dc (&(current_state->planecolour));
  output_ec (&(current_state->emissivecolour));
  output_sc (&(current_state->specularcolour));
  output_sh (current_state->shininess);
  output_tr (current_state->transparency);
  x3d_finish_node();

  cylinder_count++;
}


/*------------------------------------------------------------*/
void
x3d_helix (void)
{
  int slot, base;
  helix_segment *hs;
  dynstring *def_name = NULL;

  indent_newline();
  if (current_state->colourparts) {
    x3d_comment ("MolScript: helix");
  } else {
    x3d_comment ("MolScript: helix outer surface");
  }
  x3d_node ("Shape");
  output_appearance (FALSE, &(current_state->planecolour));
  indent_newline();
  x3d_node ("geometry IndexedFaceSet");
  x3d_s_newline ("creaseAngle 1.6");
  x3d_webgl_maybe_two_sided();
  if (current_state->colourparts) x3d_s_newline ("solid FALSE");

  indent_string ("coord");
  if (!current_state->colourparts) {
    def_name = ds_create ("_");
    ds_cat_int (def_name, def_count++);
    x3d_def (def_name->string);
  }
  x3d_node ("Coordinate");
  x3d_list ("point");
  for (slot = 0; slot < helix_segment_count; slot++) {
    hs = helix_segments + slot;
    x3d_v3 (&(hs->p1));
    x3d_v3 (&(hs->p2));
  }
  x3d_finish_list();
  x3d_finish_node();

  indent_newline();
  x3d_list ("coordIndex");
  for (slot = 0; slot < helix_segment_count - 1; slot++) {
    base = 2 * slot;
    x3d_tri_indices (base, 0, 1, 2);
    x3d_tri_indices (base, 1, 3, 2);
  }
  x3d_finish_list();
  polygon_count += 2 * helix_segment_count;

  if (current_state->colourparts) {
    colour rgb;
    int index;

    indent_newline();
    x3d_s_newline ("colorPerVertex FALSE");
    x3d_node ("color Color");
    x3d_list ("color");

    colour_to_rgb (&(helix_segments[0].c));
    rgb = helix_segments[0].c;
    x3d_rgb_colour (&rgb);

    for (slot = 1; slot < helix_segment_count; slot++) {
      hs = helix_segments + slot;

      colour_to_rgb (&(hs->c));
      if (colour_unequal (&rgb, &(hs->c))) {
	rgb = hs->c;
	x3d_rgb_colour (&rgb);
	colour_count++;
      }
    }

    x3d_finish_list();
    x3d_finish_node();

    indent_newline();
    x3d_list ("colorIndex");

    rgb = helix_segments[0].c;
    index = 0;
    for (slot = 0; slot < helix_segment_count; slot++) {
      hs = helix_segments + slot;

      if (colour_unequal (&rgb, &(hs->c))) {
	rgb = hs->c;
	index++;
      }
      x3d_i (index);
      x3d_i (index);
    }

    x3d_finish_list();

  } else {			/* not colourparts */

    x3d_finish_node();		/* finish Shape node for outer surface */
    x3d_finish_node();

    indent_newline();
    x3d_comment ("MolScript: helix inner surface");
    x3d_node ("Shape");
    output_appearance (FALSE, &(current_state->plane2colour));
    indent_newline();
    x3d_node ("geometry IndexedFaceSet");
    x3d_s_newline ("creaseAngle 1.6");

    indent_string ("coord USE");
    x3d_s_newline (def_name->string);

    x3d_list ("coordIndex");
    for (slot = 0; slot < helix_segment_count - 1; slot++) {
      base = 2 * slot;
      x3d_tri_indices (base, 0, 2, 1);
      x3d_tri_indices (base, 1, 2, 3);
    }
    x3d_finish_list();
    polygon_count += 2 * helix_segment_count;
  }

  x3d_finish_node();
  x3d_finish_node();

  if (def_name) ds_delete (def_name);
}


/*------------------------------------------------------------*/
void
x3d_label (vector3 *p, char *label, colour *c)
{
  assert (p);
  assert (label);
  assert (*label);

  indent_newline();
  x3d_node ("Label");
  indent_string ("p ");
  x3d_v3 (p);
  indent_string ("c");
  x3d_s_quoted (label);
  if (v3_length (&(current_state->labeloffset)) >= 0.001) {
    indent_string (" o ");
    x3d_v3 (&(current_state->labeloffset));
  }
  if (c) {
    output_dc (c);
  } else {
    output_dc (&(current_state->linecolour));
  }
  output_ec (&(current_state->emissivecolour));
  output_sc (&(current_state->specularcolour));
  output_sh (current_state->shininess);
  output_tr (current_state->transparency);
  indent_string ("sz");
  x3d_g3 (current_state->labelsize / 20.0);
  x3d_finish_node();

  label_count++;
}


/*------------------------------------------------------------*/
void
x3d_line (boolean polylines)
{
  int slot;

  if (line_segment_count < 2) return;

  indent_newline();
  x3d_comment ("MolScript: line, trace, bonds, coil or turn");
  x3d_node ("Shape");
  output_appearance (TRUE, &(line_segments[0].c));
  indent_newline();
  x3d_node ("geometry IndexedLineSet");

  indent_string ("coord");
  x3d_node ("Coordinate");
  x3d_list ("point");
  for (slot = 0; slot < line_segment_count; slot++)
    x3d_v3 (&(line_segments[slot].p));
  x3d_finish_list();
  x3d_finish_node();

  indent_newline();
  x3d_list ("coordIndex");
  if (polylines) {
    x3d_i (0);
    for (slot = 1; slot < line_segment_count; slot++) {
      if (line_segments[slot].new) x3d_i (-1);
      x3d_i (slot);
    }
  } else {
    for (slot = 0; slot < line_segment_count; slot += 2) {
      x3d_i (slot);
      x3d_i (slot + 1);
      x3d_i (-1);
    }
  }
  x3d_finish_list();

  if (current_state->colourparts) {

    indent_newline();
    x3d_s_newline ("colorPerVertex FALSE");
    x3d_node ("color Color");
    x3d_list ("color");

    colour_cache_init();
    if (polylines) {
      for (slot = 0; slot < line_segment_count; slot++) {
	if (line_segments[slot].new) {
	  colour_cache_add (&(line_segments[slot].c));
	}
      }
    } else {
      for (slot = 0; slot < line_segment_count; slot += 2) {
	colour_cache_add (&(line_segments[slot].c));
      }
    }
    x3d_finish_list();
    x3d_finish_node();

    indent_newline();
    x3d_list ("colorIndex");
    colour_cache_index_init();

    if (polylines) {
      for (slot = 0; slot < line_segment_count; slot++) {
	if (line_segments[slot].new) {
	  x3d_i (colour_cache_index (&(line_segments[slot].c)));
	}
      }
    } else {
      for (slot = 0; slot < line_segment_count; slot += 2) {
	x3d_i (colour_cache_index (&(line_segments[slot].c)));
      }
    }
    x3d_finish_list();
  }
  x3d_finish_node();
  x3d_finish_node();

  line_count += line_segment_count;
}


/*------------------------------------------------------------*/
void
x3d_sphere (at3d *at, double radius)
{
  assert (at);
  assert (radius > 0.0);

  indent_newline();
  x3d_node ("Ball");
  indent_string ("p ");
  x3d_v3 (&(at->xyz));
  indent_string ("rad");
  x3d_g3 (radius);
  output_dc (&(at->colour));
  output_ec (&(current_state->emissivecolour));
  output_sc (&(current_state->specularcolour));
  output_sh (current_state->shininess);
  output_tr (current_state->transparency);
  x3d_finish_node();

  sphere_count++;
}


/*------------------------------------------------------------*/
void
x3d_stick (vector3 *v1, vector3 *v2, double r1, double r2, colour *c)
{
  vector3 dir, rot, middle;
  double angle;

  assert (v1);
  assert (v2);
  assert (v3_distance (v1, v2) > 0.0);

  v3_difference (&dir, v1, v2);
  angle = v3_angle (&dir, &yaxis);
  if (angle > 0.0) {
    v3_cross_product (&rot, &yaxis, &dir);
    v3_normalize (&rot);
  } else {
    rot = yaxis;
  }
  v3_middle (&middle, v1, v2);

  indent_newline();
  x3d_node ("Stick");
  indent_string ("p ");
  x3d_v3_g (&middle);		/* x3d_v3 causes gaps between half-sticks */
  output_rot (&rot, angle);
  output_scale (current_state->stickradius,
		0.502 * v3_length (&dir), /* kludge to avoid glitches */
		current_state->stickradius);
  if (c) {
    output_dc (c);
  } else {
    output_dc (&(current_state->planecolour));
  }
  output_ec (&(current_state->emissivecolour));
  output_sc (&(current_state->specularcolour));
  output_sh (current_state->shininess);
  output_tr (current_state->transparency);
  x3d_finish_node();

  polygon_count += STICK_SEGMENTS;
}


/*------------------------------------------------------------*/
void
x3d_strand (void)
{
  int slot, base, thickness, cycle, cycles;
  strand_segment *ss;
  dynstring *def_name = NULL;

  thickness = current_state->strandthickness >= 0.01;

  indent_newline();
  if (current_state->colourparts || !thickness) {
    x3d_comment ("MolScript: strand");
  } else {
    x3d_comment ("MolScript: strand main faces");
  }
  x3d_node ("Shape");
  output_appearance (FALSE, &(current_state->planecolour));
  indent_newline();
  x3d_node ("geometry IndexedFaceSet");
  x3d_s_newline ("creaseAngle 1");
  x3d_webgl_maybe_two_sided();
  if (! thickness) x3d_s_newline ("solid FALSE");

  indent_string ("coord");
  if (! current_state->colourparts && thickness) {
    def_name = ds_create ("_");
    ds_cat_int (def_name, def_count++);
    x3d_def (def_name->string);
  }
  x3d_node ("Coordinate");
  x3d_list ("point");
  if (thickness) {		/* strand main face */
    for (slot = 0; slot < strand_segment_count - 1; slot++) { /* upper */
      ss = strand_segments + slot;
      x3d_v3 (&(ss->p1));
      x3d_v3 (&(ss->p2));
      x3d_v3 (&(ss->p3));
      x3d_v3 (&(ss->p4));
    }
    ss = strand_segments + strand_segment_count - 1;
    x3d_v3 (&(ss->p1));
    x3d_v3 (&(ss->p2));
  } else {
    for (slot = 0; slot < strand_segment_count - 1; slot++) { /* lower */
      ss = strand_segments + slot;
      x3d_v3 (&(ss->p1));
      x3d_v3 (&(ss->p3));
    }
    ss = strand_segments + strand_segment_count - 1;
    x3d_v3 (&(ss->p1));
  }
  x3d_finish_list();
  x3d_finish_node();

  indent_newline();
  x3d_list ("coordIndex");
  if (thickness) {		/* strand face, thick */
    for (slot = 0; slot < strand_segment_count - 4; slot++) { /* upper */
      base = 4 * slot;
      x3d_tri_indices (base, 0, 3, 4);
      x3d_tri_indices (base, 3, 7, 4);
    }
    polygon_count += 2 * (strand_segment_count - 4);

    base += 4;			/* arrow head upper */
    x3d_tri_indices (base, 0, 8, 4);
    x3d_tri_indices (base, 0, 3, 8);
    x3d_tri_indices (base, 3, 11, 8);
    x3d_tri_indices (base, 3, 7, 11);
    x3d_tri_indices (base, 8, 11, 12);
    polygon_count += 5;

    for (slot = 0; slot < strand_segment_count - 4; slot++) { /* lower */
      base = 4 * slot;
      x3d_tri_indices (base, 2, 1, 5);
      x3d_tri_indices (base, 5, 6, 2);
    }
    polygon_count += 2 * (strand_segment_count - 4);

    base += 4;			/* arrow head lower */
    x3d_tri_indices (base, 1, 5, 9);
    x3d_tri_indices (base, 2, 1, 9);
    x3d_tri_indices (base, 2, 9, 10);
    x3d_tri_indices (base, 2, 10, 6);
    x3d_tri_indices (base, 10, 9, 13);
    polygon_count += 5;

    if (def_name) {
      x3d_finish_list();	/* finish polygons for main faces */

      x3d_finish_node();	/* finish Shape node for main faces */
      x3d_finish_node();

      indent_newline();
      x3d_comment ("MolScript: strand side faces");
      x3d_node ("Shape");
      output_appearance (FALSE, &(current_state->plane2colour));
      indent_newline();
      x3d_node ("geometry IndexedFaceSet");
      x3d_s_newline ("creaseAngle 0.79");

      indent_string ("coord USE");
      x3d_s_newline (def_name->string);

      x3d_list ("coordIndex");	/* begin polygons for side faces */
    }

    x3d_quad_indices (0, 0, 1, 2, 3); /* strand base */
    polygon_count++;

    for (slot = 0; slot < strand_segment_count - 4; slot++) { /* side left */
      base = 4 * slot;
      x3d_tri_indices (base, 1, 0, 5);
      x3d_tri_indices (base, 0, 4, 5);
    }
    polygon_count += 2 * (strand_segment_count - 4);

    for (slot = 0; slot < strand_segment_count - 4; slot++) { /* side right */
      base = 4 * slot;
      x3d_tri_indices (base, 2, 6, 3);
      x3d_tri_indices (base, 3, 6, 7);
    }
    polygon_count += 2 * (strand_segment_count - 4);

    base += 4;
    x3d_quad_indices (base, 4, 5, 1, 0); /* arrow base */
    x3d_quad_indices (base, 3, 2, 6, 7);
    polygon_count += 2;

    base += 4;
    x3d_tri_indices (base, 1, 0, 5); /* arrow side left 1 */
    x3d_tri_indices (base, 0, 4, 5);
    x3d_tri_indices (base, 2, 6, 3); /* arrow side right 1 */
    x3d_tri_indices (base, 3, 6, 7);
    polygon_count += 4;

    base += 4;
    x3d_tri_indices (base, 1, 0, 5); /* arrow side left 2 */
    x3d_tri_indices (base, 0, 4, 5);
    x3d_tri_indices (base, 2, 5, 3); /* arrow side right 2 */
    x3d_tri_indices (base, 3, 5, 4);
    polygon_count += 4;

  } else {			/* strand face, thin */

    for (slot = 0; slot < strand_segment_count - 4; slot++) {
      base = 2 * slot;
      x3d_tri_indices (base, 0, 1, 2);
      x3d_tri_indices (base, 1, 3, 2);
    }
    polygon_count += 2 * (strand_segment_count - 4);

    base += 2;
    x3d_tri_indices (base, 0, 4, 2);
    x3d_tri_indices (base, 0, 1, 4);
    x3d_tri_indices (base, 1, 5, 4);
    x3d_tri_indices (base, 1, 3, 5);
    x3d_tri_indices (base, 4, 5, 6);
    polygon_count += 5;
  }
  x3d_finish_list();

  if (current_state->colourparts) {
    colour rgb;

    indent_newline();
    x3d_s_newline ("colorPerVertex FALSE");
    x3d_node ("color Color");
    x3d_list ("color");

    colour_to_rgb (&(strand_segments[0].c));
    rgb = strand_segments[0].c;
    x3d_rgb_colour (&rgb);

    for (slot = 1; slot < strand_segment_count - 1; slot++) {
      ss = strand_segments + slot;

      colour_to_rgb (&(strand_segments[slot].c));
      if (colour_unequal (&rgb, &(ss->c))) {
	rgb = ss->c;
	x3d_rgb_colour (&rgb);
	colour_count++;
      }
    }
    x3d_finish_list();
    x3d_finish_node();

    indent_newline();
    x3d_list ("colorIndex");

    if (thickness) cycles = 2; else cycles = 1;

    for (cycle = 0; cycle < cycles; cycle++) {
      rgb = strand_segments[0].c;
      base = 0;
      x3d_i (base);
      x3d_i (base);

      for (slot = 1; slot < strand_segment_count - 4; slot++) {
	ss = strand_segments + slot;
	if (colour_unequal (&rgb, &(ss->c))) {
	  rgb = ss->c;
	  base++;
	}
	x3d_i (base);
	x3d_i (base);
      }

      x3d_i (base);		/* arrow head first part */
      x3d_i (base);
      x3d_i (base);
      x3d_i (base);
      if (colour_unequal (&rgb, &(strand_segments[strand_segment_count-2].c))){
	rgb = strand_segments[strand_segment_count-2].c;
	base++;
      }
      x3d_i (base);		/* arrow head last part */

    }

    if (thickness) {		/* strand sides */
      rgb = strand_segments[0].c;
      base = 0;
      x3d_i (base);		/* strand base */

      for (slot = 0; slot < strand_segment_count - 4; slot++) { /* side right*/
	if (colour_unequal (&rgb, &(strand_segments[slot].c))) {
	  rgb = strand_segments[slot].c;
	  base++;
	}
	x3d_i (base);
	x3d_i (base);
      }

      rgb = strand_segments[0].c;
      base = 0;

      for (slot = 0; slot < strand_segment_count - 4; slot++) { /* side left */
	if (colour_unequal (&rgb, &(strand_segments[slot].c))) {
	  rgb = strand_segments[slot].c;
	  base++;
	}
	x3d_i (base);
	x3d_i (base);
      }

      x3d_i (base);		/* arrow base */
      x3d_i (base);

      x3d_i (base);		/* arrow side left 1 */
      x3d_i (base);
      x3d_i (base);		/* arrow side right 1 */
      x3d_i (base);

      if (colour_unequal (&rgb, &(strand_segments[strand_segment_count-2].c))){
	rgb = strand_segments[strand_segment_count-2].c;
	base++;
      }
      x3d_i (base);		/* arrow side left 2 */
      x3d_i (base);
      x3d_i (base);		/* arrow side right 2 */
      x3d_i (base);
    }

    x3d_finish_list();
  }

  x3d_finish_node();
  x3d_finish_node();

  if (def_name) ds_delete (def_name);
}


/*------------------------------------------------------------*/
void
x3d_strand2 (void)
{
  int slot, base, thickness, cycle, cycles;
  strand_segment *ss;

  thickness = current_state->strandthickness >= 0.01;

  indent_newline();
  x3d_comment ("MolScript: strand");
  x3d_node ("Shape");
  output_appearance (FALSE, &(current_state->planecolour));
  indent_newline();
  x3d_node ("geometry IndexedFaceSet");
  x3d_s_newline ("creaseAngle 0.79");
  if (!thickness) x3d_s_newline ("solid FALSE");

  indent_string ("coord");
  x3d_node ("Coordinate");
  x3d_list ("point");
  if (thickness) {		/* strand main face */
    for (slot = 0; slot < strand_segment_count - 1; slot++) { /* upper */
      ss = strand_segments + slot;
      x3d_v3 (&(ss->p1));
      x3d_v3 (&(ss->p2));
      x3d_v3 (&(ss->p3));
      x3d_v3 (&(ss->p4));
    }
    ss = strand_segments + strand_segment_count - 1;
    x3d_v3 (&(ss->p1));
    x3d_v3 (&(ss->p2));
  } else {
    for (slot = 0; slot < strand_segment_count - 1; slot++) { /* lower */
      ss = strand_segments + slot;
      x3d_v3 (&(ss->p1));
      x3d_v3 (&(ss->p3));
    }
    ss = strand_segments + strand_segment_count - 1;
    x3d_v3 (&(ss->p1));
  }
  x3d_finish_list();
  x3d_finish_node();

  indent_newline();
  x3d_list ("coordIndex");
  if (thickness) {		/* strand face, thick */
    for (slot = 0; slot < strand_segment_count - 4; slot++) { /* upper */
      base = 4 * slot;
      x3d_tri_indices (base, 0, 3, 4);
      x3d_tri_indices (base, 3, 7, 4);
    }
    polygon_count += 2 * (strand_segment_count - 4);

    base += 4;			/* arrow head upper */
    x3d_tri_indices (base, 0, 8, 4);
    x3d_tri_indices (base, 0, 3, 8);
    x3d_tri_indices (base, 3, 11, 8);
    x3d_tri_indices (base, 3, 7, 11);
    x3d_tri_indices (base, 8, 11, 12);
    polygon_count += 5;

    for (slot = 0; slot < strand_segment_count - 4; slot++) { /* lower */
      base = 4 * slot;
      x3d_tri_indices (base, 2, 1, 5);
      x3d_tri_indices (base, 5, 6, 2);
    }
    polygon_count += 2 * (strand_segment_count - 4);

    base += 4;			/* arrow head lower */
    x3d_tri_indices (base, 1, 5, 9);
    x3d_tri_indices (base, 2, 1, 9);
    x3d_tri_indices (base, 2, 9, 10);
    x3d_tri_indices (base, 2, 10, 6);
    x3d_tri_indices (base, 10, 9, 13);
    polygon_count += 5;

    x3d_quad_indices (0, 0, 1, 2, 3); /* strand base */
    polygon_count++;

    for (slot = 0; slot < strand_segment_count - 4; slot++) { /* side left */
      base = 4 * slot;
      x3d_tri_indices (base, 1, 0, 5);
      x3d_tri_indices (base, 0, 4, 5);
    }
    polygon_count += 2 * (strand_segment_count - 4);

    for (slot = 0; slot < strand_segment_count - 4; slot++) { /* side right */
      base = 4 * slot;
      x3d_tri_indices (base, 2, 6, 3);
      x3d_tri_indices (base, 3, 6, 7);
    }
    polygon_count += 2 * (strand_segment_count - 4);

    base += 4;
    x3d_quad_indices (base, 4, 5, 1, 0); /* arrow base */
    x3d_quad_indices (base, 3, 2, 6, 7);
    polygon_count += 2;

    base += 4;
    x3d_tri_indices (base, 1, 0, 5); /* arrow side left 1 */
    x3d_tri_indices (base, 0, 4, 5);
    x3d_tri_indices (base, 2, 6, 3); /* arrow side right 1 */
    x3d_tri_indices (base, 3, 6, 7);
    polygon_count += 4;

    base += 4;
    x3d_tri_indices (base, 1, 0, 5); /* arrow side left 2 */
    x3d_tri_indices (base, 0, 4, 5);
    x3d_tri_indices (base, 2, 5, 3); /* arrow side right 2 */
    x3d_tri_indices (base, 3, 5, 4);
    polygon_count += 4;

  } else {			/* strand face, thin */

    for (slot = 0; slot < strand_segment_count - 4; slot++) {
      base = 2 * slot;
      x3d_tri_indices (base, 0, 1, 2);
      x3d_tri_indices (base, 1, 3, 2);
    }
    polygon_count += 2 * (strand_segment_count - 4);

    base += 2;
    x3d_tri_indices (base, 0, 4, 2);
    x3d_tri_indices (base, 0, 1, 4);
    x3d_tri_indices (base, 1, 5, 4);
    x3d_tri_indices (base, 1, 3, 5);
    x3d_tri_indices (base, 4, 5, 6);
    polygon_count += 5;
  }
  x3d_finish_list();

  if (current_state->colourparts) {
    colour rgb;

    indent_newline();
    x3d_s_newline ("colorPerVertex FALSE");
    x3d_node ("color Color");
    x3d_list ("color");

    colour_to_rgb (&(strand_segments[0].c));
    rgb = strand_segments[0].c;
    x3d_rgb_colour (&rgb);

    for (slot = 1; slot < strand_segment_count; slot++) {
      ss = strand_segments + slot;

      colour_to_rgb (&(strand_segments[slot].c));
      if (colour_unequal (&rgb, &(ss->c))) {
	rgb = ss->c;
	x3d_rgb_colour (&rgb);
	colour_count++;
      }
    }
    x3d_finish_list();
    x3d_finish_node();

    indent_newline();
    x3d_list ("colorIndex");

    if (thickness) cycles = 2; else cycles = 1;

    for (cycle = 0; cycle < cycles; cycle++) {
      rgb = strand_segments[0].c;
      base = 0;
      x3d_i (base);
      x3d_i (base);

      for (slot = 1; slot < strand_segment_count - 4; slot++) {
	ss = strand_segments + slot;
	if (colour_unequal (&rgb, &(ss->c))) {
	  rgb = ss->c;
	  base++;
	}
	x3d_i (base);
	x3d_i (base);
      }

      x3d_i (base);		/* arrow head first part */
      x3d_i (base);
      x3d_i (base);
      x3d_i (base);
      if (colour_unequal (&rgb, &(strand_segments[strand_segment_count-2].c))){
	rgb = strand_segments[strand_segment_count-2].c;
	base++;
      }
      x3d_i (base);		/* arrow head last part */

    }

    if (thickness) {		/* strand sides */
      rgb = strand_segments[0].c;
      base = 0;
      x3d_i (base);		/* strand base */

      for (slot = 0; slot < strand_segment_count - 4; slot++) { /* side right*/
	if (colour_unequal (&rgb, &(strand_segments[slot].c))) {
	  rgb = strand_segments[slot].c;
	  base++;
	}
	x3d_i (base);
	x3d_i (base);
      }

      rgb = strand_segments[0].c;
      base = 0;

      for (slot = 0; slot < strand_segment_count - 4; slot++) { /* side left */
	if (colour_unequal (&rgb, &(strand_segments[slot].c))) {
	  rgb = strand_segments[slot].c;
	  base++;
	}
	x3d_i (base);
	x3d_i (base);
      }

      x3d_i (base);		/* arrow base */
      x3d_i (base);

      x3d_i (base);		/* arrow side left 1 */
      x3d_i (base);
      x3d_i (base);		/* arrow side right 1 */
      x3d_i (base);

      if (colour_unequal (&rgb, &(strand_segments[strand_segment_count-2].c))){
	rgb = strand_segments[strand_segment_count-2].c;
	base++;
      }
      x3d_i (base);		/* arrow side left 2 */
      x3d_i (base);
      x3d_i (base);		/* arrow side right 2 */
      x3d_i (base);
    }

    x3d_finish_list();
  }
  x3d_finish_node();
  x3d_finish_node();
}


/*------------------------------------------------------------*/
void
x3d_object (int code, vector3 *triplets, int count)
{
  int slot;
  vector3 *v;
  colour rgb = {COLOUR_RGB, 1.0, 1.0, 1.0};

  assert (triplets);
  assert (count > 0);

  indent_newline();
  x3d_node ("Shape");

  switch (code) {

  case OBJ_POINTS:
    x3d_comment ("MolScript: P object");
    output_appearance (TRUE, &(current_state->linecolour));
    indent_newline();
    x3d_node ("geometry PointSet");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot++) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    x3d_finish_node();
    point_count += count;
    break;

  case OBJ_POINTS_COLOURS:
    x3d_comment ("MolScript: PC object");
    output_appearance (TRUE, &(current_state->linecolour));
    indent_newline();
    x3d_node ("geometry PointSet");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot += 2) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_node ("color Color");
    x3d_list ("color");
    for (slot = 1; slot < count; slot += 2) {
      v = triplets + slot;
      rgb.x = v->x;
      rgb.y = v->y;
      rgb.z = v->z;
      x3d_rgb_colour (&rgb);
    }
    x3d_finish_list();
    x3d_finish_node();
    x3d_finish_node();
    x3d_finish_node();
    point_count += count;
    break;

  case OBJ_LINES:
    x3d_comment ("MolScript: L object");
    output_appearance (TRUE, &(current_state->linecolour));
    indent_newline();
    x3d_node ("geometry IndexedLineSet");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot++) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_list ("coordIndex");
    for (slot = 0; slot < count; slot++) x3d_i (slot);
    x3d_finish_list();
    x3d_finish_node();
    line_count += count - 1;
    break;

  case OBJ_LINES_COLOURS:
    x3d_comment ("MolScript: LC object");
    output_appearance (TRUE, &(current_state->linecolour));
    indent_newline();
    x3d_node ("geometry IndexedLineSet");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot += 2) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_list ("coordIndex");
    for (slot = 0; slot < count / 2; slot++) x3d_i (slot);
    x3d_finish_list();
    indent_newline();
    x3d_node ("color Color");
    x3d_list ("color");
    for (slot = 1; slot < count; slot += 2) {
      v = triplets + slot;
      rgb.x = v->x;
      rgb.y = v->y;
      rgb.z = v->z;
      x3d_rgb_colour (&rgb);
    }
    x3d_finish_list();
    x3d_finish_node();
    x3d_finish_node();
    line_count += count - 1;
    break;

  case OBJ_TRIANGLES:
    x3d_comment ("MolScript: T object");
    output_appearance (FALSE, &(current_state->planecolour));
    indent_newline();
    x3d_node ("geometry IndexedFaceSet");
    x3d_s_newline ("solid FALSE");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot++) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_list ("coordIndex");
    for (slot = 0; slot < count; slot += 3) x3d_tri_indices (slot, 0, 1, 2);
    x3d_finish_list();
    x3d_finish_node();
    polygon_count += count / 3;
    break;

  case OBJ_TRIANGLES_COLOURS:
    x3d_comment ("MolScript: TC object");
    output_appearance (FALSE, &(current_state->planecolour));
    indent_newline();
    x3d_node ("geometry IndexedFaceSet");
    x3d_s_newline ("solid FALSE");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot += 2) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_list ("coordIndex");
    for (slot = 0; slot < count/2; slot += 3) x3d_tri_indices (slot, 0, 1, 2);
    x3d_finish_list();
    indent_newline();
    x3d_node ("color Color");
    x3d_list ("color");
    for (slot = 1; slot < count; slot += 2) {
      v = triplets + slot;
      rgb.x = v->x;
      rgb.y = v->y;
      rgb.z = v->z;
      x3d_rgb_colour (&rgb);
    }
    x3d_finish_list();
    x3d_finish_node();
    x3d_finish_node();
    polygon_count += count / 6;
    break;

  case OBJ_TRIANGLES_NORMALS:
    x3d_comment ("MolScript: TN object");
    output_appearance (FALSE, &(current_state->planecolour));
    indent_newline();
    x3d_node ("geometry IndexedFaceSet");
    x3d_s_newline ("solid FALSE");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot += 2) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_list ("coordIndex");
    for (slot = 0; slot < count/2; slot += 3) x3d_tri_indices (slot, 0, 1, 2);
    x3d_finish_list();
    indent_newline();
    x3d_node ("normal Normal");
    x3d_list ("vector");
    for (slot = 1; slot < count; slot += 2) x3d_v3_g3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    x3d_finish_node();
    polygon_count += count / 6;
    break;

  case OBJ_TRIANGLES_NORMALS_COLOURS:
    x3d_comment ("MolScript: TNC object");
    output_appearance (FALSE, &(current_state->planecolour));
    indent_newline();
    x3d_node ("geometry IndexedFaceSet");
    x3d_s_newline ("solid FALSE");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot += 3) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_list ("coordIndex");
    for (slot = 0; slot < count/3; slot += 3) x3d_tri_indices (slot, 0, 1, 2);
    x3d_finish_list();
    indent_newline();
    x3d_node ("normal Normal");
    x3d_list ("vector");
    for (slot = 1; slot < count; slot += 3) x3d_v3_g3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_node ("color Color");
    x3d_list ("color");
    for (slot = 2; slot < count; slot += 3) {
      v = triplets + slot;
      rgb.x = v->x;
      rgb.y = v->y;
      rgb.z = v->z;
      x3d_rgb_colour (&rgb);
    }
    x3d_finish_list();
    x3d_finish_node();
    x3d_finish_node();
    polygon_count += count / 9;
    break;

  case OBJ_STRIP:
    x3d_comment ("MolScript: S object");
    output_appearance (FALSE, &(current_state->planecolour));
    indent_newline();
    x3d_node ("geometry IndexedFaceSet");
    x3d_s_newline ("solid FALSE");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot++) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_list ("coordIndex");
    for (slot = 0; slot < count - 2; slot++) x3d_tri_indices (slot, 0, 1, 2);
    x3d_finish_list();
    x3d_finish_node();
    polygon_count += count - 2;
    break;

  case OBJ_STRIP_COLOURS:
    x3d_comment ("MolScript: SC object");
    output_appearance (FALSE, &(current_state->planecolour));
    indent_newline();
    x3d_node ("geometry IndexedFaceSet");
    x3d_s_newline ("solid FALSE");
    x3d_s_newline ("colorPerVertex FALSE");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot += 2) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_list ("coordIndex");
    for (slot = 0; slot < count/2 - 2; slot++) x3d_tri_indices (slot, 0,1,2);
    x3d_finish_list();
    indent_newline();
    x3d_node ("color Color");
    x3d_list ("color");
    for (slot = 5; slot < count; slot += 2) {
      v = triplets + slot;
      rgb.x = v->x;
      rgb.y = v->y;
      rgb.z = v->z;
      x3d_rgb_colour (&rgb);
    }
    x3d_finish_list();
    x3d_finish_node();
    x3d_finish_node();
    polygon_count += count / 2 - 2;
    break;

  case OBJ_STRIP_NORMALS:
    x3d_comment ("MolScript: SN object");
    output_appearance (FALSE, &(current_state->planecolour));
    indent_newline();
    x3d_node ("geometry IndexedFaceSet");
    x3d_s_newline ("solid FALSE");
    x3d_s_newline ("colorPerVertex FALSE");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot += 2) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_list ("coordIndex");
    for (slot = 0; slot < count /2 - 2; slot++) {
      if (slot % 2) {
	x3d_tri_indices (slot, 1,0,2);
      } else {
	x3d_tri_indices (slot, 0,1,2);
      }
    }
    x3d_finish_list();
    indent_newline();
    x3d_node ("normal Normal");
    x3d_list ("vector");
    for (slot = 1; slot < count; slot += 2) x3d_v3_g3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    x3d_finish_node();
    polygon_count += count / 2 - 2;
    break;

  case OBJ_STRIP_NORMALS_COLOURS:
    x3d_comment ("MolScript: SNC object");
    output_appearance (FALSE, &(current_state->planecolour));
    indent_newline();
    x3d_node ("geometry IndexedFaceSet");
    x3d_s_newline ("solid FALSE");
    x3d_node ("coord Coordinate");
    x3d_list ("point");
    for (slot = 0; slot < count; slot += 3) x3d_v3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_list ("coordIndex");
    for (slot = 0; slot < count / 3 - 2; slot++) {
      if (slot % 2) {
	x3d_tri_indices (slot, 1,0,2);
      } else {
	x3d_tri_indices (slot, 0,1,2);
      }
    }
    x3d_finish_list();
    indent_newline();
    x3d_node ("normal Normal");
    x3d_list ("vector");
    for (slot = 1; slot < count; slot += 3) x3d_v3_g3 (triplets + slot);
    x3d_finish_list();
    x3d_finish_node();
    indent_newline();
    x3d_node ("color Color");
    x3d_list ("color");
    for (slot = 2; slot < count; slot += 3) {
      v = triplets + slot;
      rgb.x = v->x;
      rgb.y = v->y;
      rgb.z = v->z;
      x3d_rgb_colour (&rgb);
    }
    x3d_finish_list();
    x3d_finish_node();
    x3d_finish_node();
    polygon_count += count / 3 - 2;
    break;

  }

  x3d_finish_node();
}
