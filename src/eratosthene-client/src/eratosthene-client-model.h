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

    /*! \file   eratosthene-client-model.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - client - model module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_MODEL__
    # define __ER_CLIENT_MODEL__

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
    # include "eratosthene-client-cell.h"
    # include "eratosthene-client-geodesy.h"
    # include "eratosthene-client-view.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_MODEL_C        { _LE_SOCK_NULL, _LE_SIZE_NULL, _LE_TIME_NULL, ER_MODEL_STACK, 0, 0, NULL, LE_ARRAY_C, _LE_TRUE }

    # define ER_MODEL_C_       { _LE_SOCK_NULL, _LE_SIZE_NULL, _LE_TIME_NULL, ER_MODEL_STACK, 0, 0, NULL, NULL, LE_ARRAY_C, _LE_TRUE }

    /* define pseudo-initialiser */
    # define ER_MODEL_I(s,p,t) { s, p, t, ER_MODEL_STACK, 0, 0, NULL, LE_ARRAY_C, _LE_TRUE }

    # define ER_MODEL_I_(s,p,t) { s, p, t, ER_MODEL_STACK, 0, 0, NULL, NULL, LE_ARRAY_C, _LE_TRUE }

    /* define model stack */
    # define ER_MODEL_STACK    ( 4096 )

    /* define display array types */
    # define ER_MODEL_VERTEX   ( GL_DOUBLE )
    # define ER_MODEL_COLORS   ( GL_UNSIGNED_BYTE )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct er_model_struct
     *  \brief Model structure
     *
     *  This structure is responsible of maintaining the content of earth model
     *  provided by the remote server. Its main role is to maintain the cells
     *  stack. Each cell correspond to a query made to the remote server using
     *  a single address. The model is constantly updated, following the model
     *  point of view motion.
     *
     *  In the first place, the structure holds the remote server address and
     *  service port as it is responsible of queries made to the remote server.
     *
     *  The structure holds also a copy of the remote server configuration
     *  values that are the space and time parameters.
     *
     *  The main element of the structure is the cell stack. The stack has a
     *  predefined size (\b ER_MODEL_STACK) and the model has to be fitted in.
     *  The size field holds a copy of the cell stack and has to be used by
     *  the processes using the stack.
     *
     *  A field stores the model push index. It is used as the model is updated
     *  following a modification of the point of view. Its role is to trace the
     *  count of pushed addresses waiting for validation and query.
     *
     *  At the last place, the structure holds a cyclic parameter. It is used
     *  by a specific function responsible of searching free stack cells during
     *  model update. Its role is to ensure an homogeneous usage of the stack
     *  and to increase the time a cell spends in the stack, and especially
     *  inactive cells. This increases the probability for a cell to be reused
     *  without a query to the remote server even if no more active in the
     *  current model, reducing bandwidth usage.
     *
     *  \var er_model_struct::md_svip
     *  Server ip address
     *  \var er_model_struct::md_port
     *  Server service port
     *  \var er_model_struct::md_scfg
     *  Server space parameter
     *  \var er_model_struct::md_tcfg
     *  Server time parameter
     *  \var er_model_struct::md_size
     *  Cells stack size
     *  \var er_model_struct::md_push
     *  Cells stack push index
     *  \var er_model_struct::md_cell
     *  Cells stack
     *  \var er_model_struct::md_cycle
     *  Cyclic search index
     */

    typedef struct er_model_struct {

        le_sock_t   md_socket;

        le_size_t   md_scfg;
        le_time_t   md_tcfg;

        le_size_t   md_size;
        le_size_t   md_head;
        le_size_t   md_tail;
        er_cell_t * md_cell;

        le_array_t  md_read;

    le_enum_t _status; } er_model_t;

    typedef struct er_model_struct_ {

        le_sock_t    md_sock;

        le_size_t    md_scfg;
        le_time_t    md_tcfg;

        le_size_t    md_size;
        le_size_t    md_push;
        le_size_t    md_sync;
        er_cell_t_ * md_cell;
        er_cell_t_ * md_targ;

        le_array_t   md_read;

    le_enum_t _status; } er_model_t_;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a model structure. It initialises the
     *  structure fields and assign the provided parameters.
     *
     *  The function performs also a configuration query to the remote server
     *  to retrieve the two space and time configuration values. A copy of the
     *  two values are assigned to the structure.
     *
     *  This function is also responsible for allocating the cells stack memory
     *  and for the initialisation of the stack cell structures.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field.
     *
     *  \param er_ip    Server ip address
     *  \param er_port  Server service port
     *
     *  \return Created model structure
     */

    er_model_t_ er_model_create_( le_sock_t const er_socket, le_size_t const er_scfg, le_time_t const er_tcfg );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided model structure. It deletes each
     *  cell of the stack using their related function. It then release the
     *  stack memory allocation and clears the structure fields.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_delete_( er_model_t_ * const er_model );

    /* *** */

    le_void_t er_model_set_prep_( er_model_t_ * const er_model );

    /*! \brief mutator methods
     *
     *  This function is part of the model update process.
     *
     *  Its role is to enumerates all possible cells and to select the ones that
     *  are relevant according to the position of the point of view. A recursive
     *  enumeration strategy based on the provided address structure is applied.
     *
     *  Starting at scale zero, the possible digits are enumerated. The size of
     *  the address (zero at the beginning) is increased by one. On the first
     *  \b ER_COMMON_ENUM + 1 scales, the function simply calls itself passing
     *  the updated address as parameter. Otherwise, the function checks if the
     *  cell belong to the earth face (relevant portion of earth surface for
     *  display according to point of view position).
     *
     *  If the cell does not belong to the earth face, it is simply discarded
     *  and no recursion occurs (all the cell daughters are discarded at the
     *  same time). If the cell belong to earth face, the function checks if it
     *  is relevant for display according to the point of view. If it is and its
     *  distance to point of view satisfy the criterion, the enumeration address
     *  is pushed on the stack. If the cell is judge not relevant for display,
     *  the function calls itself to continue enumeration with the updated
     *  address.
     *
     *  To be selected during enumeration, the cell has to be judged relevant
     *  for display, in terms of size and point density, and has to pass through
     *  two distance tests. Despite the two distance tests look similar, one is
     *  performed before relevance test while the second is made after. This is
     *  so for the following reason : if the second test replaces the first one,
     *  a cell can be discarded as judge to far always while its an effect of
     *  its large size. This would discard all its daughters cells that could
     *  have satisfied the criterion at their own scale.
     *
     *  In addition, as a cell is judged not relevant for display, the function
     *  checks if the cell contains data before to calls itself to continue
     *  the enumeration. Indeed, if a cell is empty all its daughters are also
     *  empty and then not relevant for enumeration. The same test is made
     *  before to push the enumeration address on the stack when a cell is
     *  judged relevant for display.
     *
     *  The function tests if cells are empty before to push them on the stack
     *  by making a time-reduction query to the remote server. In addition to
     *  avoid pushing empty cell, these queries allows to reduce the time values
     *  hold by the enumeration address. It follows that any pushed address
     *  contains implicitly reduced times. This allows subsequent model update
     *  functions to properly compare pushed addresses with addresses already in
     *  the stack.
     *
     *  \param er_model Model structure
     *  \param er_enum  Address structure
     *  \param er_view  Point of view structure
     */

    le_void_t er_model_set_enum_( er_model_t_ * const er_model, le_address_t * const er_enum, le_size_t const er_scale, er_view_t const * const er_view );

    le_void_t er_model_set_fast_( er_model_t_ * const er_model );

    le_void_t er_model_set_sync_( er_model_t_ * const er_model );

    /*! \brief display methods
     *
     *  This function displays the earth model hold in the stack. It searches
     *  in the stack the cells that have their drawable flag set.
     *
     *  The earth-attached Cartesian coordinates set [x_e,y_e,z_e] is associated
     *  to the opengl Cartesian coordinates set [x_g,y_g,z_g] in the following
     *  way :
     *
     *      x_e = z_g
     *      y_e = x_g
     *      z_e = y_g
     *
     *  allowing to keep a natural view of earth without additional rotation.
     *
     *  The display strategy consist in keeping the deepest and nearest (point
     *  of view) cells as near as possible to the opengl frame origin. This
     *  allows to keep small values for the most relevant (point of view)
     *  points. This follows the strategy that consists in shifting the cell
     *  points to a frame that keep their coordinates small. The objective is
     *  to address single precision saturation issue. This strategy allows to
     *  consider millimetric model of earth within a single homogeneous model.
     *
     *  First, the longitute and latitude angle are used to rotate the cell
     *  points. A specific translation has to be computed then : as the points
     *  of the cell are expressed in the frame of the cell itself, the position
     *  of this frame origin in the model has to be computed in order to
     *  correctly place the cell points : the translation is obtained by
     *  applying the longitude and latitude rotation to the cell frame origin.
     *  The azimuthal angle is then applied to the points and the entire earth
     *  is shifted backward to keep the earth surface below the point of view
     *  near the opengl (0,0,0) point. Finally, the tilt (gamma) angle is
     *  applied.
     *
     *  \param er_model Model structure
     *  \param er_view  View structure
     */

    le_void_t er_model_display_cell_( er_model_t_ const * const er_model, er_view_t const * const er_view );

    /*! \brief display methods
     *
     *  This function displays a simple wireframe model of earth to provide a
     *  permanent visual reference.
     *
     *  More information on the earth display strategy are available on the
     *  documentation of \b er_model_display_cell() function.
     *
     *  \param er_view View structure
     */

    le_void_t er_model_display_earth( er_view_t const * const er_view );

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

