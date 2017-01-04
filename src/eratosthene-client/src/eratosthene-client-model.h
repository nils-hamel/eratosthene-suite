/*
 *  eratosthene-suite - eratosthene indexation server front-end
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
     *  Eratosthene client - model module
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
    # define ER_MODEL_C      { NULL, _LE_USE_PORT, _LE_SIZE_NULL, _LE_TIME_NULL, ER_MODEL_STACK, 1, NULL, 0, _LE_TRUE }

    /* define model stack */
    # define ER_MODEL_STACK  ( 4096 )

    /* define display array types */
    # define ER_MODEL_VERTEX GL_DOUBLE
    # define ER_MODEL_COLORS GL_UNSIGNED_BYTE

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
     *  \brief model structure
     *
     *  This structure holds the earth model descriptor. It contains the needed
     *  information for client/server communication and server configuration.
     *
     *  It also holds fields necessary for the earth modelisation through a
     *  simple cells array. In addition, it also stores the point of view
     *  position at model update to detect model update necessity.
     *
     *  \var er_model_struct::md_svip
     *  Server ip address
     *  \var er_model_struct::md_port
     *  Server service port
     *  \var er_model_struct::md_sparam
     *  Server spatial indexation parameter
     *  \var er_model_struct::md_tparam
     *  Server time indexation parameter
     *  \var er_model_struct::md_mtim
     *  Model update time
     *  \var er_model_struct::md_mlon
     *  Model update longitude
     *  \var er_model_struct::md_mlat
     *  Model update latitude
     *  \var er_model_struct::md_malt
     *  Model update altitude
     *  \var er_model_struct::md_push
     *  Model update pushed cell
     *  \var er_model_struct::md_size
     *  Model active cells count
     *  \var er_model_struct::md_cell
     *  Model cells descriptors array
     */

    typedef struct er_model_struct {

        le_char_t * md_svip;
        le_sock_t   md_port;

        le_size_t   md_sparam;
        le_time_t   md_tparam;

        le_size_t   md_size;
        le_size_t   md_push;
        er_cell_t * md_cell;

        le_size_t   md_cycle;

    le_enum_t _status; } er_model_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a model descriptor. It initialise
     *  descriptor fields with default values and initialise network fields
     *  according to parameters.
     *
     *  In addition, it also ask the remote server for the spatial and time
     *  indexation parameters and allocate the cells array memory.
     *
     *  If the \b md_sparam or \b md_tparam parameters are respectively set to
     *  _LE_SIZE_NULL or _LE_TIME_NULL, the descriptor creation failed.
     *
     *  \param er_cells Cells array size
     *  \param er_ip    Server ip address
     *  \param er_port  Server service port
     *
     *  \return Created model structure
     */

    er_model_t er_model_create( le_char_t * const er_ip, le_sock_t const er_port );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes a model descriptor created by the function
     *  \b er_model_create. In addition to structure fields uninitisalisation,
     *  it also unallocate cells array memory.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_delete( er_model_t * const er_model );

    le_size_t er_model_get_cell( er_model_t * const er_model );

    le_void_t er_model_set_update_cell( er_model_t * const er_model, le_address_t * const er_enum, er_view_t const * const er_view );

    /*! \brief mutator methods
     *
     *  This function is part of the model update procedure.
     *
     *  This function is responsible of detecting which cells selected by the
     *  functions \b er_model_set_update_model and \b er_model_set_update_cells
     *  are already in the model cells array in order to only perform server
     *  query for true new cells.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_update_query( er_model_t * const er_model );

    /*! \brief mutator methods
     *
     *  This function is part of the model update procedure.
     *
     *  This function is respsonsible of emptying the unused cells remaining
     *  after the model update procedure. It is then the last function called
     *  during model update procedure.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_update_terminate( er_model_t * const er_model );

    /*! \brief model display methods
     *
     *  This function is responsible of the display of the active cells in the
     *  model cells array.
     *
     *  \param er_model Model structure
     *  \param er_lon   Point of view longitude
     *  \param er_lat   Point of view latitude
     *  \param er_alt   Point of view altitude
     *  \param er_azm   Point of view azimuth
     *  \param er_gam   Point of view tilt
     */

    le_void_t er_model_display_cell( er_model_t const * const er_model, er_view_t const * const er_view );

    /*! \brief model display methods
     *
     *  This function is responsible for the display of the five degree of arc
     *  wireframe model of the earth.
     *
     *  \param er_lon   Point of view longitude
     *  \param er_lat   Point of view latitude
     *  \param er_alt   Point of view altitude
     *  \param er_azm   Point of view azimuth
     *  \param er_gam   Point of view tilt
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

