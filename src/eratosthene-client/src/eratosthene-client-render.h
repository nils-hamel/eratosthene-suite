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

    /*! \file   eratosthene-client-render.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene client software - Rendering module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_RENDER__
    # define __ER_CLIENT_RENDER__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include "eratosthene-client-model.h"
    # include <GL/gl.h>
    # include <GL/glu.h>
    # include <GL/glut.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_ENGINE_C    { 0, GLUT_UP, 0, 0, 0, 0, 0, 0, _LE_SOCK_NULL, _EARTH_RAD * 1.5, 0, 0 }

    /* Define mouse velocity */
    # define ER_ENGINE_MOVE 0.01f

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct er_engine_struct
     *  \brief Rendering engine handle
     */

    typedef struct er_engine_struct {

        int       eg_button;
        int       eg_state;
        int       eg_x;
        int       eg_y;
        int       eg_u;
        int       eg_v;
        int       eg_s;
        int       eg_t;

        le_sock_t eg_client;

        float     eg_valt;
        float     eg_vlon;
        float     eg_vlat;

    } er_engine_t;

/*
    header - function prototypes
 */

    /*! \brief rendering engine
     */

    void er_engine( le_sock_t const er_client );

    /*! \brief rendering engine - render callback
     */

    void er_engine_render( void );

    /*! \brief rendering engine - reshape callback
     */

    void er_engine_reshape( int er_width, int er_height );

    /*! \brief rendering engine - keyboard callback
     */

    void er_engine_keybd( unsigned char er_keycode, int er_x, int er_y );

    /*! \brief rendering engine - mouse callback
     */

    void er_engine_mouse( int er_button, int er_state, int er_x, int er_y );

    /*! \brief rendering engine - mouse callback
     */

    void er_engine_move( int er_x, int er_y );    

    /* Help wellcome :( */ extern void glutLeaveMainLoop( void ); /* :( */

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

