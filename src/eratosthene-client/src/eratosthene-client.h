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

    /*! \file   eratosthene-client.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene client
     */

    /*! \mainpage eratosthene-suite
     *
     *  \section eratosthene-suite
     *
     *  The _eratosthene-suite_ offers a front-end to the eratosthene indexation
     *  server implemented in the _liberatosthene_ library. It consists in a
     *  software suite implementing front-ends to the functionality of the 
     *  indexation server.
     *
     *  The main software implements the server itself allowing to simply create
     *  instance of it. The suite also offers a front-end software for the data
     *  injection in the servers storage structure. Finally, it also offers a 
     *  graphical client allowing to browse the data contained in the servers
     *  storage structure through a 4 dimensional representation of earth.
     * 
     *  \section Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016 EPFL CDH DHLAB
     *
     *  This program is licensed under the terms of the GNU GPLv3.
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT__
    # define __ER_CLIENT__

    # ifndef __OPENMP__
    # error "OpenMP required"
    # endif

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
    # include "eratosthene-client-geodesy.h"
    # include "eratosthene-client-times.h"
    # include "eratosthene-client-model.h"
    # include "eratosthene-client-movie.h"
    # include "eratosthene-client-view.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_CLIENT_C { ER_COMMON_VIEW, ER_MODEL_C, ER_TIMES_C, ER_MOVIE_C, 0, GLUT_UP, 0, 0, 1.0, 1.0, ER_VIEW_D, _LE_TRUE }

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct er_client_struct
     *  \brief Rendering engine handle
     *
     *  This structure holds the rendering engine descriptor. It contains fields
     *  for rendering configuration, client events and motions control. It also
     *  contains the model descriptor that contains the information needed to
     *  render the earth modelisation.
     *
     *  \var er_client_struct::cl_model
     *  Engine model descriptor
     *  \var er_client_struct::cl_point
     *  Engine rendering point size
     *  \var er_client_struct::cl_button
     *  Event mouse button
     *  \var er_client_struct::cl_state
     *  Event mouse state
     *  \var er_client_struct::cl_x
     *  Event mouse position
     *  \var er_client_struct::cl_y
     *  Event mouse position
     *  \var er_client_struct::cl_u
     *  Event mouse pushed position
     *  \var er_client_struct::cl_v
     *  Event mouse pushed position
     *  \var er_client_struct::cl_inertia
     *  Point of view intertial factor
     *  \var er_client_struct::cl_valt
     *  Point of view altitude (radial components)
     *  \var er_client_struct::cl_vlon
     *  Point of view longitude
     *  \var er_client_struct::cl_vlat
     *  Point of view latitude
     *  \var er_client_struct::cl_vazm
     *  Point of view azimuthal angle
     *  \var er_client_struct::cl_vgam
     *  Point of view tilt angle
     *  \var er_client_struct::cl_scale
     *  Point of view scale factor 
     *  \var er_client_struct::cl_vtim
     *  Point of view time
     */

    typedef struct er_client_struct {

        le_enum_t  cl_loops;

        er_model_t cl_model;
        er_times_t cl_times;
        er_movie_t cl_movie;

        le_enum_t  cl_button;
        le_enum_t  cl_state;
        le_diff_t  cl_x;
        le_diff_t  cl_y;
        le_real_t  cl_inertia;
        le_real_t  cl_scale;

        er_view_t  cl_view;

    le_enum_t _status; } er_client_t;

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

    er_client_t er_client_create( le_char_t * const er_ip, le_sock_t const er_port );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes a rendering engine descriptor created by the
     *  \b er_client_create function. In addition to model deletion, it also
     *  uninitialise the graphical context and model descriptor.
     *
     *  Note that, because of event management through callback functions, the
     *  instance of the rendering engine descriptor is global :(
     */

    le_void_t er_client_delete( er_client_t * const er_client );

    /*! \brief main function
     *
     *  The main function simply reads the arguments and parameters and creates
     *  the client rendering engine descriptor. It then calls the rendering
     *  engine main loop function to starts the graphical rendering. As the
     *  engine exits its main loop, the main function deletes the rendering
     *  engine descriptor and the execution stops.
     *
     *  \param argc Main function parameters
     *  \param argv Main function parameters
     *
     *  \return Standard exit code
     */

    int main( int argc, char ** argv );

    /*! \brief engine loop
     *
     *  This function holds the callback procedure for the model rendering.
     */

    le_void_t er_client_loops_render( le_void_t );

    /*! \brief engine loop
     *
     *  This function implements an infinit loop that contains the procedure
     *  needed to update the model.
     *
     *  \param er_null Null pointer
     *
     *  \return Null pointer
     */

    le_void_t er_client_loops_update( le_void_t );

    le_void_t er_client_proj_model( int er_width, int er_height );

    le_void_t er_client_proj_interface( int er_width, int er_height );

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

    le_void_t er_client_calls_reshape( int er_width, int er_height );

    /*! \brief engine callbacks - keyboard
     *
     *  This function implements the keyboard callback function called on
     *  keyboard events.
     *
     *  \param er_keycode Keyboard key code
     *  \param er_x       Mouse position at key press
     *  \param er_y       Mouse position at key press
     */

    le_void_t er_client_calls_keybd( unsigned char er_keycode, int er_x, int er_y );

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

    le_void_t er_client_calls_mouse( int er_button, int er_state, int er_x, int er_y );

    /*! \brief engine callbacks - mouse
     *
     *  This function implements the mouse motion callback function called on
     *  mouse motion events.
     *
     *  \param er_x Mouse position
     *  \param er_y Mouse position
     */

    le_void_t er_client_calls_move( int er_x, int er_y );

    le_void_t glutFinish( le_void_t );

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

