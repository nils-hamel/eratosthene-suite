/*
 *  eratosthene-suite - eratosthene indexation server front-end
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

    /*! \file   eratosthene-address.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene address - main module
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
     *  The main function simply reads the provided parameters, that are a scale
     *  value and a geographic position, and converts it into indexation server
     *  index. It then converts back the index into geographic coordinates to
     *  show the effect of scale on the position precision.
     *
     *  Position have to be provided in decimal degrees and contained in the
     *  ]-180,180] range for longitude and ]-90,90] for the latitude. Altitude
     *  are always given in metres.
     *
     *  \param argc Main function parameters
     *  \param argv Main function parameters
     *
     *  \return Standard exit code
     */

    int main( int argc, char ** argv );

    le_void_t er_address_space( int argc, char ** argv );

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

