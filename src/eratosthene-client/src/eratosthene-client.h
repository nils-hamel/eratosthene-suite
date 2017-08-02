/*
 *  eratosthene-suite - client
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

    /*! \file   eratosthene-client.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - client
     */

    /*! \mainpage eratosthene-suite
     *
     *  \section eratosthene-suite
     *
     *  The _eratosthene-suite_ offers a front-end to the eratosthene indexation
     *  server implemented in the _liberatosthene_ library. It consists in a
     *  softwares suite offering front-end solutions to the functionalities of
     *  the eratosthene library.
     *
     *  The principale software implements the server itself allowing to simply
     *  create instances and maintain services. The suite also offers a front-end
     *  software for the data injection in the available servers. It also offers
     *  a graphical software allowing to browse the 4D worlds offered by the
     *  available servers.
     *
     *  Other tools are also available as the isometric projection creation and
     *  the address and time conversion softwares
     *
     *  \section Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016-2017 EPFL CDH DHLAB
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
    # define ER_CLIENT_C { _LE_SOCK_NULL, ER_COMMON_VIEW, ER_MODEL_C, ER_TIMES_C, ER_MOVIE_C, ER_VIEW_D, ER_VIEW_C, 0, GLUT_UP, 0, 0, 1.0, 1.0, 0.0, 0.0, _LE_TRUE, _LE_TRUE }

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
     *  \brief Client structure
     *
     *  This structure holds the graphical client interface description. In
     *  addition to the execution loop and point of view management, it also
     *  holds structure of sub-modules.
     *
     *  In addition to the execution loop management variables, it holds fields
     *  dedicated to the management of the point of view, which includes mouse
     *  event management and inertial factors applied during point of view
     *  modification.
     *
     *  It also holds the structure of the sub-modules used by the software such
     *  as the model management, time interface and movie creation.
     *
     *  This structure is the central structure of this graphical clients and
     *  is used and modified by the main module functions.
     *
     *  \var er_client_struct::cl_loops
     *  Execution loop mode
     *  \var er_client_struct::cl_model
     *  Model management sub-system structure
     *  \var er_client_struct::cl_times
     *  Time interface sub-system structure
     *  \var er_client_struct::cl_movie
     *  Movie creation sub-system structure
     *  \var er_client_struct::cl_view
     *  Point of view structure
     *  \var er_client_struct::cl_push
     *  Point of view memory structure
     *  \var er_client_struct::cl_button
     *  Mouse button code
     *  \var er_client_struct::cl_state
     *  Mouse state code
     *  \var er_client_struct::cl_x
     *  Mouse x-position
     *  \var er_client_struct::cl_y
     *  Mouse y-position
     *  \var er_client_struct::cl_inertia
     *  Point of view modification inertial factor
     *  \var er_client_struct::cl_scale
     *  Model dynamic scale factor
     */

    typedef struct er_client_struct {

        le_sock_t  cl_socket;
        le_enum_t  cl_loops;

        er_model_t cl_model;
        er_times_t cl_times;
        er_movie_t cl_movie;

        er_view_t  cl_view;
        er_view_t  cl_push;

        le_enum_t  cl_button;
        le_enum_t  cl_state;
        le_size_t  cl_x;
        le_size_t  cl_y;
        le_real_t  cl_inertia;
        le_real_t  cl_scale;

        le_real_t  cl_mtimea;
        le_real_t  cl_mtimeb;
        le_enum_t  cl_motion;

    le_enum_t _status; } er_client_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates the client structure and returns it. It mainly
     *  calls the sub-modules structure creation functions.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field.
     *
     *  \param er_ip    Server ip address
     *  \param er_port  Server service port
     *
     *  \return Returns the created client structure
     */

    er_client_t er_client_create( le_char_t * const er_ip, le_sock_t const er_port );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided client structure. It deletes the
     *  sub-modules structure using their related deletion functions. It then
     *  clears the structure fields.
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_delete( er_client_t * const er_client );

    /*! \brief main method
     *
     *  The main function holds the principal execution code.
     *
     *  It starts by reading the server address and service port from the main
     *  function standard parameters.
     *
     *  It then configurates the interface window and creates the graphical
     *  context. It also set the configuration of the opengl rendering.
     *
     *  It creates the client structure and starts the execution loops. Two
     *  threads are engaged and their behaviour is driven by the execution loop
     *  mode hold in the client structure. The first thread is responsible of
     *  the scene rendering and events capture. The second thread has the
     *  responsibility of the model update management.
     *
     *  Three execution modes are available : the exit mode that indicates the
     *  threads loop to stop execution ; the view mode, which is the standard
     *  mode, that indicates the graphical thread to render the scene and the
     *  model thread to perform update depending on the point of view ; the
     *  movie mode, used for the exportation of movie frames, tells the model
     *  thread to suspend activity and indicate to the graphical thread that it
     *  has to manage the point of view trajectory and model update in addition
     *  to the scene rendering.
     *
     *  As the execution stop (exit mode), the main function deletes the client
     *  structure and close the graphical context.
     *
     *  \param argc Main function parameters
     *  \param argv Main function parameters
     *
     *  \return Standard exit code
     */

    int main( int argc, char ** argv );

    /*! \brief loop methods
     *
     *  This function is called by the main function graphical thread to trigger
     *  scene rendering according to the point of view.
     *
     *  It starts by clearing buffers and, depending on the scene elements to
     *  render, it calls the specific projection configuration functions.
     *
     *  The function render the model itself with simple wireframe model of the
     *  earth used as reference and the time interface.
     */

    le_void_t er_client_loops_render( le_void_t );

    /*! \brief loop methods ***
     *
     *  This function is called by the main function model thread to trigger
     *  model update according to the point of view.
     *
     *  Using the previous point of view, the function starts by determining if
     *  the point of view has changed. If not, the function ends.
     *
     *  If the point of view has changed, the function starts the model update
     *  procedure using the model module functions.
     */

    le_void_t er_client_loops_update( le_void_t );

    /*! \brief projection methods
     *
     *  This function is used to set the rendering projection matrix for the
     *  model display.
     *
     *  It applies a projection matrix with dynamic near and far planes driven
     *  by specific functions of the geodesy module. Using the geodesy module
     *  function, is also computes and applies the model scale factor.
     *
     *  This function also manage the dynamic configuration of the opengl fog
     *  and enable it.
     *
     *  \param er_width  Screen width, in pixels
     *  \param er_height Screen height, in pixels
     */

    le_void_t er_client_proj_model( int er_width, int er_height );

    /*! \brief projection methods
     *
     *  This function is used to set the rendering projection matrix for the
     *  time interface display.
     *
     *  It applies an orthogonal projection matrix used by the time module to
     *  render the time interface.
     *
     *  This function also disable the opengl fog.
     *
     *  \param er_width  Screen width, in pixels
     *  \param er_height Screen height, in pixels
     */

    le_void_t er_client_proj_interface( int er_width, int er_height );

    /*! \brief callback methods
     *
     *  This function implements the reshape callback.
     *
     *  The function simply sets the opengl viewport according to the provided
     *  parameters in order to occupy the entire screen.
     *
     *  \param er_width  Width, in pixel, of the rendering buffer
     *  \param er_height Height, in pixels, of the rendering buffer
     */

    le_void_t er_client_calls_reshape( int er_width, int er_height );

    /*! \brief callback methods
     *
     *  This function implements the keyboard callback.
     *
     *  It implements a simple switch on the provided keycode. It then applies
     *  the action attached to the pressed key.
     *
     *  \param er_keycode Keyboard key code
     *  \param er_x       Mouse x-position at key press
     *  \param er_y       Mouse y-position at key press
     */

    le_void_t er_client_calls_keybd( unsigned char er_keycode, int er_x, int er_y );

    /*! \brief callback methods
     *
     *  This function implements the mouse callback.
     *
     *  Depending on the pressed button, while taking into account the keyboard
     *  modifiers state, the function updates the point of view based on the
     *  mouse event.
     *
     *  \param er_button Mouse button code
     *  \param er_state  Mouse button state
     *  \param er_x      Mouse x-position at click
     *  \param er_y      Mouse y-position at click
     */

    le_void_t er_client_calls_mouse( int er_button, int er_state, int er_x, int er_y );

    /*! \brief callback methods
     *
     *  This function implements the motion callback.
     *
     *  Depending on the pressed button and on the initial mouse position, while
     *  taking into account the state of the keyboard modifiers, the function
     *  updates the point of view.
     *
     *  \param er_x Mouse x-position
     *  \param er_y Mouse y-position
     */

    le_void_t er_client_calls_move( int er_x, int er_y );

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

