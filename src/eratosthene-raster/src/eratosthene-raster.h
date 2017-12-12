/*
 *  eratosthene-suite - raster
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

    /*! \file   eratosthene-raster.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - raster
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

    # ifndef __ER_ISOMETRY__
    # define __ER_ISOMETRY__

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
    # include <math.h>
    # include <eratosthene-include.h>

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

    /*! \brief raster method
     *
     *  This function computes and export a 3D raster based on the provided
     *  array. The array has to be the answered array by a remote server using
     *  the provided address structure as query.
     *
     *  The function parses the array point and compose a 0/1 3D raster that it
     *  export using the provided path. The raster size, in terms of index,
     *  corresponds to two to the power of the address span. Each point of the
     *  array is then placed in the raster according to its position.
     *
     *  \param er_path   Path to the raster file
     *  \param er_addr   Address structure containing the query
     *  \param er_array Array received from remote server
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_raster( le_char_t const * const er_path, le_address_t const * const er_addr, le_array_t * const er_array );

    /*! \brief main function
     *
     *  This software is used to compute 3D raster from the cells queried to a
     *  remote server :
     *
     *      ./eratosthene-raster --raster/-r --query/-q --ip/-i --port/-p
     *
     *  The main function starts by establishing a connection to the remote
     *  server using the provided ip address and port number. The main function
     *  performs the authentication procedure.
     *
     *  The main function the reads the provided query address string and pack
     *  it in a array that is then send to the remote server. The answer array
     *  is read and decoded.
     *
     *  The main function finally invoke the raster creation process providing
     *  it the query address, the received array and the path to the file in
     *  which the raster is dumped.
     *
     *  \param argc Main function parameters
     *  \param argv Main function parameters
     *
     *  \return Standard exit code
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

