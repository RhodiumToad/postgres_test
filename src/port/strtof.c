/*-------------------------------------------------------------------------
 *
 * strtof.c
 *
 * Portions Copyright (c) 2019, PostgreSQL Global Development Group
 *
 *
 * IDENTIFICATION
 *	  src/port/strtof.c
 *
 *-------------------------------------------------------------------------
 */

#include "c.h"

#include <float.h>
#include <math.h>

/*
 * strtof() is part of C99; this file is only for the benefit of obsolete
 * platforms. As such, it is known to return incorrect values for edge cases,
 * which have to be allowed for in variant files for regression test results
 * for any such platform.
 */

float
strtof(const char *nptr, char **endptr)
{
	int			caller_errno = errno;
	double		dresult;
	float		fresult;

	errno = 0;
	dresult = strtod(nptr, endptr);
	fresult = (float) dresult;

	if (errno == 0)
	{
		/*
		 * Value might be in-range for double but not float.
		 */
		if (dresult != 0 && fresult == 0)
			caller_errno = ERANGE;			  /* underflow */
		if (!isinf(dresult) && isinf(fresult))
			caller_errno = ERANGE;			  /* overflow */
	}
	else
		caller_errno = errno;

	errno = caller_errno;
	return fresult;
}
