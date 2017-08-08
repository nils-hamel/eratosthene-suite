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
    # define ER_CELL_SYN ( 0x01 << 0 )
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
     *  The structure holds a flag value that is used for both model update and
     *  model display. If its bit corresponding to the display bit is set, it
     *  indicates the graphical process to draw the cell. If the synchronisation
     *  bit is set, it indicates that the cell is set in both target and actual
     *  model.
     *
     *  The structure holds an address structure that stores the query address
     *  of the data contained in the cell. This structure is also used during
     *  model update to build queries to the remote server.
     *
     *  The structure contains also an array structure that holds the actual
     *  data of the cell. This array is used by the model update process to
     *  retrieve and decompress the data coming from the remote server. It is
     *  also used by the graphical process to display the cell content.
     *
     *  The last field hold by the structure is the cell edge. This vector is
     *  used to translate the coordinates contained in the cell data array to
     *  avoid overflow of floating point precision, the entire earth displayed
     *  at the level of the centimetre being beyond the capacity of the single
     *  precision used by OpenGL. Practically, this vector contains the first
     *  point received from the remote server instead of the mathematical edge
     *  of the cell pointed by the address structure. This vector is used as the
     *  cell is drawn.
     *
     *  \var er_cell_struct::ce_flag
     *  Cell flag bits
     *  \var er_cell_struct::ce_addr
     *  Cell address structure
     *  \var er_cell_struct::ce_data
     *  Cell array containing the data
     *  \var er_cell_struct::ce_edge
     *  Cell translation vector
     */

    typedef struct er_cell_struct {

        le_byte_t    ce_flag;
        le_address_t ce_addr;
        le_array_t   ce_data;
        le_real_t    ce_edge[6];

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
     *  deletes the cell data array and clears the structures fields using
     *  default values.
     *
     *  \param er_cell Cell structure
     */

    le_void_t er_cell_delete( er_cell_t * const er_cell );

    /*! \brief accessor methods
     *
     *  This function returns the bits of the cell flag field corresponding to
     *  the provided bit pattern \b er_state.
     *
     *  Practically, the function performs a logical and between the cell flag
     *  field and the provided bit pattern.
     *
     *  \param er_cell  Cell structure
     *  \param er_state Bits pattern
     */

    le_byte_t er_cell_get_flag( er_cell_t const * const er_cell, le_byte_t const er_state );

    /*! accessor methods
     *
     *  This function compares the two provided cell addresses and return the
     *  _LE_TRUE value if both address are strictly identical and _LE_FALSE
     *  otherwise.
     *
     *  \param er_cell Cell structure
     *  \param er_targ Cell structure
     *
     *  \return Returns _LE_TRUE on address identity, _LE_FALSE otherwise
     */

    le_enum_t er_cell_get_equal( er_cell_t const * const er_cell, er_cell_t const * const er_targ );

    /*! \brief accessor methods
     *
     *  This function returns the number of points contained in the cell data
     *  array.
     *
     *  \param er_cell Cell structure
     *
     *  \return Cell points (UF3 records) count
     */

    le_size_t er_cell_get_record( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     *
     *  This function returns the address size of the cell, that is the number
     *  of digits of the cell address.
     *
     *  \param er_cell Cell structure
     *
     *  \return Returns cell address size
     */

    le_size_t er_cell_get_size( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     *
     *  This function returns the base pointer to the cell points array. One has
     *  to take into account that the cell data array interlaces the points
     *  coordinates and colours. A stride value has to be considered to jump
     *  from points to points.
     *
     *  \param er_cell Cell structure
     *
     *  \return Cell points coordinates base pointer
     */

    le_real_t * er_cell_get_pose( er_cell_t * const er_cell );

    /*! \brief accessor methods
     *
     *  This function returns the base pointer to the cell colour array. One has
     *  to take into account that colours are interlaced with points coordinates
     *  in the cell data array. A stride value has to be considered to jump
     *  from colours to colours.
     *
     *  \param er_cell Cell structure
     *
     *  \return Cell points colours base pointer
     */

    le_data_t * er_cell_get_data( er_cell_t * const er_cell );

    /*! \brief accessor methods
     *
     *  This function returns a pointer to the 3-array holding the coordinates
     *  of the cell translation vector. The points hold in the cell data array
     *  are expressed in a frame having this translation vector as origin.
     *
     *  \param er_cell Cell structure
     *
     *  \return Pointer to the array of cell frame origin
     */

    le_real_t * er_cell_get_edge( er_cell_t * const er_cell );

    /*! mutator methods
     *
     *  This function allows to set bits of the flag field of the provided cell
     *  structure. The provided \b er_state bits pattern is composed with the
     *  cell flag field using a logical or.
     *
     *  \param er_cell  Cell structure
     *  \param er_state Bits pattern
     */

    le_void_t er_cell_set_flag( er_cell_t * const er_cell, le_byte_t const er_state );

    /*! mutator methods
     *
     *  This function allows to clears the bits of the flag field of the cell
     *  provided as parameter. The logical not of the \b er_state bits pattern
     *  is composed with the flag field using a logical and.
     *
     *  \param er_cell  Cell structure
     *  \param er_state Bits pattern
     */

    le_void_t er_cell_set_zero( er_cell_t * const er_cell, le_byte_t const er_state );

    /*! mutator methods
     *
     *  This function assign the provided address structure to the provided cell
     *  address by a simple copy.
     *
     *  \param er_cell Cell structure
     *  \param er_addr Address structure
     */

    le_void_t er_cell_set_push( er_cell_t * const er_cell, le_address_t const * const er_addr );

    /*! mutator methods
     *
     *  This function synchronises the cell address with the provided target
     *  cell address. The target cell address is simply copied in the cell
     *  structure.
     *
     *  \param er_cell Cell structure
     *  \param er_targ Target cell structure
     */

    le_void_t er_cell_set_sync( er_cell_t * const er_cell, er_cell_t const * const er_targ );

    /*! mutator methods
     *
     *  This function allows to clear the content of the array of the provided
     *  cell. It uses the array specific function to clear the content of the
     *  data array.
     *
     *  \param er_cell Cell structure
     */

    le_void_t er_cell_set_empty( er_cell_t * const er_cell );

    /*! mutator methods
     *
     *  This function process the array received from the remote server after
     *  a query.
     *
     *  The incoming array is first decompressed. The cell edge is computed
     *  by considering the array first point. It is then converted from the
     *  geographic coordinates to cartesian coordinates.
     *
     *  The rest of the point contained in the array are then processed. Each
     *  point is converted, in terms if its coordinates, from geographic to
     *  cartesian, as expected by the display functions.
     *
     *  The incoming array is assumed to provided the longitude, in radian, the
     *  latitude, in radian, and the height above the WGS84 ellipsoid, expressed
     *  in metre. These values are expected in this order and are mixed during
     *  conversion to be adapted to the display frame. To summarise, the mix
     *  of dimension occurs as follows :
     *
     *      x_geographic_cartesian -> z_opengl (0->2)
     *      y_geographic_cartesian -> x_opengl (1->0)
     *      z_geographic_cartesian -> y_opengl (2->1)
     *
     *  This is done to obtain a natural view in the OpenGL frame.
     *
     *  \param er_cell  Cell structure
     *  \param er_array Array structure
     */

    le_void_t er_cell_set_data( er_cell_t * const er_cell, le_array_t * const er_array );

    /*! \brief serialisation methods
     *
     *  This function allows to serialise the address of the provided cell in
     *  the provided array structure at the offset provided through the
     *  \b er_offset parameter.
     *
     *  The function uses the serialisation method of the address structure to
     *  pack them in the array. The used method returned value is then return
     *  by this function.
     *
     *  \param er_cell   Cell structure
     *  \param er_array  Array structure
     *  \param er_offset Serialisation offset, in bytes
     *
     *  \return Returns serialisation corrected offset
     */

    le_size_t er_cell_serial( er_cell_t * const er_cell, le_array_t * const er_array, le_size_t const er_offset );

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

