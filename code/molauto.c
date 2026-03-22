/* molauto.c

   MolAuto v1.1.1 for MolScript v2.1

   Create a MolScript input file for a given PDB file.

   Copyright (C) 1997-1998 Per Kraulis
    18-Jan-1997  first attempts
    12-Mar-1997  first usable version
    29-Aug-1997  rewrote secondary structure output procedure
     5-Nov-1997  fixed single-residue coil and turn bug
    23-Jan-1998  mod's for named_data in mol3d
     5-Jun-1998  mod's for mol3d_init; PDB secondary structure now default
    17-Jun-1998  fixed bug with 1HIV: convert single aa among ligands to ligand
    25-Nov-1998  fixed -ss_hb bug: init mol properly
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <args.h>
#include <str_utils.h>
#include <dynstring.h>
#include <mol3d.h>
#include <mol3d_io.h>
#include <mol3d_init.h>
#include <mol3d_utils.h>
#include <mol3d_secstruc.h>
#include <aa_lookup.h>


/*============================================================*/
const char program_str[] = "MolAuto v1.1.1";
const char copyright_str[] = "Copyright (C) 1997-1998 Per J. Kraulis";

enum ss_modes { SS_PDB, SS_CA, SS_HB };
enum ligand_modes { LIGAND_NO, LIGAND_BONDS, LIGAND_STICK, LIGAND_CPK };
enum palette_modes { PALETTE_AUTO, PALETTE_SS, PALETTE_COLOURBLIND,
                     PALETTE_PUBLICATION };

int title_mode = TRUE;
int centre_mode = TRUE;
int cylinder_mode = FALSE;
int coil_mode = TRUE;
int nice_mode = FALSE;
int thin_mode = FALSE;
int ligand_mode = LIGAND_BONDS;
int colour_mode = TRUE;
int ss_mode = SS_PDB;
int palette_mode = PALETTE_AUTO;
int window_mode = FALSE;
int rotation_mode = FALSE;
int translation_mode = FALSE;

static double window_size = 0.0;
static double rotation_matrix[9];
static double translation_vector[3];

static double hue, decrement;

void fatal_error (const char *msg);
void fatal_error_str (const char *msg, const char *str);


/*------------------------------------------------------------*/
static double
parse_number (const char *opt, const char *str)
{
  char *end;
  double value;

  assert (opt);
  assert (str);

  value = strtod (str, &end);
  if ((end == str) || (*end != '\0')) fatal_error_str ("invalid numeric value for", opt);

  return value;
}


/*------------------------------------------------------------*/
static int
parse_number_option (const char *opt, int count, double *values)
{
  int slot, ix;

  assert (opt);
  assert (count > 0);
  assert (values);

  slot = args_exists ((char *) opt);
  if (!slot) return FALSE;

  if (slot + count >= args_number) fatal_error_str ("missing value(s) for", opt);

  args_flag (slot);
  for (ix = 0; ix < count; ix++) {
    values[ix] = parse_number (opt, args_item (slot + ix + 1));
    args_flag (slot + ix + 1);
  }

  return TRUE;
}


/*------------------------------------------------------------*/
void
output_hsb_decrement (void)
{
  if (colour_mode && !nice_mode) {
    printf ("  set planecolour hsb %.4g 1 1;\n", hue);
    hue -= decrement;
    if (hue < 0.0) hue = 0.0;
  }
}


/*------------------------------------------------------------*/
void
fatal_error (const char *msg)
{
  assert (msg);

  fprintf (stderr, "MolAuto error: %s\n", msg);
  exit (1);
}


/*------------------------------------------------------------*/
void
fatal_error_str (const char *msg, const char *str)
{
  assert (msg);
  assert (str);

  fprintf (stderr, "MolAuto error: %s %s\n", msg, str);
  exit (1);
}


