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

        /* Returned structure variables */
        er_render_t er_render = ER_RENDER_C;

        /* Projection variables */
        le_real_t er_angle = atan( 1.0 / sqrt( 2.0 ) );
        le_real_t er_unity = sqrt( 2.0 ) / cos( er_angle );

        /* Check and assign azimuth angle */
        er_render.re_azim   = er_render_get_view( er_view );

        /* Check and assign tilt */
        er_render.re_tilt   = er_tilt  >= 0.0 ? ( er_tilt <= 90.0 ? -er_tilt : -45.0 ) : -45.0;

        /* Compute projection domain */
        er_render.re_xfac   = sqrt( 2.0 );
        er_render.re_yfac   = cos( - er_angle - ( er_render.re_tilt * ER_ISOMETRY_D2R ) ) * er_unity;
        er_render.re_zfac   = cos( - er_angle - ( er_render.re_tilt * ER_ISOMETRY_D2R ) ) * er_unity * 2.0;

        /* Compute edge position and size */
        er_render.re_size   = er_render_get_edge( er_query, er_render.re_edge );

        /* Check and assign thickness */
        er_render.re_thick  = er_thick > 1 ? ( er_thick < 32 ? er_thick : 32 ) : 1;

        /* Compute projection size */
        er_render.re_width  = er_width;
        er_render.re_height = er_width * ( er_render.re_yfac / er_render.re_xfac );

        /* Return constructed structure */
        return( er_render );

    }

    er_render_t er_render_delete( er_render_t * const er_render ) {

        /* Returned value variables */
        er_render_t er_delete = ER_RENDER_C;

        /* Clear rendering structure */
        * er_render = er_delete;

        /* Return deleted structure */
        return( * er_render );

    }

