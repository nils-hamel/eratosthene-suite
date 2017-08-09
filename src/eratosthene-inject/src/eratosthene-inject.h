/*
 *  eratosthene-suite - inject
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
     *  eratosthene-suite - inject
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
     *  The principal software implements the server itself allowing to simply
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
    # define ER_INJECT ( LE_ARRAY_STEP / LE_ARRAY_UF3 )
    # define ER_INJECT_ ( 1048576 )

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
     *  that come from a file of format uf3 (universal format 3-bytes).
     *
     *  It opens and checks the provided uf3. It also prepare the injection
     *  array containing the injection time, needed at each injection segment.
     *  It then starts to read the file content by chunks and sends them to
     *  the remote server using the provided socket.
     *
     *  The injection time has to be provided in UTC through a UNIX timestamp
     *  format.
     *
     *  \param er_path   UF3 stream path
     *  \param er_time   Injection time
     *  \param er_socket Remote server socket
     *
     *  \return Returns EXIT_SUCCESS on success, EXIT_FAILURE otherwise
     */

    le_enum_t er_inject_uf3( char const * const er_path, le_time_t er_time, le_sock_t const er_socket );

    /*! \brief main function
     *
     *  The main function reads the arguments and parameters to retrieve the
     *  remote server IP address and service port. It also search for the
     *  injection time, given as a UNIX timestamp string.
     *
     *  It the creates a socket toward the remote server. After connection
     *  establishment, the function calls the specific injection function
     *  \b er_inject_uf3() searching for the UF3 path in the arguments and
     *  parameters.
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

