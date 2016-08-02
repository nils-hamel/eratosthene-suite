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

    # include <math.h>
    # include <GL/glew.h>
    # include <GL/glx.h>
    # include <common-include.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_RENDER_C         { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, { 0 }, 1, 0, 0, NULL, 0, 0, 0, 0, 0 }

    /* Define trigonometric factors */
    # define ER_ISOMETRY_D2R     ( LE_PI / 180.0 )

    /* Define default view */
    # define ER_ISOMETRY_VIEW_NE ( + 45.0 )
    # define ER_ISOMETRY_VIEW_NW ( - 45.0 )
    # define ER_ISOMETRY_VIEW_SW ( +135.0 )
    # define ER_ISOMETRY_VIEW_SE ( -135.0 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct er_render_struct
     *  \brief Rendering structure
     *
     *  This structure holds the configuration parameters required for the
     *  computation, rendering and exportation of the isometric projections.
     *
     *  \var er_render_struct::re_path
     *  Path to the exportation file
     *  \var er_render_struct::re_query
     *  Indexation server query
     *  \var er_render_struct::re_view
     *  Isometric projection orientation
     *  \var er_render_struct::re_azim
     *  Orientation angle, deduced from re_view field
     *  \var er_render_struct::re_azim
     *  Isometric tilt angle
     *  \var er_render_struct::re_xfac
     *  Isometric projection width in x dimension
     *  \var er_render_struct::re_yfac
     *  Isometric projection width in y dimension
     *  \var er_render_struct::re_zfac
     *  Isometric projection width in z dimension
     *  \var er_render_struct::re_size
     *  Indexation server rendered cell geodetic size in peseudo-metres
     *  \var er_render_struct::re_edge
     *  Indexation server rendered cell minimum corner
     *  \var er_render_struct::re_thick
     *  Isometric projection point and line size
     *  \var er_render_struct::re_width
     *  Isometric projection width in pixels
     *  \var er_render_struct::re_height
     *  Isometric projection height in pixels
     *  \var er_render_struct::re_display
     *  Graphical context handle
     *  \var er_render_struct::re_wroot
     *  Graphical context handle
     *  \var er_render_struct::re_wdisp
     *  Graphical context handle
     *  \var er_render_struct::re_visual
     *  Graphical context handle
     *  \var er_render_struct::re_context
     *  Graphical context handle
     *  \var er_render_struct::re_buffer
     *  Framebuffer handle
     *  \var er_render_struct::re_bcolor
     *  Renderbuffer handle
     *  \var er_render_struct::re_bdepth
     *  Renderbuffer handle
     */

    typedef struct er_render_struct {

        le_real_t    re_azim;
        le_real_t    re_tilt;

        le_real_t    re_xfac;
        le_real_t    re_yfac;
        le_real_t    re_zfac;

        le_real_t    re_size;
        le_real_t    re_edge[3];

        le_size_t    re_thick;
        le_size_t    re_width;
        le_size_t    re_height;

        Display    * re_display;
        Window       re_wdisp;
        GLXContext   re_context;

        GLuint       re_buffer;
        GLuint       re_bcolor;
        GLuint       re_bdepth;
        
    } er_render_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates the rendering structure required by the display
     *  methods. It revieve and analyse the projection parameters and fills the
     *  structure fields according to it. The created rendering structure is
     *  then returned by the function.
     *
     *  \param  er_path  Projection image exportation path
     *  \param  er_query Indexation server query string
     *  \param  er_view  Projection orientation
     *  \param  er_tilt  Projection tilt angle
     *  \param  er_thick Projection point and line width
     *  \param  er_width Projection image width, in pixels
     *  \param  er_array Array containing the server data
     *
     *  \return Returns created rendering structure
     */

    er_render_t er_render_create( le_char_t * const er_query, le_char_t * const er_view, le_real_t const er_tilt, le_size_t const er_thick, le_size_t const er_width );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided rendering structure. It expects a
     *  structure created using er_render_create function.
     *
     *  \param  er_render Rendering structure
     *
     *  \return Returns deleted rendering structure  
     */

    er_render_t er_render_delete( er_render_t * const er_render );

    /*! \brief display methods
     *
     *  This function initialise the graphical context that is used by the
     *  rendering engine computing the projection.
     *  
     *  \param  er_render Rendering structure
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_render_prepare( er_render_t * const er_render );

    /*! \brief display methods
     *
     *  This function uninitialise the graphical context prepared by the
     *  render_prepare function. It has to be called after any call to the
     *  render_prepare function that succeed.
     *
     *  \param er_render Rendering structure
     */

    le_void_t er_render_terminate( er_render_t * const er_render );

    /*! \brief display methods
     *
     *  This function compute and apply the projection parameters to use for the    
     *  rendering of the indexation server cell.
     *
     *  \param er_render Rendering structure
     */

    le_void_t er_render_projection( er_render_t * const er_render );

    /*! \brief display methods
     *
     *  This function simply draws the points contained in the provided array.
     *  This array has to be already filled though a query to the indexation
     *  server.
     *
     *  \param er_render Rendering structure
     */

    le_void_t er_render_primivites( er_render_t * const er_render, le_array_t * const er_array );

    /*! \brief display methods
     *
     *  This function exports the rendered isometric projection in the file
     *  specified through the rendering structure.
     *
     *  \param  er_render Rendering structure
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_render_save( er_render_t * const er_render, char const * const er_path );

    /*! \brief auxiliary functions
     *
     */

    le_real_t er_render_get_view( le_char_t const * const er_view );

    le_size_t er_render_get_edge( le_char_t const * const er_query, le_real_t * const er_pose );

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

