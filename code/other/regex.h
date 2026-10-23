/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 *
 * Copyright: see 'regex.c' file.
 *
 * Per Kraulis
 *  29-Jan-1997  converted to ANSI C
 *  18-Jun-1997  changed argument tests into assert's
 *  17-Sep-1997  added function 'regcomp_pjk', which takes a MolScript-style
 *               regular expression and compiles it into a 'regexp'
 */

#ifndef REGEX_H
#define REGEX_H 1

/* This V8 interface is not ABI-compatible with the POSIX functions used
   by shared libraries such as graphics drivers. Keep its symbols private. */
#define regcomp molscript_regcomp
#define regexec molscript_regexec
#define regerror molscript_regerror

#define NSUBEXP  10

typedef struct regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

void regerror (char *s);
regexp *regcomp (char *exp);
int regexec (regexp *prog, char *string);

regexp *regcomp_pjk (const char *str);

#endif
