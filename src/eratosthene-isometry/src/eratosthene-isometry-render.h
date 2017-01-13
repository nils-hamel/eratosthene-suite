/*
 *  eratosthene-suite - isometry
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - isometry - render module
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
    header - internal includes
 */

/*
    header - external includes
 */

    # include <math.h>
    # include <GL/glew.h>
    # include <GL/glx.h>
    # include <common-include.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_RENDER_C         { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, { 0 }, 1, 0, 0, NULL, 0, 0, 0, 0, 0 }

    /* define trigonometric factors */
    # define ER_ISOMETRY_D2R     ( LE_PI / 180.0 )

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
     *  This structure is used to store the parameters required to compute the
     *  isometric projection.
     *
     *  It holds the projection parameters in addition to the required values
     *  and descriptors used to drive the computation of the projection through
     *  opengl off-screen rendering.
     *
     *  \var er_render_struct::re_azim
     *  Azimuthal angle of the projection (NW, NE, SW, SE)
     *  \var er_render_struct::re_tilt
     *  Tilt angle of the projection in [-90,0]
     *  \var er_render_struct::re_xfac
     *  Isometric projection box size in x direction (opengl frame)
     *  \var er_render_struct::re_yfac
     *  Isometric projection box size in y direction (opengl frame)
     *  \var er_render_struct::re_zfac
     *  Isometric projection box size in z direction (opengl frame)
     *  \var er_render_struct::re_size
     *  Size of the equivalence class provided by the data array (flat-metres)
     *  \var er_render_struct::re_edge
     *  Geographic edge of the equivalence class provoded by the data array
     *  \var er_render_struct::re_thick
     *  Size, in pixels, of the projection frame and points
     *  \var er_render_struct::re_width
     *  Width, in pixels, of the projection image
     *  \var er_render_struct::re_height
     *  Height, in pixels, of the projection image
     *  \var er_render_struct::re_display
     *  Graphical context handle
     *  \var er_render_struct::re_wdisp
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
     *  This function creates and returns the rendering structure used to drive
     *  to computation of the isometric projection.
     *
     *  It assign the provided parameters and compute the required configuration
     *  values used by the rendering functions. It also checks and clamp values
     *  constrainted in specific ranges.
     *
     *  \param  er_query Query address string
     *  \param  er_view  Projection azimuthal position
     *  \param  er_tilt  Projection tilt angle
     *  \param  er_thick Projection frame and points size, in pixels
     *  \param  er_width Projection image width, in pixels
     *
     *  \return Returns created rendering structure
     */

    er_render_t er_render_create( le_char_t * const er_query, le_char_t * const er_view, le_real_t const er_tilt, le_size_t const er_thick, le_size_t const er_width );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided rendering structure.
     *
     *  \param  er_render Rendering structure
     *
     *  \return Returns deleted rendering structure
     */

    le_void_t er_render_delete( er_render_t * const er_render );

    /*! \brief display methods
     *
     *  This function initialise the graphical context and framebuffers used to
     *  compute the isometric projection.
     *
     *  \param  er_render Rendering structure
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_render_prepare( er_render_t * const er_render );

    /*! \brief display methods
     *
     *  This function deletes the graphical context and framebuffers used to
     *  compute the isometric projection.
     *
     *  \param er_render Rendering structure
     */

    le_void_t er_render_terminate( er_render_t * const er_render );

    /*! \brief display methods
     *
     *  This function set the opengl projection matrix used for the computation
     *  of the isometric projection. It uses the configuration values provided
     *  by the rendering structure to realise the desired projection.
     *
     *  \param er_render Rendering structure
     */

    le_void_t er_render_projection( er_render_t * const er_render );

    /*! \brief display methods
     *
     *  This function draws the graphical primitives using the coordinates and
     *  colours provided by the data array. It also draws the projection box
     *  frame.
     *
     *  \param er_render Rendering structure
     *  \param er_array  Data array
     */

    le_void_t er_render_primivites( er_render_t * const er_render, le_array_t * const er_array );

    /*! \brief display methods
     *
     *  This function exports the rendered isometric projection in the portable
     *  network graphic file using the specified exportation path. The image
     *  bytes of the projection are read from the texture framebuffer.
     *
     *  \param er_render Rendering structure
     *  \param er_path   Projection image exportation path
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t er_render_save( er_render_t * const er_render, char const * const er_path );

    /*! \brief auxiliary methods
     *
     *  This function converts the azimuthal angle string into the corresponding
     *  decimal degrees angle.
     *
     *  Four strings are handled by the function that are : "ne" for north-east,
     *  "nw" for north-west, "sw" for south-west and "se" for south-east. In
     *  case another string is provided, the function returns by default the
     *  angle corresponding to "nw" string.
     *
     *  \param er_view Azimuthal angle position string
     */

    le_real_t er_render_get_view( le_char_t const * const er_view );

    /*! \brief auxiliary methods
     *
     *  This function computes two different type of values of the equivalence
     *  class pointed by the provided address string.
     *
     *  In the first place, the function computes the geographic position of the
     *  edge of the pointed equivalence class. It writes the three coordinates
     *  of the edge in the \b er_pose array provided as parameter.
     *
     *  It then computes the size, in flat-metres, of the class pointed by the
     *  address and returns it.
     *
     *  \param er_query Query address string
     *  \param er_pose  Equivalence class edge coordinates
     *
     *  \return Size of the equivalence class, in flat-metres
     */

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

