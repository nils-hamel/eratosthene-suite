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

    # include "eratosthene-isometry-render.h"

/*
    source - constructor/destructor methods
 */

    er_render_t er_render_create( le_char_t * const er_query, le_char_t * const er_view, le_real_t const er_tilt, le_size_t const er_thick, le_size_t const er_width ) {

        /* returned structure variables */
        er_render_t er_render = ER_RENDER_C;

        /* compute edge position and size */
        er_render.re_size   = er_render_get_edge( er_query, er_render.re_edge );

        /* check and assign azimuth angle */
        er_render.re_azim   = er_render_get_view( er_view );

        /* check and assign tilt */
        er_render.re_tilt   = lc_clamp( -er_tilt, -90.0, 0.0 );

        /* compute projection domain */
        er_render.re_xfac   = er_render.re_size * 0.5 * sqrt( 2.0 );
        er_render.re_yfac   = er_render.re_size * 0.5 * sqrt( 2.0 ) * sin( atan( sqrt( 2.0 ) ) - er_render.re_tilt * ER_ISOMETRY_D2R ) / sin( atan( sqrt( 2.0 ) ) );
        er_render.re_zfac   = er_render.re_size * 0.5 * sin( atan( 1.0 / sqrt( 2.0 ) ) - er_render.re_tilt * ER_ISOMETRY_D2R ) / sin( atan( 1.0 / sqrt( 2.0 ) ) );

        /* check and assign thickness */
        er_render.re_thick  = lc_clamp( er_thick, 1, 32 );

        /* compute projection size */
        er_render.re_width  = er_width;
        er_render.re_height = er_width * ( er_render.re_yfac / er_render.re_xfac );

        /* return constructed structure */
        return( er_render );

    }

    le_void_t er_render_delete( er_render_t * const er_render ) {

        /* deleted structure variables */
        er_render_t er_delete = ER_RENDER_C;

        /* clear rendering structure */
        * er_render = er_delete;

    }

