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

    /*! \file   eratosthene-client-render.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene client - Rendering module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_ENGINE__
    # define __ER_CLIENT_ENGINE__

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
    # include "eratosthene-client-geodesy.h"
    # include <math.h>
    # include <string.h>
    # include <unistd.h>
    # include <pthread.h>
    # include <GL/gl.h>
    # include <GL/glu.h>
    # include <GL/freeglut.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_ENGINE_C {\
        { 0 },\
        _LE_USE_PORT,\
        ER_MODEL_C,\
        1,\
        0,\
        GLUT_UP,\
        0,\
        0,\
        0,\
        0,\
        1.5 * ER_ERA,\
        9,\
        46,\
        0.0,\
        0.0,\
        1.0,\
        950486422\
    }

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

        char       eg_ip[256];
        int        eg_port;

        er_model_t eg_model;
        int        eg_point;

        int        eg_button;
        int        eg_state;
        int        eg_x;
        int        eg_y;
        int        eg_u;
        int        eg_v;

        float      eg_valt;
        float      eg_vlon;
        float      eg_vlat;
        float      eg_vazm;
        float      eg_vgam;
        double     eg_vscl;
        le_time_t  eg_vtim;

    } er_engine_t;

/*
    header - function prototypes
 */

    /*! \brief rendering engine
     */

    void er_engine_main( le_char_t const * const er_ip, le_sock_t const er_port );

    /*! \brief rendering engine
     */

    void * er_engine_second( void * er_void );

    /*! \brief engine callbacks - primary
     */

    void er_engine_render( void );

    /*! \brief engine callbacks - primary
     */

    void er_engine_update( void );

    /*! \brief engine callbacks - reshape
     */

    void er_engine_reshape( int er_width, int er_height );

    /*! \brief engine callbacks - keyboard
     */

    void er_engine_keybd( unsigned char er_keycode, int er_x, int er_y );

    /*! \brief engine callbacks - mouse
     */

    void er_engine_mouse( int er_button, int er_state, int er_x, int er_y );

    /*! \brief engine callbacks - mouse
     */

    void er_engine_move( int er_x, int er_y );    

    /*! \brief engine callbacks - ranges
     */

    void er_engine_range();

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

