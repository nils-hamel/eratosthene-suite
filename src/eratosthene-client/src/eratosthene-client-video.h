/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2019 DHLAB, EPFL
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
     *  This structure holds the required elements for video frame computation
     *  and extraction. It holds the video frame buffer and its properties along
     *  with the spatio-temporal trajectory information.
     *
     *  The first field contains the exportation path of the video frames. The
     *  two next fields stores the video frame computation index and count.
     *
     *  The three next fields stores the video frame buffer bytes and resolution
     *  information.
     *
     *  The two last fields stores the amount of trajectory interpolation nodes
     *  the nodes themselves that are simple view structures.
     *
     *  The video computation is based on the creation of a spatio-temporal
     *  trajectory between the pushed view nodes. At each interpolation steps,
     *  based on the amount of pushed view and video frame count, the OpenGL
     *  render buffer is queried and exported as a portable network graphic
     *  image.
     *
     *  \var er_video_struct::vd_path
     *  Video frame exportation path
     *  \var er_video_struct::vd_index
     *  Video frame computation index
     *  \var er_video_struct::vd_count
     *  Video frame count
     *  \var er_video_struct::vd_width
     *  Video frame buffer width, in pixels
     *  \var er_video_struct::vd_height
     *  Video frame buffer height, in pixels
     *  \var er_video_struct::vd_buffer
     *  Video frame buffer bytes, in 24 bpp RGB
     *  \var er_video_struct::vd_push
     *  Amount of pushed view - interpolation control nodes
     *  \var er_video_struct::vd_view
     *  View structure stack
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
     *  The function starts by assigning the provided value to the video
     *  structure before to allocate the frame buffer memory.
     *
     *  As the function returns a video structure, the creation status is
     *  provided by the _status field of the returned structure.
     *
     *  \param er_path   Video frame exportation path
     *  \param er_count  Video frame count
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
     *  It starts by releasing the buffer memory, if allocated, before to reset
     *  the structure fields with default values.
     *
     *  \param er_video Video structure
     */

    le_void_t er_video_delete( er_video_t * const er_video );

    /*! \brief accessor methods
     *
     *  This function is responsible of computing the interpolated view based on
     *  the provided video structure view stack and the current index of the
     *  video frame.
     *
     *  In the first place, a non-linear near-interpolation procedure is applied
     *  to compute the position in space of the view.
     *
     *  In the second place, a linear interpolation is applied to compute the
     *  position in time and comb size.
     *
     *  The interpolated position in space and time are assigned to a view
     *  structure that is returned by the function.
     *
     *  The interpolation incremental index is homogeneously spread on the
     *  pushed views of the video structure stack.
     *
     *  \param er_video Video structure
     *
     *  \return Return interpolated view structure
     */

    er_view_t er_video_get( er_video_t const * const er_video );

    /*! \brief accessor methods
     *
     *  This function checks if the required element are available in the video
     *  structure provided as parameter.
     *
     *  It checks the exportation path state, the video frame count value and
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
     *  the provided video structure buffer and to export it in the current
     *  video frame portable network graphic file.
     *
     *  This function is also responsible of increasing the video frame current
     *  index.
     *
     *  Depending on the video frame index, the function returns the required
     *  execution mode. If the frame index is lower than the frame count, the
     *  function returns the video mode value and the standard view mode
     *  otherwise.
     *
     *  \param er_video Video structure
     *
     *  \return Returns ER_COMMON_VIEW if all frames have been computed and
     *  exported, ER_COMMON_AUTO otherwise
     */

    le_enum_t er_video_set( er_video_t * const er_video );

    /*! \brief mutator methods
     *
     *  This function allows to push a view structure in the view stack of the
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
     *  This function allows to empty the provided video structure view stack.
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

