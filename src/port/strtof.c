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

#ifndef HAVE_STRTOF
/*
 * strtof() is part of C99; this version is only for the benefit of obsolete
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

#elif HAVE_BUGGY_WINDOWS_STRTOF
/*
 * On Windows, there's a slightly different problem: VS2013 has a strtof()
 * that returns the correct results for valid input, but may fail to report an
 * error for underflow or overflow, returning 0 instead. Work around that by
 * trying strtod() when strtof() returns 0.0 or [+-]Inf, and calling it an
 * error if the result differs.
 */
float
pg_strtof(const char *nptr, char **endptr)
{
	int			caller_errno = errno;
	float		fresult;

	errno = 0;
	fresult = (strtof)(nptr, endptr);
	if (errno)
	{
		/* On error, just return the error to the caller. */
		return fresult;
	}
	else if ((*endptr == nptr) ||
			 (fresult != 0.0 && !isinf(fresult)))
	{
		/*
		 * If we got nothing parseable, or if we got a non-0 finite value (or
		 * NaN) without error, then return that to the caller without error.
		 */
		errno = caller_errno;
		return fresult;
	}
	else
	{
		/*
		 * Try again.
		 */
		double	dresult = strtod(nptr, NULL);
		if (errno)
		{
			/* On error, just return the error */
			return fresult;
		}
		else if ((dresult == 0.0 && fresult == 0.0) ||
				 (isinf(dresult) && isinf(fresult) && (fresult == dresult)))
		{
			/* both values are 0 or infinities of the same sign */
			errno = caller_errno;
			return fresult;
		}
		else
		{
			errno = ERANGE;
			return fresult;
		}
	}
}

#endif
