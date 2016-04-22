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

/*
    header - preprocessor definitions
 */

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief Arguments and parameters parsers
     *
     *  This function searches in the provided arguments list if the specified
     *  argument is present. When it is, the function returns the pointer to
     *  the string containing the parameter of the found argument, NULL is
     *  returned otherwise.
     *
     *  \param argc     Standard parameter
     *  \param argv     Standard parameter
     *  \param er_long  Long form argument
     *  \param er_short Short form argument
     */

    char * lc_read_string( int const argc, char ** argv, char const * const er_long, char const * const er_short );

    /*! \brief Arguments and parameters parsers
     *
     *  This function searches in the provided argument list if the specified
     *  argument is present. As it is, the function converts into unsigned int
     *  the value of the parameter corresponding to the found argument.
     *
     *  When the argument is not found in the list, the function simply returns
     *  the provided default value.
     *
     *  \param argc       Standard parameter
     *  \param argv       Standard parameter
     *  \param er_long    Long form argument
     *  \param er_short   Short form argument
     *  \param er_default Default value
     */

    unsigned int lc_read_uint( int const argc, char ** argv, char const * const er_long, char const * const er_short, unsigned int er_default );

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

