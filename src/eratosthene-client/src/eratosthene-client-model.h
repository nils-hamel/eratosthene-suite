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

    # include "eratosthene-client-geodesy.h"
    # include "eratosthene-client-cell.h"
    # include <GL/gl.h>
    # include <GL/glu.h>
    # include <GL/glut.h>
    # include <math.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_MODEL_C { 1, { ER_CELL_C } }

    /* Define fundamental segmentation */
    # define ER_MODEL_SEG   512

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

        le_size_t md_psiz;
        er_cell_t md_cell[ER_MODEL_SEG];

    } er_model_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     */

    er_model_t er_model_create( le_void_t );

    /*! \brief constructor/destructor methods
     */

    le_void_t er_model_delete( er_model_t * const er_model );

    /*! \brief accessor methods
     */

    le_size_t er_model_get_psiz( er_model_t const * const er_model );

    /*! \brief mutator methods
     */

    le_void_t er_model_set_psiz( er_model_t * const er_model, le_size_t er_psiz );

    /*! \brief model display
     */

    void er_model_main( er_model_t * const er_model );

    /*! \brief model management */

    void er_model_update( er_model_t * const er_model, le_time_t const er_time, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt );

    /*! \brief model management */

    void er_model_client( le_char_t const * const er_ip, le_sock_t const er_port, er_model_t * const er_model );

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

