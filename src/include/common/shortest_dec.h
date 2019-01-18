/*---------------------------------------------------------------------------
 *
 * Ryu floating-point output.
 *
 * Portions Copyright (c) 2018, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *	  src/include/common/shortest_dec.h
 *
 * This is a modification of code taken from github.com/ulfjack/ryu under the
 * terms of the Boost license (not the Apache license). The original copyright
 * notice follows:
 *
 * Copyright 2018 Ulf Adams
 *
 * The contents of this file may be used under the terms of the Apache
 * License, Version 2.0.
 *
 *     (See accompanying file LICENSE-Apache or copy at
 *      http://www.apache.org/licenses/LICENSE-2.0)
 *
 * Alternatively, the contents of this file may be used under the terms of the
 * Boost Software License, Version 1.0.
 *
 *     (See accompanying file LICENSE-Boost or copy at
 *      https://www.boost.org/LICENSE_1_0.txt)
 *
 * Unless required by applicable law or agreed to in writing, this software is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.
 *
 *---------------------------------------------------------------------------
 */
#ifndef SHORTEST_DEC_H
#define SHORTEST_DEC_H

int			double_to_shortest_decimal_bufn(double f, char *result);
void		double_to_shortest_decimal_buf(double f, char *result);
char	   *double_to_shortest_decimal(double f);

int			float_to_shortest_decimal_bufn(float f, char *result);
void		float_to_shortest_decimal_buf(float f, char *result);
char	   *float_to_shortest_decimal(float f);

#endif							/* SHORTEST_DEC_H */
