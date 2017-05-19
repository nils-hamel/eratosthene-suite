/*
 *  eratosthene-suite - client
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

    /*! \file   eratosthene-client-cell.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - client - cells module
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

    # include "eratosthene-client-common.h"
    # include "eratosthene-client-geodesy.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_CELL_C   { 0, LE_ADDRESS_C, LE_ARRAY_C, { 0.0 } }

    /* define flags */
    # define ER_CELL_QRY ( 0x01 << 0 )
    # define ER_CELL_DIS ( 0x01 << 1 )

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
     *  This structure holds the definition and data of an equivalence class
     *  obtained through a query to the remote server.
     *
     *  The structure holds two flags that are used by the model update process.
     *  The state flags indicates the usage of the cell. The drawable flag tells
     *  the graphical process if the cell has to be displayed.
     *
     *  The structure holds two address structures. The first address is used to
     *  store the address that was used to query the cell to the remote server.
     *  This address is assumed to be reduced from a temporal point of view.
     *
     *  The second address is used by the model update process to push the cells
     *  that are part of the updated model. It is done so in order to keep the
     *  previous model unchanged during update. This allows the update process
     *  to checks in the previous model if cells can be reused as they are in
     *  the updated model, reducing the bandwidth requirements between the
     *  server and the client.
     *
     *  The structure holds the array as answered by the server after cell
     *  query. The size fields hold the amount of points contained in the array
     *  for efficiency purpose.
     *
     *  At last, the structure holds a 3-vector that stores the origin point of
     *  frame in which points are expressed. This technique is used because this
     *  graphical client has to render a worldwide model. This allows to keep
     *  the coordinates of the points of the cell as small as possible, most of
     *  GPU being only able to consider single precision floating point values,
     *
     *  \var er_cell_struct::ce_flag
     *  Cell state flag
     *  \var er_cell_struct::ce_draw
     *  Cell drawable flag
     *  \var er_cell_struct::ce_addr
     *  Cell effective address
     *  \var er_cell_struct::ce_push
     *  Cell requested address
     *  \var er_cell_struct::ce_size
     *  Cell number of points
     *  \var er_cell_struct::ce_array
     *  Cell points coordinates and color array
     *  \var er_cell_struct::ce_edge
     *  Cell frame origin point coordinates
     */

    typedef struct er_cell_struct {

        le_byte_t    ce_flag;
        le_address_t ce_addr;
        le_array_t   ce_data;

        le_real_t    ce_edge[3];

    } er_cell_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a cell structure. The structure fields
     *  are initialised using default values.
     *
     *  \return Returns the created cell structure
     */

    er_cell_t er_cell_create( le_void_t );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the cell structure provided as parameter. It
     *  deletes the cell data array and clears the structures fields.
     *
     *  \param er_cell Cell structure
     */

    le_void_t er_cell_delete( er_cell_t * const er_cell );

    /* *** */

    le_byte_t er_cell_get_flag( er_cell_t const * const er_cell, le_byte_t const er_state );

    /* *** */

    le_enum_t er_cell_get_match( er_cell_t const * const er_cell, le_address_t const * const er_addr );

    /*! \brief accessor methods
     *
     *  This function returns the number of points contained in the cell data
     *  array.
     *
     *  \param er_cell Cell structure
     *
     *  \return Cell points count
     */

    le_size_t er_cell_get_count( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     *
     *  This function returns the base pointer to the cell points array. One has
     *  to take into account that the cell data array interlaces the points
     *  coordinates and colours. A stride value has to be considered to jumps
     *  from point to point.
     *
     *  \param er_cell Cell structure
     *
     *  \return Cell points coordinates array pointer
     */

    le_real_t * er_cell_get_pose( er_cell_t * const er_cell );

    /*! \brief accessor methods
     *
     *  This function returns the base pointer to the cell colour array. One has
     *  to take into account that colours are interlaced with points coordinates
     *  in the cell data array. A stride value has to be considered to jumps
     *  from colour to colour.
     *
     *  \param er_cell Cell structure
     *
     *  \return Cell points colours array pointer
     */

    le_data_t * er_cell_get_data( er_cell_t * const er_cell );

    /*! \brief accessor methods
     *
     *  This function returns a pointer to the 3-array holding the coordinates
     *  of the cell frame origin. The Points hold in the cell data array are
     *  expressed in this frame.
     *
     *  \param er_cell Cell structure
     *
     *  \return Pointer to the array of cell frame origin
     */

    le_real_t * er_cell_get_edge( er_cell_t * const er_cell );

    /* *** */

    le_void_t er_cell_set_flag( er_cell_t * const er_cell, le_byte_t const er_state );

    /* *** */
    le_void_t er_cell_set_clear( er_cell_t * const er_cell, le_byte_t const er_state );

    /* *** */

    le_void_t er_cell_set_push( er_cell_t * const er_cell, le_address_t const * const er_addr );

    /* *** */

    le_size_t er_cell_serial( er_cell_t * const er_cell, le_array_t * const er_array, le_size_t const er_offset );

    /*! \brief i/o methods
     *
     *  This function is used to perform the query to the remote server using
     *  the address of the cell. The answer of the server is stored in the cell
     *  data array.
     *
     *  In addition, the function converts the received points from geographic
     *  coordinates to earth attached Cartesian coordinates. It then shifts the
     *  points coordinates in a frame with origin given by the queried class
     *  low edge position. This allows to keep points coordinates near zeros.
     *
     *  The server always answer points in geographic coordinates that are
     *  longitude (radian), latitude (radian) and height above WGS84 ellipsoid
     *  (metres), in this order. These coordinates are converted by the function
     *  to obtain Cartesian coordinates. Considering the opengl frame, the
     *  geographic Cartesian coordinates are associated the the opengl one in
     *  the following way :
     *
     *      x_geographic -> z_opengl (0->2)
     *      y_geographic -> x_opengl (1->0)
     *      z_geographic -> y_opengl (2->1)
     *
     *  allowing to keep a natural view of earth in the opengl coordinate set.
     *
     *  \param  er_cell Cell structure
     *  \param  er_ip   Server ip address
     *  \param  er_port Server service port
     *
     *  \return Returns cell amount of received points
     */

    le_void_t er_cell_io_read( er_cell_t * const er_cell, le_array_t * const er_array );

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

