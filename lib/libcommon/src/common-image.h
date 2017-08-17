/*
 *  eratosthene-suite - common library
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 DHLAB, EPFL
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

    /*! \file   common-image.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - common library - images module
     */

/*
    header - inclusion guard
 */

    # ifndef __LC_IMAGE__
    # define __LC_IMAGE__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

    # include "common.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define image depth variables */
    # define LC_IMAGE_RGB  0
    # define LC_IMAGE_RGBA 1

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief portable network graphics i/o function
     *
     *  This function exports the image provided through the bytes array in a
     *  portable network graphic file.
     *
     *  The supported image format are the 24 bits RGB and the 32 bits RGBA
     *  formats : LC_IMAGE_RGB and LC_IMAGE_RGBA.
     *
     *  \param  lc_path   Exportation file path
     *  \param  lc_width  Width, in pixels, of the image
     *  \param  lc_height Height, in pixels, of the image
     *  \param  lc_format Image color format
     *  \param  lc_bytes  Image bytes
     *
     *  \return Returns LC_TRUE on success, LC_FALSE otherwise
     */

    int lc_image_png_write( char const * const lc_path, int const lc_width, int const lc_height, int const lc_format, unsigned char * const lc_bytes );

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

