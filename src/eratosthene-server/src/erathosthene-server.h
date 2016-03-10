/*
 *  liberatosthene - geodetic system
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

    /*! \file   eratosthene-server.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene server software
     */

    /*! \mainpage eratosthene-suite
     *
     *  \section eratosthene-suite
     * 
     *  ...
     *
     *  \section Copyright and License
     *
     *  ...
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_SERVER__
    # define __ER_SERVER__

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
    # include <stdint.h>
    # include <string.h>
    # include <eratosthene.h>

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

    /*! \brief Arguments and parameters parser
     */

    char * er_read_string( int const argc, char ** argv, char const * const er_argument );

    /*! \brief Arguments and parameters parser
     */

    unsigned int er_read_uint( int const argc, char ** argv, char const * const er_long, char const * const er_short, unsigned int er_default );

    /*! \brief Main function
     */

    int main( int argc, char ** argv );

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

