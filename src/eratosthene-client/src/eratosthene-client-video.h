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

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_VIDEO_C { NULL, 0, 0, 0, NULL, _LE_FALSE }

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
     */

    typedef struct er_video_struct {

        le_char_t * vd_path;

        le_size_t   vd_index;

        le_size_t   vd_width;
        le_size_t   vd_height;
        le_byte_t * vd_buffer;

    int _status; } er_video_t;


/*
    header - function prototypes
 */

    /* *** */

    er_video_t er_video_create( le_char_t * const er_path, le_size_t const er_width, le_size_t const er_height );

    /* *** */

    le_void_t er_video_delete( er_video_t * const er_video );

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

