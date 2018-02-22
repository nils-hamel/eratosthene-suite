/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
 *
 *  Contributors
 *
 *      Ciprian Tomoiaga - ciprian.tomoiaga@gmail.com
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
    # define ER_MODEL_C        { _LE_SOCK_NULL, _LE_SIZE_NULL, _LE_TIME_NULL, ER_MODEL_STACK, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, LE_ARRAY_C, LE_ARRAY_C, _LE_TRUE }

    /* define pseudo-initialiser */
    # define ER_MODEL_I(s,p,t) { s, p, t, ER_MODEL_STACK, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, LE_ARRAY_C, LE_ARRAY_C, _LE_TRUE }

    /* define model stack */
    # define ER_MODEL_STACK    ( 6144 )

    /* define display array types */
    # define ER_MODEL_VERTEX   ( GL_DOUBLE )
    # define ER_MODEL_COLORS   ( GL_UNSIGNED_BYTE )

    /* define model color */
    # define ER_MODEL_SPACE    { 0.03, 0.04, 0.05, 0.00 }

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct er_model_struct (revoked)
     *  \brief Model structure
     *
     *  This structure is responsible of maintaining the content of earth model
     *  provided by the remote server. Its role is to maintain the earth model
     *  up to date according to the motion of the point of view.
     *
     *  The first three fields holds the socket opened toward the remote server
     *  and the two configuration values of the server. The rest of the fields
     *  are related to the cells stacks.
     *
     *  The structure holds two cells stacks, the actual stack, used by the
     *  graphical process and the target stack. The target stack is the one
     *  that is updated at high frequency, following the motion of the point
     *  of view. The actual stack, that contain the data received from the
     *  remote server, is updated with a lower frequency through synchronisation
     *  procedure.
     *
     *  The size field gives the amount of cell available in each stack while
     *  the \b mc_cell and \b md_virt are the base pointers of the actual and
     *  target stacks.
     *
     *  The update of the actual cells stack is made by the synchronisation
     *  procedure. As soon as the point of view has moved, the target stack is
     *  updated. A first process checks the cells in common between the target
     *  and actual stacks to avoid to query them. Then, the synchronisation
     *  reduces the differences between the target stack and the actual one
     *  step by step. The synchronisation continues even the point of view
     *  has not changed, until both stacks are identical.
     *
     *  Of course, the target stack is only considered in terms of addresses of
     *  cells, only the actual stack contains the proper data received form the
     *  remote server and used by the graphical processes.
     *
     *  The \b md_push field is used to keep the amount of cells that have been
     *  pushed in the target stack in order to avoid to parse it entirely. The
     *  synchronisation fields \b md_syna, \b md_synb, \b md_sync and \b md_free
     *  are used by the synchronisation method to decrease the differences
     *  between the actual and target stacks. They are hold in the structure to
     *  maintain their value between method calls.
     *
     *  This dual stack procedure is set up in order to avoid high frequency
     *  update of the actual cells stack. This allows to considerably decrease
     *  the amount of request to the server and also allows to cushion high
     *  frequency motion of the point of view.
     *
     *  A last field is used to keep the scket-array used to pack the query
     *  address and to receive the data coming from the remote server. It is
     *  hold in the structure in order to take advantage of the socket-array
     *  memory management that minimise memory allocation.
     *
     *  \var er_model_struct::md_sock
     *  Socket toward the remote server
     *  \var er_model_struct::md_scfg
     *  Server space parameter
     *  \var er_model_struct::md_tcfg
     *  Server time parameter
     *  \var er_model_struct::md_size
     *  Cells stacks size
     *  \var er_model_struct::md_push
     *  Target stack used cells
     *  \var er_model_struct::md_syna
     *  Synchronisation memory
     *  \var er_model_struct::md_synb
     *  Synchronisation boundary
     *  \var er_model_struct::md_sync
     *  Synchronisation index
     *  \var er_model_struct::md_cell
     *  Cells stack
     *  \var er_model_struct::md_virt
     *  Cells stack
     *  \var er_model_struct::md_iosa
     *  Array used for the communication with the remote server
     */

    typedef struct er_model_struct {

        le_sock_t   md_sock;
        le_size_t   md_scfg;
        le_time_t   md_tcfg;

        le_size_t   md_size;
        le_size_t   md_maxd;
        le_size_t   md_push;
        le_size_t   md_frea;
        le_size_t   md_free;
        le_size_t   md_syna;
        le_size_t   md_synb;
        le_size_t   md_sync;

        er_cell_t * md_cell;
        er_cell_t * md_virt;

        le_array_t  md_iosa;
        le_array_t  md_read;

    le_enum_t _status; } er_model_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a model structure. It initialises the
     *  structure fields and assign the provided parameters.
     *
     *  In addition, this function also allocate the two cells stack needed to
     *  maintain and update the model. The cells of both stacks are initialised.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field.
     *
     *  \param er_socket Socket toward remote server
     *  \param er_scfg   Server configuration value
     *  \param er_tcfg   Server configuration value
     *
     *  \return Created model structure
     */

    er_model_t er_model_create( le_sock_t const er_socket, le_size_t const er_scfg, le_time_t const er_tcfg );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided model structure. It deletes each
     *  cell of the stacks using their related function. It then release the
     *  stacks memory allocations and clears the structure fields using default
     *  values.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_delete( er_model_t * const er_model );

    /*! \brief accessor methods
     *
     *  This function determines if an address point to a cell that can be
     *  dropped during model update before it have been queried to the remote
     *  server.
     *
     *  The function checks in the actual cells stack if a cell contains an
     *  address that is a parent to the provided address. In this case, the
     *  function checks if the found cell contains data, the cell being assumed
     *  to contain the server answer to its address. If the found cell is
     *  empty, it implies that all its daughters are also empty. In this case,
     *  the cell pointed by the provided address can be discarded during a model
     *  update.
     *
     *  \param er_model Model structure
     *  \param er_addr  Address structure
     *
     *  \return Returns _LE_TRUE if the address can be dropped, _LE_FALSE if not
     */

    le_enum_t er_model_get_drop( er_model_t const * const er_model, le_address_t const * const er_addr );

    /* *** */

    le_enum_t er_model_get_discare( er_model_t const * const er_model, le_address_t const * const er_addr );

    /*! \brief mutator methods
     *
     *  This function is used to prepare an update of the earth model contain in
     *  the provided structure, typically after point of view motion.
     *
     *  Its role is to reset the synchronisation value, as the synchronisation
     *  will have to restart, the target cells push value and the flags of the
     *  target and actual stack cells, the flag controlling the progression of
     *  the stacks synchronisation.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_prep( er_model_t * const er_model );

    /*! \brief mutator methods
     *
     *  The role of this function is to enumerates all possible cells and to
     *  select the ones that are relevant according to the position of the point
     *  of view. A recursive enumeration strategy based on the provided address
     *  structure is applied.
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
     *  is pushed on the target stack. If the cell is judge irrelevant for
     *  display, the function calls itself to continue enumeration with the
     *  updated address.
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
     *  \param er_model Model structure
     *  \param er_enum  Enumeration address structure
     *  \param er_scale Enumeration address explicit size
     *  \param er_view  Point of view structure
     */

    le_void_t er_model_set_enum( er_model_t * const er_model, le_address_t * const er_enum, le_size_t const er_scale, er_view_t const * const er_view );

    /*! mutator methods
     *
     *  This function is used just after the target stacks updates. Its role is
     *  to rapidly checks which cells in the target stack are already in the
     *  actual stack. This allows to flag them as so, indicating to the
     *  synchronisation process not to consider these actual stack cells.
     *
     *  This function allows to re-enable actual stacks cells in order to allow
     *  them being part of the rendered model as quickly as possible. This also
     *  allows the decrease to work load of the synchronisation process and to
     *  reduce bandwidth toward remote server.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_fast( er_model_t * const er_model );

    /*! mutator methods
     *
     *  This function is responsible of the step by step synchronisation
     *  process of the target and actual cells stacks. At each function call,
     *  one step of the synchronisation is performed.
     *
     *  After a target stack update, usually following a modification of the
     *  point of view, the synchronisation index is set to zero. At each call,
     *  the function searches in the target stacks the cells that have to be
     *  queried to the remote server. It considers 64 cells before to make a
     *  query to the server. As the server answer the query, the function
     *  receives and dispatch the data in the corresponding cells of the actual
     *  stack. The process is repeated at each function call until the
     *  synchronisation is complete.
     *
     *  In the first place, the function checks if the synchronisation is done
     *  or incomplete. It case of fully synchronised stacks, the function does
     *  nothing.
     *
     *  After the last query, the function checks the actual stack for cells
     *  that were displayed at the previous state and that no more used in the
     *  current state in order to disable their graphical representation.
     *
     *  Such a process allows to be able to interrupt earth model update.
     *  Indeed, updating the model asking for all the pushed cell content lead
     *  to a process that consume time. Motion of the point of view will only
     *  be considered at end of such a global process. Using the step by step
     *  synchronisation allows to stop one mode update to ensure a better
     *  following of the point of view motion, saving also large amount of
     *  data transfer toward the remote server.
     *
     *  \param er_model Model structure
     *
     *  \return Returns _LE_TRUE on synchronised stacks, _LE_FALSE otherwise
     */

    le_enum_t er_model_set_sync( er_model_t * const er_model );

    /*! \brief display methods
     *
     *  This function displays the earth model hold in the actual stack. It
     *  searches in the actual stack the cells that have their drawable flag
     *  set.
     *
     *  The earth-attached Cartesian coordinates are related to the OpenGL
     *  Cartesian one in the following way :
     *
     *      x_geographic_cartesian -> z_opengl (0->2)
     *      y_geographic_cartesian -> x_opengl (1->0)
     *      z_geographic_cartesian -> y_opengl (2->1)
     *
     *  allowing to keep a natural view of earth without additional rotation.
     *
     *  The display strategy consist in keeping the deepest and nearest (point
     *  of view) cells as near as possible to the OpenGL frame origin. This
     *  allows to keep small values for the most relevant (point of view)
     *  points. This follows the strategy that consists in shifting the cell
     *  points to a frame that keep their coordinates small. The objective is
     *  to address single precision saturation issue. This strategy allows to
     *  consider millimetric models of earth within a single homogeneous model.
     *
     *  First, the longitude and latitude angles are used to rotate the cell
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

    le_void_t er_model_display_cell( er_model_t const * const er_model, er_view_t const * const er_view );

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

