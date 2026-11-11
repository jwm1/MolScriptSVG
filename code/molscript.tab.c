/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "molscript.y"

/* MolScriptSVG v2.3
 
   bison (GNU's yacc) parser definition

   Copyright (C) 1997-1998 Per Kraulis
     1-Dec-1996  first attempts
    17-Dec-1996  nearly complete syntax
     5-Jan-1997  fairly finished
     5-Feb-1997  added command_group for anchor and similar
    15-Sep-1997  basically finished
    24-Feb-1998  problem with token X under Linux: changed to XAXIS
    23-Jul-1998  rearranged syntax for anchor command
*/

#include "global.h"
#include "lex.h"
#include "col.h"
#include "coord.h"
#include "xform.h"
#include "state.h"
#include "graphics.h"
#include "select.h"
#include "postscript.h"
#include "raster3d.h"
#include "vrml.h"
#include "figure.h"

/* To avoid warning with 'cc -ansi -fullwarn' on SGI IRIX; ugly! */
static void
__yy_bcopy (char *from, char *to, int count);

static void
__yy_memcpy (char *from, char *to, int count);



#line 109 "molscript.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "molscript.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INTEGER = 3,                    /* INTEGER  */
  YYSYMBOL_REAL = 4,                       /* REAL  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_ITEM = 6,                       /* ITEM  */
  YYSYMBOL_ON = 7,                         /* ON  */
  YYSYMBOL_OFF = 8,                        /* OFF  */
  YYSYMBOL_POSITION = 9,                   /* POSITION  */
  YYSYMBOL_TITLE = 10,                     /* TITLE  */
  YYSYMBOL_MACRO = 11,                     /* MACRO  */
  YYSYMBOL_PLOT = 12,                      /* PLOT  */
  YYSYMBOL_END_PLOT = 13,                  /* END_PLOT  */
  YYSYMBOL_COMMENT = 14,                   /* COMMENT  */
  YYSYMBOL_DEBUG = 15,                     /* DEBUG  */
  YYSYMBOL_POSTSCRIPT = 16,                /* POSTSCRIPT  */
  YYSYMBOL_RASTER3D = 17,                  /* RASTER3D  */
  YYSYMBOL_VRML = 18,                      /* VRML  */
  YYSYMBOL_NOFRAME = 19,                   /* NOFRAME  */
  YYSYMBOL_FRAME = 20,                     /* FRAME  */
  YYSYMBOL_AREA = 21,                      /* AREA  */
  YYSYMBOL_BACKGROUND = 22,                /* BACKGROUND  */
  YYSYMBOL_WINDOW = 23,                    /* WINDOW  */
  YYSYMBOL_SLAB = 24,                      /* SLAB  */
  YYSYMBOL_HEADLIGHT = 25,                 /* HEADLIGHT  */
  YYSYMBOL_SHADOWS = 26,                   /* SHADOWS  */
  YYSYMBOL_FOG = 27,                       /* FOG  */
  YYSYMBOL_READ = 28,                      /* READ  */
  YYSYMBOL_INLINE_PDB = 29,                /* INLINE_PDB  */
  YYSYMBOL_DELETE = 30,                    /* DELETE  */
  YYSYMBOL_COPY = 31,                      /* COPY  */
  YYSYMBOL_ANCHOR = 32,                    /* ANCHOR  */
  YYSYMBOL_DESCRIPTION = 33,               /* DESCRIPTION  */
  YYSYMBOL_PARAMETER = 34,                 /* PARAMETER  */
  YYSYMBOL_VIEWPOINT = 35,                 /* VIEWPOINT  */
  YYSYMBOL_ORIGIN = 36,                    /* ORIGIN  */
  YYSYMBOL_DIRECTIONALLIGHT = 37,          /* DIRECTIONALLIGHT  */
  YYSYMBOL_POINTLIGHT = 38,                /* POINTLIGHT  */
  YYSYMBOL_SPOTLIGHT = 39,                 /* SPOTLIGHT  */
  YYSYMBOL_LEVEL_OF_DETAIL = 40,           /* LEVEL_OF_DETAIL  */
  YYSYMBOL_TRANSFORM = 41,                 /* TRANSFORM  */
  YYSYMBOL_BY = 42,                        /* BY  */
  YYSYMBOL_CENTRE = 43,                    /* CENTRE  */
  YYSYMBOL_TRANSLATION = 44,               /* TRANSLATION  */
  YYSYMBOL_ROTATION = 45,                  /* ROTATION  */
  YYSYMBOL_XAXIS = 46,                     /* XAXIS  */
  YYSYMBOL_YAXIS = 47,                     /* YAXIS  */
  YYSYMBOL_ZAXIS = 48,                     /* ZAXIS  */
  YYSYMBOL_AXIS = 49,                      /* AXIS  */
  YYSYMBOL_STORE_MATRIX = 50,              /* STORE_MATRIX  */
  YYSYMBOL_RECALL_MATRIX = 51,             /* RECALL_MATRIX  */
  YYSYMBOL_NOT = 52,                       /* NOT  */
  YYSYMBOL_REQUIRE = 53,                   /* REQUIRE  */
  YYSYMBOL_AND = 54,                       /* AND  */
  YYSYMBOL_EITHER = 55,                    /* EITHER  */
  YYSYMBOL_OR = 56,                        /* OR  */
  YYSYMBOL_BACKBONE = 57,                  /* BACKBONE  */
  YYSYMBOL_PEPTIDE = 58,                   /* PEPTIDE  */
  YYSYMBOL_HYDROGENS = 59,                 /* HYDROGENS  */
  YYSYMBOL_ATOM = 60,                      /* ATOM  */
  YYSYMBOL_RES_ATOM = 61,                  /* RES_ATOM  */
  YYSYMBOL_B_FACTOR = 62,                  /* B_FACTOR  */
  YYSYMBOL_OCCUPANCY = 63,                 /* OCCUPANCY  */
  YYSYMBOL_IN = 64,                        /* IN  */
  YYSYMBOL_SPHERE = 65,                    /* SPHERE  */
  YYSYMBOL_CLOSE = 66,                     /* CLOSE  */
  YYSYMBOL_MODEL = 67,                     /* MODEL  */
  YYSYMBOL_AMINO_ACIDS = 68,               /* AMINO_ACIDS  */
  YYSYMBOL_WATERS = 69,                    /* WATERS  */
  YYSYMBOL_NUCLEOTIDES = 70,               /* NUCLEOTIDES  */
  YYSYMBOL_LIGANDS = 71,                   /* LIGANDS  */
  YYSYMBOL_MOLECULE = 72,                  /* MOLECULE  */
  YYSYMBOL_FROM = 73,                      /* FROM  */
  YYSYMBOL_TO = 74,                        /* TO  */
  YYSYMBOL_RESIDUE = 75,                   /* RESIDUE  */
  YYSYMBOL_TYPE = 76,                      /* TYPE  */
  YYSYMBOL_CONTAINS = 77,                  /* CONTAINS  */
  YYSYMBOL_CHAIN = 78,                     /* CHAIN  */
  YYSYMBOL_ELEMENT = 79,                   /* ELEMENT  */
  YYSYMBOL_SEGID = 80,                     /* SEGID  */
  YYSYMBOL_SET = 81,                       /* SET  */
  YYSYMBOL_PUSH = 82,                      /* PUSH  */
  YYSYMBOL_POP = 83,                       /* POP  */
  YYSYMBOL_ATOMCOLOUR = 84,                /* ATOMCOLOUR  */
  YYSYMBOL_ATOMRADIUS = 85,                /* ATOMRADIUS  */
  YYSYMBOL_BONDDISTANCE = 86,              /* BONDDISTANCE  */
  YYSYMBOL_BONDCROSS = 87,                 /* BONDCROSS  */
  YYSYMBOL_COILRADIUS = 88,                /* COILRADIUS  */
  YYSYMBOL_COLOURPARTS = 89,               /* COLOURPARTS  */
  YYSYMBOL_COLOURRAMP = 90,                /* COLOURRAMP  */
  YYSYMBOL_CYLINDERRADIUS = 91,            /* CYLINDERRADIUS  */
  YYSYMBOL_DEPTHCUE = 92,                  /* DEPTHCUE  */
  YYSYMBOL_EMISSIVECOLOUR = 93,            /* EMISSIVECOLOUR  */
  YYSYMBOL_HELIXTHICKNESS = 94,            /* HELIXTHICKNESS  */
  YYSYMBOL_HELIXWIDTH = 95,                /* HELIXWIDTH  */
  YYSYMBOL_HSBRAMPREVERSE = 96,            /* HSBRAMPREVERSE  */
  YYSYMBOL_LABELBACKGROUND = 97,           /* LABELBACKGROUND  */
  YYSYMBOL_LABELCENTRE = 98,               /* LABELCENTRE  */
  YYSYMBOL_LABELCLIP = 99,                 /* LABELCLIP  */
  YYSYMBOL_LABELFONT = 100,                /* LABELFONT  */
  YYSYMBOL_LABELMASK = 101,                /* LABELMASK  */
  YYSYMBOL_LABELOFFSET = 102,              /* LABELOFFSET  */
  YYSYMBOL_LABELROTATION = 103,            /* LABELROTATION  */
  YYSYMBOL_LABELSIZE = 104,                /* LABELSIZE  */
  YYSYMBOL_LIGHTAMBIENTINTENSITY = 105,    /* LIGHTAMBIENTINTENSITY  */
  YYSYMBOL_LIGHTATTENUATION = 106,         /* LIGHTATTENUATION  */
  YYSYMBOL_LIGHTCOLOUR = 107,              /* LIGHTCOLOUR  */
  YYSYMBOL_LIGHTINTENSITY = 108,           /* LIGHTINTENSITY  */
  YYSYMBOL_LIGHTRADIUS = 109,              /* LIGHTRADIUS  */
  YYSYMBOL_LINECOLOUR = 110,               /* LINECOLOUR  */
  YYSYMBOL_LINEDASH = 111,                 /* LINEDASH  */
  YYSYMBOL_LINEWIDTH = 112,                /* LINEWIDTH  */
  YYSYMBOL_OBJECTTRANSFORM = 113,          /* OBJECTTRANSFORM  */
  YYSYMBOL_PLANECOLOUR = 114,              /* PLANECOLOUR  */
  YYSYMBOL_PLANE2COLOUR = 115,             /* PLANE2COLOUR  */
  YYSYMBOL_REGULAREXPRESSION = 116,        /* REGULAREXPRESSION  */
  YYSYMBOL_RESIDUECOLOUR = 117,            /* RESIDUECOLOUR  */
  YYSYMBOL_SEGMENTS = 118,                 /* SEGMENTS  */
  YYSYMBOL_SEGMENTSIZE = 119,              /* SEGMENTSIZE  */
  YYSYMBOL_SHADING = 120,                  /* SHADING  */
  YYSYMBOL_SHADINGEXPONENT = 121,          /* SHADINGEXPONENT  */
  YYSYMBOL_SHININESS = 122,                /* SHININESS  */
  YYSYMBOL_SMOOTHSTEPS = 123,              /* SMOOTHSTEPS  */
  YYSYMBOL_SPECULARCOLOUR = 124,           /* SPECULARCOLOUR  */
  YYSYMBOL_SPLINEFACTOR = 125,             /* SPLINEFACTOR  */
  YYSYMBOL_STICKRADIUS = 126,              /* STICKRADIUS  */
  YYSYMBOL_STICKTAPER = 127,               /* STICKTAPER  */
  YYSYMBOL_STRANDTHICKNESS = 128,          /* STRANDTHICKNESS  */
  YYSYMBOL_STRANDWIDTH = 129,              /* STRANDWIDTH  */
  YYSYMBOL_TRANSPARENCY = 130,             /* TRANSPARENCY  */
  YYSYMBOL_BALL_AND_STICK = 131,           /* BALL_AND_STICK  */
  YYSYMBOL_BONDS = 132,                    /* BONDS  */
  YYSYMBOL_COIL = 133,                     /* COIL  */
  YYSYMBOL_CYLINDER = 134,                 /* CYLINDER  */
  YYSYMBOL_CPK = 135,                      /* CPK  */
  YYSYMBOL_HELIX = 136,                    /* HELIX  */
  YYSYMBOL_LABEL = 137,                    /* LABEL  */
  YYSYMBOL_LINE = 138,                     /* LINE  */
  YYSYMBOL_OBJECT = 139,                   /* OBJECT  */
  YYSYMBOL_INLINE = 140,                   /* INLINE  */
  YYSYMBOL_STRAND = 141,                   /* STRAND  */
  YYSYMBOL_TRACE = 142,                    /* TRACE  */
  YYSYMBOL_TURN = 143,                     /* TURN  */
  YYSYMBOL_DOUBLE_HELIX = 144,             /* DOUBLE_HELIX  */
  YYSYMBOL_RGB = 145,                      /* RGB  */
  YYSYMBOL_HSB = 146,                      /* HSB  */
  YYSYMBOL_GREY = 147,                     /* GREY  */
  YYSYMBOL_RAINBOW = 148,                  /* RAINBOW  */
  YYSYMBOL_BWRAMP = 149,                   /* BWRAMP  */
  YYSYMBOL_AFRAMP = 150,                   /* AFRAMP  */
  YYSYMBOL_AFGRADIENT = 151,               /* AFGRADIENT  */
  YYSYMBOL_LEGEND = 152,                   /* LEGEND  */
  YYSYMBOL_DISTANCE = 153,                 /* DISTANCE  */
  YYSYMBOL_ANGLE = 154,                    /* ANGLE  */
  YYSYMBOL_CONNECTOR = 155,                /* CONNECTOR  */
  YYSYMBOL_SCALE = 156,                    /* SCALE  */
  YYSYMBOL_CURVETOLERANCE = 157,           /* CURVETOLERANCE  */
  YYSYMBOL_COLOURINTERPOLATE = 158,        /* COLOURINTERPOLATE  */
  YYSYMBOL_VERTICAL = 159,                 /* VERTICAL  */
  YYSYMBOL_TICKS = 160,                    /* TICKS  */
  YYSYMBOL_KEY = 161,                      /* KEY  */
  YYSYMBOL_162_ = 162,                     /* ';'  */
  YYSYMBOL_163_ = 163,                     /* '{'  */
  YYSYMBOL_164_ = 164,                     /* '}'  */
  YYSYMBOL_165_ = 165,                     /* ','  */
  YYSYMBOL_YYACCEPT = 166,                 /* $accept  */
  YYSYMBOL_file_contents = 167,            /* file_contents  */
  YYSYMBOL_title = 168,                    /* title  */
  YYSYMBOL_plots = 169,                    /* plots  */
  YYSYMBOL_plot = 170,                     /* plot  */
  YYSYMBOL_171_1 = 171,                    /* $@1  */
  YYSYMBOL_plot_contents = 172,            /* plot_contents  */
  YYSYMBOL_macro_defs = 173,               /* macro_defs  */
  YYSYMBOL_macro_def = 174,                /* macro_def  */
  YYSYMBOL_header_commands = 175,          /* header_commands  */
  YYSYMBOL_header_command = 176,           /* header_command  */
  YYSYMBOL_body_commands = 177,            /* body_commands  */
  YYSYMBOL_body_command = 178,             /* body_command  */
  YYSYMBOL_coord_command = 179,            /* coord_command  */
  YYSYMBOL_180_2 = 180,                    /* $@2  */
  YYSYMBOL_181_3 = 181,                    /* $@3  */
  YYSYMBOL_182_4 = 182,                    /* $@4  */
  YYSYMBOL_183_5 = 183,                    /* $@5  */
  YYSYMBOL_coordinates = 184,              /* coordinates  */
  YYSYMBOL_185_6 = 185,                    /* $@6  */
  YYSYMBOL_xforms = 186,                   /* xforms  */
  YYSYMBOL_xform = 187,                    /* xform  */
  YYSYMBOL_geom_command = 188,             /* geom_command  */
  YYSYMBOL_189_7 = 189,                    /* $@7  */
  YYSYMBOL_190_8 = 190,                    /* $@8  */
  YYSYMBOL_191_9 = 191,                    /* $@9  */
  YYSYMBOL_192_10 = 192,                   /* $@10  */
  YYSYMBOL_193_11 = 193,                   /* $@11  */
  YYSYMBOL_194_12 = 194,                   /* $@12  */
  YYSYMBOL_195_13 = 195,                   /* $@13  */
  YYSYMBOL_196_14 = 196,                   /* $@14  */
  YYSYMBOL_197_15 = 197,                   /* $@15  */
  YYSYMBOL_198_16 = 198,                   /* $@16  */
  YYSYMBOL_199_17 = 199,                   /* $@17  */
  YYSYMBOL_200_18 = 200,                   /* $@18  */
  YYSYMBOL_distance_start = 201,           /* distance_start  */
  YYSYMBOL_measurement_vector = 202,       /* measurement_vector  */
  YYSYMBOL_key_layout = 203,               /* key_layout  */
  YYSYMBOL_key_ticks = 204,                /* key_ticks  */
  YYSYMBOL_205_19 = 205,                   /* $@19  */
  YYSYMBOL_tick_items = 206,               /* tick_items  */
  YYSYMBOL_207_20 = 207,                   /* $@20  */
  YYSYMBOL_key_items = 208,                /* key_items  */
  YYSYMBOL_209_21 = 209,                   /* $@21  */
  YYSYMBOL_lines = 210,                    /* lines  */
  YYSYMBOL_211_22 = 211,                   /* $@22  */
  YYSYMBOL_object = 212,                   /* object  */
  YYSYMBOL_213_23 = 213,                   /* $@23  */
  YYSYMBOL_state_command = 214,            /* state_command  */
  YYSYMBOL_215_24 = 215,                   /* $@24  */
  YYSYMBOL_216_25 = 216,                   /* $@25  */
  YYSYMBOL_217_26 = 217,                   /* $@26  */
  YYSYMBOL_state_changes = 218,            /* state_changes  */
  YYSYMBOL_state_change = 219,             /* state_change  */
  YYSYMBOL_utility_command = 220,          /* utility_command  */
  YYSYMBOL_221_27 = 221,                   /* $@27  */
  YYSYMBOL_222_28 = 222,                   /* $@28  */
  YYSYMBOL_ctrl_command = 223,             /* ctrl_command  */
  YYSYMBOL_224_29 = 224,                   /* $@29  */
  YYSYMBOL_225_30 = 225,                   /* $@30  */
  YYSYMBOL_226_31 = 226,                   /* $@31  */
  YYSYMBOL_227_32 = 227,                   /* $@32  */
  YYSYMBOL_228_33 = 228,                   /* $@33  */
  YYSYMBOL_229_34 = 229,                   /* $@34  */
  YYSYMBOL_anchor_description = 230,       /* anchor_description  */
  YYSYMBOL_anchor_parameters = 231,        /* anchor_parameters  */
  YYSYMBOL_anchor_parameter = 232,         /* anchor_parameter  */
  YYSYMBOL_basic_commands = 233,           /* basic_commands  */
  YYSYMBOL_basic_command = 234,            /* basic_command  */
  YYSYMBOL_lod_blocks = 235,               /* lod_blocks  */
  YYSYMBOL_lod_block = 236,                /* lod_block  */
  YYSYMBOL_237_35 = 237,                   /* $@35  */
  YYSYMBOL_lod_group = 238,                /* lod_group  */
  YYSYMBOL_view_definition = 239,          /* view_definition  */
  YYSYMBOL_atom_selection = 240,           /* atom_selection  */
  YYSYMBOL_atom_and = 241,                 /* atom_and  */
  YYSYMBOL_242_36 = 242,                   /* $@36  */
  YYSYMBOL_atom_or = 243,                  /* atom_or  */
  YYSYMBOL_244_37 = 244,                   /* $@37  */
  YYSYMBOL_atom_specification = 245,       /* atom_specification  */
  YYSYMBOL_246_38 = 246,                   /* $@38  */
  YYSYMBOL_residue_selection = 247,        /* residue_selection  */
  YYSYMBOL_residue_and = 248,              /* residue_and  */
  YYSYMBOL_249_39 = 249,                   /* $@39  */
  YYSYMBOL_residue_or = 250,               /* residue_or  */
  YYSYMBOL_251_40 = 251,                   /* $@40  */
  YYSYMBOL_residue_specification = 252,    /* residue_specification  */
  YYSYMBOL_253_41 = 253,                   /* $@41  */
  YYSYMBOL_vector = 254,                   /* vector  */
  YYSYMBOL_direction = 255,                /* direction  */
  YYSYMBOL_colour = 256,                   /* colour  */
  YYSYMBOL_ramp = 257,                     /* ramp  */
  YYSYMBOL_258_42 = 258,                   /* $@42  */
  YYSYMBOL_number_as_id = 259,             /* number_as_id  */
  YYSYMBOL_number = 260,                   /* number  */
  YYSYMBOL_id = 261                        /* id  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   965

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  166
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  96
/* YYNRULES -- Number of rules.  */
#define YYNRULES  280
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  573

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   416


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   165,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   162,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   163,     2,   164,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    69,    69,    71,    72,    75,    76,    79,    79,    82,
      83,    86,    87,    90,    92,    93,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   110,   111,
     114,   115,   116,   117,   118,   121,   121,   122,   122,   123,
     123,   125,   125,   127,   130,   130,   131,   134,   135,   138,
     139,   140,   141,   142,   143,   144,   147,   150,   151,   153,
     154,   155,   156,   157,   158,   159,   160,   160,   161,   161,
     162,   162,   163,   163,   164,   165,   166,   167,   168,   167,
     170,   171,   170,   173,   175,   174,   176,   176,   178,   179,
     178,   182,   183,   185,   186,   188,   188,   189,   191,   191,
     192,   194,   194,   195,   198,   199,   199,   202,   203,   203,
     206,   206,   207,   207,   208,   208,   211,   212,   215,   216,
     217,   218,   219,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   280,
     280,   281,   281,   282,   285,   286,   285,   288,   288,   289,
     288,   290,   290,   291,   292,   293,   294,   295,   298,   299,
     302,   303,   306,   308,   309,   312,   313,   314,   317,   318,
     321,   321,   323,   324,   327,   328,   329,   332,   333,   334,
     335,   338,   339,   339,   342,   343,   343,   346,   347,   347,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   360,
     361,   362,   363,   366,   367,   367,   370,   371,   371,   374,
     375,   376,   376,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   391,   392,   395,   397,   398,   399,   400,   403,
     403,   405,   406,   407,   408,   411,   412,   415,   416,   419,
     420
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INTEGER", "REAL",
  "STRING", "ITEM", "ON", "OFF", "POSITION", "TITLE", "MACRO", "PLOT",
  "END_PLOT", "COMMENT", "DEBUG", "POSTSCRIPT", "RASTER3D", "VRML",
  "NOFRAME", "FRAME", "AREA", "BACKGROUND", "WINDOW", "SLAB", "HEADLIGHT",
  "SHADOWS", "FOG", "READ", "INLINE_PDB", "DELETE", "COPY", "ANCHOR",
  "DESCRIPTION", "PARAMETER", "VIEWPOINT", "ORIGIN", "DIRECTIONALLIGHT",
  "POINTLIGHT", "SPOTLIGHT", "LEVEL_OF_DETAIL", "TRANSFORM", "BY",
  "CENTRE", "TRANSLATION", "ROTATION", "XAXIS", "YAXIS", "ZAXIS", "AXIS",
  "STORE_MATRIX", "RECALL_MATRIX", "NOT", "REQUIRE", "AND", "EITHER", "OR",
  "BACKBONE", "PEPTIDE", "HYDROGENS", "ATOM", "RES_ATOM", "B_FACTOR",
  "OCCUPANCY", "IN", "SPHERE", "CLOSE", "MODEL", "AMINO_ACIDS", "WATERS",
  "NUCLEOTIDES", "LIGANDS", "MOLECULE", "FROM", "TO", "RESIDUE", "TYPE",
  "CONTAINS", "CHAIN", "ELEMENT", "SEGID", "SET", "PUSH", "POP",
  "ATOMCOLOUR", "ATOMRADIUS", "BONDDISTANCE", "BONDCROSS", "COILRADIUS",
  "COLOURPARTS", "COLOURRAMP", "CYLINDERRADIUS", "DEPTHCUE",
  "EMISSIVECOLOUR", "HELIXTHICKNESS", "HELIXWIDTH", "HSBRAMPREVERSE",
  "LABELBACKGROUND", "LABELCENTRE", "LABELCLIP", "LABELFONT", "LABELMASK",
  "LABELOFFSET", "LABELROTATION", "LABELSIZE", "LIGHTAMBIENTINTENSITY",
  "LIGHTATTENUATION", "LIGHTCOLOUR", "LIGHTINTENSITY", "LIGHTRADIUS",
  "LINECOLOUR", "LINEDASH", "LINEWIDTH", "OBJECTTRANSFORM", "PLANECOLOUR",
  "PLANE2COLOUR", "REGULAREXPRESSION", "RESIDUECOLOUR", "SEGMENTS",
  "SEGMENTSIZE", "SHADING", "SHADINGEXPONENT", "SHININESS", "SMOOTHSTEPS",
  "SPECULARCOLOUR", "SPLINEFACTOR", "STICKRADIUS", "STICKTAPER",
  "STRANDTHICKNESS", "STRANDWIDTH", "TRANSPARENCY", "BALL_AND_STICK",
  "BONDS", "COIL", "CYLINDER", "CPK", "HELIX", "LABEL", "LINE", "OBJECT",
  "INLINE", "STRAND", "TRACE", "TURN", "DOUBLE_HELIX", "RGB", "HSB",
  "GREY", "RAINBOW", "BWRAMP", "AFRAMP", "AFGRADIENT", "LEGEND",
  "DISTANCE", "ANGLE", "CONNECTOR", "SCALE", "CURVETOLERANCE",
  "COLOURINTERPOLATE", "VERTICAL", "TICKS", "KEY", "';'", "'{'", "'}'",
  "','", "$accept", "file_contents", "title", "plots", "plot", "$@1",
  "plot_contents", "macro_defs", "macro_def", "header_commands",
  "header_command", "body_commands", "body_command", "coord_command",
  "$@2", "$@3", "$@4", "$@5", "coordinates", "$@6", "xforms", "xform",
  "geom_command", "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13",
  "$@14", "$@15", "$@16", "$@17", "$@18", "distance_start",
  "measurement_vector", "key_layout", "key_ticks", "$@19", "tick_items",
  "$@20", "key_items", "$@21", "lines", "$@22", "object", "$@23",
  "state_command", "$@24", "$@25", "$@26", "state_changes", "state_change",
  "utility_command", "$@27", "$@28", "ctrl_command", "$@29", "$@30",
  "$@31", "$@32", "$@33", "$@34", "anchor_description",
  "anchor_parameters", "anchor_parameter", "basic_commands",
  "basic_command", "lod_blocks", "lod_block", "$@35", "lod_group",
  "view_definition", "atom_selection", "atom_and", "$@36", "atom_or",
  "$@37", "atom_specification", "$@38", "residue_selection", "residue_and",
  "$@39", "residue_or", "$@40", "residue_specification", "$@41", "vector",
  "direction", "colour", "ramp", "$@42", "number_as_id", "number", "id", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-475)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-106)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      14,    57,    38,    37,  -475,  -475,  -475,  -475,    57,  -475,
      65,    59,    37,  -475,  -475,  -475,  -475,   634,    57,    57,
    -475,    81,   104,    17,   104,   104,   107,   112,   104,    57,
      57,    57,    57,    57,    33,    66,    66,  -475,   886,   -84,
    -475,  -475,  -475,   886,   886,   242,   242,   886,   242,   810,
      66,  -475,   242,   242,   242,   242,    57,  -475,  -475,  -475,
      57,    61,   -76,   668,   259,  -475,   668,  -475,  -475,    66,
    -475,  -475,  -475,  -475,  -475,   -70,   -61,  -475,  -475,   104,
     104,   104,   104,   -37,  -475,    25,    26,    32,    34,    39,
      40,    43,  -475,  -475,  -475,  -475,  -475,   886,    66,    46,
      48,   104,    49,    33,   104,   886,   886,   886,  -475,  -475,
    -475,    57,    57,   104,   104,   242,    66,   886,    57,  -475,
    -475,  -475,   806,    50,    51,   531,   677,   242,   242,   242,
     113,  -475,  -475,  -475,  -475,    57,   127,    57,    57,   886,
      57,    57,    52,  -475,    53,    56,    60,    57,    57,  -475,
      -2,    67,    68,    74,    82,  -475,    66,    66,  -475,  -475,
    -475,  -475,  -475,  -475,    66,  -475,    83,    84,  -475,  -475,
     104,   104,   104,  -475,  -475,  -475,  -475,  -475,  -475,  -475,
    -475,  -475,    35,    85,   886,   158,   -20,  -475,   121,  -475,
    -475,   104,  -475,   104,   104,  -475,   104,  -475,  -475,   -53,
     -47,  -475,  -475,   104,   104,  -475,   104,   104,  -475,   136,
     886,   886,   104,   104,   104,   116,    -4,   104,   104,    17,
     104,   104,   143,   104,   145,   147,    57,    57,    66,   152,
     104,   104,    66,    17,   104,   104,    17,   104,   104,   165,
      17,    17,   176,   242,   194,   104,   104,   104,   104,   224,
      17,   104,   104,   104,   104,   104,   104,   104,   178,    86,
      54,  -475,  -475,  -475,    88,  -475,    89,  -475,   -52,   -44,
    -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,
    -475,  -475,  -475,  -475,  -475,  -475,   154,    91,  -475,  -475,
    -475,  -475,  -475,  -475,    66,    66,    66,    66,  -110,  -475,
    -475,   104,   104,   104,    92,  -475,  -475,  -475,    93,    57,
     226,    66,    96,   104,    66,  -475,    99,   101,   102,  -475,
     102,   886,   886,  -475,   886,   886,  -475,    57,  -475,  -475,
    -475,  -475,     6,   105,    29,   104,  -475,  -475,  -475,  -475,
    -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,
    -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,
    -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,
    -475,  -475,  -475,  -475,  -475,    20,  -475,  -475,  -475,  -475,
    -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,
    -475,  -475,  -475,   806,  -475,  -475,   242,   242,  -475,   242,
     242,  -475,   198,   114,   128,    66,   129,  -475,   130,   104,
      66,    57,   104,   104,  -475,   131,  -475,  -475,  -475,   134,
    -475,  -475,    57,  -475,   226,   104,  -475,  -475,  -475,  -475,
    -475,   486,  -475,  -475,  -475,  -475,  -475,  -475,  -475,    66,
      66,    80,  -475,   136,  -475,   104,  -475,  -475,   104,    17,
    -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,
    -475,   127,  -475,  -475,   200,  -475,  -475,   104,   140,  -475,
     104,    57,  -475,  -475,  -475,   135,  -475,  -475,  -475,  -475,
    -475,  -475,   111,   699,   141,   -53,   -47,  -475,  -475,   104,
     104,   104,   104,   104,  -475,   104,   104,  -475,   -52,   -44,
    -475,   154,   104,  -475,   161,  -475,  -475,   699,  -475,  -475,
    -475,  -475,  -475,  -475,  -475,  -475,   104,   104,   -46,   -46,
     231,  -475,  -475,  -475,   104,  -475,   148,   162,   163,   104,
     104,  -475,  -475,    17,   -46,  -475,   153,  -475,   166,   104,
     104,  -475,  -475,    17,  -475,  -475,   104,   148,   168,    57,
     104,   169,   171,  -475,   104,  -475,   172,  -475,   173,   104,
     167,  -475,    17,  -475,   104,  -475,   174,    57,  -475,  -475,
     175,   104,  -475
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       4,     0,     0,    12,   279,   280,     3,     1,     0,     2,
       5,     0,    12,    13,     6,     7,    11,     0,     0,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   187,     0,     0,
     110,   112,   114,     0,     0,     0,     0,     0,     0,     0,
       0,    72,     0,     0,     0,     0,     0,    91,    86,    88,
       0,     0,     0,     0,    14,    10,    28,    30,    31,     0,
      32,    33,    34,   179,   181,     0,     0,   277,   278,     0,
       0,     0,     0,     0,   268,     0,     0,     0,     0,     0,
       0,     0,    35,    37,    39,   184,   191,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   236,   235,
     237,     0,     0,     0,     0,     0,     0,     0,     0,    41,
     220,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   257,   258,   259,   260,     0,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,    77,     0,     0,    80,     8,
     183,     9,    15,    29,     0,    92,     0,     0,    18,    17,
       0,     0,     0,   267,    20,    21,    22,    23,    24,    25,
      26,    27,     0,     0,     0,   199,     0,   262,     0,   193,
     194,     0,   195,     0,     0,   188,   208,   210,   217,     0,
       0,   227,   228,     0,     0,   232,     0,     0,   238,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,   113,   115,    57,     0,    59,     0,   239,     0,     0,
     250,   249,   251,   275,   276,   253,   254,   256,   255,   261,
      61,    62,    63,    65,    68,    66,     0,     0,    73,   108,
      74,    75,    76,    64,     0,     0,     0,     0,     0,   180,
     182,     0,     0,     0,     0,    36,    44,    38,     0,     0,
     201,     0,     0,   214,     0,   263,     0,     0,     0,   209,
       0,     0,     0,   218,     0,     0,   219,     0,   231,   230,
     233,   234,     0,     0,     0,     0,   124,   125,   126,   127,
     128,   130,   129,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   144,   143,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   121,
     119,   120,   111,     0,    58,    60,     0,     0,   240,     0,
       0,   241,     0,     0,     0,     0,     0,   107,     0,     0,
       0,     0,     0,     0,    83,     0,   265,   266,    46,     0,
      40,   198,     0,   185,   201,     0,   192,   215,   264,   196,
     197,     0,   189,   211,   221,   222,   224,   225,   229,     0,
       0,     0,    56,    47,    42,     0,   118,   123,     0,     0,
     271,   272,   273,   274,   163,   165,   117,   243,   244,   246,
     247,     0,    69,    67,   104,    71,   109,     0,     0,    89,
       0,     0,    19,    45,   202,     0,   200,   216,   212,   205,
     206,   207,     0,   203,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,    48,     0,     0,   269,     0,     0,
     252,     0,     0,    87,     0,    81,    84,     0,   213,   204,
     190,   223,   226,    51,    52,    53,     0,     0,     0,     0,
       0,   245,   248,   106,     0,    90,    94,     0,     0,     0,
       0,   122,   164,     0,     0,    93,     0,    85,     0,     0,
       0,   270,    78,   103,   186,    54,     0,    94,     0,     0,
       0,    97,     0,   101,     0,    95,     0,    82,     0,     0,
       0,    79,   103,    55,   100,   102,     0,     0,    96,    98,
       0,   100,    99
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -475,  -475,  -475,   315,  -475,  -475,  -475,   324,    44,   276,
    -475,    -5,  -475,  -475,  -475,  -475,  -475,  -475,  -475,  -475,
    -102,  -475,  -411,  -475,  -475,  -475,  -475,  -475,  -475,  -475,
    -475,  -475,  -475,  -475,  -475,  -475,  -151,  -201,  -475,  -475,
    -224,  -475,  -214,  -475,  -152,  -475,  -475,  -475,  -410,  -475,
    -475,  -475,   -42,  -475,  -398,  -475,  -475,  -475,  -475,  -475,
    -475,  -475,  -475,  -475,  -475,   -72,  -475,  -464,  -475,   157,
    -475,  -475,    41,  -475,   257,  -130,  -475,  -129,  -475,  -475,
    -475,   216,  -140,  -475,  -139,  -475,  -475,  -475,    45,   -88,
      16,  -474,  -475,   -96,   -14,    -1
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     2,     3,     9,    10,    17,    61,    11,    62,    63,
      64,    65,    66,    67,   182,   183,   184,   209,   305,   419,
     333,   443,    68,   404,   403,   286,   150,   294,   547,   297,
     526,   527,   156,   157,   504,    69,   164,   536,   556,   560,
     566,   570,   548,   558,   406,   501,   288,   408,    70,   122,
     123,   124,   259,   260,    71,   166,   167,    72,   185,   475,
     104,   318,   484,   186,   310,   423,   424,   482,   483,   195,
     196,   320,   432,   312,   119,   323,   485,   326,   486,   120,
     327,   142,   398,   498,   401,   499,   143,   402,   165,   100,
     549,   455,   520,   272,   101,    84
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       6,   321,   396,     4,     5,   295,   296,    13,    79,   324,
      85,    86,   399,   298,    91,   194,   311,    73,    74,   509,
     479,   480,     4,     5,     1,     4,     5,   449,    92,    93,
      94,    95,    96,   481,     4,     5,    77,    78,     7,    83,
       4,     5,    97,   528,   531,   532,   413,    12,     8,   439,
     440,   441,   414,    98,    12,   155,    12,   442,   161,   158,
     542,   163,     4,     5,   304,   170,   171,   172,   173,    77,
      78,    15,   479,   480,   159,    97,     8,   -12,   121,    99,
     102,   103,   448,    77,    78,   481,   160,   191,    75,    76,
     197,   445,   168,   449,   148,   149,   479,   480,   313,   203,
     204,   169,   450,   451,   452,   453,    98,    77,    78,   481,
     201,   202,   322,   397,    87,    88,   270,   208,   325,    89,
      90,   400,   273,   339,   340,   174,   489,   490,   491,   492,
      77,    78,     4,     5,   271,   274,   275,   276,   287,   278,
     279,   341,   342,   188,   410,   411,   284,   285,   193,   289,
     348,   349,   351,   352,   353,   354,   301,   302,   303,   358,
     359,   206,    80,    81,    82,    80,    81,    82,   450,   451,
     452,   453,   369,   370,    80,    81,    82,   315,   332,   316,
     317,   306,   197,   373,   374,   390,   391,   175,   176,   328,
     329,   309,   330,   331,   177,   314,   178,   376,   336,   337,
     338,   179,   180,   343,   344,   181,   346,   347,   189,   350,
     190,   192,   261,   262,   280,   281,   360,   361,   282,   393,
     364,   365,   283,   367,   368,   355,   356,   381,   405,   290,
     291,   377,   378,   379,   380,   345,   292,   383,   384,   385,
     386,   387,   388,   389,   293,   299,   300,   307,   392,   363,
     394,   395,   366,   407,   418,   420,   371,   372,   426,   468,
     422,   429,   144,   430,   146,   431,   382,   444,   151,   152,
     153,   154,   461,   357,  -105,   508,   462,   362,    20,    21,
      22,    23,    24,    25,    26,    27,    28,   415,   416,   417,
     463,   465,   466,   472,   127,   128,   473,   129,   507,   427,
     125,   126,   503,   510,   145,   533,   147,   535,   421,   130,
     131,   132,   133,   134,   135,   136,   543,   137,   138,   139,
     140,   447,   141,   525,   537,    14,   438,   538,   544,   555,
     564,   205,   552,   557,   561,   562,    16,   571,   568,   409,
     162,   494,   412,   267,   268,   269,   551,   572,   565,   523,
     446,   456,   476,   319,   187,   511,   425,   512,   521,   428,
     522,   433,   198,   199,   200,   500,     0,     0,     0,     0,
       0,     0,     0,     0,   207,     0,     0,     0,     0,     0,
       0,     0,   264,   266,     0,     0,     0,     0,     0,     0,
       0,   454,     0,     0,     0,   467,   277,     0,   470,   471,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     469,   477,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   474,     0,     0,     0,     0,     0,   493,     0,     0,
       0,   495,     0,     0,   496,     0,     0,     0,     0,     0,
       0,   308,     0,     0,     0,     0,     0,   273,     0,     0,
     464,     0,     0,   502,     0,     0,   505,     0,     0,   375,
     274,     0,     0,     0,     0,   497,     0,   334,   335,     0,
     506,     0,     0,     0,     0,   513,   514,   515,   516,   517,
       0,   518,   519,     0,   487,   488,     0,     0,   524,     0,
       0,     0,     0,     0,     0,     0,     0,     8,     0,     0,
      18,    19,   529,   530,     0,     0,     0,     0,     0,     0,
     534,     0,     0,     0,     0,   539,   540,     0,     0,     0,
       0,     0,     0,     0,     0,   545,   546,     0,     0,     0,
       0,     0,   550,     0,     0,     0,   554,     0,     0,     0,
     559,     0,     0,     0,     0,   563,     0,     0,   553,   541,
     567,     0,     0,     0,     0,     0,     0,   567,     0,     0,
       0,     0,     0,     0,     0,     0,   569,    40,    41,    42,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   105,   106,     0,   107,     0,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     118,     0,   457,   458,     0,   459,   460,    43,    44,    45,
      46,    47,    48,    49,    50,    51,     0,    52,    53,    54,
      55,     0,     0,     0,     0,     0,     0,     0,    56,    57,
      58,    59,     0,     0,     0,     8,     0,    60,    18,    19,
     478,     0,     0,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,     0,    30,    31,    32,     0,     0,    33,
       0,    34,    35,    36,    37,    38,     0,     0,     0,     8,
       0,     0,    18,    19,    39,     0,     0,     0,     0,     0,
       0,     0,     0,   263,     0,     0,    29,     0,    30,    31,
      32,     0,     0,    33,     0,    34,    35,    36,    37,    38,
       8,     0,     0,    18,    19,    40,    41,    42,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   105,
     106,     0,   107,     0,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,     0,     0,     0,     0,     0,    40,
      41,    42,     0,     0,     0,     0,   118,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    44,    45,    46,    47,
      48,    49,    50,    51,     0,    52,    53,    54,    55,     0,
      40,    41,    42,     0,     0,     0,    56,    57,    58,    59,
       0,     0,     0,     0,     0,    60,     0,     0,     0,    43,
      44,    45,    46,    47,    48,    49,    50,    51,     0,    52,
      53,    54,    55,    77,    78,     0,     0,     0,     0,    97,
      56,    57,    58,    59,     0,     0,     0,     0,     0,    60,
      43,    44,    45,    46,    47,    48,    49,    50,    51,   265,
      52,    53,    54,    55,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,     0,
      60,     0,   105,   106,     0,   107,     0,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   118,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,     0,   105,   106,
       0,   107,     0,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   257,   258,   118
};

