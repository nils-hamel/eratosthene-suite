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

    /*! \file   eratosthene-isometry-render-io.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene isometry - rendering module - portable network graphics
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_ISOMETRIC_RENDER_IO__
    # define __ER_ISOMETRIC_RENDER_IO__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include <stdio.h>
    # include <stdlib.h>
    # include <png.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

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

    /*! \brief portable network graphics i/o methods
     *
     *  This function exports the provided image in a portable network graphics
     *  file. The function expects an byte array containing the red, green, blue
     *  and alpha components coded on eight bits each.
     *
     *  \param  er_path   Portable network graphics file location
     *  \param  er_width  Image width, in pixels
     *  \param  er_height Image height, in pixels
     *  \param  er_bytes  Byte array containing image pixels
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_png_write( le_char_t const * const er_path, le_size_t const er_width, le_size_t const er_height, le_byte_t * const er_bytes );

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

