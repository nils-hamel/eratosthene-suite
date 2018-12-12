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
     *  eratosthene-suite - client - model
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
    # define ER_MODEL_C        { _LE_SOCK_NULL, _LE_SIZE_NULL, _LE_TIME_NULL, ER_MODEL_STACK, 0, 0, 0, 0, NULL, NULL, LE_ARRAY_C, _LE_FALSE }

    /* define pseudo-initialiser */
    # define ER_MODEL_I(s,p,t) { s, p, t, ER_MODEL_STACK, 0, 0, 0, 0, NULL, NULL, LE_ARRAY_C, _LE_FALSE }

    /* define model stack */
    # define ER_MODEL_STACK    ( 6144 )

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
     *  This structure is responsible of maintaining the content of Earth model
     *  provided by the remote server. Its role is to maintain the Earth model
     *  up to date according to the motion of the point of view.
     *
     *  The first three fields hold the socket opened toward the remote server
     *  and the two configuration values of the server. The rest of the fields
     *  are related to the cells stacks management.
     *
     *  The structure holds two cells stacks, the actual stack, used by the
     *  graphical process and the target stack. The target stack is the one
     *  that is updated at high frequency, following the motion of the point
     *  of view. The actual stack, that contain the data received from the
     *  remote server, is updated with a lower frequency through synchronisation
     *  procedure.
     *
     *  The size field gives the amount of cells available in each stack while
     *  the \b md_cell and \b md_virt are the base pointers of the actual and
     *  target stacks.
     *
     *  The update of the actual cells stack is made by the synchronisation
     *  procedure. As soon as the point of view has moved, the target stack is
     *  updated. A first process checks the cells in common between the target
     *  and actual stacks to avoid to query them. Then, the synchronisation
     *  reduces the differences between the target stack and the actual one
     *  step by step. The synchronisation continues, even if the point of view
     *  has not changed, until both stacks are identical.
     *
     *  Of course, the target stack is only considered in terms of addresses of
     *  cells, only the actual stack contains the proper data received form the
     *  remote server and used by the graphical processes.
     *
     *  The \b md_push field is used to keep the amount of cells that have been
     *  pushed in the target stack in order to avoid to parse it entirely. The
     *  synchronisation fields \b md_syna, \b md_synb and \b md_free are used by
     *  the synchronisation method to decrease the differences between the
     *  actual and target stacks. They are hold in the structure to maintain
     *  their value between method calls.
     *
     *  This dual stack procedure is set up in order to avoid high frequency
     *  update of the actual cells stack. This allows to considerably decrease
     *  the amount of requests to the server and also allows to cushion high
     *  frequency motion of the point of view.
     *
     *  The last field is a socket-array used for the communication with the
     *  server.
     *
     *  \var er_model_struct::md_sock
     *  Socket descriptor toward remote server
     *  \var er_model_struct::md_scfg
     *  Server spatial parameter
     *  \var er_model_struct::md_tcfg
     *  Server temporal parameter
     *  \var er_model_struct::md_size
     *  Cell stacks size
     *  \var er_model_struct::md_push
     *  Target stack pushed cells
     *  \var er_model_struct::md_free
     *  Synchronisation index
     *  \var er_model_struct::md_syna
     *  Synchronisation index
     *  \var er_model_struct::md_synb
     *  Synchronisation index
     *  \var er_model_struct::md_cell
     *  Cells stack (actual)
     *  \var er_model_struct::md_virt
     *  Cells stack (target)
     *  \var er_model_struct::md_addr
     *  Communication socket-array
     *  \var er_model_struct::_status
     *  Standard status field
     */

    typedef struct er_model_struct {

        le_sock_t   md_sock;
        le_size_t   md_scfg;
        le_time_t   md_tcfg;

        le_size_t   md_size;
        le_size_t   md_push;
        le_size_t   md_free;
        le_size_t   md_syna;
        le_size_t   md_synb;

        er_cell_t * md_cell;
        er_cell_t * md_virt;

        le_array_t  md_addr;

    le_enum_t _status; } er_model_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a model structure. It initialises the
     *  structure fields and assign the provided parameters.
     *
     *  In addition, this function also allocate the two cell stacks needed to
     *  maintain and update the model. The cells of both stack are initialised.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field.
     *
     *  \param er_socket Socket toward remote server
     *  \param er_scfg   Server spatial parameter
     *  \param er_tcfg   Server temporal parameter
     *
     *  \return Created model structure
     */

    er_model_t er_model_create( le_sock_t const er_socket, le_size_t const er_scfg, le_time_t const er_tcfg );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided model structure. It deletes each
     *  cell of the stacks using their related function.
     *
     *  It then release the stacks memory allocations and clears the structure
     *  fields using default values.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_delete( er_model_t * const er_model );

    /*! \brief accessor methods (deletion)
     *
     *  This function checks if the provided address is identical to a cell that
     *  is already present in the display cell stack (actual stack).
     *
     *  In case such cell is found, the function returns the index of the cell
     *  in the actual stack. Otherwise, the size of the actual cell stack is
     *  returned.
     *
     *  \param er_model Model structure
     *  \param er_addr  Address structure
     *
     *  \return Returns the stack cell index if found, the stack size otherwise
     */

    le_size_t er_model_get_drop( er_model_t const * const er_model, le_address_t const * const er_addr );

    /* *** */

    le_void_t er_model_set_next( er_model_t * const er_model );

    /*! \brief mutator methods
     *
     *  This function is used to prepare an update of the Earth model contain in
     *  the provided model structure, typically after point of view motion.
     *
     *  Its role is to reset the synchronisation values, as the synchronisation
     *  will have to restart, the target stack push value and the flags of the
     *  target and actual stack cells.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_prep( er_model_t * const er_model );

    /*! \brief mutator methods (revoked)
     *
     *  The role of this function is to enumerates all possible cells and to
     *  select the ones that are relevant to query and display according to the
     *  position of the point of view. A recursive enumeration strategy based on
     *  the provided view and address structure is applied.
     *
     *  Starting at scale zero, the possible digits are enumerated. The size of
     *  the address (zero at the beginning) is increased by one. On the first
     *  \b ER_COMMON_ENUM + 1 scales, the function simply calls itself passing
     *  the updated address as parameter. Otherwise, the function checks if the
     *  cell belong to the Earth face (relevant portion of Earth surface for
     *  display according to point of view position).
     *
     *  If the cell does not belong to the Earth face, it is simply discarded
     *  and no recursion occurs (all the cell daughters are discarded at the
     *  same time). If the cell belong to Earth face, the function checks if it
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

    /*! mutator methods (revoked)
     *
     *  This function is used just after the target stacks updates. Its role is
     *  to rapidly checks which cells of the target stack are already in the
     *  actual stack, with their data already queried. This allows to flag them
     *  as so, indicating to the synchronisation process not to consider these
     *  actual and target stack cells.
     *
     *  This function allows to re-enable actual stacks cells in order to allow
     *  them being part of the rendered model as quickly as possible. This also
     *  allows to decrease the work load of the synchronisation process and to
     *  reduce bandwidth between the client and server.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_fast( er_model_t * const er_model );

    /*! mutator methods (revoked)
     *
     *  This function is responsible of the step by step synchronisation
     *  process of the target and actual cell stacks. At each function call,
     *  one step of the synchronisation is performed.
     *
     *  After a target stack update, usually following a modification of the
     *  point of view, the synchronisation index is set to zero. At each call,
     *  the function searches in the target stack the cells that have to be
     *  queried to the remote server. Cell query are packed together to limit
     *  communication with the server. As the server answer the query, the
     *  function receives and stores the data in cells determined as free. The
     *  process is repeated at each function call until the synchronisation is
     *  complete.
     *
     *  In the first place, the function checks if the synchronisation is done
     *  or incomplete. It case of fully synchronised stacks, the function does
     *  nothing.
     *
     *  After the last query, the function checks the actual stack for cells
     *  that were displayed at the previous state and that no more used in the
     *  current state in order to disable their graphical representation.
     *
     *  Such step by step strategy allows to be able to interrupt Earth model
     *  update. Indeed, updating the model leads to a process that consume time.
     *  Motion of the point of view will only be considered at end of such a
     *  global process. Using the step by step synchronisation allows to stop
     *  one model update to ensure a better tracking of the point of view
     *  motion, saving also large amounts of data transfer between the client
     *  and server.
     *
     *  \param er_model Model structure
     *
     *  \return Returns _LE_TRUE on synchronised stacks, _LE_FALSE otherwise
     */

    le_enum_t er_model_set_sync( er_model_t * const er_model );

    /*! mutator methods (revoked)
     *
     *  This function is responsible of the composition of the query pack to
     *  the remote server.
     *
     *  The function search in the target stack which cell have to be queried
     *  and pack them into a single query socket-array.
     *
     *  \param er_model Model structure
     *
     *  \return Returns address array size, in bytes
     */

    le_size_t er_model_set_sync_pack( er_model_t * const er_model );

    /*! mutator methods (revoked)
     *
     *  This function is responsible of the cleaning of the tail of the actual
     *  cell stack as the end of the synchronisation is reached.
     *
     *  Its task is to search for cells that are not engaged by the target stack
     *  and remains displayed due to the previous state of the actual stack. As
     *  such cells are found, the function disable their display flag.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_sync_tail( er_model_t * const er_model );

    /*! \brief display methods
     *
     *  This function displays the Earth model hold in the actual stack. It
     *  searches in the actual stack the cells that have their display flag
     *  set.
     *
     *  The Earth-attached Cartesian coordinates are related to the OpenGL
     *  Cartesian one in the following way :
     *
     *      x_geographic_cartesian -> z_opengl (0->2)
     *      y_geographic_cartesian -> x_opengl (1->0)
     *      z_geographic_cartesian -> y_opengl (2->1)
     *
     *  allowing to keep a natural view of Earth without additional rotation.
     *
     *  The display strategy consist in keeping the deepest and nearest (point
     *  of view) cells as near as possible to the OpenGL frame origin. This
     *  allows to keep small values for the most relevant (point of view)
     *  points. This follows the strategy that consists in shifting the cell
     *  points to a frame that keep their coordinates small. The objective is
     *  to address single precision saturation issues. This strategy allows to
     *  consider millimetric models of Earth within a single homogeneous model.
     *
     *  The function applies the point of view rotation and translation before
     *  to search cells of the actual stacks that have to be rendered. For each
     *  found cell, the function invokes the cell specialised display function.
     *  The trigonometric values of the point of view longitude and latitude are
     *  pre-computed by the function and provided to the specialised cell
     *  rendering function.
     *
     *  \param er_model Model structure
     *  \param er_view  View structure
     */

    le_void_t er_model_display_cell( er_model_t const * const er_model, er_view_t const * const er_view );

    /*! \brief display methods
     *
     *  This function displays a simple wire frame model of Earth to provide a
     *  permanent visual reference.
     *
     *  The point of view rotation and translation are applied to the model
     *  matrix before to create and render the wire frame model.
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