static const yytype_int16 yycheck[] =
{
       1,    54,    54,     5,     6,   156,   157,     8,    22,    56,
      24,    25,    56,   164,    28,   103,    36,    18,    19,   483,
     431,   431,     5,     6,    10,     5,     6,    73,    29,    30,
      31,    32,    33,   431,     5,     6,     3,     4,     0,    23,
       5,     6,     9,   507,   518,   519,   156,     3,    11,    43,
      44,    45,   162,    73,    10,    56,    12,    51,    63,    60,
     534,    66,     5,     6,    29,    79,    80,    81,    82,     3,
       4,    12,   483,   483,    13,     9,    11,    12,   162,    34,
      35,    36,    62,     3,     4,   483,   162,   101,     7,     8,
     104,    62,   162,    73,    49,    50,   507,   507,   186,   113,
     114,   162,   148,   149,   150,   151,    73,     3,     4,   507,
     111,   112,   165,   165,     7,     8,     3,   118,   165,     7,
       8,   165,   136,     7,     8,   162,    46,    47,    48,    49,
       3,     4,     5,     6,   135,   136,   137,   138,   140,   140,
     141,   145,   146,    98,   295,   296,   147,   148,   103,   150,
       7,     8,     7,     8,     7,     8,   170,   171,   172,     7,
       8,   116,   145,   146,   147,   145,   146,   147,   148,   149,
     150,   151,     7,     8,   145,   146,   147,   191,    42,   193,
     194,   182,   196,     7,     8,     7,     8,   162,   162,   203,
     204,    33,   206,   207,   162,    74,   162,     3,   212,   213,
     214,   162,   162,   217,   218,   162,   220,   221,   162,   223,
     162,   162,   162,   162,   162,   162,   230,   231,   162,   165,
     234,   235,   162,   237,   238,   226,   227,     3,    74,   162,
     162,   245,   246,   247,   248,   219,   162,   251,   252,   253,
     254,   255,   256,   257,   162,   162,   162,   162,   162,   233,
     162,   162,   236,   162,   162,   162,   240,   241,   162,   410,
      34,   162,    46,   162,    48,   163,   250,   162,    52,    53,
      54,    55,    74,   228,    74,   164,   162,   232,    19,    20,
      21,    22,    23,    24,    25,    26,    27,   301,   302,   303,
     162,   162,   162,   162,    52,    53,   162,    55,   163,   313,
      43,    44,   162,   162,    47,    74,    49,   159,   309,    67,
      68,    69,    70,    71,    72,    73,   163,    75,    76,    77,
      78,   335,    80,   162,   162,    10,   327,   164,   162,   160,
     163,   115,   164,   162,   162,   162,    12,   162,   164,   294,
      64,   443,   297,   127,   128,   129,   547,   571,   562,   501,
     334,   393,   424,   196,    97,   485,   311,   486,   498,   314,
     499,   320,   105,   106,   107,   461,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   375,    -1,    -1,    -1,   409,   139,    -1,   412,   413,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     411,   425,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   422,    -1,    -1,    -1,    -1,    -1,   441,    -1,    -1,
      -1,   445,    -1,    -1,   448,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    -1,    -1,    -1,    -1,    -1,   461,    -1,    -1,
     405,    -1,    -1,   467,    -1,    -1,   470,    -1,    -1,   243,
     461,    -1,    -1,    -1,    -1,   449,    -1,   210,   211,    -1,
     471,    -1,    -1,    -1,    -1,   489,   490,   491,   492,   493,
      -1,   495,   496,    -1,   439,   440,    -1,    -1,   502,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,
      14,    15,   516,   517,    -1,    -1,    -1,    -1,    -1,    -1,
     524,    -1,    -1,    -1,    -1,   529,   530,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   539,   540,    -1,    -1,    -1,
      -1,    -1,   546,    -1,    -1,    -1,   550,    -1,    -1,    -1,
     554,    -1,    -1,    -1,    -1,   559,    -1,    -1,   549,   533,
     564,    -1,    -1,    -1,    -1,    -1,    -1,   571,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   567,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   321,   322,
      -1,   324,   325,    52,    53,    -1,    55,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,   396,   397,    -1,   399,   400,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   153,
     154,   155,    -1,    -1,    -1,    11,    -1,   161,    14,    15,
     164,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    30,    31,    32,    -1,    -1,    35,
      -1,    37,    38,    39,    40,    41,    -1,    -1,    -1,    11,
      -1,    -1,    14,    15,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,    -1,    -1,    28,    -1,    30,    31,
      32,    -1,    -1,    35,    -1,    37,    38,    39,    40,    41,
      11,    -1,    -1,    14,    15,    81,    82,    83,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      53,    -1,    55,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,   141,   142,   143,   144,    -1,
      81,    82,    83,    -1,    -1,    -1,   152,   153,   154,   155,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,   141,
     142,   143,   144,     3,     4,    -1,    -1,    -1,    -1,     9,
     152,   153,   154,   155,    -1,    -1,    -1,    -1,    -1,   161,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   162,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,   153,   154,   155,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    52,    53,    -1,    55,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,    52,    53,
      -1,    55,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,    79
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,    10,   167,   168,     5,     6,   261,     0,    11,   169,
     170,   173,   174,   261,   169,    12,   173,   171,    14,    15,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      30,    31,    32,    35,    37,    38,    39,    40,    41,    50,
      81,    82,    83,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   141,   142,   143,   144,   152,   153,   154,   155,
     161,   172,   174,   175,   176,   177,   178,   179,   188,   201,
     214,   220,   223,   261,   261,     7,     8,     3,     4,   260,
     145,   146,   147,   256,   261,   260,   260,     7,     8,     7,
       8,   260,   261,   261,   261,   261,   261,     9,    73,   254,
     255,   260,   254,   254,   226,    52,    53,    55,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    79,   240,
     245,   162,   215,   216,   217,   240,   240,    52,    53,    55,
      67,    68,    69,    70,    71,    72,    73,    75,    76,    77,
      78,    80,   247,   252,   247,   240,   247,   240,   254,   254,
     192,   247,   247,   247,   247,   261,   198,   199,   261,    13,
     162,   177,   175,   177,   202,   254,   221,   222,   162,   162,
     260,   260,   260,   260,   162,   162,   162,   162,   162,   162,
     162,   162,   180,   181,   182,   224,   229,   240,   254,   162,
     162,   260,   162,   254,   255,   235,   236,   260,   240,   240,
     240,   261,   261,   260,   260,   247,   254,   240,   261,   183,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   157,   158,   218,
     219,   162,   162,   162,   240,   162,   240,   247,   247,   247,
       3,   261,   259,   260,   261,   261,   261,   240,   261,   261,
     162,   162,   162,   162,   261,   261,   191,   140,   212,   261,
     162,   162,   162,   162,   193,   202,   202,   195,   202,   162,
     162,   260,   260,   260,    29,   184,   261,   162,   240,    33,
     230,    36,   239,   255,    74,   260,   260,   260,   227,   235,
     237,    54,   165,   241,    56,   165,   243,   246,   260,   260,
     260,   260,    42,   186,   240,   240,   260,   260,   260,     7,
       8,   145,   146,   260,   260,   256,   260,   260,     7,     8,
     260,     7,     8,     7,     8,   261,   261,   254,     7,     8,
     260,   260,   254,   256,   260,   260,   256,   260,   260,     7,
       8,   256,   256,     7,     8,   247,     3,   260,   260,   260,
     260,     3,   256,   260,   260,   260,   260,   260,   260,   260,
       7,     8,   162,   165,   162,   162,    54,   165,   248,    56,
     165,   250,   253,   190,   189,    74,   210,   162,   213,   254,
     202,   202,   254,   156,   162,   260,   260,   260,   162,   185,
     162,   261,    34,   231,   232,   254,   162,   260,   254,   162,
     162,   163,   238,   238,   240,   240,   240,   240,   261,    43,
      44,    45,    51,   187,   162,    62,   256,   260,    62,    73,
     148,   149,   150,   151,   256,   257,   218,   247,   247,   247,
     247,    74,   162,   162,   254,   162,   162,   260,   202,   261,
     260,   260,   162,   162,   261,   225,   231,   260,   164,   188,
     214,   220,   233,   234,   228,   242,   244,   254,   254,    46,
      47,    48,    49,   260,   186,   260,   260,   256,   249,   251,
     259,   211,   260,   162,   200,   260,   261,   163,   164,   233,
     162,   241,   243,   260,   260,   260,   260,   260,   260,   260,
     258,   248,   250,   210,   260,   162,   196,   197,   233,   260,
     260,   257,   257,    74,   260,   159,   203,   162,   164,   260,
     260,   256,   257,   163,   162,   260,   260,   194,   208,   256,
     260,   203,   164,   261,   260,   160,   204,   162,   209,   260,
     205,   162,   162,   260,   163,   208,   206,   260,   164,   261,
     207,   162,   206
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   166,   167,   168,   168,   169,   169,   171,   170,   172,
     172,   173,   173,   174,   175,   175,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   177,   177,
     178,   178,   178,   178,   178,   180,   179,   181,   179,   182,
     179,   183,   179,   179,   185,   184,   184,   186,   186,   187,
     187,   187,   187,   187,   187,   187,   187,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   189,   188,   190,   188,
     191,   188,   192,   188,   188,   188,   188,   193,   194,   188,
     195,   196,   188,   188,   197,   188,   198,   188,   199,   200,
     188,   201,   202,   203,   203,   205,   204,   204,   207,   206,
     206,   209,   208,   208,   210,   211,   210,   212,   213,   212,
     215,   214,   216,   214,   217,   214,   218,   218,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   221,
     220,   222,   220,   220,   224,   225,   223,   226,   227,   228,
     223,   229,   223,   223,   223,   223,   223,   223,   230,   230,
     231,   231,   232,   233,   233,   234,   234,   234,   235,   235,
     237,   236,   238,   238,   239,   239,   239,   240,   240,   240,
     240,   241,   242,   241,   243,   244,   243,   245,   246,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   247,
     247,   247,   247,   248,   249,   248,   250,   251,   250,   252,
     252,   253,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   254,   254,   255,   256,   256,   256,   256,   258,
     257,   257,   257,   257,   257,   259,   259,   260,   260,   261,
     261
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     0,     1,     2,     0,     5,     2,
       1,     2,     0,     2,     1,     2,     1,     3,     3,     6,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     2,
       1,     1,     1,     1,     1,     0,     4,     0,     4,     0,
       5,     0,     5,     2,     0,     3,     2,     2,     3,     2,
       2,     3,     3,     3,     6,    10,     1,     3,     4,     3,
       4,     3,     3,     3,     3,     3,     0,     5,     0,     5,
       0,     5,     0,     3,     3,     3,     3,     0,     0,    13,
       0,     0,    12,     4,     0,     8,     0,     6,     0,     0,
       7,     1,     1,     1,     0,     0,     5,     0,     0,     5,
       0,     0,     5,     0,     2,     0,     4,     2,     0,     3,
       0,     4,     0,     3,     0,     3,     1,     3,     3,     2,
       2,     2,     6,     3,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     6,     3,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     0,
       4,     0,     4,     2,     0,     0,    10,     0,     0,     0,
       7,     0,     5,     3,     3,     3,     5,     5,     2,     0,
       2,     0,     2,     1,     2,     1,     1,     1,     1,     2,
       0,     3,     2,     3,     1,     2,     3,     2,     3,     3,
       1,     2,     0,     4,     2,     0,     4,     2,     0,     4,
       3,     3,     2,     3,     3,     1,     1,     1,     2,     2,
       3,     3,     1,     2,     0,     4,     2,     0,     4,     2,
       2,     0,     5,     2,     2,     2,     2,     1,     1,     1,
       1,     2,     2,     3,     4,     4,     4,     2,     1,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 3: /* title: TITLE id  */
#line 71 "molscript.y"
                 { set_title (yytext); }
#line 1868 "molscript.tab.c"
    break;

  case 7: /* $@1: %empty  */
#line 79 "molscript.y"
                       { start_plot(); }
#line 1874 "molscript.tab.c"
    break;

  case 8: /* plot: macro_defs PLOT $@1 plot_contents END_PLOT  */
#line 80 "molscript.y"
                                { view_export(); output_finish_plot(); }
#line 1880 "molscript.tab.c"
    break;

  case 13: /* macro_def: MACRO id  */
#line 90 "molscript.y"
                     { lex_define_macro (yytext); }
#line 1886 "molscript.tab.c"
    break;

  case 16: /* header_command: NOFRAME  */
#line 96 "molscript.y"
                                       { frame = FALSE; }
#line 1892 "molscript.tab.c"
    break;

  case 17: /* header_command: FRAME OFF ';'  */
#line 97 "molscript.y"
                                       { frame = FALSE; }
#line 1898 "molscript.tab.c"
    break;

  case 18: /* header_command: FRAME ON ';'  */
#line 98 "molscript.y"
                                       { frame = TRUE; }
#line 1904 "molscript.tab.c"
    break;

  case 19: /* header_command: AREA number number number number ';'  */
#line 99 "molscript.y"
                                                      { set_area(); }
#line 1910 "molscript.tab.c"
    break;

  case 20: /* header_command: BACKGROUND colour ';'  */
#line 100 "molscript.y"
                                       { set_background(); }
#line 1916 "molscript.tab.c"
    break;

  case 21: /* header_command: WINDOW number ';'  */
#line 101 "molscript.y"
                                       { set_window(); }
#line 1922 "molscript.tab.c"
    break;

  case 22: /* header_command: SLAB number ';'  */
#line 102 "molscript.y"
                                       { set_slab(); }
#line 1928 "molscript.tab.c"
    break;

  case 23: /* header_command: HEADLIGHT ON ';'  */
#line 103 "molscript.y"
                                       { headlight = TRUE; }
#line 1934 "molscript.tab.c"
    break;

  case 24: /* header_command: HEADLIGHT OFF ';'  */
#line 104 "molscript.y"
                                       { headlight = FALSE; }
#line 1940 "molscript.tab.c"
    break;

  case 25: /* header_command: SHADOWS ON ';'  */
#line 105 "molscript.y"
                                       { shadows = TRUE; }
#line 1946 "molscript.tab.c"
    break;

  case 26: /* header_command: SHADOWS OFF ';'  */
#line 106 "molscript.y"
                                       { shadows = FALSE; }
#line 1952 "molscript.tab.c"
    break;

  case 27: /* header_command: FOG number ';'  */
#line 107 "molscript.y"
                                       { set_fog(); }
#line 1958 "molscript.tab.c"
    break;

  case 35: /* $@2: %empty  */
#line 121 "molscript.y"
                                 { store_molname (yytext); }
#line 1964 "molscript.tab.c"
    break;

  case 37: /* $@3: %empty  */
#line 122 "molscript.y"
                                 { delete_molecule (yytext); }
#line 1970 "molscript.tab.c"
    break;

  case 39: /* $@4: %empty  */
#line 123 "molscript.y"
                        { lex_yytext_push(); }
#line 1976 "molscript.tab.c"
    break;

  case 40: /* coord_command: COPY id $@4 atom_selection ';'  */
#line 124 "molscript.y"
                  { lex_yytext_pop(); copy_molecule (yytext); }
#line 1982 "molscript.tab.c"
    break;

  case 41: /* $@5: %empty  */
#line 125 "molscript.y"
                                         { xform_init(); }
#line 1988 "molscript.tab.c"
    break;

  case 42: /* coord_command: TRANSFORM atom_selection $@5 xforms ';'  */
#line 126 "molscript.y"
                             { xform_atoms(); }
#line 1994 "molscript.tab.c"
    break;

  case 43: /* coord_command: STORE_MATRIX ';'  */
#line 127 "molscript.y"
                                 { xform_store(); }
#line 2000 "molscript.tab.c"
    break;

  case 44: /* $@6: %empty  */
#line 130 "molscript.y"
                 { read_coordinate_file (yytext); }
#line 2006 "molscript.tab.c"
    break;

  case 46: /* coordinates: INLINE_PDB ';'  */
#line 131 "molscript.y"
                             { read_coordinate_file (NULL); }
#line 2012 "molscript.tab.c"
    break;

  case 49: /* xform: CENTRE vector  */
#line 138 "molscript.y"
                                      { xform_centre(); }
#line 2018 "molscript.tab.c"
    break;

  case 50: /* xform: TRANSLATION vector  */
#line 139 "molscript.y"
                                      { xform_translation(); }
#line 2024 "molscript.tab.c"
    break;

  case 51: /* xform: ROTATION XAXIS number  */
#line 140 "molscript.y"
                                      { xform_rotation_x(); }
#line 2030 "molscript.tab.c"
    break;

  case 52: /* xform: ROTATION YAXIS number  */
#line 141 "molscript.y"
                                      { xform_rotation_y(); }
#line 2036 "molscript.tab.c"
    break;

  case 53: /* xform: ROTATION ZAXIS number  */
#line 142 "molscript.y"
                                      { xform_rotation_z(); }
#line 2042 "molscript.tab.c"
    break;

  case 54: /* xform: ROTATION AXIS number number number number  */
#line 143 "molscript.y"
                                                  { xform_rotation_axis(); }
#line 2048 "molscript.tab.c"
    break;

  case 55: /* xform: ROTATION number number number number number number number number number  */
#line 146 "molscript.y"
                                      { xform_rotation_matrix(); }
#line 2054 "molscript.tab.c"
    break;

  case 56: /* xform: RECALL_MATRIX  */
#line 147 "molscript.y"
                                      { xform_recall_matrix(); }
#line 2060 "molscript.tab.c"
    break;

  case 57: /* geom_command: BALL_AND_STICK atom_selection ';'  */
#line 150 "molscript.y"
                                                   { drawing_begin ("ball-and-stick"); ball_and_stick (TRUE); }
#line 2066 "molscript.tab.c"
    break;

  case 58: /* geom_command: BALL_AND_STICK atom_selection atom_selection ';'  */
#line 152 "molscript.y"
                                                   { drawing_begin ("ball-and-stick"); ball_and_stick (FALSE); }
#line 2072 "molscript.tab.c"
    break;

  case 59: /* geom_command: BONDS atom_selection ';'  */
#line 153 "molscript.y"
                                                   { drawing_begin ("bonds"); bonds (TRUE); }
#line 2078 "molscript.tab.c"
    break;

  case 60: /* geom_command: BONDS atom_selection atom_selection ';'  */
#line 154 "molscript.y"
                                                       { drawing_begin ("bonds"); bonds (FALSE); }
#line 2084 "molscript.tab.c"
    break;

  case 61: /* geom_command: COIL residue_selection ';'  */
#line 155 "molscript.y"
                                                   { drawing_begin ("coil"); coil (TRUE, TRUE); }
#line 2090 "molscript.tab.c"
    break;

  case 62: /* geom_command: CYLINDER residue_selection ';'  */
#line 156 "molscript.y"
                                                   { drawing_begin ("cylinder"); cylinder(); }
#line 2096 "molscript.tab.c"
    break;

  case 63: /* geom_command: CPK atom_selection ';'  */
#line 157 "molscript.y"
                                                   { drawing_begin ("cpk"); cpk(); }
#line 2102 "molscript.tab.c"
    break;

  case 64: /* geom_command: DOUBLE_HELIX residue_selection ';'  */
#line 158 "molscript.y"
                                                   { drawing_begin ("double-helix"); coil (FALSE, FALSE); }
#line 2108 "molscript.tab.c"
    break;

  case 65: /* geom_command: HELIX residue_selection ';'  */
#line 159 "molscript.y"
                                                   { drawing_begin ("helix"); helix(); }
#line 2114 "molscript.tab.c"
    break;

  case 66: /* $@7: %empty  */
#line 160 "molscript.y"
                               { drawing_begin ("label"); label_position (yytext); }
#line 2120 "molscript.tab.c"
    break;

  case 68: /* $@8: %empty  */
#line 161 "molscript.y"
                                       { drawing_begin ("label"); label_atoms (yytext); }
#line 2126 "molscript.tab.c"
    break;

  case 70: /* $@9: %empty  */
#line 162 "molscript.y"
                           { drawing_begin ("line"); line_start(); }
#line 2132 "molscript.tab.c"
    break;

  case 71: /* geom_command: LINE vector $@9 lines ';'  */
#line 162 "molscript.y"
                                                                               { output_line (TRUE); }
#line 2138 "molscript.tab.c"
    break;

  case 72: /* $@10: %empty  */
#line 163 "molscript.y"
                      { drawing_begin ("object"); }
#line 2144 "molscript.tab.c"
    break;

  case 74: /* geom_command: STRAND residue_selection ';'  */
#line 164 "molscript.y"
                                                   { drawing_begin ("strand"); strand(); }
#line 2150 "molscript.tab.c"
    break;

  case 75: /* geom_command: TRACE residue_selection ';'  */
#line 165 "molscript.y"
                                                   { drawing_begin ("trace"); trace(); }
#line 2156 "molscript.tab.c"
    break;

  case 76: /* geom_command: TURN residue_selection ';'  */
#line 166 "molscript.y"
                                                   { drawing_begin ("turn"); coil (TRUE, FALSE); }
#line 2162 "molscript.tab.c"
    break;

  case 77: /* $@11: %empty  */
#line 167 "molscript.y"
                         { lex_yytext_push(); }
#line 2168 "molscript.tab.c"
    break;

  case 78: /* $@12: %empty  */
#line 168 "molscript.y"
                 { lex_yytext_pop(); figure_key_begin (yytext, FALSE); }
#line 2174 "molscript.tab.c"
    break;

  case 79: /* geom_command: LEGEND id $@11 vector number number number number ramp $@12 key_layout key_ticks ';'  */
#line 169 "molscript.y"
                                          { drawing_begin ("legend"); figure_key_finish(); }
#line 2180 "molscript.tab.c"
    break;

  case 80: /* $@13: %empty  */
#line 170 "molscript.y"
                      { lex_yytext_push(); }
#line 2186 "molscript.tab.c"
    break;

  case 81: /* $@14: %empty  */
#line 171 "molscript.y"
                 { lex_yytext_pop(); figure_key_begin (yytext, TRUE); }
#line 2192 "molscript.tab.c"
    break;

  case 82: /* geom_command: KEY id $@13 vector number number $@14 key_layout '{' key_items '}' ';'  */
#line 172 "molscript.y"
                                                  { drawing_begin ("key"); figure_key_finish(); }
#line 2198 "molscript.tab.c"
    break;

  case 83: /* geom_command: distance_start measurement_vector measurement_vector ';'  */
#line 173 "molscript.y"
                                                                        { drawing_begin ("distance"); measurement (0, NULL, 1); }
#line 2204 "molscript.tab.c"
    break;

  case 84: /* $@15: %empty  */
#line 175 "molscript.y"
                 { drawing_begin ("distance"); measurement (0, yytext, 1); }
#line 2210 "molscript.tab.c"
    break;

  case 86: /* $@16: %empty  */
#line 176 "molscript.y"
                     { measurement_begin(); }
#line 2216 "molscript.tab.c"
    break;

  case 87: /* geom_command: ANGLE $@16 measurement_vector measurement_vector measurement_vector ';'  */
#line 177 "molscript.y"
                 { drawing_begin ("angle"); measurement (1, NULL, 1); }
#line 2222 "molscript.tab.c"
    break;

  case 88: /* $@17: %empty  */
#line 178 "molscript.y"
                         { measurement_begin(); }
#line 2228 "molscript.tab.c"
    break;

  case 89: /* $@18: %empty  */
#line 179 "molscript.y"
                 { drawing_begin ("connector"); measurement (2, yytext, 1); }
#line 2234 "molscript.tab.c"
    break;

  case 91: /* distance_start: DISTANCE  */
#line 182 "molscript.y"
                          { measurement_begin(); }
#line 2240 "molscript.tab.c"
    break;

  case 92: /* measurement_vector: vector  */
#line 183 "molscript.y"
                            { measurement_point(); }
#line 2246 "molscript.tab.c"
    break;

  case 93: /* key_layout: VERTICAL  */
#line 185 "molscript.y"
                      { legend_config.vertical = TRUE; }
#line 2252 "molscript.tab.c"
    break;

  case 95: /* $@19: %empty  */
#line 188 "molscript.y"
                  { legend_config.custom_ticks = TRUE; }
#line 2258 "molscript.tab.c"
    break;

  case 98: /* $@20: %empty  */
#line 191 "molscript.y"
                       { figure_key_item (yytext); }
#line 2264 "molscript.tab.c"
    break;

  case 101: /* $@21: %empty  */
#line 194 "molscript.y"
                      { figure_key_item (yytext); }
#line 2270 "molscript.tab.c"
    break;

  case 104: /* lines: TO vector  */
#line 198 "molscript.y"
                  { line_next(); }
#line 2276 "molscript.tab.c"
    break;

  case 105: /* $@22: %empty  */
#line 199 "molscript.y"
                  { line_next(); }
#line 2282 "molscript.tab.c"
    break;

  case 107: /* object: INLINE ';'  */
#line 202 "molscript.y"
                                    { object (NULL); }
#line 2288 "molscript.tab.c"
    break;

  case 108: /* $@23: %empty  */
#line 203 "molscript.y"
            { lex_yytext_push(); }
#line 2294 "molscript.tab.c"
    break;

  case 109: /* object: id $@23 ';'  */
#line 203 "molscript.y"
                                       { lex_yytext_pop(); object (yytext); }
#line 2300 "molscript.tab.c"
    break;

  case 110: /* $@24: %empty  */
#line 206 "molscript.y"
                      { new_state(); }
#line 2306 "molscript.tab.c"
    break;

  case 112: /* $@25: %empty  */
#line 207 "molscript.y"
                      { push_state(); }
#line 2312 "molscript.tab.c"
    break;

  case 114: /* $@26: %empty  */
#line 208 "molscript.y"
                      { pop_state(); }
#line 2318 "molscript.tab.c"
    break;

  case 118: /* state_change: ATOMCOLOUR atom_selection colour  */
#line 215 "molscript.y"
                                                { set_atomcolour(); }
#line 2324 "molscript.tab.c"
    break;

  case 119: /* state_change: COLOURINTERPOLATE ON  */
#line 216 "molscript.y"
                                    { current_state->colourinterpolate = TRUE; }
#line 2330 "molscript.tab.c"
    break;

  case 120: /* state_change: COLOURINTERPOLATE OFF  */
#line 217 "molscript.y"
                                     { current_state->colourinterpolate = FALSE; }
#line 2336 "molscript.tab.c"
    break;

  case 121: /* state_change: CURVETOLERANCE number  */
#line 218 "molscript.y"
                                     { set_curvetolerance(); }
#line 2342 "molscript.tab.c"
    break;

  case 122: /* state_change: ATOMCOLOUR atom_selection B_FACTOR number number ramp  */
#line 220 "molscript.y"
                                                { set_atomcolour_bfactor(); }
#line 2348 "molscript.tab.c"
    break;

  case 123: /* state_change: ATOMRADIUS atom_selection number  */
#line 221 "molscript.y"
                                                { set_atomradius(); }
#line 2354 "molscript.tab.c"
    break;

  case 124: /* state_change: BONDDISTANCE number  */
#line 222 "molscript.y"
                                                { set_bonddistance(); }
#line 2360 "molscript.tab.c"
    break;

  case 125: /* state_change: BONDCROSS number  */
#line 223 "molscript.y"
                                                { set_bondcross(); }
#line 2366 "molscript.tab.c"
    break;

  case 126: /* state_change: COILRADIUS number  */
#line 224 "molscript.y"
                                                { set_coilradius(); }
#line 2372 "molscript.tab.c"
    break;

  case 127: /* state_change: COLOURPARTS ON  */
#line 225 "molscript.y"
                                                { set_colourparts (TRUE); }
#line 2378 "molscript.tab.c"
    break;

  case 128: /* state_change: COLOURPARTS OFF  */
#line 226 "molscript.y"
                                                { set_colourparts (FALSE); }
#line 2384 "molscript.tab.c"
    break;

  case 129: /* state_change: COLOURRAMP HSB  */
#line 227 "molscript.y"
                                                { set_colourramphsb (TRUE); }
#line 2390 "molscript.tab.c"
    break;

  case 130: /* state_change: COLOURRAMP RGB  */
#line 228 "molscript.y"
                                                { set_colourramphsb (FALSE); }
#line 2396 "molscript.tab.c"
    break;

  case 131: /* state_change: CYLINDERRADIUS number  */
#line 229 "molscript.y"
                                                { set_cylinderradius(); }
#line 2402 "molscript.tab.c"
    break;

  case 132: /* state_change: DEPTHCUE number  */
#line 230 "molscript.y"
                                                { set_depthcue(); }
#line 2408 "molscript.tab.c"
    break;

  case 133: /* state_change: EMISSIVECOLOUR colour  */
#line 231 "molscript.y"
                                                { set_emissivecolour(); }
#line 2414 "molscript.tab.c"
    break;

  case 134: /* state_change: HELIXTHICKNESS number  */
#line 232 "molscript.y"
                                                { set_helixthickness(); }
#line 2420 "molscript.tab.c"
    break;

  case 135: /* state_change: HELIXWIDTH number  */
#line 233 "molscript.y"
                                                { set_helixwidth(); }
#line 2426 "molscript.tab.c"
    break;

  case 136: /* state_change: HSBRAMPREVERSE ON  */
#line 234 "molscript.y"
                                                { set_hsbrampreverse (TRUE); }
#line 2432 "molscript.tab.c"
    break;

  case 137: /* state_change: HSBRAMPREVERSE OFF  */
#line 235 "molscript.y"
                                                { set_hsbrampreverse (FALSE); }
#line 2438 "molscript.tab.c"
    break;

  case 138: /* state_change: LABELBACKGROUND number  */
#line 236 "molscript.y"
                                                { set_labelbackground(); }
#line 2444 "molscript.tab.c"
    break;

  case 139: /* state_change: LABELCENTRE ON  */
#line 237 "molscript.y"
                                                { set_labelcentre (TRUE); }
#line 2450 "molscript.tab.c"
    break;

  case 140: /* state_change: LABELCENTRE OFF  */
#line 238 "molscript.y"
                                                { set_labelcentre (FALSE); }
#line 2456 "molscript.tab.c"
    break;

  case 141: /* state_change: LABELCLIP ON  */
#line 239 "molscript.y"
                                                { set_labelclip (TRUE); }
#line 2462 "molscript.tab.c"
    break;

  case 142: /* state_change: LABELCLIP OFF  */
#line 240 "molscript.y"
                                                { set_labelclip (FALSE); }
#line 2468 "molscript.tab.c"
    break;

  case 143: /* state_change: LABELMASK id  */
#line 241 "molscript.y"
                                                { set_labelmask (yytext); }
#line 2474 "molscript.tab.c"
    break;

  case 144: /* state_change: LABELFONT id  */
#line 242 "molscript.y"
                                                { set_labelfont (yytext); }
#line 2480 "molscript.tab.c"
    break;

  case 145: /* state_change: LABELOFFSET vector  */
#line 243 "molscript.y"
                                                { set_labeloffset(); }
#line 2486 "molscript.tab.c"
    break;

  case 146: /* state_change: LABELROTATION ON  */
#line 244 "molscript.y"
                                                { set_labelrotation (TRUE); }
#line 2492 "molscript.tab.c"
    break;

  case 147: /* state_change: LABELROTATION OFF  */
#line 245 "molscript.y"
                                                { set_labelrotation (FALSE); }
#line 2498 "molscript.tab.c"
    break;

  case 148: /* state_change: LABELSIZE number  */
#line 246 "molscript.y"
                                                { set_labelsize(); }
#line 2504 "molscript.tab.c"
    break;

  case 149: /* state_change: LIGHTAMBIENTINTENSITY number  */
#line 247 "molscript.y"
                                                { set_lightambientintensity (); }
#line 2510 "molscript.tab.c"
    break;

  case 150: /* state_change: LIGHTATTENUATION vector  */
#line 248 "molscript.y"
                                                { set_lightattenuation(); }
#line 2516 "molscript.tab.c"
    break;

  case 151: /* state_change: LIGHTCOLOUR colour  */
#line 249 "molscript.y"
                                                { set_lightcolour(); }
#line 2522 "molscript.tab.c"
    break;

  case 152: /* state_change: LIGHTINTENSITY number  */
#line 250 "molscript.y"
                                                { set_lightintensity (); }
#line 2528 "molscript.tab.c"
    break;

  case 153: /* state_change: LIGHTRADIUS number  */
#line 251 "molscript.y"
                                                { set_lightradius (); }
#line 2534 "molscript.tab.c"
    break;

  case 154: /* state_change: LINECOLOUR colour  */
#line 252 "molscript.y"
                                                { set_linecolour(); }
#line 2540 "molscript.tab.c"
    break;

  case 155: /* state_change: LINEDASH number  */
#line 253 "molscript.y"
                                                { set_linedash(); }
#line 2546 "molscript.tab.c"
    break;

  case 156: /* state_change: LINEWIDTH number  */
#line 254 "molscript.y"
                                                { set_linewidth(); }
#line 2552 "molscript.tab.c"
    break;

  case 157: /* state_change: OBJECTTRANSFORM ON  */
#line 255 "molscript.y"
                                                { set_objecttransform (TRUE); }
#line 2558 "molscript.tab.c"
    break;

  case 158: /* state_change: OBJECTTRANSFORM OFF  */
#line 256 "molscript.y"
                                                { set_objecttransform (FALSE); }
#line 2564 "molscript.tab.c"
    break;

  case 159: /* state_change: PLANECOLOUR colour  */
#line 257 "molscript.y"
                                                { set_planecolour(); }
#line 2570 "molscript.tab.c"
    break;

  case 160: /* state_change: PLANE2COLOUR colour  */
#line 258 "molscript.y"
                                                { set_plane2colour(); }
#line 2576 "molscript.tab.c"
    break;

  case 161: /* state_change: REGULAREXPRESSION ON  */
#line 259 "molscript.y"
                                                { set_regularexpression (TRUE); }
#line 2582 "molscript.tab.c"
    break;

  case 162: /* state_change: REGULAREXPRESSION OFF  */
#line 260 "molscript.y"
                                                { set_regularexpression (FALSE); }
#line 2588 "molscript.tab.c"
    break;

  case 163: /* state_change: RESIDUECOLOUR residue_selection colour  */
#line 261 "molscript.y"
                                                      { set_residuecolour(); }
#line 2594 "molscript.tab.c"
    break;

  case 164: /* state_change: RESIDUECOLOUR residue_selection B_FACTOR number number ramp  */
#line 263 "molscript.y"
                                                { set_residuecolour_bfactor(); }
#line 2600 "molscript.tab.c"
    break;

  case 165: /* state_change: RESIDUECOLOUR residue_selection ramp  */
#line 264 "molscript.y"
                                                    { set_residuecolour_seq(); }
#line 2606 "molscript.tab.c"
    break;

  case 166: /* state_change: SEGMENTS INTEGER  */
#line 265 "molscript.y"
                                                { set_segments(); }
#line 2612 "molscript.tab.c"
    break;

  case 167: /* state_change: SEGMENTSIZE number  */
#line 266 "molscript.y"
                                                { set_segmentsize(); }
#line 2618 "molscript.tab.c"
    break;

  case 168: /* state_change: SHADING number  */
#line 267 "molscript.y"
                                                { set_shading(); }
#line 2624 "molscript.tab.c"
    break;

  case 169: /* state_change: SHADINGEXPONENT number  */
#line 268 "molscript.y"
                                                { set_shadingexponent(); }
#line 2630 "molscript.tab.c"
    break;

  case 170: /* state_change: SHININESS number  */
#line 269 "molscript.y"
                                                { set_shininess(); }
#line 2636 "molscript.tab.c"
    break;

  case 171: /* state_change: SMOOTHSTEPS INTEGER  */
#line 270 "molscript.y"
                                                { set_smoothsteps(); }
#line 2642 "molscript.tab.c"
    break;

  case 172: /* state_change: SPECULARCOLOUR colour  */
#line 271 "molscript.y"
                                                { set_specularcolour(); }
#line 2648 "molscript.tab.c"
    break;

  case 173: /* state_change: SPLINEFACTOR number  */
#line 272 "molscript.y"
                                                { set_splinefactor(); }
#line 2654 "molscript.tab.c"
    break;

  case 174: /* state_change: STICKRADIUS number  */
#line 273 "molscript.y"
                                                { set_stickradius(); }
#line 2660 "molscript.tab.c"
    break;

  case 175: /* state_change: STICKTAPER number  */
#line 274 "molscript.y"
                                                { set_sticktaper(); }
#line 2666 "molscript.tab.c"
    break;

  case 176: /* state_change: STRANDTHICKNESS number  */
#line 275 "molscript.y"
                                                { set_strandthickness(); }
#line 2672 "molscript.tab.c"
    break;

  case 177: /* state_change: STRANDWIDTH number  */
#line 276 "molscript.y"
                                                { set_strandwidth(); }
#line 2678 "molscript.tab.c"
    break;

  case 178: /* state_change: TRANSPARENCY number  */
#line 277 "molscript.y"
                                                { set_transparency(); }
#line 2684 "molscript.tab.c"
    break;

  case 179: /* $@27: %empty  */
#line 280 "molscript.y"
                                { output_comment (yytext); }
#line 2690 "molscript.tab.c"
    break;

  case 181: /* $@28: %empty  */
#line 281 "molscript.y"
                                { debug (yytext); }
#line 2696 "molscript.tab.c"
    break;

  case 184: /* $@29: %empty  */
#line 285 "molscript.y"
                         { anchor_start (yytext); }
#line 2702 "molscript.tab.c"
    break;

  case 185: /* $@30: %empty  */
#line 286 "molscript.y"
                                   { anchor_start_geometry(); }
#line 2708 "molscript.tab.c"
    break;

  case 186: /* ctrl_command: ANCHOR id $@29 anchor_description anchor_parameters $@30 '{' basic_commands '}' ';'  */
#line 287 "molscript.y"
                                            { anchor_finish(); }
#line 2714 "molscript.tab.c"
    break;

  case 187: /* $@31: %empty  */
#line 288 "molscript.y"
                               { lod_start(); }
#line 2720 "molscript.tab.c"
    break;

  case 188: /* $@32: %empty  */
#line 288 "molscript.y"
                                                           { lod_start_group(); }
#line 2726 "molscript.tab.c"
    break;

  case 189: /* $@33: %empty  */
#line 289 "molscript.y"
                           { lod_finish_group(); }
#line 2732 "molscript.tab.c"
    break;

  case 190: /* ctrl_command: LEVEL_OF_DETAIL $@31 lod_blocks $@32 lod_group $@33 ';'  */
#line 289 "molscript.y"
                                                       { lod_finish(); }
#line 2738 "molscript.tab.c"
    break;

  case 191: /* $@34: %empty  */
#line 290 "molscript.y"
                            { viewpoint_start (yytext); }
#line 2744 "molscript.tab.c"
    break;

  case 193: /* ctrl_command: DIRECTIONALLIGHT vector ';'  */
#line 291 "molscript.y"
                                           { output_directionallight(); }
#line 2750 "molscript.tab.c"
    break;

  case 194: /* ctrl_command: DIRECTIONALLIGHT direction ';'  */
#line 292 "molscript.y"
                                              { output_directionallight(); }
#line 2756 "molscript.tab.c"
    break;

  case 195: /* ctrl_command: POINTLIGHT vector ';'  */
#line 293 "molscript.y"
                                     { output_pointlight(); }
#line 2762 "molscript.tab.c"
    break;

  case 196: /* ctrl_command: SPOTLIGHT vector vector number ';'  */
#line 294 "molscript.y"
                                                  { output_spotlight(); }
#line 2768 "molscript.tab.c"
    break;

  case 197: /* ctrl_command: SPOTLIGHT vector direction number ';'  */
#line 295 "molscript.y"
                                                     { output_spotlight(); }
#line 2774 "molscript.tab.c"
    break;

  case 198: /* anchor_description: DESCRIPTION id  */
#line 298 "molscript.y"
                                    { anchor_description (yytext); }
#line 2780 "molscript.tab.c"
    break;

  case 202: /* anchor_parameter: PARAMETER id  */
#line 306 "molscript.y"
                                { anchor_parameter (yytext); }
#line 2786 "molscript.tab.c"
    break;

  case 210: /* $@35: %empty  */
#line 321 "molscript.y"
                   { lod_start_group(); }
#line 2792 "molscript.tab.c"
    break;

  case 211: /* lod_block: number $@35 lod_group  */
#line 321 "molscript.y"
                                                    { lod_finish_group(); }
#line 2798 "molscript.tab.c"
    break;

  case 214: /* view_definition: direction  */
#line 327 "molscript.y"
                            { viewpoint_output(); }
#line 2804 "molscript.tab.c"
    break;

  case 215: /* view_definition: direction number  */
#line 328 "molscript.y"
                                   { viewpoint_output(); }
#line 2810 "molscript.tab.c"
    break;

  case 216: /* view_definition: ORIGIN vector number  */
#line 329 "molscript.y"
                                       { viewpoint_output(); }
#line 2816 "molscript.tab.c"
    break;

  case 217: /* atom_selection: NOT atom_selection  */
#line 332 "molscript.y"
                                    { select_atom_not(); }
#line 2822 "molscript.tab.c"
    break;

  case 221: /* atom_and: AND atom_selection  */
#line 338 "molscript.y"
                              { select_atom_and(); }
#line 2828 "molscript.tab.c"
    break;

  case 222: /* $@36: %empty  */
#line 339 "molscript.y"
                              { select_atom_and(); }
#line 2834 "molscript.tab.c"
    break;

  case 224: /* atom_or: OR atom_selection  */
#line 342 "molscript.y"
                            { select_atom_or(); }
#line 2840 "molscript.tab.c"
    break;

  case 225: /* $@37: %empty  */
#line 343 "molscript.y"
                             { select_atom_or(); }
#line 2846 "molscript.tab.c"
    break;

  case 227: /* atom_specification: ATOM id  */
#line 346 "molscript.y"
                                                 { select_atom_id (yytext); }
#line 2852 "molscript.tab.c"
    break;

  case 228: /* $@38: %empty  */
#line 347 "molscript.y"
                                 { lex_yytext_push(); }
#line 2858 "molscript.tab.c"
    break;

  case 229: /* atom_specification: RES_ATOM id $@38 id  */
#line 348 "molscript.y"
                                 { select_atom_res_id (yytext); lex_yytext_pop(); }
#line 2864 "molscript.tab.c"
    break;

  case 230: /* atom_specification: OCCUPANCY number number  */
#line 349 "molscript.y"
                                                 { select_atom_occupancy(); }
#line 2870 "molscript.tab.c"
    break;

  case 231: /* atom_specification: B_FACTOR number number  */
#line 350 "molscript.y"
                                                 { select_atom_b_factor(); }
#line 2876 "molscript.tab.c"
    break;

  case 232: /* atom_specification: IN residue_selection  */
#line 351 "molscript.y"
                                                 { select_atom_in(); }
#line 2882 "molscript.tab.c"
    break;

  case 233: /* atom_specification: SPHERE vector number  */
#line 352 "molscript.y"
                                                 { select_atom_sphere(); }
#line 2888 "molscript.tab.c"
    break;

  case 234: /* atom_specification: CLOSE atom_selection number  */
#line 353 "molscript.y"
                                                 { select_atom_close(); }
#line 2894 "molscript.tab.c"
    break;

  case 235: /* atom_specification: PEPTIDE  */
#line 354 "molscript.y"
                                                 { select_atom_peptide(); }
#line 2900 "molscript.tab.c"
    break;

  case 236: /* atom_specification: BACKBONE  */
#line 355 "molscript.y"
                                                 { select_atom_backbone(); }
#line 2906 "molscript.tab.c"
    break;

  case 237: /* atom_specification: HYDROGENS  */
#line 356 "molscript.y"
                                                 { select_atom_hydrogens(); }
#line 2912 "molscript.tab.c"
    break;

  case 238: /* atom_specification: ELEMENT id  */
#line 357 "molscript.y"
                                                 { select_atom_element (yytext); }
#line 2918 "molscript.tab.c"
    break;

  case 239: /* residue_selection: NOT residue_selection  */
#line 360 "molscript.y"
                                          { select_residue_not(); }
#line 2924 "molscript.tab.c"
    break;

  case 243: /* residue_and: AND residue_selection  */
#line 366 "molscript.y"
                                    { select_residue_and(); }
#line 2930 "molscript.tab.c"
    break;

  case 244: /* $@39: %empty  */
#line 367 "molscript.y"
                                    { select_residue_and(); }
#line 2936 "molscript.tab.c"
    break;

  case 246: /* residue_or: OR residue_selection  */
#line 370 "molscript.y"
                                  { select_residue_or(); }
#line 2942 "molscript.tab.c"
    break;

  case 247: /* $@40: %empty  */
#line 371 "molscript.y"
                                   { select_residue_or(); }
#line 2948 "molscript.tab.c"
    break;

  case 249: /* residue_specification: MOLECULE id  */
#line 374 "molscript.y"
                                          { select_residue_molecule (yytext); }
#line 2954 "molscript.tab.c"
    break;

  case 250: /* residue_specification: MODEL INTEGER  */
#line 375 "molscript.y"
                                          { select_residue_model(); }
#line 2960 "molscript.tab.c"
    break;

  case 251: /* $@41: %empty  */
#line 376 "molscript.y"
                                          { lex_yytext_push(); }
#line 2966 "molscript.tab.c"
    break;

  case 252: /* residue_specification: FROM number_as_id $@41 TO number_as_id  */
#line 377 "molscript.y"
                                          { select_residue_from_to
					      (lex_yytext_str(), yytext);
			                    lex_yytext_pop(); }
#line 2974 "molscript.tab.c"
    break;

  case 253: /* residue_specification: RESIDUE id  */
#line 380 "molscript.y"
                                          { select_residue_id (yytext); }
#line 2980 "molscript.tab.c"
    break;

  case 254: /* residue_specification: TYPE id  */
#line 381 "molscript.y"
                                          { select_residue_type (yytext); }
#line 2986 "molscript.tab.c"
    break;

  case 255: /* residue_specification: CHAIN id  */
#line 382 "molscript.y"
                                          { select_residue_chain (yytext); }
#line 2992 "molscript.tab.c"
    break;

  case 256: /* residue_specification: CONTAINS atom_selection  */
#line 383 "molscript.y"
                                                { select_residue_contains(); }
#line 2998 "molscript.tab.c"
    break;

  case 257: /* residue_specification: AMINO_ACIDS  */
#line 384 "molscript.y"
                                          { select_residue_amino_acids(); }
#line 3004 "molscript.tab.c"
    break;

  case 258: /* residue_specification: WATERS  */
#line 385 "molscript.y"
                                          { select_residue_waters(); }
#line 3010 "molscript.tab.c"
    break;

  case 259: /* residue_specification: NUCLEOTIDES  */
#line 386 "molscript.y"
                                          { select_residue_nucleotides(); }
#line 3016 "molscript.tab.c"
    break;

  case 260: /* residue_specification: LIGANDS  */
#line 387 "molscript.y"
                                          { select_residue_ligands(); }
#line 3022 "molscript.tab.c"
    break;

  case 261: /* residue_specification: SEGID id  */
#line 388 "molscript.y"
                                          { select_residue_segid (yytext); }
#line 3028 "molscript.tab.c"
    break;

  case 262: /* vector: POSITION atom_selection  */
#line 391 "molscript.y"
                                 { position(); lex_numbers(); }
#line 3034 "molscript.tab.c"
    break;

  case 265: /* colour: RGB number number number  */
#line 397 "molscript.y"
                                  { set_rgb(); }
#line 3040 "molscript.tab.c"
    break;

  case 266: /* colour: HSB number number number  */
#line 398 "molscript.y"
                                  { set_hsb(); }
#line 3046 "molscript.tab.c"
    break;

  case 267: /* colour: GREY number  */
#line 399 "molscript.y"
                                  { set_grey(); }
#line 3052 "molscript.tab.c"
    break;

  case 268: /* colour: id  */
#line 400 "molscript.y"
                                  { set_colour (yytext); }
#line 3058 "molscript.tab.c"
    break;

  case 269: /* $@42: %empty  */
#line 403 "molscript.y"
                   { ramp_from_colour = given_colour; }
#line 3064 "molscript.tab.c"
    break;

  case 270: /* ramp: FROM colour $@42 TO colour  */
#line 404 "molscript.y"
                   { set_colour_ramp (&given_colour); }
#line 3070 "molscript.tab.c"
    break;

  case 271: /* ramp: RAINBOW  */
#line 405 "molscript.y"
                   { set_rainbow_ramp(); }
#line 3076 "molscript.tab.c"
    break;

  case 272: /* ramp: BWRAMP  */
#line 406 "molscript.y"
                   { set_named_ramp ("blue-white-red"); }
#line 3082 "molscript.tab.c"
    break;

  case 273: /* ramp: AFRAMP  */
#line 407 "molscript.y"
                   { set_named_ramp ("alphafold"); }
#line 3088 "molscript.tab.c"
    break;

  case 274: /* ramp: AFGRADIENT  */
#line 408 "molscript.y"
                   { set_named_ramp ("alphafold-gradient"); }
#line 3094 "molscript.tab.c"
    break;

  case 275: /* number_as_id: number  */
#line 411 "molscript.y"
                      { pop_dstack (1); }
#line 3100 "molscript.tab.c"
    break;


#line 3104 "molscript.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 423 "molscript.y"



/*------------------------------------------------------------*/
int
yyerror (char *s)
{
  if (s) {
    fprintf (stderr, "Error: %s\n", s);
  } else {
    fprintf (stderr, "Error\n");
  }
  lex_info();
  if (exit_on_error) exit (1);
  lex_cleanup();
  clear_dstack();
  return 0;
}


/*------------------------------------------------------------*/
void
yywarning (char *s)
{
  if (check_mode) check_warnings++;
  if (s) {
    fprintf (stderr, "Warning: %s\n", s);
  } else {
    fprintf (stderr, "Warning\n");
  }
  lex_info();
}


/*------------------------------------------------------------*/
int
main (int argc, char *argv[])
{
  global_init();
  lex_init();
  process_arguments (&argc, argv);
  banner();
  if (yyparse()) return 1;
  output_finish_output();
  if (check_mode) {
    fprintf (stderr, "Check complete: %d warning(s)\n", check_warnings);
    return check_warnings ? 2 : 0;
  }
  banner();
  return 0;
}
