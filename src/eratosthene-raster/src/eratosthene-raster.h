/*
 *  eratosthene-suite - raster
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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
     *  \section overview Overview
     *
     *  The _eratosthene-suite_ offers a front-end to the eratosthene indexation
     *  server implemented in the _liberatosthene_ library. It consists in a
     *  softwares suite offering front-end solutions to the functionalities of
     *  the eratosthene library.
     *
     *  The principal software implements the server itself allowing to simply
     *  create instances and maintain services. The suite also offers a
     *  front-end software for the data injection in the available servers. It
     *  also offers a graphical software allowing to browse the 4D worlds
     *  offered by the available servers.
     *
     *  Other tools are also available as the isometric projection creation and
     *  the address and time conversion softwares.
     *
     *  \section project Eratosthene Project
     *
     *  The Eratosthene Project was initially imagined to answer the question of
     *  very large point-based models of cities and environments. Indeed, mapping
     *  a whole city at the level of the centimetre induces very large and heavy
     *  models that are difficult to manipulate, explore and exploit. Moreover,
     *  considering the time dimension implies a quick increase of the size of
     *  the models, making them almost impossible to handle as a single object.
     *
     *  The Eratosthene Project is designed to answer this question by
     *  implementing an space-time indexation formalism able to handle, store
     *  and access very large 4D models of cities and environments. The
     *  indexation formalism is used to implement a server able to manage,
     *  store and broadcast very large 4D models distributed all over the earth
     *  surface and covering large period of time. In addition to the server,
     *  the Eratosthene Project also implements a graphical client able to
     *  communicate with the server and to offer a convenient way of browsing
     *  the models in both space and time.
     *
     *  The space-time indexation formalism is designed to efficiently manage
     *  the storage of the incoming 4D models and to offer and simple and
     *  efficient ways of addressing queries to the server allowing models
     *  broadcasting. The space-time indexation also allows to perform queries
     *  to the server in a intuitive way giving the possibility to both address
     *  the questions of the earth structures levels of detail and time
     *  management. This allows to explore the entire earth surface in terms of
     *  4D structures from large mountain chains in 2000 to very small details
     *  of cities in 1850.
     *
     *  \section license Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016-2018 DHLAB, EPFL
     *
     *  This program is licensed under the terms of the GNU GPLv3.
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_RASTER__
    # define __ER_RASTER__

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
    # include <string.h>
    # include <math.h>
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

    /*! \brief auxiliary methods
     *
     *  This function computes and returns the exportation path of computed
     *  raster based on the provided directory path and raster address string.
     *  Using these elements, the function composes the exportation path by
     *  appending the address string to the provided directory path.
     *
     *  In the first place, the raster address string is modified to replace the
     *  '/' and ',' characters by simple minus symbols.
     *
     *  \param er_addr Raster address string
     *  \param er_path Exportation directory path
     *
     *  \return Returns the composed exportation path
     */

    le_char_t * er_raster_path( le_char_t * const er_addr, le_char_t const * const er_path );

    /*! \brief raster method
     *
     *  This function computes and export a 3D raster based on the provided
     *  array. The array has to be the answered array by a remote server using
     *  the provided address structure as query.
     *
     *  The function parses the array points and composes the binary 3D raster
     *  that it exports using the provided path. The raster size, in terms of
     *  array, boundaries, corresponds to two to the power of the address span.
     *  Each point of the array is then placed in the raster according to its
     *  position in the limit of the discretisation of the spatial index and
     *  span.
     *
     *  \param er_path  Raster exportation path
     *  \param er_addr  Address structure
     *  \param er_array Remote server array
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_raster( le_char_t const * const er_path, le_address_t const * const er_addr, le_array_t * const er_array );

    /*! \brief enumeration methods
     *
     *  This function is used to query more than one raster based on the unique
     *  address provided to the function. Based on the provided target scale,
     *  the function recursively enumerates all the sub-cells of the original
     *  address up to the specified depth. For each enumerated cell, the
     *  function queries the data to the remote server and triggers the
     *  computation of the corresponding raster.
     *
     *  Before any raster computation and exportation, the function checks the
     *  amount of points of the queried cell. If the amount of point is less
     *  than the provided limit value, the raster is not computed and not
     *  exported.
     *
     *  In addition, during enumeration process, as the target scale is not
     *  reached, the function performs a query towards the remote server on the
     *  actual state of the address to check its content. If the address leads
     *  to an empty cell, it follows that any of its subsequent cell is empty
     *  and the address is dropped from enumeration.
     *
     *  \param er_addr   Address structure (enumeration)
     *  \param er_scale  Current scale
     *  \param er_target Target scale
     *  \param er_limit  Exportation limit
     *  \param er_path   Exportation directory path
     *  \param er_socket Socket to the remote server
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_raster_enum( le_address_t * const er_addr, le_size_t const er_scale, le_size_t const er_target, le_size_t const er_limit, le_char_t const * const er_path, le_sock_t const er_socket );

    /*! \brief main function
     *
     *  This software is used to compute and export rasters from cells queried
     *  to the specified remote server :
     *
     *      ./-raster --ip/-i, --port/-p [remote server access]
     *                 --query/-q [query string specification]
     *                 --depth/-d [enumeration depth specification]
     *                 --export/-e [raster exportation path]
     *                 --limit/-l [raster minimal size]
     *
     *  The main function starts by establishing a connection to the remote
     *  server using the provided ip address and port number. The main function
     *  performs the authentication procedure to retrieve the parameter of the
     *  server configuration.
     *
     *  The main function then reads the provided address strings and depth
     *  value that is uses to invoke the raster enumeration procedure. The
     *  address is considered as the mother cell while the depth is used to
     *  compute the target scale at which the enumerated address are used to
     *  perform query and to compute the subsequent rasters.
     *
     *  In addition, the limit value provided as parameter is also sent to the
     *  enumeration process. This value is used to discard all cells that have
     *  an amount of point that is less than the limit value. The default value
     *  is zero, meaning all cells are used to compute and export raster, even
     *  when empty.
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

