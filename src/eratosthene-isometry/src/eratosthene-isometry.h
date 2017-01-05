/*
 *  eratosthene-suite - isometry
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

    /*! \file   eratosthene-isometry.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite isometry
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

    # include "eratosthene-isometry-render.h"

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

    /*! \brief isometry function
     *
     *  This function drives the rendering of the isometric projection of the
     *  data contained in the array resulting of a query on a remote server.
     *
     *  It expects the main function standard parameters in addition to the data
     *  array. It reads in the standard parameters the configuration of the
     *  isometric projection and starts the rendering process.
     *
     *  It calls the rendering module functions that use opengl to draw the
     *  projection and saves the result into a portable network graphics file.
     *
     *  \param argc     Main function parameters
     *  \param argv     Main function parameters
     *  \param er_array Query data array
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_isometry( int argc, char ** argv, le_array_t * const er_array );

    /*! \brief main function
     *
     *  The main function establishes the connexion to the server using the
     *  address and service port provided as standard parameters. It then sends
     *  the client query handshake.
     *
     *  As the handshake succeeded, the main function sends the query address
     *  provided as standard parameter. It then waits the server for reduced
     *  address and data array. The array is then sent to the specialised
     *  rendering function for isometry projection computation.
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

