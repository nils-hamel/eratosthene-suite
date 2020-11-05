/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@alumni.epfl.ch
 *      Copyright (c) 2016-2020 DHLAB, EPFL
 *      Copyright (c) 2020 Republic and Canton of Geneva
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
     *  \author Nils Hamel <nils.hamel@alumni.epfl.ch>
     *
     *  eratosthene-suite - client
     */

    /*! \mainpage eratosthene-suite
     *
     *  \section license Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016-2020 DHLAB, EPFL
     *  Copyright (c) 2020 Republic and Canton of Geneva
     *
     *  This program is licensed under the terms of the GNU GPLv3. Documentation    
     *  and illustrations are licensed under the terms of the CC BY 4.0.
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
    # include "eratosthene-client-gui.h"
    # include "eratosthene-client-model.h"
    # include "eratosthene-client-video.h"
    # include "eratosthene-client-view.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_CLIENT_C      { _LE_SOCK_NULL, 0, 0, ER_COMMON_VIEW, _LE_TRUE, _LE_TRUE, _LE_TRUE, ER_MODEL_C, ER_GUI_C, ER_VIDEO_C, ER_VIEW_D, ER_VIEW_C, _LE_TIME_MAX, 0, 0, 0, 0, 1.0, 1.0, _LE_FALSE }

    /* define pseudo-constructor */
    # define ER_CLIENT_I(w,h) { _LE_SOCK_NULL, 0, 0, ER_COMMON_VIEW, _LE_TRUE, _LE_TRUE, _LE_TRUE, ER_MODEL_C, ER_GUI_C, ER_VIDEO_C, ER_VIEW_D, ER_VIEW_C, _LE_TIME_MAX, w, h, 0, 0, 1.0, 1.0, _LE_FALSE }

    /* define window properties */
    # define ER_SDL_FLAGS     ( SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN )

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
     *  asked through a query just after connection establishment with the
     *  remote server.
     *
     *  The next field is used to control the state of the execution of the
     *  software. Its value indicates the execution mode that can take several
     *  value depending on the mode. Two execution modes are available : the
     *  user controlled interface and the automatic point of view used to create
     *  video frames along a user pre-defined path. If the value is in the third
     *  mode, it simply indicates the execution to stop.
     *
     *  The three next fields are related to the rendering display management.
     *  Thier value, that are simple boolean values, indicates if the model, the
     *  Earth wireframe and the GUI information have to be displayed or not.
     *
     *  The three next fields hold the structure of the sub-modules. Sub-modules
     *  are initialised and used through their representation in this structure.
     *  The first sub-module is related to the Earth model management through
     *  communication with the remote server. The second sub-module is related
     *  to the display of the GUI information. The last sub-module controls the
     *  automatic path used to create video frames.
     *
     *  The next three fields are related to the management of the point of view
     *  modification detection. The actual point of view structure is hold along
     *  with the pushed point of view to detect motion. A clock value is also
     *  used to delay a bit the model update after a modification of the point
     *  of view in order to save bandwidth between the client and server.
     *
     *  The two next fields are holding the size, in pixels, of the interface
     *  display. As the client is expected to launch in full screen mode, these
     *  values should hold the current resolution of the display.
     *
     *  The three next fields are related to mouse click management and motion
     *  inertia, allowing to move to point of view orientation and position. The
     *  coordinates are used to store the previous position of the last mouse
     *  click while the inertia field controls the force with which the point of
     *  view is modified.
     *
     *  The last field is holding the overall Earth model scale factor. This
     *  factor is dynamically adapted to keep the Earth model in a reasonable
     *  range. This allows to avoid saturation of the simple precision while
     *  displaying a continuous centimetric model of the Earth.
     *
     *  \var er_client_struct::cl_socket
     *  Socket toward remote server - main connection
     *  \var er_client_struct::cl_scfg
     *  Remote server spatial configuration parameter
     *  \var er_client_struct::cl_tcfg
     *  Remote server temporal configuration parameter
     *  \var er_client_struct::cl_loops
     *  Execution state
     *  \var er_client_struct::cl_mshow
     *  Model display flag
     *  \var er_client_struct::cl_wshow
     *  Wireframe display flag
     *  \var er_client_struct::cl_tshow
     *  GUI information display flag
     *  \var er_client_struct::cl_model
     *  Model sub-module structure
     *  \var er_client_struct::cl_gui
     *  GUI sub-module structure
     *  \var er_client_struct::cl_video
     *  Video sub-module structure
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

        le_enum_t  cl_mshow;
        le_enum_t  cl_wshow;
        le_enum_t  cl_tshow;

        er_model_t cl_model;
        er_gui_t   cl_gui;
        er_video_t cl_video;

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

    /*! \brief constructor/destructor methods
     *
     *  This function creates the client structure and returns it. It mainly
     *  calls the sub-modules structure creation functions to initialise them.
     *
     *  In addition to sub-modules creation, this function also creates the
     *  socket to the remote server according to the IP address and service
     *  port. It also retrieves the server configuration parameter through a
     *  call to the specialised function. The received configuration is then
     *  broadcast to the sub-modules requiring it.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field. On error, the
     *  \b _status field is returned with value \b _LE_FALSE and \b _LE_TRUE
     *  otherwise.
     *
     *  \param er_ip     Server IP address
     *  \param er_port   Server service port
     *  \param er_width  Screen horizontal resolution, in pixels
     *  \param er_height Screen vertical resolution, in pixels
     *  \param er_export Video frame exportation path
     *  \param er_frame  Video frame count
     *
     *  \return Returns the created client structure
     */

    er_client_t er_client_create( le_char_t * const er_ip, le_sock_t const er_port, le_size_t const er_width, le_size_t const er_height, le_char_t * const er_export, le_size_t const er_frame );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided client structure. It deletes the
     *  sub-modules structure using their related deletion functions.
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
     *  is analysed to extract the server parameters.
     *
     *  \param er_client Client structure
     *
     *  \return Return _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_client_set_server( er_client_t * const er_client );

    /*! \brief main method
     *
     *  The main function allows to query and display a four dimensional model
     *  of the Earth through communication with a remote server :
     *
     *      ./-client --ip/-i --port/-p [remote server address and service]
     *                --export/-e [video frame exportation directory]
     *                --frame/-f [video frame count]
     *
     *  The main function starts by creating the graphical context of the
     *  client by creating the render window and context.
     *
     *  It then initialises the client main structure that is responsible of
     *  sub-modules creation and connection to the remove server. The IP address
     *  of the remote server is provided through the '--ip' argument while the
     *  service port is set through the '--port' argument.
     *
     *  As both graphical context and client structure are created, the main
     *  function invoke the function responsible of the main execution loop and
     *  threads management. Two main threads are created : the first one is
     *  responsible of the graphical model display and interface events handling
     *  while the second is responsible of the Earth model update through the
     *  current viewpoint and queries to the remote server.
     *
     *  When the 'video' mode is enable, the '--export' path is used to export
     *  the computed frame. The '--frame' allows to determine the number of
     *  frames to compute along the pre-defined path. In such a case, only the
     *  first thread is used being responsible of both rendering and model
     *  update. The interface events are not handled in this execution mode.
     *
     *  As the main execution loop is terminated, the main function destroys the
     *  client main structure and the graphical context.
     *
     *  \param argc Standard parameters
     *  \param argv Standard parameters
     *
     *  \return Standard exit code
     */

    int main( int argc, char ** argv );

    /*! \brief loop methods
     *
     *  This function is responsible of the OpenGL configuration and main loop
     *  threads management.
     *
     *  Two threads are created by this function. The principal thread is
     *  responsible of the graphical process and events management. The second
     *  thread is responsible of the Earth model update according to the motion
     *  of the point of view through queries to the remote server.
     *
     *  The user events are processed through the principal thread. As an event
     *  is catch, the specialised callback processes are called to handle the
     *  event. The user events are also used to interrupt the main loop causing
     *  the function to exit.
     *
     *  \param er_client Client structure
     *  \param er_window SDL window structure
     */

    le_void_t er_client_loops( er_client_t * const er_client, SDL_Window * const er_window );

    /*! \brief loop methods
     *
     *  This function is called by the main loop function to trigger Earth model
     *  rendering according to the point of view.
     *
     *  It starts by clearing buffers and, depending on the scene elements to
     *  render, it calls the specific projection configuration functions.
     *
     *  The function displays the Earth model itself, the GUI information and a
     *  simple wireframe model of the Earth according to their display flag.
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_loops_render( er_client_t * const er_client );

    /*! \brief loop methods
     *
     *  This function is responsible of the user events catching and processing.
     *  It is called by the principal thread.
     *
     *  As the events stack is not empty, the function pop each event one by one
     *  and call they respective specific processing callbacks. The function
     *  returns as the events stack is empty.
     *
     *  \param er_client Client structure
     */

    le_void_t er_client_loops_event( er_client_t * const er_client );

    /*! \brief loop methods
     *
     *  This function is called by the main and secondary thread to trigger the
     *  Earth model update according to the point of view.
     *
     *  Using the pushed point of view, the function determine if a modification
     *  of the current point of view occurs. Before to trigger a model update,
     *  the function waits for a delay value, provided as parameter. This allows
     *  to reduce the frequency, and so bandwidth usage, when successive and
     *  close modifications of the point of view are performed.
     *
     *  As the delay passed, the function prepares and triggers the model update
     *  procedure. The target cell stack is updated and the fast update process
     *  is executed. The cell state detection is also made through a specific
     *  query to the remove server. The delay clock is also reset at this point.
     *
     *  In any case, the function ends with the cell stacks synchronisation
     *  process. This allows to perform model update cell by cell allowing to
     *  cancel a model update as a new one is triggered by a modification of the
     *  viewpoint.
     *
     *  If the point of view has not changed, only this last synchronisation
     *  takes place until both target and render cell stacks are identical.
     *
     *  \param er_client Client structure
     *  \param er_delay  Update delay value, in CLOCKS_PER_SEC
     */

    le_enum_t er_client_loops_update( er_client_t * const er_client, le_size_t const er_delay );

    /*! \brief projection methods
     *
     *  This function is used to set the rendering projection matrix for the
     *  Earth model display.
     *
     *  It applies a projection matrix with dynamic near and far planes driven
     *  by specific functions of the geodesy module. Using the geodesy module
     *  functions, it also computes and applies the model scale factor. Finally,
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
     *  GUI information display.
     *
     *  It applies an orthogonal projection matrix used by the GUI module to
     *  render the interface. It also disable the OpenGL depth test and fog
     *  feature.
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
     *  The function initialises the click event for subsequent mouse motion
     *  callback by pushing the middle screen position in the provided client
     *  structure. The function also forces the position of the mouse at the
     *  pushed middle screen position.
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
     *  view position or orientation. It detects the mouse motion on the screen
     *  to apply the relevant modification of the orientation or position.
     *
     *  The function also sets the position of the mouse cursor to the middle of
     *  the screen to implement limit-less mouse motion.
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

