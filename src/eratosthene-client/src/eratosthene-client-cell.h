/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2019 DHLAB, EPFL
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
     *  eratosthene-suite - client - cell
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
    # define ER_CELL_C   { 0, LE_ADDRESS_C, LE_ARRAY_C, { 0.0 }, LE_ARRAY_C, { 0 } }

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
     *  \brief Cell structure
     *
     *  This structure holds the definition and data of an equivalence class
     *  obtained through a query to the remote server.
     *
     *  The structure holds a flag value that is used for both model update and
     *  model display. If its bit corresponding to the display bit is set, it
     *  indicates the graphical process to draw the cell. If the synchronisation
     *  bit is set, it indicates that the cell is set in both target and actual
     *  model stack, indicating it can not be considered as free.
     *
     *  The structure holds an address structure that stores the query address
     *  of the data contained in the cell. This structure is also used during
     *  model update to build queries to the remote server, mainly on the cell
     *  virtual stack.
     *
     *  The structure contains also an array structure that holds the actual
     *  data of the cell. This array is used by the model update process to
     *  retrieve the data coming from the remote server. It is also used by the
     *  graphical process to display the cell content as it contains the
     *  definition of the graphical primitive to display.
     *
     *  An edge of the cell is defined and usually set using the first element
     *  coordinates of the cell array. This position is used to translate the
     *  cell element positions to avoid overflow of single precision. This edge
     *  position is then used by the graphical process to move the cell elements
     *  on the correct display position.
     *
     *  The structure hold also to socket-array that are used to store the cell
     *  data normal vectors and displayed element index. The normals are only
     *  used for triangle primitive to apply light on them. The index array is
     *  used to create list of the element to display according to their type
     *  (point, line, triangle). This allows to use efficient display commands
     *  during cell data rendering.
     *
     *  The normal array stores the vector in double precision packed using a
     *  repetitive x,y,z sequence. The index array contains only the index for
     *  line and triangle primitives. The index are packed in two successive
     *  segments, the first one collecting the line index while the second
     *  stores the triangle index. The index are specified using the GLuint
     *  type.
     *
     *  In the last place, the cell structure holds and small array that is used
     *  to count the primitive according to their type. This allows to known how
     *  many points, lines and triangles are stored by the cell. This array is
     *  mainly used as the cell is prepared for rendering, just after its data
     *  reception and coordinates conversion.
     *
     *  \var er_cell_struct::ce_flag
     *  Flag bits field
     *  \var er_cell_struct::ce_addr
     *  Address structure
     *  \var er_cell_struct::ce_data
     *  Data array - graphical elements definition
     *  \var er_cell_struct::ce_edge
     *  Translation vector - usually the first element of the data array
     *  \var er_cell_struct::ce_norm
     *  Normal array
     *  \var er_cell_struct::ce_list
     *  Graphical primitives index
     *  \var er_cell_struct::ce_type
     *  Graphical primitives count per type
     */

    typedef struct er_cell_struct {

        le_byte_t    ce_flag;
        le_address_t ce_addr;
        le_array_t   ce_data;
        le_real_t    ce_edge[3];
        le_array_t   ce_norm;
        le_array_t   ce_list;
        le_size_t    ce_type[3];

    } er_cell_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a cell structure. The structure fields
     *  are initialised using default values.
     *
     *  The array of the cell structure are also simple initialised and their
     *  memory content is initially empty.
     *
     *  \return Returns the created cell structure
     */

    er_cell_t er_cell_create( le_void_t );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the cell structure provided as parameter. It
     *  deletes all array by releasing their memory and clears the structure
     *  fields using default values.
     *
     *  \param er_cell Cell structure
     */

    le_void_t er_cell_delete( er_cell_t * const er_cell );

    /*! \brief accessor methods
     *
     *  This function returns the bits of the cell flag field corresponding to
     *  the provided bits pattern \b er_state.
     *
     *  Practically, the function performs a logical and between the cell flag
     *  field and the provided bits pattern before to return it.
     *
     *  \param er_cell  Cell structure
     *  \param er_state Bits pattern
     */

    le_byte_t er_cell_get_flag( er_cell_t const * const er_cell, le_byte_t const er_state );

    /*! \brief accessor methods
     *
     *  This function compares the two provided cell addresses and return the
     *  _LE_TRUE value if both addresses are strictly identical and _LE_FALSE
     *  otherwise.
     *
     *  \param er_cell Cell structure
     *  \param er_targ Cell structure
     *
     *  \return Returns _LE_TRUE on addresses identity, _LE_FALSE otherwise
     */

    le_enum_t er_cell_get_equal( er_cell_t const * const er_cell, er_cell_t const * const er_targ );

    /*! \brief accessor methods
     *
     *  This function returns the number of points, that is the number of uv3
     *  records contained in the cell data array.
     *
     *  \param er_cell Cell structure
     *
     *  \return Cell data number of uv3 records
     */

    le_size_t er_cell_get_record( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     *
     *  This function returns the address size of the cell, that is the number
     *  of digits of the cell address.
     *
     *  \param er_cell Cell structure
     *
     *  \return Returns cell address size (number of digits)
     */

    le_size_t er_cell_get_size( er_cell_t const * const er_cell );

    /*! \brief accessor methods
     *
     *  This function allows to serialise the provided cell address structure
     *  in the provided socket-array at the specified offset.
     *
     *  After serialisation, the function returns the next serialisation offset
     *  of the array.
     *
     *  \param er_cell   Cell structure
     *  \param er_array  Array structure
     *  \param er_offset Serialisation offset, in bytes
     *
     *  \return Returns the array next serialisation offset
     */

    le_size_t er_cell_get_sync( er_cell_t * const er_cell, le_array_t * const er_array, le_size_t const er_offset );

    /*! \brief accessor methods
     *
     *  This function allows to query the rendering of the cell data. It is
     *  usually used by the main graphical process during cell stack display.
     *
     *  In the first place, the function computes the translation to apply to
     *  the cell content according to the cell edge vector and the position of
     *  the point of view. The point of view is provided to the function using
     *  trigonometric representations of the longitude and latitude for
     *  efficiency purpose.
     *
     *  The function then applies the translation and position rotation before
     *  to check which primitive is present in the provided cell data array.
     *
     *  The point are the first to be display as the server ensuring that they
     *  always come first in the socket-arrays. The lines and triangle are drawn
     *  in this order just after the points rendering. During triangle rendering
     *  the OpenGL light is enabled and disabled just after rendering.
     *
     *  If one type of graphical primitive is not found, the function simply
     *  ignores their rendering.
     *
     *  \param er_cell Cell structure
     *  \param er_lon  Point of view longitude
     *  \param er_lat  Point of view latitude
     *  \param er_cosl Point of view longitude cosine
     *  \param er_sinl Point of view longitude sine
     *  \param er_cosa Point of view latitude cosine
     *  \param er_sina Point of view latitude sine
     */

    le_void_t er_cell_get_render( er_cell_t * const er_cell, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_cosl, le_real_t const er_sinl, le_real_t const er_cosa, le_real_t const er_sina );

    /*! mutator methods
     *
     *  This function allows to set bits of the flag field of the provided cell
     *  structure. The provided \b er_state bits pattern is composed with the
     *  cell flag field using a logical or operator.
     *
     *  \param er_cell  Cell structure
     *  \param er_state Bits pattern
     */

    le_void_t er_cell_set_flag( er_cell_t * const er_cell, le_byte_t const er_state );

    /*! mutator methods
     *
     *  This function allows to clear the bits of the flag field of the cell
     *  provided as parameter. The logical not of the \b er_state bits pattern
     *  is composed with the flag field using a logical and operator.
     *
     *  \param er_cell  Cell structure
     *  \param er_state Bits pattern
     */

    le_void_t er_cell_set_zero( er_cell_t * const er_cell, le_byte_t const er_state );

    /*! mutator methods
     *
     *  This function assigns the provided address structure to the provided
     *  cell structure address by a simple copy. The address of the cell is then
     *  erased by the copy.
     *
     *  \param er_cell Cell structure
     *  \param er_addr Address structure
     */

    le_void_t er_cell_set_push( er_cell_t * const er_cell, le_address_t const * const er_addr );

    /*! mutator methods
     *
     *  This function allows to un-serialise the address from the provided array
     *  to the provided cell structure address. The provided offset is used to
     *  find the serialisation array offset. The function returns the array next
     *  un-serialisation offset.
     *
     *  \param er_cell   Cell structure
     *  \param er_array  Array structure
     *  \param er_offset Serialisation offset
     *
     *  \return Returns the array next un-serialisation offset
     */

    le_size_t er_cell_set_sync( er_cell_t * const er_cell, le_array_t * const er_array, le_size_t const er_offset );

    /*! mutator methods
     *
     *  This function allows to use the data socket-array of the provided cell
     *  structure for reading data from the remote server through the provided
     *  socket descriptor. This function is usually used during model update
     *  to retrieve the queried data.
     *
     *  The data socket-array of the provided cell is then erased during the
     *  reading operation and its previous content discarded.
     *
     *  \param er_cell   Cell structure
     *  \param er_socket Remote server socket descriptor
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t er_cell_set_read( er_cell_t * const er_cell, le_sock_t const er_socket );

    /*! mutator methods
     *
     *  This function process the array received from the remote server after
     *  a query on the provided cell structure.
     *
     *  The cell edge vector is computed by considering the data array first
     *  element. It is then converted from the geographic coordinates to the
     *  associated Cartesian frame.
     *
     *  The rest of the elements contained in the cell data array are then
     *  processed. Each element (uv3 record) is converted, in terms if its
     *  coordinates, from geographic to Cartesian coordinates, as expected by
     *  the display functions.
     *
     *  The function takes advantage of the loop performed on the data array to
     *  compute element count per type (point, line and triangle).
     *
     *  The incoming data array is assumed to provide the longitude in radian,
     *  the latitude in radian, and the meters above the WGS84 ellipsoid. These
     *  values are expected in this order and are mixed during conversion to be
     *  adapted to the display frame. To summarise, the mix of dimension occurs
     *  as follows :
     *
     *      x_geographic_cartesian -> z_OpenGL (0->2)
     *      y_geographic_cartesian -> x_OpenGL (1->0)
     *      z_geographic_cartesian -> y_OpenGL (2->1)
     *
     *  This is done to obtain a natural view in the OpenGL frame.
     *
     *  In the last place, the function invokes the specialised process that
     *  computes the normal array and the primitive display index required by
     *  the rendering process.
     *
     *  The function returns finally the size of the array of the provided cell
     *  in bytes.
     *
     *  \param er_cell Cell structure
     *
     *  \return Returns cell array size in bytes
     */

    le_size_t er_cell_set_data( er_cell_t * const er_cell );

    /*! mutator methods
     *
     *  This function initialises and computes the provided cell normal and
     *  index array based on the analysis of the cell data array content.
     *
     *  The normal and index array memory is corrected, when needed, according
     *  to the content of the cell data array content.
     *
     *  The function performs then a loop on the data array to associate each
     *  uv3 record corresponding to triangle a normal vector. The index array
     *  is filled along with the normal computation loop.
     *
     *  The computation of the normals of the triangles are performed by simply
     *  considering the triangle edges to compute a cross-product. The obtained
     *  normal are then normalised to norm one.
     *
     *  \param er_cell Cell structure
     */

    le_void_t er_cell_set_render( er_cell_t * const er_cell );

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