/*------------------------------------------------------------*/
char *
process_arguments (void)
{
  int slot;

  args_flag (0);

  if ((args_number <= 1) || (args_exists ("-h"))) {
    fprintf (stderr, "Usage: molauto [options] pdbfile\n");
    fprintf (stderr, "  -notitle   do not output a title\n");
    fprintf (stderr, "  -nocentre  do not centre the molecule\n");
    fprintf (stderr, "  -window n  emit a MolScript window command\n");
    fprintf (stderr, "  -rotate a11 a12 a13 a21 a22 a23 a31 a32 a33\n");
    fprintf (stderr, "             apply a manual rotation matrix\n");
    fprintf (stderr, "  -translate x y z\n");
    fprintf (stderr, "             apply a manual translation after centring/rotation\n");
    fprintf (stderr, "  -cylinder  use cylinders for helices\n");
    fprintf (stderr, "  -turns     use turns when specified, otherwise coil\n");
    fprintf (stderr, "  -nice      nicer rendering; rainbow colours, more segments\n");
    fprintf (stderr, "  -thin      set helix, strand, coil thickness to 0\n");
    fprintf (stderr, "  -ss_palette use a fixed secondary-structure palette\n");
    fprintf (stderr, "  -colourblind use a colourblind-friendly secondary-structure palette\n");
    fprintf (stderr, "  -publication nicer geometry plus a muted publication palette\n");
    fprintf (stderr, "  -noligand  do not render ligand(s)\n");
    fprintf (stderr, "  -bonds     render ligand(s) using bonds (default)\n");
    fprintf (stderr, "  -stick     render ligand(s) using ball-and-stick\n");
    fprintf (stderr, "  -cpk       render ligand(s) using cpk\n");
    fprintf (stderr, "  -nocolour  do not use colour for schematics\n");
    fprintf (stderr, "  -ss_pdb    secondary structure as defined in PDB file (default)\n");
    fprintf (stderr, "  -ss_hb     secondary structure by H-bonds (DSSP-like) criteria\n");
    fprintf (stderr, "  -ss_ca     secondary structure by CA-geometry criteria \n");
    fprintf (stderr, "  -h         output this message\n");
    fprintf (stderr, "----- %s, %s\n", program_str, copyright_str);
    fprintf (stderr, "----- http://www.avatar.se/molscript/\n");

    exit (0);
  }

  slot = args_exists ("-nocentre");
  if (slot) {
    centre_mode = FALSE;
    args_flag (slot);
  }

  if (parse_number_option ("-window", 1, &window_size)) {
    window_mode = TRUE;
  }

  if (parse_number_option ("-rotate", 9, rotation_matrix)) {
    rotation_mode = TRUE;
  }

  if (parse_number_option ("-translate", 3, translation_vector)) {
    translation_mode = TRUE;
  }

  slot = args_exists ("-notitle");
  if (slot) {
    title_mode = FALSE;
    args_flag (slot);
  }

  slot = args_exists ("-cylinder");
  if (slot) {
    cylinder_mode = TRUE;
    args_flag (slot);
  }

  slot = args_exists ("-turns");
  if (slot) {
    coil_mode = FALSE;
    args_flag (slot);
  }

  slot = args_exists ("-nice");
  if (slot) {
    nice_mode = TRUE;
    args_flag (slot);
  }

  slot = args_exists ("-thin");
  if (slot) {
    thin_mode = TRUE;
    args_flag (slot);
  }

  slot = args_exists ("-noligand");
  if (slot) {
    ligand_mode = LIGAND_NO;
    args_flag (slot);
  }

  slot = args_exists ("-bonds");
  if (slot) {
    ligand_mode = LIGAND_BONDS;
    args_flag (slot);
  }

  slot = args_exists ("-stick");
  if (slot) {
    ligand_mode = LIGAND_STICK;
    args_flag (slot);
  }

  slot = args_exists ("-cpk");
  if (slot) {
    ligand_mode = LIGAND_CPK;
    args_flag (slot);
  }

  slot = args_exists ("-nocolour");
  if (slot) {
    colour_mode = FALSE;
    args_flag (slot);
  }

  slot = args_exists ("-ss_palette");
  if (slot) {
    palette_mode = PALETTE_SS;
    args_flag (slot);
  }

  slot = args_exists ("-colourblind");
  if (slot) {
    palette_mode = PALETTE_COLOURBLIND;
    args_flag (slot);
  }

  slot = args_exists ("-publication");
  if (slot) {
    palette_mode = PALETTE_PUBLICATION;
    nice_mode = TRUE;
    args_flag (slot);
  }

  slot = args_exists ("-ss_hb");
  if (slot) {
    ss_mode = SS_HB;
    args_flag (slot);
  }

  slot = args_exists ("-ss_pdb");
  if (slot) {
    ss_mode = SS_PDB;
    args_flag (slot);
  }

  slot = args_exists ("-ss_ca");
  if (slot) {
    ss_mode = SS_CA;
    args_flag (slot);
  }

  slot = args_unflagged();
  if (slot <= 0) fatal_error ("no PDB file name given");
  if (args_item (slot) [0] == '-')
    fatal_error_str ("invalid argument: ", args_item (slot));

  return args_item (slot);
}


