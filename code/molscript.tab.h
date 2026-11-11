/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_MOLSCRIPT_TAB_H_INCLUDED
# define YY_YY_MOLSCRIPT_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INTEGER = 258,                 /* INTEGER  */
    REAL = 259,                    /* REAL  */
    STRING = 260,                  /* STRING  */
    ITEM = 261,                    /* ITEM  */
    ON = 262,                      /* ON  */
    OFF = 263,                     /* OFF  */
    POSITION = 264,                /* POSITION  */
    TITLE = 265,                   /* TITLE  */
    MACRO = 266,                   /* MACRO  */
    PLOT = 267,                    /* PLOT  */
    END_PLOT = 268,                /* END_PLOT  */
    COMMENT = 269,                 /* COMMENT  */
    DEBUG = 270,                   /* DEBUG  */
    POSTSCRIPT = 271,              /* POSTSCRIPT  */
    RASTER3D = 272,                /* RASTER3D  */
    VRML = 273,                    /* VRML  */
    NOFRAME = 274,                 /* NOFRAME  */
    FRAME = 275,                   /* FRAME  */
    AREA = 276,                    /* AREA  */
    BACKGROUND = 277,              /* BACKGROUND  */
    WINDOW = 278,                  /* WINDOW  */
    SLAB = 279,                    /* SLAB  */
    HEADLIGHT = 280,               /* HEADLIGHT  */
    SHADOWS = 281,                 /* SHADOWS  */
    FOG = 282,                     /* FOG  */
    READ = 283,                    /* READ  */
    INLINE_PDB = 284,              /* INLINE_PDB  */
    DELETE = 285,                  /* DELETE  */
    COPY = 286,                    /* COPY  */
    ANCHOR = 287,                  /* ANCHOR  */
    DESCRIPTION = 288,             /* DESCRIPTION  */
    PARAMETER = 289,               /* PARAMETER  */
    VIEWPOINT = 290,               /* VIEWPOINT  */
    ORIGIN = 291,                  /* ORIGIN  */
    DIRECTIONALLIGHT = 292,        /* DIRECTIONALLIGHT  */
    POINTLIGHT = 293,              /* POINTLIGHT  */
    SPOTLIGHT = 294,               /* SPOTLIGHT  */
    LEVEL_OF_DETAIL = 295,         /* LEVEL_OF_DETAIL  */
    TRANSFORM = 296,               /* TRANSFORM  */
    BY = 297,                      /* BY  */
    CENTRE = 298,                  /* CENTRE  */
    TRANSLATION = 299,             /* TRANSLATION  */
    ROTATION = 300,                /* ROTATION  */
    XAXIS = 301,                   /* XAXIS  */
    YAXIS = 302,                   /* YAXIS  */
    ZAXIS = 303,                   /* ZAXIS  */
    AXIS = 304,                    /* AXIS  */
    STORE_MATRIX = 305,            /* STORE_MATRIX  */
    RECALL_MATRIX = 306,           /* RECALL_MATRIX  */
    NOT = 307,                     /* NOT  */
    REQUIRE = 308,                 /* REQUIRE  */
    AND = 309,                     /* AND  */
    EITHER = 310,                  /* EITHER  */
    OR = 311,                      /* OR  */
    BACKBONE = 312,                /* BACKBONE  */
    PEPTIDE = 313,                 /* PEPTIDE  */
    HYDROGENS = 314,               /* HYDROGENS  */
    ATOM = 315,                    /* ATOM  */
    RES_ATOM = 316,                /* RES_ATOM  */
    B_FACTOR = 317,                /* B_FACTOR  */
    OCCUPANCY = 318,               /* OCCUPANCY  */
    IN = 319,                      /* IN  */
    SPHERE = 320,                  /* SPHERE  */
    CLOSE = 321,                   /* CLOSE  */
    MODEL = 322,                   /* MODEL  */
    AMINO_ACIDS = 323,             /* AMINO_ACIDS  */
    WATERS = 324,                  /* WATERS  */
    NUCLEOTIDES = 325,             /* NUCLEOTIDES  */
    LIGANDS = 326,                 /* LIGANDS  */
    MOLECULE = 327,                /* MOLECULE  */
    FROM = 328,                    /* FROM  */
    TO = 329,                      /* TO  */
    RESIDUE = 330,                 /* RESIDUE  */
    TYPE = 331,                    /* TYPE  */
    CONTAINS = 332,                /* CONTAINS  */
    CHAIN = 333,                   /* CHAIN  */
    ELEMENT = 334,                 /* ELEMENT  */
    SEGID = 335,                   /* SEGID  */
    SET = 336,                     /* SET  */
    PUSH = 337,                    /* PUSH  */
    POP = 338,                     /* POP  */
    ATOMCOLOUR = 339,              /* ATOMCOLOUR  */
    ATOMRADIUS = 340,              /* ATOMRADIUS  */
    BONDDISTANCE = 341,            /* BONDDISTANCE  */
    BONDCROSS = 342,               /* BONDCROSS  */
    COILRADIUS = 343,              /* COILRADIUS  */
    COLOURPARTS = 344,             /* COLOURPARTS  */
    COLOURRAMP = 345,              /* COLOURRAMP  */
    CYLINDERRADIUS = 346,          /* CYLINDERRADIUS  */
    DEPTHCUE = 347,                /* DEPTHCUE  */
    EMISSIVECOLOUR = 348,          /* EMISSIVECOLOUR  */
    HELIXTHICKNESS = 349,          /* HELIXTHICKNESS  */
    HELIXWIDTH = 350,              /* HELIXWIDTH  */
    HSBRAMPREVERSE = 351,          /* HSBRAMPREVERSE  */
    LABELBACKGROUND = 352,         /* LABELBACKGROUND  */
    LABELCENTRE = 353,             /* LABELCENTRE  */
    LABELCLIP = 354,               /* LABELCLIP  */
    LABELFONT = 355,               /* LABELFONT  */
    LABELMASK = 356,               /* LABELMASK  */
    LABELOFFSET = 357,             /* LABELOFFSET  */
    LABELROTATION = 358,           /* LABELROTATION  */
    LABELSIZE = 359,               /* LABELSIZE  */
    LIGHTAMBIENTINTENSITY = 360,   /* LIGHTAMBIENTINTENSITY  */
    LIGHTATTENUATION = 361,        /* LIGHTATTENUATION  */
    LIGHTCOLOUR = 362,             /* LIGHTCOLOUR  */
    LIGHTINTENSITY = 363,          /* LIGHTINTENSITY  */
    LIGHTRADIUS = 364,             /* LIGHTRADIUS  */
    LINECOLOUR = 365,              /* LINECOLOUR  */
    LINEDASH = 366,                /* LINEDASH  */
    LINEWIDTH = 367,               /* LINEWIDTH  */
    OBJECTTRANSFORM = 368,         /* OBJECTTRANSFORM  */
    PLANECOLOUR = 369,             /* PLANECOLOUR  */
    PLANE2COLOUR = 370,            /* PLANE2COLOUR  */
    REGULAREXPRESSION = 371,       /* REGULAREXPRESSION  */
    RESIDUECOLOUR = 372,           /* RESIDUECOLOUR  */
    SEGMENTS = 373,                /* SEGMENTS  */
    SEGMENTSIZE = 374,             /* SEGMENTSIZE  */
    SHADING = 375,                 /* SHADING  */
    SHADINGEXPONENT = 376,         /* SHADINGEXPONENT  */
    SHININESS = 377,               /* SHININESS  */
    SMOOTHSTEPS = 378,             /* SMOOTHSTEPS  */
    SPECULARCOLOUR = 379,          /* SPECULARCOLOUR  */
    SPLINEFACTOR = 380,            /* SPLINEFACTOR  */
    STICKRADIUS = 381,             /* STICKRADIUS  */
    STICKTAPER = 382,              /* STICKTAPER  */
    STRANDTHICKNESS = 383,         /* STRANDTHICKNESS  */
    STRANDWIDTH = 384,             /* STRANDWIDTH  */
    TRANSPARENCY = 385,            /* TRANSPARENCY  */
    BALL_AND_STICK = 386,          /* BALL_AND_STICK  */
    BONDS = 387,                   /* BONDS  */
    COIL = 388,                    /* COIL  */
    CYLINDER = 389,                /* CYLINDER  */
    CPK = 390,                     /* CPK  */
    HELIX = 391,                   /* HELIX  */
    LABEL = 392,                   /* LABEL  */
    LINE = 393,                    /* LINE  */
    OBJECT = 394,                  /* OBJECT  */
    INLINE = 395,                  /* INLINE  */
    STRAND = 396,                  /* STRAND  */
    TRACE = 397,                   /* TRACE  */
    TURN = 398,                    /* TURN  */
    DOUBLE_HELIX = 399,            /* DOUBLE_HELIX  */
    RGB = 400,                     /* RGB  */
    HSB = 401,                     /* HSB  */
    GREY = 402,                    /* GREY  */
    RAINBOW = 403,                 /* RAINBOW  */
    BWRAMP = 404,                  /* BWRAMP  */
    AFRAMP = 405,                  /* AFRAMP  */
    AFGRADIENT = 406,              /* AFGRADIENT  */
    LEGEND = 407,                  /* LEGEND  */
    DISTANCE = 408,                /* DISTANCE  */
    ANGLE = 409,                   /* ANGLE  */
    CONNECTOR = 410,               /* CONNECTOR  */
    SCALE = 411,                   /* SCALE  */
    CURVETOLERANCE = 412,          /* CURVETOLERANCE  */
    COLOURINTERPOLATE = 413,       /* COLOURINTERPOLATE  */
    VERTICAL = 414,                /* VERTICAL  */
    TICKS = 415,                   /* TICKS  */
    KEY = 416                      /* KEY  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MOLSCRIPT_TAB_H_INCLUDED  */
