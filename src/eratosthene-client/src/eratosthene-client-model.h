/*
 *  liberatosthene - geodetic system
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
     *  Eratosthene client software - Model module
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
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_MODEL_C { 0, NULL, NULL }

    /* Define earth parameters and limits */
    # define ER_ERA     ( LE_GEODESY_WGS84_A / 1000.0 )
    # define ER_ER2     ( ER_ERA / 2.0 )
    # define ER_ERL     ( ER_ERA * 1.0 )
    # define ER_ERU     ( ER_ERA * 2.0 )

    /* Define tiling parameters */
    # define ER_TI      (  2 )
    # define ER_TL      ( 13 )

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

        le_size_t    md_size;
        float      * md_vert;
        float      * md_colo;
        float      * md_swap;

    } er_model_t;

/*
    header - function prototypes
 */

    /*! \brief model function
     */

    void er_model_display( float er_scale );

    /*! \brief query management
     */

    void er_model_query( er_model_t * const er_model, le_sock_t const er_socket, le_char_t const * const er_query );

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