/*------------------------------------------------------------*/
static void
output_rgb_colour (double r, double g, double b)
{
  printf ("rgb %.4g %.4g %.4g", r, g, b);
}


/*------------------------------------------------------------*/
static void
output_palette_colour (char ss)
{
  assert (colour_mode);

  switch (palette_mode) {

  case PALETTE_SS:
    switch (toupper (ss)) {
    case 'H': output_rgb_colour (1.0, 0.0, 0.0); break;
    case 'E': output_rgb_colour (1.0, 1.0, 0.0); break;
    case 'T': output_rgb_colour (0.0, 0.7, 0.0); break;
    default:  output_rgb_colour (0.65, 0.65, 0.65); break;
    }
    break;

  case PALETTE_COLOURBLIND:
    switch (toupper (ss)) {
    case 'H': output_rgb_colour (0.835, 0.369, 0.0); break;
    case 'E': output_rgb_colour (0.0, 0.447, 0.698); break;
    case 'T': output_rgb_colour (0.902, 0.624, 0.0); break;
    default:  output_rgb_colour (0.0, 0.620, 0.451); break;
    }
    break;

  case PALETTE_PUBLICATION:
    switch (toupper (ss)) {
    case 'H': output_rgb_colour (0.78, 0.27, 0.25); break;
    case 'E': output_rgb_colour (0.86, 0.72, 0.24); break;
    case 'T': output_rgb_colour (0.39, 0.58, 0.26); break;
    default:  output_rgb_colour (0.60, 0.60, 0.60); break;
    }
    break;

  case PALETTE_AUTO:
  default:
    break;
  }
}


/*------------------------------------------------------------*/
static void
output_scheme_colour (char ss)
{
  if (!colour_mode) return;

  if (palette_mode == PALETTE_AUTO) {
    output_hsb_decrement();
  } else {
    printf ("  set colourparts off, planecolour ");
    output_palette_colour (ss);
    printf (";\n");
  }
}


