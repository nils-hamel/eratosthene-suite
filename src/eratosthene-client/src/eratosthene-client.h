/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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
     *  \section overview Overview
     *
     *  The _eratosthene-suite_ offers a front-end to the eratosthene indexation
     *  server implemented in the _liberatosthene_ library. It consists in a
     *  software suite offering front-end solutions to the functionalities of
     *  the eratosthene library.
     *
     *  The principal software implements the server itself, allowing to simply
     *  create server instances and to maintain network services. The suite also
     *  offers a front-end software for the data injection in server instances.
     *  It also offers a graphical 4D solution allowing to browse the 4D model
     *  of Earth offered by servers.
     *
     *  \section project Eratosthene Project
     *
     *  The Eratosthene Project was initially designed to address the challenge
     *  of very large point-based models of cities and environments. Indeed,
     *  mapping a whole city at the level of the centimeter induces very large
     *  and heavy models that are difficult to manipulate, explore and exploit.
     *  Moreover, considering the time dimension implies a quick increase of the
     *  size of the models, making them impossible to handle as a single and
     *  monolithic object.
     *
     *  The Eratosthene Project is designed to provide a solution by considering
     *  a space-time indexation formalism. This indexation formalism is used to
     *  implement a server able to manage, store and broadcast over the network
     *  very large 4D models distributed all over the earth surface and covering
     *  large periods of time. In addition to the server, the Eratosthene
     *  Project also implements a graphical client able to communicate with the
     *  remote servers to offer a convenient way to browse the models in both
     *  space and time.
     *
     *  The space-time indexation formalism is designed to efficiently manage
     *  the storage of the incoming 4D models and to offer a simple and
     *  efficient way of addressing queries to the server to allow models
     *  broadcasting. The space-time indexation also allows to perform queries
     *  to the server in an intuitive way giving the possibility to both address
     *  the challenge of the earth structures level of details and time
     *  management. This allows to explore the entire earth surface in terms of
     *  4D structures from large mountain chains in 2000 to very small details
     *  of cities in 1850.
     *
     *  The subsequent indexation formalism used to drive the storage and access
     *  of the data also naturally brings a standardised discretisation of space
     *  and time. Point-based models injected in the Eratosthene information
     *  system are then standardised from both spatial and temporal point of
     *  views. This allows the implementation of binary and logical operators
     *  when considering a model at two different times. The operators allow to
     *  deduce a third model on the fly providing a differential point of view
     *  of the two models separated in time. The evolution of cities and
     *  environments can be enhanced and analysed in an efficient way, opening
     *  the way to more advanced geographic services.
     *
     *  The ability of indexation formalism to consider polygonal models in
     *  addition to large scale point-based models allows to enhance the
     *  possibilities in terms of 4D geographic services. More than the
     *  possibility to automatically manage the polygonal models level of
     *  details and large scale display, the indexation formalism also allows
     *  compositions on the fly of mixed models allowing to compare different
     *  type of models, from lidar point-based models to land registers.
     *
     *  \section license Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016-2018 DHLAB, EPFL
     *
     *  This program is licensed under the terms of the GNU GPLv3.
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT__
    # define __ER_CLIENT__

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
    # include "eratosthene-client-view.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_CLIENT_C      { _LE_SOCK_NULL, 0, 0, ER_COMMON_VIEW, ER_MODEL_C, ER_TIMES_C, ER_VIEW_D, ER_VIEW_C, _LE_TIME_MAX, 0, 0, 0, 0, 1.0, 1.0, _LE_FALSE }

    /* define pseudo-constructor */
    # define ER_CLIENT_I(w,h) { _LE_SOCK_NULL, 0, 0, ER_COMMON_VIEW, ER_MODEL_C, ER_TIMES_C, ER_VIEW_D, ER_VIEW_C, _LE_TIME_MAX, w, h, 0, 0, 1.0, 1.0, _LE_FALSE }

    /* define window properties */
    # define ER_SDL_FLAGS     ( SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL )

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
     *  This structure is the main graphical client structure. It holds fields
     *  related to graphical instance, sub-modules and event management.
     *
     *  In the first place, the structure holds the socket descriptor used for
     *  communication with the remote server.
     *
     *  The two next field holds the server configuration parameters that are
     *  asked through a query just after connection establishment.
     *
     *  The next field is used to control the state of the execution as it
     *  indicates the execution loop to continue or interrupt their tasks.
     *
     *  The two next fields holds the structures of the sub-modules. Sub-modules
     *  are initialised and used through their representation in this structure.
     *
     *  The next three fields are related to the management of the point of
     *  view. The actual point of view structure is hold along with the pushed
     *  point of view to detect motion. A clock value is also used to delay a
     *  bit the trigger of the model update after a modification of the point of
     *  view.
     *
     *  The two next fields are holding the size, in pixels, of the interface
     *  display.
     *
     *  The three next fields are related to mouse click management and motion
     *  inertia, allowing to move to point of view.
     *
     *  The last field is holding the overall Earth model scale factor. This
     *  factor is dynamically adapted to keep the Earth model in a reasonable
     *  range to handle simple precision of most display devices.
     *
     *  \var er_client_struct::cl_socket
     *  Socket toward remote server - main connection
     *  \var er_client_struct::cl_scfg
     *  Remote server spatial configuration parameter
     *  \var er_client_struct::cl_tcfg
     *  Remote server temporal configuration parameter
     *  \var er_client_struct::cl_loops
     *  Execution state value
     *  \var er_client_struct::cl_model
     *  Model sub-module structure
     *  \var er_client_struct::cl_times
     *  Time sub-module structure
     *  \var er_client_struct::cl_view
     *  Active point of view
     *  \var er_client_struct::cl_push
     *  Pushed point of view
     *  \var er_client_struct::cl_last
     *  Delayed model update clock
     *  \var er_client_struct::cl_width
     *  Display x-resolution, in pixels
     *  \var er_client_struct::cl_height
     *  Display y-resolution, in pixels
     *  \var er_client_struct::cl_x
     *  Mouse click x-position - according to screen
     *  \var er_client_struct::cl_y
     *  Mouse click y-position - according to screen
     *  \var er_client_struct::cl_inertia
     *  Motion inertial factor
     *  \var er_client_struct::cl_scale
     *  Overall model dynamical scale value
     *  \var er_client_struct::_status
     *  Standard status field
     */

    typedef struct er_client_struct {

        le_sock_t  cl_socket;

        le_size_t  cl_scfg;
        le_time_t  cl_tcfg;

        le_enum_t  cl_loops;

        er_model_t cl_model;
        er_times_t cl_times;

        er_view_t  cl_view;
        er_view_t  cl_push;
        le_time_t  cl_last;

        le_size_t  cl_width;
        le_size_t  cl_height;

        le_size_t  cl_x;
        le_size_t  cl_y;
        le_real_t  cl_inertia;

        le_real_t  cl_scale;

    le_enum_t _status; } er_client_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods (revoked)
     *
     *  This function creates the client structure and returns it. It mainly
     *  calls the sub-modules structure creation functions to initialise them.
     *
     *  In addition to sub-modules creation, this function also creates the
     *  socket to the remote server according to the IP address and service
     *  port. It also retrieves the server configuration parameter through a
     *  call to the specialised function. The received configuration is then
     *  broadcast to the sub-modules.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field.
     *
     *  \param er_ip     Server IP address
     *  \param er_port   Server service port
     *  \param er_width  Screen horizontal resolution, in pixels
     *  \param er_height Screen vertical resolution, in pixels
     *
     *  \return Returns the created client structure
     */

    er_client_t er_client_create( le_char_t * const er_ip, le_sock_t const er_port, le_size_t const er_width, le_size_t const er_height );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided client structure. It deletes the
     *  sub-modules structures using their related deletion functions.
     *
     *  In addition, the socket toward the remote server is closed and the
     *  structure fields are cleared using default values.
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_delete( er_client_t * const er_client );

    /*! \brief mutator methods
     *
     *  This function allows to query the remote server configuration values and
     *  to store them in the provided client structure.
     *
     *  The configuration query is made toward the remote server and the answer
     *  is analysed to extract the parameters.
     *
     *  \param er_client Client structure
     *
     *  \return Return _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_client_set_server( er_client_t * const er_client );

    /*! \brief main method
     *
     *  The main function allows to query and display a four dimensional model
     *  of the Earth :
     *
     *      ./-client --ip/-i, --port/-p [remote server access]
     *
     *  The main function starts by creating the graphical context of the
     *  client by creating the display window. It also configure the OpenGL
     *  graphical context.
     *
     *  It then initialises the client main structure that is responsible of
     *  sub-modules creation and connection to the remove server creation.
     *  The IP address of the remote server is provided through the '--ip'
     *  argument while the service port is set through the '--port' argument.
     *
     *  As both graphical context and client structure are created, the main
     *  function invoke the function responsible of the main execution loop and
     *  thread management.
     *
     *  As the main execution loop is terminated, the main function destroy the
     *  client main structure and the graphical context.
     *
     *  \param argc Main function parameters
     *  \param argv Main function parameters
     *
     *  \return Standard exit code
     */

    int main( int argc, char ** argv );

    /*! \brief loop methods
     *
     *  This function is responsible of the OpenGL configuration and the main
     *  execution loops management.
     *
     *  Two threads are created by this function. The principal thread is
     *  responsible of the graphical process of the client. The secondary thread
     *  is responsible of the Earth model update according to the motion of the
     *  point of view.
     *
     *  The user events are processed through the principal thread. As an event
     *  is catch, the specialised callback processes are called to handle the
     *  event. The user events are also used to interrupt the main loop causing
     *  the function to exit.
     *
     *  \param er_client Client structure
     *  \param er_window SDL window structure
     */

    le_void_t er_client_loops(  er_client_t * const er_client, SDL_Window * const er_window );

    /*! \brief loop methods
     *
     *  This function is called by the main loop function to trigger Earth model
     *  rendering according to the point of view.
     *
     *  It starts by clearing buffers and, depending on the scene elements to
     *  render, it calls the specific projection configuration functions.
     *
     *  The function displays the Earth model itself, the time navigation
     *  interface and a simple wire frame model of the Earth used as a guide.
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_loops_render( er_client_t * const er_client );

    /*! \brief loop methods
     *
     *  This function is responsible of the user events catching and processing.
     *  It is called by the principal thread.
     *
     *  As the event stack is not empty, the function pop each event one by one
     *  and call they respective specific processing callbacks. The function
     *  returns as the event stack is empty.
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_loops_event( er_client_t * const er_client );

    /*! \brief loop methods
     *
     *  This function is called by the secondary thread to trigger the Earth
     *  model update according to the point of view.
     *
     *  Using the pushed point of view, the function determine if a modification
     *  of the point of view occurs. Before to trigger a model update, the
     *  function waits half a second. This allows to reduce the frequency, and
     *  bandwidth usage, when successive and close modification of the point of
     *  view are performed.
     *
     *  As the delay passed, the function prepare and triggers the model update
     *  procedure. The virtual cell stack is updated and the quick update
     *  process is executed. The delay clock is also reset.
     *
     *  In any case, the function ends with the cell stacks synchronisation
     *  process. This allows to perform model update by small chunk allowing to
     *  cancel a model update as a new one is necessary.
     *
     *  If the point of view has not changed, only this last synchronisation
     *  takes place.
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_loops_update( er_client_t * const er_client );

    /*! \brief projection methods
     *
     *  This function is used to set the rendering projection matrix for the
     *  Earth model display.
     *
     *  It applies a projection matrix with dynamic near and far planes driven
     *  by specific functions of the geodesy module. Using the geodesy module
     *  functions, is also computes and applies the model scale factor. Finally,
     *  the function enables the OpenGL depth test.
     *
     *  This function also manage the dynamic configuration of the OpenGL fog
     *  and enable it.
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_proj_model( er_client_t * const er_client );

    /*! \brief projection methods
     *
     *  This function is used to set the rendering projection matrix for the
     *  time navigation interface display.
     *
     *  It applies an orthogonal projection matrix used by the time module to
     *  render the time interface. It also disable the OpenGL depth test.
     *
     *  This function also disable the OpenGL fog feature.
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_proj_interface( er_client_t * const er_client );

    /*! \brief callback methods
     *
     *  This function implements the keyboard callback function.
     *
     *  It implements a simple switch on the provided keycode. It then applies
     *  the action attached to the pressed key.
     *
     *  \param er_event  SDL event structure
     *  \param er_client Client structure
     */

    le_void_t er_client_callback_keydown( SDL_KeyboardEvent er_event, er_client_t * const er_client );

    /*! \brief callback methods
     *
     *  This function implements the mouse wheel callback function.
     *
     *  Depending on the keyboard modifiers state, the function updates the time
     *  position, the time area range or the spatial position.
     *
     *  \param er_event  SDL event structure
     *  \param er_client Client structure
     */

    le_void_t er_client_callback_wheel( SDL_MouseWheelEvent er_event, er_client_t * const er_client );

    /*! \brief callback methods
     *
     *  This function implements the mouse button callback function.
     *
     *  Depending on the mouse buttons state, the function pushes the screen
     *  middle pixel on the mouse click fields of the provided client structure
     *  and moves the position of the mouse to it.
     *
     *  \param er_event  SDL event structure
     *  \param er_client Client structure
     */

    le_void_t er_client_callback_button( SDL_MouseButtonEvent er_event, er_client_t * const er_client );

    /*! \brief callback methods
     *
     *  This function implements the mouse button callback function.
     *
     *  Depending on the mouse buttons state, the function updates the point of
     *  view position or orientation. In addition, the function also moves the
     *  position of the mouse cursor to implement a virtual limit-less screen,
     *  allowing infinite motion.
     *
     *  \param er_event  SDL event structure
     *  \param er_client Client structure
     */

    le_void_t er_client_callback_motion( SDL_MouseMotionEvent er_event, er_client_t * const er_client );

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

