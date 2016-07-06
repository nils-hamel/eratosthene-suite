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

    /*! \file   eratosthene-client-model.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
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
    header - includes
 */

    # include <GL/freeglut.h>
    # include <GL/glu.h>
    # include <math.h>
    # include <eratosthene-include.h>
    # include "eratosthene-client-cell.h"
    # include "eratosthene-client-geodesy.h"
    # include "eratosthene-client-server.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_MODEL_C {\
        NULL,\
        _LE_USE_PORT,\
        _LE_SIZE_NULL,\
        _LE_TIME_NULL,\
        0.0,\
        0.0,\
        0.0,\
        0.0,\
        0,\
        0,\
        NULL,\
        _LE_TRUE\
    }

    /* Define model parameters */
    # define ER_MODEL_DPT 7

    /* Define display array types */
    # define ER_MODEL_VA  GL_DOUBLE
    # define ER_MODEL_CA  GL_UNSIGNED_BYTE

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
     *  \var er_model_struct::md_sdis
     *  Server spatial indexation parameter
     *  \var er_model_struct::md_tdis
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

        le_size_t   md_sdis;
        le_time_t   md_tdis;

        le_time_t   md_mtim;
        le_real_t   md_mlon;
        le_real_t   md_mlat;
        le_real_t   md_malt;

        le_size_t   md_push;

        le_size_t   md_size;
        er_cell_t * md_cell;

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
     *  If the \b md_sdis or \b md_tdis parameters are respectively set to
     *  _LE_SIZE_NULL or _LE_TIME_NULL, the descriptor creation failed.
     *
     *  \param er_cells Cells array size
     *  \param er_ip    Server ip address
     *  \param er_port  Server service port
     *
     *  \return Created model structure
     */

    er_model_t er_model_create( le_size_t er_cells, le_char_t * const er_ip, le_sock_t const er_port );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes a model descriptor created by the function
     *  \b er_model_create. In addition to structure fields uninitisalisation,
     *  it also unallocate cells array memory.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_delete( er_model_t * const er_model );

    /*! \brief accessor methods
     *
     *  Returns model spatial indexation parameter.
     *
     *  \param er_model Model structure
     *
     *  \return Spatial indexation parameter
     */

    le_size_t er_model_get_sdisc( er_model_t const * const er_model );

    /*! \brief accessor methods
     *
     *  Returns model time indexation parameter.
     *
     *  \param er_model Model structure
     *
     *  \return Time indexation parameter
     */

    le_size_t er_model_get_tdisc( er_model_t const * const er_model );

    /*! \brief accessor methods
     *
     *  This function checks the model update requirements. It checks the motion
     *  of the point of view provided as parameter to determine if model has to
     *  be updated.
     *
     *  \param er_model Model structure
     *  \param er_time  Point of view time
     *  \param er_lon   Point of view longitude
     *  \param er_lat   Point of view latitude
     *  \param er_alt   Point of view altitude  
     *
     *  \return Returns _LE_TRUE on necessary update, _LE_FALSE otherwise
     */

    le_enum_t er_model_get_update( er_model_t const * const er_model, le_time_t const er_time, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt );

    /*! \brief mutator methods
     *
     *  This function is part of the model update procedure.
     *
     *  Its role is to prepare the cells array to the incoming cells gathered
     *  during update.
     *
     *  \param er_model Model structure
     */

    le_void_t er_model_set_update_prepare( er_model_t * const er_model );

    /*! \brief mutator methods
     *
     *  This function is part of the model update procedure.
     *
     *  Its role is to determine the largest cell to consider near the point of
     *  view. These cells are then decomposed along their daughters in function
     *  of the position of the point of view.
     *
     *  \param er_model Model structure
     *  \param er_time  Point of view time
     *  \param er_lon   Point of view longitude
     *  \param er_lat   Point of view latitude
     *  \param er_alt   Point of view altitude
     */

    le_void_t er_model_set_update_model( er_model_t * const er_model, le_time_t const er_time, le_real_t const er_lon, le_real_t const er_lat, le_real_t er_alt );

    /*! \brief mutator methods
     *
     *  This function is part of the model update procedure.
     *
     *  Its role is to decompose the cells selected by the function
     *  \b er_model_set_update_model. According to the distance to the point
     *  of view, cells or their daughters are recursively considered for query.
     *
     *  \param er_model Model structure
     *  \param er_addr  Selected cell address structure
     *  \param er_lon   Point of view longitude
     *  \param er_lat   Point of view latitude
     *  \param er_alt   Point of view altitude
     */

    le_void_t er_model_set_update_cells( er_model_t * const er_model, le_address_t * const er_addr, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt );

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

    le_void_t er_model_set_update_destroy( er_model_t * const er_model );

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

    le_void_t er_model_display_cell( er_model_t * const er_model, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt, le_real_t const er_azm, le_real_t const er_gam );

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

    le_void_t er_model_display_earth( le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt, le_real_t const er_azm, le_real_t const er_gam );

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