/*------------------------------------------------------------*/
void
set_secondary_structure (mol3d *mol)
{
  res3d *res, *first;
  int count;
  char ss;

  assert (mol);

  switch (ss_mode) {

  case SS_PDB:			/* has already been set, if data was there */
    if (! (mol->init & MOL3D_INIT_SECSTRUC)) {
      mol3d_secstruc_ca_geom (mol); /* fall-back: CA-geometry criteria */
    }
    break;

  case SS_CA:
    mol3d_secstruc_ca_geom (mol);
    break;

  case SS_HB:
    if (mol3d_secstruc_hbonds (mol)) {
      for (res = mol->first; res; res = res->next) {
	switch (res->secstruc) {
	case 'i':
	case 'I':
	case 'b':
	case 'B':
	  res->secstruc = ' ';
	  break;
	case 'g':
	  res->secstruc = 'h';
	  break;
	case 'G':
	  res->secstruc = 'H';
	  break;
	default:
	  break;
	}
      }
    } else {
      mol3d_secstruc_ca_geom (mol); /* fall-back: CA-geometry criteria */
    }
    break;
  }

  if (coil_mode) {		/* change turn into coil */
    for (res = mol->first; res; res = res->next) {
      switch (res->secstruc) {
      case 't':
      case 'T':
	res->secstruc = ' ';
	break;
      default:
	break;
      }
    }

  } else {
				/* convert single coil residue between turns */
    for (res = mol->first; res; res = res->next) {
      if ((res->secstruc == ' ') && (res->prev) && (res->next) &&
	  (res->prev->secstruc == 'T') && (res->next->secstruc == 't')) {
	res->secstruc = 'T';
      }
    }
				/* convert single turn starts between turns */
    for (res = mol->first; res; res = res->next) {
      if ((res->secstruc == 't') && (res->prev) &&
	  (res->prev->secstruc == 'T')) res->secstruc = 'T';
    }
  }
				/* remove helix/strand if shorter than 3 */
  first = mol->first;
  ss = toupper (first->secstruc);
  count = 1;
  for (res = mol->first; res; res = res->next) {
    if (res->secstruc == ss) {
      count++;
    } else {
      if (((ss == 'H') || (ss == 'E')) && (count < 3)) {
	for (; first != res; first = first->next) first->secstruc = ' ';
      }
      first = res;
      ss = toupper (res->secstruc);
      count = 1;
    }
  }
				/* convert single aa among ligands to ligand */
  for (res = mol->first; res; res = res->next) {
    if (res->secstruc == '-') continue;
    if (res->prev) {
      if (res->next) {
	if ((res->prev->secstruc == '-') &&
	    (res->next->secstruc == '-')) res->secstruc = '-';
      } else {
	if (res->prev->secstruc == '-') res->secstruc = '-';
      }
    } else {
      if (res->next) {
	if (res->next->secstruc == '-') res->secstruc = '-';
      } else {
	res->secstruc = '-';
      }
    }
  }
}


/*------------------------------------------------------------*/
void
output_secondary_structure (mol3d *mol)
{
  res3d *res, *first, *prev;
  char ss;

  assert (mol);

  res = res3d_create();		/* add a sentinel residue, to simplify */
  strcpy (res->name, "NONE");	/* case with last residue being an AA */
  strcpy (res->type, res->name);
  mol3d_append_residue (mol, res);

  if (colour_mode) {
    hue = 0.666666;

    if ((palette_mode == PALETTE_AUTO) && nice_mode) {
      printf ("  set colourparts on, residuecolour amino-acids rainbow;\n\n");
    } else if (palette_mode != PALETTE_AUTO) {
      printf ("  set colourparts off;\n\n");
    } else {
      int parts = 0;
      ss = mol->first->secstruc;
      for (res = mol->first; res; res = res->next) {
	if (res->secstruc != ss) parts++;
	ss = toupper (res->secstruc);
      }
      if (parts > 1) {
	decrement = hue / (double) (parts - 1);
      } else {
	decrement = 0.0;
      }
    }
  }

  first = mol->first;
  ss = first->secstruc;

  for (res = mol->first; res; res = res->next) {
    if (res->secstruc != ss) {

	      switch (ss) {

	      case '-':
	first = res;
	break;

	      case ' ':
		output_scheme_colour (ss);
		printf ("  coil from %s to ", first->name);
	switch (toupper (res->secstruc)) {
	case '-':
	  printf ("%s;\n", prev->name);
	  first = NULL;
	  break;
	case 'T':
	  printf ("%s;\n", prev->name);
	  first = prev;
	  break;
	case 'H':
	case 'E':
	  printf ("%s;\n", res->name);
	  first = res;
	  break;
	}
	break;

	      case 'T':
		output_scheme_colour (ss);
		printf ("  turn from %s to ", first->name);
	switch (toupper (res->secstruc)) {
	case '-':
	  printf ("%s;\n", prev->name);
	  first = NULL;
	  break;
	case ' ':
	case 'H':
	case 'E':
	  printf ("%s;\n", res->name);
	  first = res;
	  break;
	}
	break;

	      case 'H':
		output_scheme_colour (ss);
		if (cylinder_mode) {
	  printf ("  cylinder from %s to ", first->name);
	} else {
	  printf ("  helix from %s to ", first->name);
	}
	switch (toupper (res->secstruc)) {
	case '-':
	  printf ("%s;\n", prev->name);
	  first = NULL;
	  break;
	case ' ':
	case 'T':
	  printf ("%s;\n", prev->name);
	  first = prev;
	  break;
	case 'H':
	case 'E':
	  printf ("%s;\n", prev->name);
	  printf ("  turn from %s to %s;\n", prev->name, res->name);
	  first = res;
	  break;
	}
	break;

	      case 'E':
		output_scheme_colour (ss);
		printf ("  strand from %s to ", first->name);
	switch (toupper (res->secstruc)) {
	case '-':
	  printf ("%s;\n", prev->name);
	  first = NULL;
	  break;
	case ' ':
	case 'T':
	  printf ("%s;\n", prev->name);
	  first = prev;
	  break;
	case 'H':
	case 'E':
	  printf ("%s;\n", prev->name);
	  printf ("  turn from %s to %s;\n", prev->name, res->name);
	  first = res;
	  break;
	}
	break;
      }
      ss = toupper (res->secstruc);
    }
    prev = res;
  }
}


