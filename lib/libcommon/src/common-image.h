/*
 *  eratosthene-suite - common library
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

    /*! \file   common-image.h
     *  \author Nils Hamel <nils.hamel@alumni.epfl.ch>
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

    # include <png.h>

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

    /*! \brief i/o methods
     *
     *  This function exports the image provided through the bytes array in a
     *  portable network graphic file using the provided exportation path and
     *  image byte buffer.
     *
     *  The byte buffer is expected to encode the image in row-wise RGB format
     *  using one byte per pixel components. The buffer is then expected to be :
     *
     *      lc_width * lc_height * 3
     *
     *  bytes long.
     *
     *  On success, the function returns LC_TRUE and LC_FALSE on error.
     *
     *  \param  lc_path   Exportation path
     *  \param  lc_width  Width, in pixels, of the image
     *  \param  lc_height Height, in pixels, of the image
     *  \param  lc_bytes  Image bytes (RGB888)
     *
     *  \return Returns LC_TRUE on success, LC_FALSE otherwise
     */

    int lc_image_write( char const * const lc_path, int const lc_width, int const lc_height, unsigned char * const lc_bytes );

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

