/*
 *  eratosthene-suite - geodetic system
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016 EPFL CDH DHLAB
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

    /*! \file   common-args.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Arguments and parameters parsers
     */

/*
    header - inclusion guard
 */

    # ifndef __LC_ARGS__
    # define __LC_ARGS__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include <stdio.h>
    # include <stdlib.h>
    # include <string.h>
    # include "common.h"

/*
    header - preprocessor definitions
 */

/*
    header - preprocessor macros
 */

    /* String to unsigned int */
    # define LC_ATOUI( p ) strtoul( p, NULL, 10 )

    /* String to long */
    # define LC_ATOL( p )  strtol( p, NULL, 10 )

    /* String to double precision */
    # define LC_ATODP( p ) atof( p )

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief switches parsers
     *
     *  This function searches in the provided arguments list if the specified
     *  switch is present.
     *
     *  \param  argc     Main function parameters
     *  \param  argv     Main function parameters
     *  \param  er_long  Argument string - long form
     *  \param  er_short Argument string - short form
     *
     *  \return Returns LC_TRUE if switch is found, LC_FALSE otherwise
     */

    int lc_read_flag( int const argc, char ** argv, char const * const er_long, char const * const er_short );

    /*! \brief arguments and parameters parsers
     *
     *  This function searches in the provided arguments list if the specified
     *  argument is present. When it is, the function returns the pointer to
     *  the string containing the parameter of the found argument, NULL is
     *  returned otherwise.
     *
     *  \param  argc     Main function parameters
     *  \param  argv     Main function parameters
     *  \param  er_long  Argument string - long form
     *  \param  er_short Argument string - short form
     *
     *  \return Pointer to parameter string on success, NULL otherwise
     */

    char * lc_read_string( int const argc, char ** argv, char const * const er_long, char const * const er_short );

    /*! \brief arguments and parameters parsers
     *
     *  This function searches in the provided argument list if the specified
     *  argument is present. As it is, the function converts into unsigned int
     *  the value of the parameter corresponding to the found argument.
     *
     *  When the argument is not found in the list, the function simply returns
     *  the provided default value.
     *
     *  \param  argc       Main function parameters
     *  \param  argv       Main function parameters
     *  \param  er_long    Argument string - long form
     *  \param  er_short   Argument string - short form
     *  \param  er_default Parameter default value
     *
     *  \return Parameter value on success, the default value otherwise
     */

    unsigned int lc_read_uint( int const argc, char ** argv, char const * const er_long, char const * const er_short, unsigned int er_default );

    /*! \brief arguments and parameters parsers
     *
     *  This function searches in the provided argument list if the specified
     *  argument is present. As it is, the function converts into long the
     *  value of the parameter corresponding to the found argument.
     *
     *  When the argument is not found in the list, the function simply returns
     *  the provided default value.
     *
     *  \param  argc       Main function parameters
     *  \param  argv       Main function parameters
     *  \param  er_long    Argument string - long form
     *  \param  er_short   Argument string - short form
     *  \param  er_default Parameter default value
     *
     *  \return Parameter value on success, the default value otherwise
     */

    long lc_read_long( int const argc, char ** argv, char const * const er_long, char const * const er_short, long const er_default );

    /*! \brief arguments and parameters parsers
     *
     *  This function searches in the provided argument list if the specified
     *  argument is present. As it is, the function converts into double the
     *  value of the parameter corresponding to the found argument.
     *
     *  When the argument is not found in the list, the function simply returns
     *  the provided default value.
     *
     *  \param  argc       Main function parameters
     *  \param  argv       Main function parameters
     *  \param  er_long    Argument string - long form
     *  \param  er_short   Argument string - short form
     *  \param  er_default Parameter default value
     *
     *  \return Parameter value on success, the default value otherwise
     */

    double lc_read_double( int const argc, char ** argv, char const * const er_long, char const * const er_short, double er_default );

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    header - inclusion guard
 */

    # endif