/*------------------------------------------------------------*/
void
output_nucleotides (mol3d *mol)
{
  res3d *res;

  assert (mol);

  for (res = mol->first; res; res = res->next) {
    if (is_nucleic_acid_type (res->type)) {
      if (colour_mode) {
	if ((palette_mode == PALETTE_AUTO) && nice_mode) {
	  printf ("\n  set residuecolour nucleotides rainbow;");
	} else if (palette_mode != PALETTE_AUTO) {
	  printf ("\n  set colourparts off, planecolour ");
	  switch (palette_mode) {
	  case PALETTE_SS:
	    output_rgb_colour (0.60, 0.20, 0.80);
	    break;
	  case PALETTE_COLOURBLIND:
	    output_rgb_colour (0.80, 0.47, 0.65);
	    break;
	  case PALETTE_PUBLICATION:
	    output_rgb_colour (0.58, 0.40, 0.72);
	    break;
	  case PALETTE_AUTO:
	  default:
	    break;
	  }
	  putchar (';');
	} else {
	  printf ("\n  set planecolour rgb 1 0.2 0.2;");
	}
      }
      printf ("\n  double-helix nucleotides;\n");
      break;
    }
  }
}


/*------------------------------------------------------------*/
void
output_ligand (mol3d *mol)
{
  res3d *res;
  char *render_type;
  int first_ligand = TRUE;

  assert (mol);

  switch (ligand_mode) {
  case LIGAND_NO:
    return;
  case LIGAND_BONDS:
    render_type = "bonds";
    break;
  case LIGAND_STICK:
    render_type = "ball-and-stick";
    break;
  case LIGAND_CPK:
    render_type = "cpk";
    break;
  }

  for (res = mol->first; res; res = res->next) {
    if (res->secstruc != '-') continue;
    if (is_water_type (res->type)) continue;
    if (is_nucleic_acid_type (res->type)) continue;
    if (res3d_count_atoms (res) == 0) continue;	/* sentinel residue */

    if (first_ligand) {
      if (colour_mode) {
	printf ("\n  set colourparts on;\n");
      } else {
	printf ("\n");
      }
      first_ligand = FALSE;
    }

    if (res3d_count_atoms (res) == 1) {
      if (res3d_unique_name (mol, res)) {
	printf ("  cpk in residue %s;\n", res->name);
      } else {
	printf ("  cpk in require residue %s and type %s;\n",
		res->name, res->type);
      }
    } else if (res3d_unique_name (mol, res)) {
      printf ("  %s in residue %s;\n", render_type, res->name);
    } else {
      printf ("  %s in require residue %s and type %s;\n", render_type,
	      res->name, res->type);
    }
  }
}


