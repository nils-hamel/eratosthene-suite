/*
 *  eratosthene-suite - geodetic system
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
     *  Eratosthene client - Model module
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

    # include <GL/gl.h>
    # include <GL/glu.h>
    # include <GL/glut.h>
    # include <math.h>
    # include <eratosthene-include.h>
    # include "eratosthene-client-cell.h"
    # include "eratosthene-client-geodesy.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_MODEL_C   { { 0 }, _LE_USE_PORT, 0, 0, 0, 0, NULL }

    /* Define model parameters */
    # define ER_MODEL_ASL 10
    # define ER_MODEL_ASH 20

    # define ER_MODEL_DPT 6

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

    typedef struct er_model_struct {

        le_char_t   md_svip[256];
        le_sock_t   md_port;

        le_size_t   md_sdis;
        le_time_t   md_tdis;

        le_size_t   md_push;
        le_size_t   md_size;
        er_cell_t * md_cell;

    } er_model_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     */

    er_model_t er_model_create( le_size_t er_cells, le_char_t const * const er_ip, le_sock_t const er_port );

    /*! \brief constructor/destructor methods
     */

    le_void_t er_model_delete( er_model_t * const er_model );

    /*! \brief accessor methods
     */

    le_size_t er_model_get_sdisc( er_model_t const * const er_model );

    /*! \brief accessor methods
     */

    le_size_t er_model_get_tdisc( er_model_t const * const er_model );

    /*! \brief mutator methods
     */

    le_void_t er_model_set_push( er_model_t * const er_model );

    /*! \brief mutator methods
     */

    le_void_t er_model_set_model( er_model_t * const er_model, le_time_t const er_time, le_real_t const er_lon, le_real_t const er_lat, le_real_t er_alt );

    /*! \brief mutator methods
     */

    le_void_t er_model_set_addr( er_model_t * const er_model, le_address_t * const er_addr, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt );

    /*! \brief model display methods
     */

    le_void_t er_model_display_cell( er_model_t * const er_model );

    /*! \brief model display methods
     */

    le_void_t er_model_display_earth( le_void_t );

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

