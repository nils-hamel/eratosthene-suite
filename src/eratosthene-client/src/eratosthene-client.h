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
     *  softwares suite offering front-end solutions to the functionalities of
     *  the eratosthene library.
     *
     *  The principal software implements the server itself allowing to simply
     *  create instances and maintain services. The suite also offers a
     *  front-end software for the data injection in the available servers. It
     *  also offers a graphical software allowing to browse the 4D worlds
     *  offered by the available servers.
     *
     *  Other tools are also available as the isometric projection creation and
     *  the address and time conversion softwares.
     *
     *  \section project Eratosthene Project
     *
     *  The Eratosthene Project was initially imagined to answer the question of
     *  very large point-based models of cities and environments. Indeed, mapping
     *  a whole city at the level of the centimetre induces very large and heavy
     *  models that are difficult to manipulate, explore and exploit. Moreover,
     *  considering the time dimension implies a quick increase of the size of
     *  the models, making them almost impossible to handle as a single object.
     *
     *  The Eratosthene Project is designed to answer this question by
     *  implementing an space-time indexation formalism able to handle, store
     *  and access very large 4D models of cities and environments. The
     *  indexation formalism is used to implement a server able to manage,
     *  store and broadcast very large 4D models distributed all over the earth
     *  surface and covering large period of time. In addition to the server,
     *  the Eratosthene Project also implements a graphical client able to
     *  communicate with the server and to offer a convenient way of browsing
     *  the models in both space and time.
     *
     *  The space-time indexation formalism is designed to efficiently manage
     *  the storage of the incoming 4D models and to offer and simple and
     *  efficient ways of addressing queries to the server allowing models
     *  broadcasting. The space-time indexation also allows to perform queries
     *  to the server in a intuitive way giving the possibility to both address
     *  the questions of the earth structures levels of detail and time
     *  management. This allows to explore the entire earth surface in terms of
     *  4D structures from large mountain chains in 2000 to very small details
     *  of cities in 1850.
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
    # define ER_CLIENT_C      { _LE_SOCK_NULL, ER_COMMON_VIEW, ER_MODEL_C, ER_TIMES_C, ER_VIEW_D, ER_VIEW_C, 0, 0, 0, 0, 1.0, 1.0, _LE_FALSE }

    /* define pseudo-constructor */
    # define ER_CLIENT_I(w,h) { _LE_SOCK_NULL, ER_COMMON_VIEW, ER_MODEL_C, ER_TIMES_C, ER_VIEW_D, ER_VIEW_C, w, h, 0, 0, 1.0, 1.0, _LE_FALSE }

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
     *  This structure holds the graphical client main instance. It holds
     *  fields related to point of view motion management and sub-structures
     *  related to the different client features.
     *
     *  The structure holds the client socket descriptor created with this
     *  structure. It contains also the value used to maintain the software
     *  loop.
     *
     *  In addition to sub-structures and point of view, this structures also
     *  holds fields related to the management of the mouse motion.
     *
     *  Finally, this structure also holds a scale value used to adapt the
     *  displayed model scale according to the point of view to avoid the
     *  saturation of the single precision format, as the client has to be able
     *  to provide the display of earth-scale centimetric models.
     *
     *  \var er_client_struct::cl_socket
     *  Client socket
     *  \var er_client_struct::cl_loops
     *  Execution loop state
     *  \var er_client_struct::cl_model
     *  Model management sub-structure
     *  \var er_client_struct::cl_times
     *  Time interface sub-structure
     *  \var er_client_struct::cl_view
     *  Active point of view structure
     *  \var er_client_struct::cl_push
     *  Pushed point of view structure
     *  \var er_client_struct::cl_width
     *  Display resolution width, in pixels
     *  \var er_client_struct::cl_height
     *  Display resolution height, in pixels
     *  \var er_client_struct::cl_x
     *  Mouse click x-position
     *  \var er_client_struct::cl_y
     *  Mouse click y-position
     *  \var er_client_struct::cl_inertia
     *  Point of view inertia factor
     *  \var er_client_struct::cl_scale
     *  Model dynamic scale factor
     *  \var er_client_struct::_status
     *  Standard status field
     */

    typedef struct er_client_struct {

        le_sock_t  cl_socket;
        le_enum_t  cl_loops;

        er_model_t cl_model;
        er_times_t cl_times;

        er_view_t  cl_view;
        er_view_t  cl_push;

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

    /*! \brief constructor/destructor methods
     *
     *  This function creates the client structure and returns it. It mainly
     *  calls the sub-modules structure creation functions to initialise them.
     *
     *  In addition to sub-modules creation, this function also creates the
     *  socket to the remote server according to the IP address and service
     *  port. It then requests the configuration of the remote server, needed
     *  to perform data queries. The received configuration is then broadcast
     *  to the sub-modules.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field.
     *
     *  \param er_ip     Server ip address
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
     *  sub-modules structures using their related deletion functions. The
     *  socket toward the remote server is closed and the structure fields are
     *  cleared.
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_delete( er_client_t * const er_client );

    /*! \brief main method
     *
     *  The main function allows to query and display a four dimensional model
     *  of the earth :
     *
     *      ./-client --ip/-i, --port/-p [remote server access]
     *
     *  The main function starts by creating the graphical context of the
     *  client by creating the display window. It also configure the opengl
     *  graphical context.
     *
     *  It then initialises the client main structure that is responsible of
     *  sub-structures creation and connection to the remove server creation.
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
     *  loops management.
     *
     *  Two threads are instanced by this function. The principal thread is
     *  responsible of the graphical process of the client. The secondary thread
     *  is responsible of the earth model update according to the motion of the
     *  point of view.
     *
     *  The user events are processed through the principal thread. As an event
     *  is catch, the specialised callback process are called to handle the
     *  event. The user events are also used to interrupt the main loop causing
     *  the function to exit.
     *
     *  \param er_client Client structure
     *  \param er_window SDL window structure
     */

    le_void_t er_client_loops(  er_client_t * const er_client, SDL_Window * const er_window );

    /*! \brief loop methods
     *
     *  This function is called by the main loop function to trigger earth model
     *  rendering according to the point of view.
     *
     *  It starts by clearing buffers and, depending on the scene elements to
     *  render, it calls the specific projection configuration functions.
     *
     *  The function displays the earth model itself, the time navigation
     *  interface and a simple wireframe model of the earth used as a guide
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_loops_render( er_client_t * const er_client );

    /*! \brief loop methods
     *
     *  This function is responsible of the user event catching and processing.
     *  It is called by the main loop function.
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
     *  This function is called by the main loop function to trigger the earth
     *  model update according to the point of view.
     *
     *  Using the pushed point of view, the function starts by determining if
     *  the point of view has changed. If the point of view has changed, the
     *  function update the target model. It then calls the synchronisation
     *  process that updates the actual model to fit the target model using
     *  a step by step procedure. The current point of view is finally pushed.
     *
     *  If the point of view has not changed, only the synchronisation takes
     *  place.
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_loops_update( er_client_t * const er_client );

    /*! \brief projection methods
     *
     *  This function is used to set the rendering projection matrix for the
     *  earth model display.
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
     *  This function also disable the OpenGL fog.
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
     *  Depending on the keyboard modifier state, the function updates the time
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
     *  Depending on the mouse button state, stacks the middle position of the
     *  screens and moves the mouse cursor to this position.
     *
     *  \param er_event  SDL event structure
     *  \param er_client Client structure
     */

    le_void_t er_client_callback_button( SDL_MouseButtonEvent er_event, er_client_t * const er_client );

    /*! \brief callback methods
     *
     *  This function implements the mouse button callback function.
     *
     *  Depending on the mouse button state, the function updates the point of
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

