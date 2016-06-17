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

    /*! \file   eratosthene-isometry-render.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene isometry - rendering module
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
    # include <string.h>
    # include <GL/gl.h>
    # include <GL/glx.h>
    # include <GL/glext.h>
    # include <png.h>
    # include <eratosthene-include.h>
    # include "eratosthene-isometry-render-io.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_RENDER_C { NULL, NULL, NULL, 0.0, 0.0, 0.0, 0.0, { 0 }, 1, 0, 0, NULL, 0, 0, NULL, 0, 0, 0, 0 }

    /* Trigonometric conversion */
    # define ER_D2R      ( LE_PI / 180.0 )
    # define ER_R2D      ( 180.0 / LE_PI )

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

        le_char_t   * re_path;
        le_char_t   * re_query;
        le_char_t   * re_view;

        le_real_t     re_azim;
        le_real_t     re_tilt;

        le_real_t     re_fact;
        le_real_t     re_size;
        le_real_t     re_edge[3];

        le_size_t     re_thick;
        le_size_t     re_width;
        le_size_t     re_height;

        Display     * re_display;
        Window        re_wroot;
        Window        re_wdisp;
        XVisualInfo * re_visual;
        GLXContext    re_context;

        GLuint        re_fb;
        GLuint        re_fbcolor;
        GLuint        re_fbdepth;
        
    } er_render_t;

/*
    header - function prototypes
 */

    /*! \brief
     *
     */

    er_render_t er_render_create( le_char_t * const er_path, le_char_t * const er_query, le_char_t * const er_view, le_real_t const er_tilt, le_size_t const er_thick, le_size_t const er_width );

    /*! \brief
     *
     */

    er_render_t er_render_delete( er_render_t * const er_render );

    /*! \brief
     *
     */

    int er_render_display_prepare( er_render_t * const er_render );

    /*! \brief
     *
     */

    void er_render_display_terminate( er_render_t * const er_render );

    /*! \brief
     *
     */

    void er_render_display_projection( er_render_t * const er_render );

    /*! \brief
     *
     */

    void er_render_display_cell( er_render_t * const er_render, le_array_t const * const er_array );

    /*! \brief
     *
     */

    void er_render_display_bound( er_render_t * const er_render );

    /*! \brief
     *
     */

    le_enum_t er_render_display_save( er_render_t * const er_render );

    /* Don't ask why ... any help welcome ... */
    extern void     glGenFramebuffers           (GLsizei n, GLuint *framebuffers);
    extern void     glBindFramebuffer           (GLenum target, GLuint framebuffer);
    extern void     glFramebufferTexture2D      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    extern void     glGenRenderbuffers          (GLsizei n, GLuint *renderbuffers);
    extern void     glBindRenderbuffer          (GLenum target, GLuint renderbuffer);
    extern void     glRenderbufferStorage       (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
    extern void     glFramebufferRenderbuffer   (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    extern GLenum   glCheckFramebufferStatus    (GLenum target);
    extern void     glDeleteRenderbuffers       (GLsizei n, const GLuint *renderbuffers);
    extern void     glDeleteFramebuffers        (GLsizei n, const GLuint *framebuffers);

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

