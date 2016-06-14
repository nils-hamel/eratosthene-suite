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

    /*! \file   eratosthene-isometric-render.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene query - rendering module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_ISOMETRIC_RENDER__
    # define __ER_ISOMETRIC_RENDER__

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
    # include <math.h>
    # include <GL/gl.h>
    # include <GL/glu.h>
    # include <GL/freeglut.h>
    # include <eratosthene-include.h>
    # include "eratosthene-isometric-png.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_RENDER_C { NULL, NULL, 0, 0, NULL, LE_ADDRESS_C, { 0.0 }, 0.0 }

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct er_render_struct {

        le_char_t  * re_path;
        le_array_t * re_array;
        le_size_t    re_width;
        le_size_t    re_height;
        le_char_t  * re_query;
        le_address_t re_addr;
        le_real_t    re_edge[3];
        le_real_t    re_size;

    } er_render_t;

/*
    header - function prototypes
 */

    /*! \brief
     *
     */

    void er_render_isometry( le_char_t * const er_path, le_char_t * const er_query, le_array_t * const er_array, le_size_t const er_width );

    /*! \brief
     *
     */

    void er_render_isometry_init( void );

    /*! \brief
     *
     */

    void er_render_isometry_cell( void );

    /*! \brief
     *
     */

    void er_render_isometry_disp( void );

    /*! \brief
     *
     */

    void er_render_isometry_disp_elem( void );

    /*! \brief
     *
     */

    void er_render_isometry_disp_edge( void );

    /*! \brief
     *
     */

    void er_render_isometry_disp_save( void );

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

