/*
 *  eratosthene-suite - common library
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite common library
     */

    /*! \mainpage eratosthene-suite
     *
     *  \section eratosthene-suite
     *
     *  The _eratosthene-suite_ offers a front-end to the eratosthene indexation
     *  server implemented in the _liberatosthene_ library. It consists in a
     *  softwares suite offering front-end solutions to the functionalities of
     *  the eratosthene library.
     *
     *  The principale software implements the server itself allowing to simply
     *  create instances and maintain services. The suite also offers a front-end
     *  software for the data injection in the available servers. It also offers
     *  a graphical software allowing to browse the 4D worlds offered by the
     *  available servers.
     *
     *  Other tools are also available as the isometric projection creation and
     *  the address and time conversion softwares
     *
     *  \section Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016-2017 EPFL CDH DHLAB
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
    header - internal includes
 */

/*
    header - external includes
 */

    # include <stdio.h>
    # include <stdlib.h>
    # include <stdint.h>
    # include <inttypes.h>
    # include <string.h>
    # define __USE_XOPEN
    # include <time.h>
    # undef __USE_XOPEN
    # include <png.h>

/*
    header - preprocessor definitions
 */

    /* define boolean values */
    # define LC_FALSE            ( 0 )
    # define LC_TRUE             ( 1 )

/*
    header - preprocessor macros
 */

    /* string comparison */
    # define lc_strc( a, b )     ( strcmp( ( char * ) a, ( char * ) b ) == 0 ? LC_TRUE : LC_FALSE )

    /* strict clamping */
    # define lc_clamp( v, a, b ) ( ( v ) < ( a ) ? ( a ) : ( ( v ) > ( b ) ? ( b ) : ( v ) ) )

    /* cyclic clamping */
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