/*------------------------------------------------------------*/
void
output_quoted_string (char *str)
{
  assert (str);
  assert (*str);

  putchar ('"');
  for ( ; *str; str++) {
    if (*str == '"') putchar ('\\');
    putchar (*str);
  }
  putchar ('"');
}


/*------------------------------------------------------------*/
int
main (int argc, char *argv[])
{
  char *pdbfilename;
  mol3d *mol;

  args_initialize (argc, argv);
  pdbfilename = process_arguments();
  mol = mol3d_read_pdb_filename (pdbfilename);
  if ((mol == NULL) &&
      mol3d_is_pdb_code (pdbfilename)) mol = mol3d_read_pdb_code (pdbfilename);
  if (mol == NULL) fatal_error ("could not read the PDB file");
  mol3d_init (mol,
	      MOL3D_INIT_NOBLANKS | MOL3D_INIT_COLOURS |
	      MOL3D_INIT_RADII | MOL3D_INIT_AACODES |
	      MOL3D_INIT_CENTRALS | MOL3D_INIT_RESIDUE_ORDINALS);
  set_secondary_structure (mol);

  printf ("! MolScript v2.1 input file\n");
  printf ("! generated by %s\n\n", program_str);

  if (title_mode && mol->data) {
    dynstring *ds = (dynstring *) nd_search_data (mol->data, "COMPND");
    if (ds) {
      char *pos = strstr (ds->string, "MOLECULE:");
      if (pos) {
	dynstring *ds2;
	pos += 9;
	ds2 = ds_allocate (20);
	while ((*pos != '\0') && (*pos != ';')) ds_add (ds2, *pos++);
	ds_right_adjust (ds2);
	ds_left_adjust (ds2);
	printf ("title ");
	output_quoted_string (ds2->string);
	printf ("\n\n");
	ds_delete (ds2);
      } else {
	ds_right_adjust (ds);
	ds_left_adjust (ds);
	printf ("title ");
	output_quoted_string (ds->string);
	printf ("\n\n");
      }
    }
  }

  printf ("plot\n\n");

  if (window_mode) {
    printf ("  window %.4g;\n\n", window_size);
  }

  printf ("  read mol ");
  output_quoted_string (pdbfilename);
  printf (";\n\n");

  if (centre_mode || rotation_mode || translation_mode) {
    printf ("  transform atom *\n");
    if (centre_mode) {
      printf ("    by centre position atom *\n");
    }
    if (rotation_mode) {
      printf ("    by rotation\n");
      printf ("      %.8g %.8g %.8g\n",
	      rotation_matrix[0], rotation_matrix[1], rotation_matrix[2]);
      printf ("      %.8g %.8g %.8g\n",
	      rotation_matrix[3], rotation_matrix[4], rotation_matrix[5]);
      printf ("      %.8g %.8g %.8g\n",
	      rotation_matrix[6], rotation_matrix[7], rotation_matrix[8]);
    }
    if (translation_mode) {
      printf ("    by translation %.8g %.8g %.8g\n",
	      translation_vector[0], translation_vector[1], translation_vector[2]);
    }
    printf ("    ;\n\n");
  }

  if (!nice_mode) printf ("  set segments 2;\n\n");

  if (thin_mode)
    printf ("  set strandthickness 0, helixthickness 0, coilradius 0;\n\n");

  if (palette_mode == PALETTE_PUBLICATION) {
    printf ("  set specularcolour white, smoothsteps 2;\n\n");
  }

  output_secondary_structure (mol);
  output_nucleotides (mol);
  output_ligand (mol);

  printf ("\nend_plot\n");

  return 0;
}
