/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2020 DHLAB, EPFL
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

    /*! \file   eratosthene-client-video.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - client - video
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_VIDEO__
    # define __ER_CLIENT_VIDEO__

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
    # include "eratosthene-client-view.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_VIDEO_C     { NULL, 0, 0, 0, 0, NULL, 0, { ER_VIEW_C }, _LE_FALSE }

    /* define view stack size */
    # define ER_VIDEO_STACK ( 16 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct er_video_struct
     *  \brief Video structure
     *
     *  This structure holds the required elements for video frames computation
     *  along a user pre-defined path. It holds the video frame buffer and its
     *  properties along with the spatio-temporal elements of the pre-defined
     *  trajectory.
     *
     *  The pre-defined path set by the user is a sequence of view structures
     *  that are interpolated in terms of position and orientation to create a
     *  continuous trajectory. As the client is set in video mode, this module
     *  computes and drives the point of view based on the interpolated view
     *  structure. At each step, defined by the length of the trajectory and the
     *  amount of frame to compute, the render buffer is saved in a image file
     *  in the specified exportation path. At the end of the process, this
     *  module sets back the client to the user view mode.
     *
     *  The first field contains the exportation path of the video frames. The
     *  two next fields stores the video frame computation index and count. The
     *  frames count describe how the trajectory is discretized.
     *
     *  The three next fields stores the video frame buffer bytes and resolution
     *  information. The resolution of the frame always takes the resolution of
     *  the client render display.
     *
     *  The two last fields stores the amount of trajectory interpolation nodes
     *  the nodes themselves that are simple view structures pushed by the user
     *  through the interface.
     *
     *  \var er_video_struct::vd_path
     *  Video frames exportation path
     *  \var er_video_struct::vd_index
     *  Video frames computation index
     *  \var er_video_struct::vd_count
     *  Video frames count
     *  \var er_video_struct::vd_width
     *  Video frames buffer width, in pixels
     *  \var er_video_struct::vd_height
     *  Video frame buffer height, in pixels
     *  \var er_video_struct::vd_buffer
     *  Video frame buffer bytes, in RGB888 format
     *  \var er_video_struct::vd_push
     *  Amount of pushed views (interpolation nodes)
     *  \var er_video_struct::vd_view
     *  View structures stack
     *  \var er_video_struct::_status
     *  Standard status field
     */

    typedef struct er_video_struct {

        le_char_t * vd_path;

        le_size_t   vd_index;
        le_size_t   vd_count;

        le_size_t   vd_width;
        le_size_t   vd_height;
        le_byte_t * vd_buffer;

        le_size_t   vd_push;
        er_view_t   vd_view[ER_VIDEO_STACK];

    int _status; } er_video_t;


/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates a video structure using the provided exportation
     *  path, frame buffer resolution and frame count.
     *
     *  The function starts by assigning the provided parameters to the video
     *  structure before to allocate the frames buffer memory.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field. On error, the
     *  \b _status field is returned with value \b _LE_FALSE and \b _LE_TRUE
     *  otherwise.
     *
     *  \param er_path   Video frames exportation path
     *  \param er_count  Video frames count
     *  \param er_width  Video buffer width, in pixels
     *  \param er_height Video buffer height, in pixels
     *
     *  \return Returns the created video structure
     */

    er_video_t er_video_create( le_char_t * const er_path, le_size_t const er_count, le_size_t const er_width, le_size_t const er_height );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided video structure.
     *
     *  It starts by releasing the frames buffer memory before to reset the
     *  structure fields with default values.
     *
     *  \param er_video Video structure
     */

    le_void_t er_video_delete( er_video_t * const er_video );

    /*! \brief accessor methods
     *
     *  This function is responsible of computing the interpolated view based on
     *  the provided video structure views stack and the current index of the
     *  video frame.
     *
     *  In the first place, a non-linear near-interpolation procedure is applied
     *  to compute the position in space of the frame viewpoint.
     *
     *  In the second place, a linear interpolation is applied to compute the
     *  position in time and temporal comb size.
     *
     *  The interpolated position in space and time are assigned to a view
     *  structure that is returned by the function in order for the client to
     *  create the trajectory.
     *
     *  \param er_video Video structure
     *
     *  \return Return interpolated view structure
     */

    er_view_t er_video_get( er_video_t const * const er_video );

    /*! \brief accessor methods
     *
     *  This function checks if the required elements are available in the video
     *  structure provided as parameter.
     *
     *  It checks the exportation path state, the video frames count value and
     *  the amount of pushed views on the stack.
     *
     *  If all the information needed to computed the video frames are present,
     *  the function returns LC_TRUE, LC_FALSE otherwise.
     *
     *  \param er_video Video structure
     *
     *  \return Returns LC_TRUE on valid structure, LC_FALSE otherwise
     */

    le_enum_t er_video_get_state( er_video_t const * const er_video );

    /*! \brief mutator methods
     *
     *  This function is responsible of extracting the OpenGL render buffer in
     *  the provided video structure buffer. The buffer is then exported in a
     *  portable network graphic file using the exportation path of the provided
     *  video structure.
     *
     *  This function is also responsible of increasing the video frames index.
     *
     *  Depending on the video frames index, the function returns the required
     *  execution mode. If the frames index is lower than the frames count, the
     *  function returns the video mode value in order to continue the frames
     *  computation and trajectory computation. As the frames count is reached,
     *  this function returns the user view mode.
     *
     *  \param er_video Video structure
     *
     *  \return Returns ER_COMMON_VIEW if all frames have been computed and
     *  exported, ER_COMMON_AUTO otherwise
     */

    le_enum_t er_video_set( er_video_t * const er_video );

    /*! \brief mutator methods
     *
     *  This function allows to push a view structure in the views stack of the
     *  provided video structure.
     *
     *  The function starts by checking if space is remaining in the stack of
     *  the video structure before to push the provided view structure.
     *
     *  \param er_video Video structure
     *  \param er_view  View structure
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_video_set_push( er_video_t * const er_video, er_view_t const * const er_view );

    /*! \brief mutator methods
     *
     *  This function allows to empty the provided video structure views stack.
     *  It then allows to reset the interpolation nodes of the pre-defined path
     *  for the user to specify a new one.
     *
     *  \param er_video Video structure
     */

    le_void_t er_video_set_reset( er_video_t * const er_video );

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