/*
    source - rendering methods
 */

    le_enum_t er_render_prepare( er_render_t * const er_render ) {

        /* windows attribute variables */
        XSetWindowAttributes er_wattrib = { 0 };

        /* context variables */
        XVisualInfo * er_visual = 0;

        /* indows variables */
        Window er_wroot = 0;

        /* display attribute variables */
        GLint er_vattrib[] = { GLX_RGBA, None };

        /* create display handle */
        if ( ( er_render->re_display = XOpenDisplay( NULL ) ) == NULL ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* choose visual */
        if ( ( er_visual = glXChooseVisual( er_render->re_display, 0, er_vattrib ) ) == NULL ) {

            /* close display */
            XCloseDisplay( er_render->re_display );

            /* send message */
            return( _LE_FALSE );

        }

        /* retrieve root window */
        er_wroot = DefaultRootWindow( er_render->re_display );

        /* configure graphical context */
        er_wattrib.colormap   = XCreateColormap ( er_render->re_display, er_wroot, er_visual->visual, AllocNone );;
        er_render->re_wdisp   = XCreateWindow   ( er_render->re_display, er_wroot, 0, 0, 64, 64, 0, er_visual->depth, InputOutput, er_visual->visual, CWColormap | CWEventMask, & er_wattrib );
        er_render->re_context = glXCreateContext( er_render->re_display, er_visual, NULL, GL_TRUE );

        /* activate graphical context */
        glXMakeCurrent( er_render->re_display, er_render->re_wdisp, er_render->re_context );

        /* initialise extension */
        glewInit();

        /* create render buffers */
        glGenFramebuffers ( 1, & er_render->re_buffer );
        glGenTextures     ( 1, & er_render->re_bcolor );
        glGenRenderbuffers( 1, & er_render->re_bdepth );

        /* bind render buffers */
        glBindFramebuffer ( GL_FRAMEBUFFER , er_render->re_buffer );
        glBindTexture     ( GL_TEXTURE_2D  , er_render->re_bcolor );
        glBindRenderbuffer( GL_RENDERBUFFER, er_render->re_bdepth );

        /* format render buffers - color */
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, er_render->re_width, er_render->re_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL );

        /* format render buffers - depth */
        glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, er_render->re_width, er_render->re_height );

        /* activate render buffers */
        glFramebufferTexture2D   ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D  , er_render->re_bcolor, 0);
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER, er_render->re_bdepth );

        /* check rendering buffer */
        if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE ) {

            /* distroy rendering buffer */
            er_render_terminate( er_render );

            /* send message */
            return( _LE_FALSE );

        }

        /* send message */
        return( _LE_TRUE );

    }

    le_void_t er_render_terminate( er_render_t * const er_render ) {

        /* check display state */
        if ( er_render->re_display == NULL ) return;

        /* unselect rendering buffers */
        glBindRenderbuffer( GL_RENDERBUFFER, 0 );        
        glBindTexture     ( GL_TEXTURE_2D  , 0 );
        glBindFramebuffer ( GL_FRAMEBUFFER , 0 );

        /* delete rendering buffers */
        glDeleteRenderbuffers( 1, & er_render->re_bdepth );
        glDeleteTextures     ( 1, & er_render->re_bcolor );
        glDeleteFramebuffers ( 1, & er_render->re_buffer );

        /* destroy display handle */
 		glXDestroyContext( er_render->re_display, er_render->re_context );
 		XDestroyWindow   ( er_render->re_display, er_render->re_wdisp   );
        XCloseDisplay    ( er_render->re_display );

    }

    le_void_t er_render_projection( er_render_t * const er_render ) {

        /* initialise buffer clear values */
        glClearColor( 0.0, 0.0, 0.0, 0.0 );
        glClearDepth( 1.0 );

        /* enable depth test */
        glEnable( GL_DEPTH_TEST );

        /* initialise viewport */
        glViewport( 0, 0, er_render->re_width, er_render->re_height );

        /* matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* clear projection matrix */
        glLoadIdentity();

        /* compute porjection matrix */
        glOrtho( -er_render->re_xfac, +er_render->re_xfac, -er_render->re_yfac, +er_render->re_yfac, -er_render->re_zfac, +er_render->re_zfac );

        /* matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* clear modelview matrix */
        glLoadIdentity();

    }

    le_void_t er_render_primivites( er_render_t * const er_render, le_array_t * const er_array ) {

        /* line pointer variables */
        le_real_t * er_pose = NULL;
        le_data_t * er_data = NULL;

        /* cell dimension variables */
        le_real_t er_cmid = er_render->re_size / 2.0;

        /* array pointer variables */
        le_byte_t * er_byte = le_array_get_byte( er_array );

        /* clear buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* apply isometric rotation */
        glRotatef( er_render->re_tilt, 1.0, 0.0, 0.0 );
        glRotatef( er_render->re_azim, 0.0, 0.0, 1.0 );

        /* primitive bloc */
        glPointSize( er_render->re_thick ); glBegin( GL_POINTS ); {

            /* display points */
            for ( le_size_t er_parse = 0; er_parse < le_array_get_size( er_array ); er_parse += LE_ARRAY_RFL ) {

                /* compute line pointers */
                er_pose = ( le_real_t * ) ( er_byte + er_parse );
                er_data = ( le_data_t * ) ( er_pose + 3 ); 

                /* push element color */
                glColor4f( er_data[0] / 255.0, er_data[1] / 255.0, er_data[2] / 255.0, 1.0 );

                /* convert geographical coordinates to projection coordinates */
                er_pose[0] = - er_cmid + ( er_pose[0] - er_render->re_edge[0] ) * LE_GEODESY_WGS84_A;
                er_pose[1] = - er_cmid + ( er_pose[1] - er_render->re_edge[1] ) * LE_GEODESY_WGS84_A;
                er_pose[2] = - er_cmid + ( er_pose[2] - er_render->re_edge[2] );

                /* push element vertex */
                glVertex3f( er_pose[0], er_pose[1], er_pose[2] );

            }

        /* primitive bloc */
        } glEnd();

        /* push edge color */
        glColor4f( 0.2, 0.2, 0.2, 1.0 );

        /* primitive bloc */
        glLineWidth( er_render->re_thick + 1 ); glBegin( GL_LINES ); {

            /* edge lines coordinates */
            glVertex3f( - er_cmid, - er_cmid, - er_cmid );
            glVertex3f( - er_cmid, - er_cmid, + er_cmid );
            glVertex3f( - er_cmid, - er_cmid, + er_cmid );
            glVertex3f( - er_cmid, + er_cmid, + er_cmid );
            glVertex3f( - er_cmid, + er_cmid, + er_cmid );
            glVertex3f( - er_cmid, + er_cmid, - er_cmid );
            glVertex3f( - er_cmid, + er_cmid, - er_cmid );
            glVertex3f( - er_cmid, - er_cmid, - er_cmid );
            glVertex3f( + er_cmid, - er_cmid, - er_cmid );
            glVertex3f( + er_cmid, - er_cmid, + er_cmid );
            glVertex3f( + er_cmid, - er_cmid, + er_cmid );
            glVertex3f( + er_cmid, + er_cmid, + er_cmid );
            glVertex3f( + er_cmid, + er_cmid, + er_cmid );
            glVertex3f( + er_cmid, + er_cmid, - er_cmid );
            glVertex3f( + er_cmid, + er_cmid, - er_cmid );
            glVertex3f( + er_cmid, - er_cmid, - er_cmid );
            glVertex3f( - er_cmid, - er_cmid, - er_cmid );
            glVertex3f( + er_cmid, - er_cmid, - er_cmid );
            glVertex3f( - er_cmid, - er_cmid, + er_cmid );
            glVertex3f( + er_cmid, - er_cmid, + er_cmid );
            glVertex3f( - er_cmid, + er_cmid, + er_cmid );
            glVertex3f( + er_cmid, + er_cmid, + er_cmid );
            glVertex3f( - er_cmid, + er_cmid, - er_cmid );
            glVertex3f( + er_cmid, + er_cmid, - er_cmid );

        /* primitive bloc */
        } glEnd();
        

    }

    le_enum_t er_render_save( er_render_t * const er_render, char const * const er_path ) {

        /* message variables */
        le_enum_t er_message = _LE_TRUE;

        /* buffer variables */
        le_byte_t * er_buffer = ( le_byte_t * ) malloc( er_render->re_width * er_render->re_height * 4 );

        /* check consistency - send message */
        if ( er_buffer == NULL ) return( _LE_FALSE );

        /* read buffer pixels */
        glFinish(), glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, er_buffer );

        /* export buffer pixels */
        er_message = lc_image_png_write( er_path, er_render->re_width, er_render->re_height, LC_IMAGE_RGBA, er_buffer );

        /* unallocate buffer memory */
        free( er_buffer );

        /* send message */
        return( er_message == LC_TRUE ? _LE_TRUE : _LE_FALSE );

    }

/*
    source - auxiliary methods
 */

    le_real_t er_render_get_view( le_char_t const * const er_view ) {

        /* check pointer validity - return default value */
        if ( er_view == NULL ) return( -45.0 );

        /* check view parameter - return value */
        if ( lc_strc( er_view, "ne" ) == LC_TRUE ) return( + 45.0 );
        if ( lc_strc( er_view, "nw" ) == LC_TRUE ) return( - 45.0 );
        if ( lc_strc( er_view, "sw" ) == LC_TRUE ) return( +135.0 );
        if ( lc_strc( er_view, "se" ) == LC_TRUE ) return( -135.0 );

        /* return default value */
        return( -45.0 );

    }

    le_size_t er_render_get_edge( le_char_t const * const er_query, le_real_t * const er_pose ) {

        /* address variables */
        le_address_t er_address = LE_ADDRESS_C;

        /* convert query into address */
        le_address_cf_string( & er_address, er_query );

        /* extract position from address index */
        le_address_get_pose( & er_address, er_pose );

        /* compute equivalence class size */
        return( ( LE_2P * LE_GEODESY_WGS84_A ) / pow( 2.0, le_address_get_size( & er_address ) ) );

    }

