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

    /*! \file   eratosthene-client-engine.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene client - rendering engine module
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

    # include <math.h>
    # include <string.h>
    # include <unistd.h>
    # include <pthread.h>
    # include <GL/gl.h>
    # include <GL/glu.h>
    # include <GL/freeglut.h>
    # include <eratosthene-include.h>
    # include "eratosthene-client-model.h"
    # include "eratosthene-client-geodesy.h"

    # include "eratosthene-client-cinema.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_ENGINE_C {\
        ER_MODEL_C,\
        1,\
        0,\
        GLUT_UP,\
        0,\
        0,\
        0,\
        0,\
        1.0,\
        2.0 * ER_ERA,\
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
     *
     *  This structure holds the rendering engine descriptor. It contains fields
     *  for rendering configuration, client events and motions control. It also
     *  contains the model descriptor that contains the information needed to
     *  render the earth modelisation.
     *
     *  \var er_engine_struct::eg_model
     *  Engine model descriptor
     *  \var er_engine_struct::eg_point
     *  Engine rendering point size
     *  \var er_engine_struct::eg_button
     *  Event mouse button
     *  \var er_engine_struct::eg_state
     *  Event mouse state
     *  \var er_engine_struct::eg_x
     *  Event mouse position
     *  \var er_engine_struct::eg_y
     *  Event mouse position
     *  \var er_engine_struct::eg_u
     *  Event mouse pushed position
     *  \var er_engine_struct::eg_v
     *  Event mouse pushed position
     *  \var er_engine_struct::eg_mult
     *  Point of view intertial factor
     *  \var er_engine_struct::eg_valt
     *  Point of view altitude (radial components)
     *  \var er_engine_struct::eg_vlon
     *  Point of view longitude
     *  \var er_engine_struct::eg_vlat
     *  Point of view latitude
     *  \var er_engine_struct::eg_vazm
     *  Point of view azimuthal angle
     *  \var er_engine_struct::eg_vgam
     *  Point of view tilt angle
     *  \var er_engine_struct::eg_vscl
     *  Point of view scale factor 
     *  \var er_engine_struct::eg_vtim
     *  Point of view time
     */

    typedef struct er_engine_struct {

        er_model_t eg_model;
        le_size_t  eg_point;

        le_enum_t  eg_button;
        le_enum_t  eg_state;
        le_diff_t  eg_x;
        le_diff_t  eg_y;
        le_diff_t  eg_u;
        le_diff_t  eg_v;
        le_real_t  eg_mult;

        le_real_t  eg_valt;
        le_real_t  eg_vlon;
        le_real_t  eg_vlat;
        le_real_t  eg_vazm;
        le_real_t  eg_vgam;
        le_real_t  eg_vscl;

        le_time_t  eg_vtim;

    } er_engine_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns the rendering engine descriptor. In
     *  addition to the initialisation of the fields, it also initialise the
     *  graphical context and setup callbacks functions. It is also responsible
     *  of the model descriptor creation.
     *
     *  \param er_stack Size of the model cells array
     *  \param er_ip    Server ip address
     *  \param er_port  Server service port
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_engine_create( le_size_t const er_stack, le_char_t * const er_ip, le_sock_t const er_port );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes a rendering engine descriptor created by the
     *  \b er_engine_create function. In addition to model deletion, it also
     *  uninitialise the graphical context and model descriptor.
     *
     *  Note that, because of event management through callback functions, the
     *  instance of the rendering engine descriptor is global :(
     */

    le_void_t er_engine_delete( le_void_t );

    /*! \brief rendering engine
     *
     *  This function manage the rendering engine main loops. It creates two
     *  threads, one responsible of rendering and events, the other being
     *  responsible of model update and client/server communication.
     */

    le_void_t er_engine_loops( le_void_t );

    /*! \brief engine loop
     *
     *  This function holds the callback procedure for the model rendering.
     */

    le_void_t er_engine_render( le_void_t );

    /*! \brief engine loop
     *
     *  This function implements an infinit loop that contains the procedure
     *  needed to update the model.
     *
     *  \param er_null Null pointer
     *
     *  \return Null pointer
     */

    le_void_t * er_engine_update( le_void_t * er_null );

    /*! \brief engine callbacks - reshape
     *
     *  This function implements the reshape callback called on rendering
     *  buffer reshape.
     *
     *  This project requiering dynamic near and far planes, this function is
     *  called each time a model rendering starts.
     *
     *  \param er_width  Width, in pixel, of the rendering buffer
     *  \param er_height Height, in pixels, of the rendering buffer
     */

    le_void_t er_engine_reshape( int er_width, int er_height );

    /*! \brief engine callbacks - keyboard
     *
     *  This function implements the keyboard callback function called on
     *  keyboard events.
     *
     *  \param er_keycode Keyboard key code
     *  \param er_x       Mouse position at key press
     *  \param er_y       Mouse position at key press
     */

    le_void_t er_engine_keybd( unsigned char er_keycode, int er_x, int er_y );

    /*! \brief engine callbacks - mouse
     *
     *  This function implements the mouse click callback function called on
     *  mouse click events.
     *
     *  \param er_button Mouse button code
     *  \param er_state  Mouse button state
     *  \param er_x      Mouse position at click
     *  \param er_y      Mouse position at click
     */

    le_void_t er_engine_mouse( int er_button, int er_state, int er_x, int er_y );

    /*! \brief engine callbacks - mouse
     *
     *  This function implements the mouse motion callback function called on
     *  mouse motion events.
     *
     *  \param er_x Mouse position
     *  \param er_y Mouse position
     */

    le_void_t er_engine_move( int er_x, int er_y );    

    /*! \brief engine callbacks - ranges
     *
     *  This function implements the range verification callback function. It
     *  is typically called each time a model rendering starts. It verify that
     *  point of view angle and distance remains in the determined ranges.
     */

    le_void_t er_engine_range();

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

