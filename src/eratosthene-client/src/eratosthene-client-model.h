/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2019 DHLAB, EPFL
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
    # define ER_MODEL_C        { _LE_SOCK_NULL, _LE_SIZE_NULL, _LE_TIME_NULL, ER_COMMON_SIZE, 0, 0, 0, 0, NULL, NULL, LE_ARRAY_C, LE_ARRAY_C, _LE_FALSE, _LE_FALSE }

    /* define pseudo-initialiser */
    # define ER_MODEL_I(s,p,t) { s, p, t, ER_COMMON_SIZE, 0, 0, 0, 0, NULL, NULL, LE_ARRAY_C, LE_ARRAY_C, _LE_TRUE, _LE_FALSE }

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
     *  up to date according to the motion of the point of view to present the
     *  most relevant information to the user.
     *
     *  The model is kept up to date according to the motion of the point of
     *  view through the dynamical update procedure implemented in this module.
     *  Two stacks of cells, each represented by an indexation address and it
     *  corresponding content, are defined. The first stack contains the cell
     *  that are rendered and so, containing the actual data obtained through
     *  a query to the remote server using its address. The second stack only
     *  contains 'virtual' cell only represented by and address.
     *
     *  The second stack then contains the target cells that are set by the
     *  enumeration process responsible of creating the list of all relevant
     *  address to query to the remote server according to the current point
     *  of view. As a motion of the point of view is detected, the target stack
     *  is updated. The render cell stack is then synchronised step by step with
     *  the target stack. The synchronisation is made in two phases.
     *
     *  The first phase, that immediately follows each update of the target
     *  stack checks if some required cells are already present in the render
     *  stack. In such case, they are re-activated and flagged as already up to
     *  date. The first phase ends by a specific query to the server to get the
     *  list of all cells of the target stack that contains actual data.
     *
     *  On the basis of this last list, the second phase takes place. Each non-
     *  empty cell address of the target are associated to a free cell of the
     *  render stack in order to query their data to the remote server. This
     *  synchronisation is made cell by cell allowing to interrupt the process
     *  in case a new target stack is defined before the end of this phase.
     *
     *  The cell of the target stack are processed using their address depth to
     *  order the query. This allows to query the deeper cells, those close to
     *  the user point of view in the first place. After cells of a given scale
     *  are all synchronised, the cells of the render stack of the same or
     *  greater scale of the previous model are deactivated.
     *
     *  The two stacks along with all the information needed to perform the
     *  model update procedure are hold in this structure.
     *
     *  The first three fields hold the socket opened toward the remote server
     *  and the two configuration values of the server.
     *
     *  The next five field stores the information of the memory size of the two
     *  stacks, the size of the target stack, the index of the detected free
     *  cell of the render stack and the two synchronisation index. These index
     *  are used to drive the step by step synchronisation process. The field
     *  \b md_syna gives the current index in the target stack while the field
     *  \b md_synb gives the current depth to process.
     *
     *  The two next fields are the base pointer of the target and render cells
     *  stacks.
     *
     *  The two next fields are socket-arrays used to pack the cell address to
     *  be queried to the remote server and to receive the list of the non-empty
     *  cells.
     *
     *  The last field holds the synchronisation flag. The value \b _LE_FALSE
     *  indicates that the step by step synchronisation process is underway and
     *  the value \b _LE_TRUE indicates that it ended.
     *
     *  \var er_model_struct::md_sock
     *  Socket toward remote server
     *  \var er_model_struct::md_scfg
     *  Remote server spatial configuration parameter
     *  \var er_model_struct::md_tcfg
     *  Remote server temporal configuration parameter
     *  \var er_model_struct::md_size
     *  Cell stacks size (render and target)
     *  \var er_model_struct::md_push
     *  Amount of pushed address in the target stack
     *  \var er_model_struct::md_free
     *  Index of the last detected free cell in the render stack
     *  \var er_model_struct::md_syna
     *  Synchronisation cell index
     *  \var er_model_struct::md_synb
     *  Synchronisation cell depth
     *  \var er_model_struct::md_cell
     *  Render cell stack
     *  \var er_model_struct::md_virt
     *  Target cell stack
     *  \var er_model_struct::md_addr
     *  Communication socket-array for addresses query
     *  \var er_model_struct::md_dete
     *  Communication socket-array for non-empty cell list
     *  \var er_model_struct::md_sync
     *  Step by step synchronisation state flag
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
        le_array_t  md_dete;

        le_enum_t   md_sync;

    le_enum_t _status; } er_model_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a model structure. It initialises the
     *  structure fields and assign the provided remote server socket and
     *  parameters.
     *
     *  In addition, this function also allocate the two cell stacks needed to
     *  maintain and update the model. The cells of both stack are initialised
     *  by the function.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field. On error, the
     *  \b _status field is returned with value \b _LE_FALSE and \b _LE_TRUE
     *  otherwise.
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
     *  cell of the two stacks using their related deletion function.
     *
     *  It then release the stacks memory allocations and clears the structure
     *  fields using default values.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_delete( er_model_t * const er_model );

    /*! \brief accessor methods
     *
     *  This function allows to retrieve the synchronisation process state flag
     *  used during model update.
     *
     *  \param er_model Model Structure
     *
     *  \return Returns synchronisation process state flag
     */

    le_enum_t er_model_get_sync( er_model_t const * const er_model );

    /*! \brief mutator methods
     *
     *  This function sets the provided model structure free cell index on the
     *  immediate next available cell for data reception. The function uses the
     *  value of the free cell as an index and increases it until an available
     *  is pointed.
     *
     *  This function is used during model update to find a cell able to read
     *  the answer of an address query made toward the remote server.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_next( er_model_t * const er_model );

    /*! \brief mutator methods
     *
     *  This function is used to prepare an update of the Earth model contain in
     *  the provided model structure after a point of view modification.
     *
     *  All the values of the provided model structure are prepared for an new
     *  update of the model. The cells of the two stacks are also prepared by
     *  resetting their synchronisation flag.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_prep( er_model_t * const er_model );

    /*! \brief mutator methods
     *
     *  The role of this function is to enumerates all possible cells and to
     *  select the ones that are relevant to query and display according to the
     *  position of the point of view. A recursive enumeration strategy based on
     *  the provided view and address structure is applied.
     *
     *  Starting at scale zero, the possible digits are enumerated. The size of
     *  the address (zero at the beginning) is increased by one and the new
     *  digit is used to receive the enumerated values.
     *
     *  On the first \b ER_COMMON_ENUM + 1 scales, the function simply calls
     *  itself passing the updated address as parameter. On the other scales,
     *  the function checks if each enumerated cell address belong to the Earth
     *  face (relevant portion of Earth surface for display according to point
     *  of view position).
     *
     *  If the cell does not belong to the Earth face, it is simply discarded
     *  and no recursion occurs (all the cell daughters are discarded at the
     *  same time). If the cell belong to Earth face, the function checks if it
     *  is relevant for display according to the point of view. If it is and its
     *  distance to point of view satisfy the criterion, the enumeration address
     *  is pushed on the target stack. If the cell is judge irrelevant for
     *  display, the function calls itself to continue enumeration with the
     *  updated address to search through the daughter cells.
     *
     *  At the end of the process, the target stack is filled with the relevant
     *  cells that have then to be queried to the remote server.
     *
     *  \param er_model Model structure
     *  \param er_enum  Enumeration address structure
     *  \param er_scale Enumeration address explicit size
     *  \param er_view  Point of view structure
     */

    le_void_t er_model_set_enum( er_model_t * const er_model, le_address_t * const er_enum, le_size_t const er_scale, er_view_t const * const er_view );

    /*! \brief mutator methods
     *
     *  This function is used just after the target stacks updates through the
     *  enumeration process. Its role is to rapidly checks which cells of the
     *  target stack are already in the render stack, with their data already
     *  queried. This allows to flag them indicating to the synchronisation
     *  process not to consider them during the remote server queries.
     *
     *  This function allows to re-enable render stack cells in order to allow
     *  them being part of the rendered model as quickly as possible. This also
     *  allows to decrease the work load of the synchronisation process and to
     *  reduce bandwidth between the client and server.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_fast( er_model_t * const er_model );

    /*! \brief mutator methods
     *
     *  This function implements a specific query to the remote server and takes
     *  place immediately after a target stack update and fast synchronisation
     *  process.
     *
     *  The function packs all the cell addresses of the target stack into a
     *  single query to the remote server in order to retrieve a list of flag
     *  indicating for each cell if they contain actual data in the server.
     *
     *  This allows the subsequent step by step synchronisation process not to
     *  query the cell addresses that contains no data. This allows to improve
     *  the efficiency of the model update and to save bandwidth between the
     *  client and remote server.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_detect( er_model_t * const er_model );

    /*! \brief mutator methods
     *
     *  This function implements the step by step synchronisation process after
     *  an update of the target stack. The process consists in a succession of
     *  call to this function until the render stack is fully synchronised with
     *  the target one.
     *
     *  The function works considering the cell address scale as a main index
     *  and the cell index in the target stack as a second index. This allows
     *  to query the deepest cells, which are the most relevant for the user
     *  point of view, in the first place.
     *
     *  As a cell is selected in the target stack, which has not been already
     *  re-activated in the render stack, its addressed is packed in a socket-
     *  array and send to the remote server through a data query. A free cell
     *  in the render stack is selected and the data are received in it. The
     *  address of the target and render stack cell are synchronised.
     *
     *  As the cell is processed, it is flagged as so in both stack and the
     *  synchronisation index are updated to point at the next cell.
     *
     *  When all the cell of a specific scale have been processed, a secondary
     *  process is called to de-activate in the render stack all remaining cell
     *  that have the same or greater scale. This allows to create an efficient
     *  update effect in the rendering.
     *
     *  As all the cells of the target stack are processed and inserted in free
     *  cells of the render stack, the function ends the synchronisation by
     *  updating the flag value.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_sync( er_model_t * const er_model );

    /* *** */

    le_void_t er_model_set_hide_fast( er_model_t * const er_model );

    /*! \brief mutator methods
     *
     *  This function allows to deactivate the cells of the render stack that
     *  are no more in use.
     *
     *  The function selects such cell by searching in the render stack which
     *  cell have their display flag set and their synchronisation flag unset.
     *  In addition, this function only searches cells with address equal or
     *  deeper than the scale-base synchronisation index of the provided model
     *  structure.
     *
     *  This function is typically used as all the cells of a given depth are
     *  synchronised from the target stack to the render one.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_hide( er_model_t * const er_model );

    /*! \brief display methods
     *
     *  This function displays the Earth model hold in the render stack. It
     *  searches in the target stack the cells that have their display flag
     *  set.
     *
     *  For the rendering, the Earth-attached Cartesian coordinates are related
     *  to the OpenGL Cartesian one in the following way :
     *
     *      x_geographic_cartesian -> z_opengl_cartesian (0->2)
     *      y_geographic_cartesian -> x_opengl_cartesian (1->0)
     *      z_geographic_cartesian -> y_opengl_cartesian (2->1)
     *
     *  allowing to keep a natural view of Earth without additional rotation.
     *
     *  The display strategy consist in keeping the deepest and nearest (point
     *  of view) cells as near as possible to the OpenGL frame origin. This
     *  allows to keep small values for the most relevant graphical primitives.
     *  This follows the strategy that consists in shifting the cell primitives
     *  to a frame that keep their coordinates small. The goal is to address
     *  single precision saturation issues. This strategy allows to consider
     *  millimetric models of Earth within a single homogeneous model.
     *
     *  The function applies the point of view rotation and translation before
     *  to search cells of the actual stacks that have to be rendered. For each
     *  found cell, the function invokes the cell specialised render function.
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
     *  This function renders a simple wire frame model of Earth to provide a
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

