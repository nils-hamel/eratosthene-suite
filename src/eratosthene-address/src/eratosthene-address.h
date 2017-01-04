/*
 *  eratosthene-suite - eratosthene indexation server front-end
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

    /*! \file   eratosthene-address.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  Eratosthene address - main module
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

    # ifndef __ER_ADDRESS__
    # define __ER_ADDRESS__

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
    # include <common-include.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* trigonometric conversion factors */
    # define ER_ADDRESS_D2R ( LE_PI / 180.0 )
    # define ER_ADDRESS_R2D ( 180.0 / LE_PI )

    /* display format specifier */
    # define ER_ADDRESS_PFS "%" _LE_REAL_P ", %" _LE_REAL_P ", %" _LE_REAL_P "\n"

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

    /*! \brief main function
     *
     *  The main function searches in the arguments and paremeter the execution
     *  switches. Depending on the provdied switch, the main function calls the
     *  specialised process.
     *
     *  \param argc Main function parameters
     *  \param argv Main function parameters
     *
     *  \return Standard exit code
     */

    int main( int argc, char ** argv );

    /*! \brief space conversion methods
     *
     *  This function, that expects the standard main function parameters, reads
     *  the provided geographic coordinates and the scale value. It then compute
     *  the spatial index obtained using the geographic coordinates. Depending
     *  on the scale value, it computes the geographic coordinates back from the
     *  spatial index.
     *
     *  It then displays on the standard output the original coordinates, the
     *  computed spatial index and the coordinates obtiained by converting back
     *  the spatial index.
     *
     *  The provided coordinate has to be given in decimal degrees in the usual
     *  ranges : [-180,+180], [-90,+90]. The heights has to be given in metres
     *  above the WGS84 ellipsoid.
     *
     *  \param argc Main function parameters
     *  \param argv Main function parameters
     */

    le_void_t er_address_space( int argc, char ** argv );

    /*! \brief time conversion methods
     *
     *  This function, that expects the standard main function parameters, reads
     *  the provided date and converts it into a UTC UNIX timestamps. After the
     *  conversion, the function displays the result on the standard output.
     *
     *  The date has to be provided following the pattern : YYYY-MM-DD-HH-MM-SS
     *
     *  \param argc Main function parameters
     *  \param argv Main function parameters
     */

    le_void_t er_address_time( int argc, char ** argv );

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

