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

    /*! \file   common.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Common header
     */

    /*! \mainpage eratosthene-suite
     *
     *  \section eratosthene-suite
     *
     *  The _eratosthene-suite_ offers a front-end to the eratosthene indexation
     *  server implemented in the _liberatosthene_ library. It consists in a
     *  software suite implementing front-ends to the functionality of the 
     *  indexation server.
     *
     *  The main software implements the server itself allowing to simply create
     *  instance of it. The suite also offers a front-end software for the data
     *  injection in the servers storage structure. Finally, it also offers a 
     *  graphical client allowing to browse the data contained in the servers
     *  storage structure through a 4 dimensional representation of earth.
     * 
     *  \section Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016 EPFL CDH DHLAB
     *
     *  This program is licensed under the terms of the GNU GPLv3.
     */

/*
    header - inclusion guard
 */

    # ifndef __LC__
    # define __LC__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include <string.h>

/*
    header - preprocessor definitions
 */

    /* Define boolean values */
    # define LC_TRUE  1
    # define LC_FALSE 0

/*
    header - preprocessor macros
 */

    /* String comparison */
    # define lc_strc( a, b )     ( strcmp( ( char * ) a, ( char * ) b ) == 0 ? LC_TRUE : LC_FALSE )

    /* Range clamping */
    # define lc_clamp( v, a, b ) ( ( v ) < ( a ) ? ( a ) : ( ( v ) > ( b ) ? ( b ) : ( v ) ) )
    # define lc_angle( v )       ( ( v ) > +180.0 ? ( v ) - 360.0 : ( ( v ) < -180.0 ? ( v ) + 360 : ( v ) ) )

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

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

