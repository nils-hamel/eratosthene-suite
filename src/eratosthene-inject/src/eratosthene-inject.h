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

    /*! \file   eratosthene-inject.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite inject
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

    # ifndef __ER_INJECT__
    # define __ER_INJECT__

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

    /* define segmentation size */
    # define ER_INJECT ( 4096 )

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

    /*! \brief injection procedure
     *
     *  This function is responsible of data injection in the specified server
     *  that come from a file of format uf3 (universal format 3-byte).
     *
     *  The function reads the injection time parameter provided as standard
     *  parameter. It the opens the uf3 input file using the path provided as
     *  standard parameter.
     *
     *  The function then establish the connexion to the server using the tcp/ip
     *  address and the port provided as standard parameters. It sends to the
     *  server the injection handshake and, on success, starts to send the
     *  content of the file by segments of size \b ER_INJECT. It ends injecting
     *  the content as the end of the uf3 file is reached. The last data segment
     *  size can be smaller than \b ER_INJECT.
     *
     *  \param argc Main function parameters
     *  \param argv Main function parameters
     *
     *  \return Standard exit code
     */

    le_enum_t er_inject_uf3( int argc, char ** argv );

    /*! \brief main function
     *
     *  The main function searches in the arguments and parameters which switch
     *  is provided. Depending on the found switch, the main function calls the
     *  specialised injection process.
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