/*
    source - rendering methods
 */

    le_enum_t er_render_prepare( er_render_t * const er_render ) {

        /* Windows attribute variables */
        XSetWindowAttributes er_wattrib = { 0 };

        /* Context variables */
        XVisualInfo * er_visual = 0;

        /* Windows variables */
        Window er_wroot = 0;

        /* Display attribute variables */
        GLint er_vattrib[] = { GLX_RGBA, GLX_DEPTH_SIZE, 32, GLX_DOUBLEBUFFER, None };

        /* Create display handle */
        if ( ( er_render->re_display = XOpenDisplay( NULL ) ) == NULL ) {

            return( _LE_FALSE );

        } else {

            /* Choose visual */
            if ( ( er_visual = glXChooseVisual( er_render->re_display, 0, er_vattrib ) ) == NULL ) {

                /* Close display */
                XCloseDisplay( er_render->re_display );

                /* Send message */
                return( _LE_FALSE );

            }

        }

        /* Retrieve root window */
        er_wroot = DefaultRootWindow( er_render->re_display );

        /* Configure graphical context */
        er_wattrib.colormap   = XCreateColormap ( er_render->re_display, er_wroot, er_visual->visual, AllocNone );;
        er_render->re_wdisp   = XCreateWindow   ( er_render->re_display, er_wroot, 0, 0, 64, 64, 0, er_visual->depth, InputOutput, er_visual->visual, CWColormap | CWEventMask, & er_wattrib );
        er_render->re_context = glXCreateContext( er_render->re_display, er_visual, NULL, GL_TRUE );

        /* Activate graphical context */
        glXMakeCurrent( er_render->re_display, er_render->re_wdisp, er_render->re_context );

        /* Initialise extension */
        glewInit();

        /* Create render buffers */
        glGenFramebuffers ( 1, & er_render->re_buffer );
        glGenTextures     ( 1, & er_render->re_bcolor );
        glGenRenderbuffers( 1, & er_render->re_bdepth );

        /* Bind render buffers */
        glBindFramebuffer ( GL_FRAMEBUFFER , er_render->re_buffer );
        glBindTexture     ( GL_TEXTURE_2D  , er_render->re_bcolor );
        glBindRenderbuffer( GL_RENDERBUFFER, er_render->re_bdepth );

        /* Format render buffers - color */
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, er_render->re_width, er_render->re_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL );

        /* Format render buffers - depth */
        glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, er_render->re_width, er_render->re_height );

        /* Activate render buffers */
        glFramebufferTexture2D   ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D  , er_render->re_bcolor, 0);
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER, er_render->re_bdepth );

        /* Check rendering buffer */
        if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE ) {

            /* Distroy rendering buffer */
            er_render_terminate( er_render );

            /* Send message */
            return( _LE_FALSE );

        } else {

            /* Send message */
            return( _LE_TRUE );

        }

    }

    le_void_t er_render_terminate( er_render_t * const er_render ) {

        /* Check display state */
        if ( er_render->re_display == NULL ) return;

        /* Unselect rendering buffers */
        glBindRenderbuffer( GL_RENDERBUFFER, 0 );        
        glBindTexture     ( GL_TEXTURE_2D  , 0 );
        glBindFramebuffer ( GL_FRAMEBUFFER , 0 );

        /* Delete rendering buffers */
        glDeleteRenderbuffers( 1, & er_render->re_bdepth );
        glDeleteTextures     ( 1, & er_render->re_bcolor );
        glDeleteFramebuffers ( 1, & er_render->re_buffer );

        /* Destroy display handle */
 		glXDestroyContext( er_render->re_display, er_render->re_context );
 		XDestroyWindow   ( er_render->re_display, er_render->re_wdisp   );
        XCloseDisplay    ( er_render->re_display );

    }

    le_void_t er_render_projection( er_render_t * const er_render ) {

        /* Projection edge variables */
        le_real_t er_xbound = ( er_render->re_size / 2.0 ) * er_render->re_xfac;
        le_real_t er_ybound = ( er_render->re_size / 2.0 ) * er_render->re_yfac;
        le_real_t er_zbound = ( er_render->re_size / 2.0 ) * er_render->re_zfac;

        /* Initialise buffer clear values */
        glClearColor( 0.0, 0.0, 0.0, 0.0 );
        glClearDepth( 1.0 );

        /* Enable depth test */
        glEnable( GL_DEPTH_TEST );

        /* Initialise viewport */
        glViewport( 0, 0, er_render->re_width, er_render->re_height );

        /* Matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* Clear projection matrix */
        glLoadIdentity();

        /* Compute porjection matrix */
        glOrtho( -er_xbound, +er_xbound, -er_ybound, +er_ybound, -er_zbound, +er_zbound );

        /* Matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* Clear modelview matrix */
        glLoadIdentity();

        /* Clear buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* Apply isometric rotation */
        glRotatef( er_render->re_tilt, 1.0, 0.0, 0.0 );
        glRotatef( er_render->re_azim, 0.0, 0.0, 1.0 );

    }

    le_void_t er_render_primivites( er_render_t * const er_render, le_array_t * const er_array ) {

        /* Cell dimension variables */
        le_real_t er_cmid = er_render->re_size / 2.0;

        /* Line pointer variables */
        le_real_t * er_pose = NULL;
        le_data_t * er_data = NULL;

        /* Array pointer variables */
        le_byte_t * er_byte = le_array_get_byte( er_array );

        /* Primitive bloc */
        glPointSize( er_render->re_thick ); glBegin( GL_POINTS ); {

            /* Display points */
            for ( le_size_t er_parse = 0; er_parse < le_array_get_size( er_array ); er_parse += LE_ARRAY_RFL ) {

                /* Compute line pointers */
                er_pose = ( le_real_t * ) ( er_byte + er_parse );
                er_data = ( le_data_t * ) ( er_pose + 3 ); 

                /* Push element color */
                glColor4f( er_data[0] / 255.0, er_data[1] / 255.0, er_data[2] / 255.0, 1.0 );

                /* Convert geographical coordinates to projection coordinates */
                er_pose[0] = - er_cmid + ( er_pose[0] - er_render->re_edge[0] ) * LE_GEODESY_WGS84_A;
                er_pose[1] = - er_cmid + ( er_pose[1] - er_render->re_edge[1] ) * LE_GEODESY_WGS84_A;
                er_pose[2] = - er_cmid + ( er_pose[2] - er_render->re_edge[2] );

                /* Push element vertex */
                glVertex3f( er_pose[0], er_pose[1], er_pose[2] );

            }

        /* Primitive bloc */
        } glEnd();

        /* Push edge color */
        glColor4f( 0.2, 0.2, 0.2, 1.0 );

        /* Primitive bloc */
        glLineWidth( er_render->re_thick + 1 ); glBegin( GL_LINES ); {

            /* Edge lines coordinates */
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

        /* Primitive bloc */
        } glEnd();
        

    }

    le_enum_t er_render_save( er_render_t * const er_render, char const * const er_path ) {

        /* Message variables */
        le_enum_t er_message = _LE_TRUE;

        /* Buffer variables */
        le_byte_t * er_buffer = ( le_byte_t * ) malloc( er_render->re_width * er_render->re_height * 4 );

        /* Check consistency - send message */
        if ( er_buffer == NULL ) return( _LE_FALSE );

        /* Read buffer pixels */
        glFinish(); glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, er_buffer );

        /* Export buffer pixels */
        er_message = lc_image_png_write( er_path, er_render->re_width, er_render->re_height, LC_IMAGE_RGBA, er_buffer );

        /* Unallocate buffer memory */
        free( er_buffer );

        /* Send message */
        return( er_message == LC_TRUE ? _LE_TRUE : _LE_FALSE );

    }

/*
    source - auxiliary methods
 */

    le_real_t er_render_get_view( le_char_t const * const er_view ) {

        /* Check pointer validity - return default value */
        if ( er_view == NULL ) return( ER_ISOMETRY_VIEW_NW );

        /* Check view parameter - return value */
        if ( lc_strc( er_view, "ne" ) == LC_TRUE ) return( ER_ISOMETRY_VIEW_NE );
        else 
        if ( lc_strc( er_view, "nw" ) == LC_TRUE ) return( ER_ISOMETRY_VIEW_NW );
        else
        if ( lc_strc( er_view, "sw" ) == LC_TRUE ) return( ER_ISOMETRY_VIEW_SW );
        else 
        if ( lc_strc( er_view, "se" ) == LC_TRUE ) return( ER_ISOMETRY_VIEW_SE );

        /* Return default value */
        return( ER_ISOMETRY_VIEW_NW );

    }

    le_size_t er_render_get_edge( le_char_t const * const er_query, le_real_t * const er_pose ) {

        /* Address variables */
        le_address_t er_address = LE_ADDRESS_C;

        /* Convert query into address */
        le_address_cf_string( & er_address, er_query );

        /* Extract position from address index */
        le_address_get_pose( & er_address, er_pose );

        /* Compute equivalence class size */
        return( ( LE_2P * LE_GEODESY_WGS84_A ) / pow( 2.0, le_address_get_size( & er_address ) ) );

    }

