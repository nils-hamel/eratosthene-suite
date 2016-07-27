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

    /*! \file   eratosthene-client-cell.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene client - cell module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_CELL__
    # define __ER_CLIENT_CELL__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

    # include "eratosthene-client-geodesy.h"

/*
    header - external includes
 */

    # include <math.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_CELL_C      { _LE_FALSE, _LE_FALSE, { 0 }, { 0 }, 0, NULL, NULL, { 0.0 } }

    /* Define cell array size */
    # define ER_CELL_ARRAY  884736

    /* Define cell transfert protocol */
    # define ER_CELL_TFMT   LE_ARRAY_64R
    # define ER_CELL_TLEN   LE_ARRAY_64R_LEN

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct er_cell_struct
     *  \brief cell structure
     *
     *  This structure holds the description and the data of a space-time cell
     *  associated to an equivalence class defined by the eratosthene indexation
     *  server. It also contains fields used by the rendering engine model
     *  update procedure to decrease as much as possible the network traffic.
     *
     *  More documentation on frames and coordinates system can be found in the
     *  documentation of the liberatosthene documentation.
     *
     *  \var er_cell_struct::ce_flag
     *  State of cell allocation
     *  \var er_cell_struct::ce_addr
     *  Standard address of the cell
     *  \var er_cell_struct::ce_push
     *  Pushed standard address of the cell
     *  \var er_cell_struct::ce_cell
     *  Address structure of the cell
     *  \var er_cell_struct::ce_size
     *  Element count contained in the cell arrays
     *  \var er_cell_struct::ce_pose
     *  Cell elements position array
     *  \var er_cell_struct::ce_data
     *  Cell elements color array
     *  \var er_cell_struct::ce_edge
     *  Cell edge position
     */

    typedef struct er_cell_struct {

        le_enum_t    ce_flag;
        le_enum_t    ce_draw;

        le_char_t    ce_addr[LE_NETWORK_SB_ADDR];
        le_char_t    ce_push[LE_NETWORK_SB_ADDR];

        le_size_t    ce_size;
        le_real_t *  ce_pose;
        le_data_t *  ce_data;

        le_real_t    ce_edge[3];
        
    } er_cell_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a cell structure. The cell is created
     *  using default values and the function allocates the cell array memory.
     *
     *  \return Created cell structure
     */

    er_cell_t er_cell_create( le_void_t );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes a cell structure created by the \b er_cell_create
     *  function.
     *
     *  \param er_cell Cell structure
     */

    le_void_t er_cell_delete( er_cell_t * const er_cell );

    /*! \brief accessor methods
     *
     *  Returns cell state flag.
     *
     *  \param er_cell Cell structure
     *
     *  \return Cell state flag
     */

    le_enum_t er_cell_get_flag( er_cell_t const * const er_cell );

    le_enum_t er_cell_get_draw( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     *
     *  This function returns true if the cell contains a pushed address.
     *
     *  \param er_cell Cell structure
     *
     *  \return Returns _LE_TRUE on pushed address, _LE_FALSE otherwise
     */

    le_enum_t er_cell_get_push( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     *
     *  This function checks if the pushed address of the \b er_push cell is
     *  identical to the address of the \b er_cell cell.
     *
     *  \param er_cell Cell structure
     *  \param er_push Cell structure
     *
     *  \return Returns _LE_TRUE on identity, _LE_FALSE otherwise
     */

    le_enum_t er_cell_get_match( er_cell_t const * const er_cell, er_cell_t const * const er_push );

    /*! \brief accessor methods
     *
     *  Returns the cell size, i. e. the elements count it contains.
     *
     *  \param er_cell Cell structure
     *
     *  \return Cell elements count
     */

    le_size_t er_cell_get_size( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     *
     *  Returns the cell elements positions array.
     *
     *  \param er_cell Cell structure
     *
     *  \return Cell elements positions array
     */

    le_real_t * er_cell_get_pose( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     *
     *  Returns the cell elements colors array.
     *
     *  \param er_cell Cell structure
     *
     *  \return Cell elements colors array
     */

    le_data_t * er_cell_get_data( er_cell_t const * const er_cell );

    /*! \brief mutator methods
     *
     *  Sets the cell state flags.
     *
     *  \param er_cell Cell structure
     *  \param er_flag State flag
     */

    le_void_t er_cell_set_flag( er_cell_t * const er_cell, le_enum_t const er_flag );

    le_void_t er_cell_set_draw( er_cell_t * const er_cell, le_enum_t const er_draw );

    /*! \brief mutator methods
     *
     *  This function assign the standard query address to the cell based on the
     *  provided address structure.
     *
     *  \param er_cell    Cell structure
     *  \param er_address Address structure
     */

    le_void_t er_cell_set_addr( er_cell_t * const er_cell, le_address_t const * const er_address );

    /*! \brief mutator methods
     *
     *  This function assign the standard query address to the cell push address
     *  base on the provided address structure.
     *
     *  \param er_cell    Cell structure
     *  \param er_address Address structure
     */

    le_void_t er_cell_set_push( er_cell_t * const er_cell, le_address_t const * const er_address );

    /*! \brief mutator methods
     *
     *  This function pops the pushed address of the provided cell. It simply   
     *  empty the string containing the pushed standard address.
     *
     *  \param er_cell Cell structure
     */

    le_void_t er_cell_set_pop( er_cell_t * const er_cell );

    /*! \brief mutator methods
     *
     *  This function swaps the pushed standard address of the \b er_push cell
     *  with the standard address of the \b er_addr cell.
     *
     *  \param er_addr Cell structure
     *  \param er_push Cell structure
     */

    le_void_t er_cell_set_swap( er_cell_t * const er_addr, er_cell_t * const er_push );

    /*! \brief mutator methods
     *
     *  This function empty the provided cell. It clears the cell size and
     *  standard address, but memory allocation is left unchanged.
     *
     *  \param er_cell Cell structure
     */

    le_void_t er_cell_set_empty( er_cell_t * const er_cell );

    le_size_t er_cell_io_query( er_cell_t * const er_cell, le_char_t const * const er_ip, le_sock_t const er_port );

    le_size_t er_cell_io_read( er_cell_t * const er_cell, le_sock_t const er_socket );

    /*! \brief mutator methods
     *
     *  This function performs the queries to the server based on the provided
     *  query standard address of the cell and a socket descriptor. It sends the
     *  handshake and query to the server and waits the answers.
     *
     *  In addition to receiving the server data in the cell arrays, it also
     *  computes the cell edge position that is used by the rendering engine
     *  for precision range management.
     *
     *  The server answering with elements expressed in space-time coordinates,
     *  that are longitude, latitude and radial distance, this function also
     *  is in charge of inverting this order for display purpose :
     *
     *      x_geo -> z_opengl (0->2)
     *      y_geo -> x_opengl (1->0)
     *      z_geo -> y_opengl (2->1)
     *
     *  The socket descriptor has to be already created and pointing a valid
     *  remove server.
     *
     *  \param er_cell   Cell structure
     *  \param er_socket Socket descriptor
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

