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

    er_render_t er_render_create( le_char_t * const er_path, le_char_t * const er_query, le_char_t * const er_view, le_real_t const er_tilt, le_size_t const er_thick, le_size_t const er_width, le_array_t * const er_array ) {

        /* Address variables */
        le_address_t er_addr = LE_ADDRESS_C;

        /* Returned value variables */
        er_render_t er_render = ER_RENDER_C;

        /* Projection variables */
        le_real_t er_angle = atan( 1.0 / sqrt( 2.0 ) );
        le_real_t er_unity = sqrt( 2.0 ) / cos( er_angle );

        /* Assign rendering parameters */
        er_render.re_path  = er_path;
        er_render.re_query = er_query;
        er_render.re_view  = er_view;
        er_render.re_array = er_array;

        /* Check and assign tilt angle */
        er_render.re_tilt  = er_tilt  > 0.0 ? ( er_tilt  < 90.0 ? -er_tilt : -45.0 ) : -45.0;

        /* Check and assign thickness */
        er_render.re_thick = er_thick > 1 ? ( er_thick < 32 ? er_thick : 32 ) : 1;

        /* Check and assign azimuth angle */
        er_render.re_azim  = er_render_get_view( er_view );

        /* Compute projection domain */
        er_render.re_xfac = sqrt( 2.0 );
        er_render.re_yfac = cos( - er_angle - ( er_render.re_tilt * ER_D2R ) ) * er_unity;
        er_render.re_zfac = cos( - er_angle - ( er_render.re_tilt * ER_D2R ) ) * er_unity * 2.0;

        /* Compute projection size */
        er_render.re_width  = er_width;
        er_render.re_height = er_width * ( er_render.re_yfac / er_render.re_xfac );

        /* Convert query into address structure */
        le_address_cvsa( & er_addr, er_render.re_query );

        /* Compute cell edge */
        le_address_get_pose( & er_addr, er_render.re_edge );

        /* Compute cell egde size */
        er_render.re_size = ( LE_2P * LE_GEODESY_WGS84_A ) / pow( 2.0, le_address_get_size( & er_addr ) );

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
    source - display methods
 */

    int er_render_prepare( er_render_t * const er_render ) {

        /* Windows attribute variables */
        XSetWindowAttributes er_wattrib = { 0 };

        /* Display attribute variables */
        GLint er_vattrib[] = { GLX_RGBA, GLX_DEPTH_SIZE, 32, GLX_DOUBLEBUFFER, None };

        /* Create display handle */
        if ( ( er_render->re_display = XOpenDisplay( NULL ) ) == NULL ) return( _LE_FALSE );

        /* Retrieve root window */
        er_render->re_wroot = DefaultRootWindow( er_render->re_display );

        /* Choose visual */
        if ( ( er_render->re_visual = glXChooseVisual( er_render->re_display, 0, er_vattrib ) ) == NULL ) {

            /* Close display */
            XCloseDisplay( er_render->re_display );

            /* Send message */
            return( _LE_FALSE );

        }

        /* Configure graphical context */
        er_wattrib.colormap   = XCreateColormap ( er_render->re_display, er_render->re_wroot, er_render->re_visual->visual, AllocNone );;
        er_render->re_wdisp   = XCreateWindow   ( er_render->re_display, er_render->re_wroot, 0, 0, 64, 64, 0, er_render->re_visual->depth, InputOutput, er_render->re_visual->visual, CWColormap | CWEventMask, & er_wattrib );
        er_render->re_context = glXCreateContext( er_render->re_display, er_render->re_visual, NULL, GL_TRUE );

        /* Activate graphical context */
        glXMakeCurrent( er_render->re_display, er_render->re_wdisp, er_render->re_context );

        /* Create render buffers */
        glGenFramebuffers ( 1, & er_render->re_fb      );
        glGenTextures     ( 1, & er_render->re_fbcolor );
        glGenRenderbuffers( 1, & er_render->re_fbdepth );

        /* Bind render buffers */
        glBindFramebuffer ( GL_FRAMEBUFFER , er_render->re_fb      );
        glBindTexture     ( GL_TEXTURE_2D  , er_render->re_fbcolor );
        glBindRenderbuffer( GL_RENDERBUFFER, er_render->re_fbdepth );

        /* Format render buffers - color */
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, er_render->re_width, er_render->re_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL );

        /* Format render buffers - depth */
        glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, er_render->re_width, er_render->re_height );

        /* Activate render buffers */
        glFramebufferTexture2D   ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D  , er_render->re_fbcolor, 0);
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER, er_render->re_fbdepth );
                
        /* Check render buffers state */
        if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE ) {

            /* Destroy display handle */
     		glXDestroyContext( er_render->re_display, er_render->re_context );
     		XDestroyWindow   ( er_render->re_display, er_render->re_wdisp   );
            XCloseDisplay    ( er_render->re_display );

            /* Send message */
            return( _LE_FALSE );

        }

        /* Send message */
        return( _LE_TRUE );

    }

    void er_render_terminate( er_render_t * const er_render ) {

        /* Check display state */
        if ( er_render->re_display == NULL ) return;

        /* Unselect rendering buffers */
        glBindRenderbuffer( GL_RENDERBUFFER, 0 );        
        glBindTexture     ( GL_TEXTURE_2D  , 0 );
        glBindFramebuffer ( GL_FRAMEBUFFER , 0 );

        /* Delete rendering buffers */
        glDeleteRenderbuffers( 1, & er_render->re_fbdepth );
        glDeleteTextures     ( 1, & er_render->re_fbcolor );
        glDeleteFramebuffers ( 1, & er_render->re_fb      );

        /* Destroy display handle */
 		glXDestroyContext( er_render->re_display, er_render->re_context );
 		XDestroyWindow   ( er_render->re_display, er_render->re_wdisp   );
        XCloseDisplay    ( er_render->re_display );

    }

    void er_render_projection( er_render_t * const er_render ) {

        /* Projection edge variables */
        le_real_t er_xbound = ( er_render->re_size / 2.0 ) * er_render->re_xfac;
        le_real_t er_ybound = ( er_render->re_size / 2.0 ) * er_render->re_yfac;
        le_real_t er_zbound = ( er_render->re_size / 2.0 ) * er_render->re_zfac;

        /* Initialise buffer clear values */
        glClearColor( 0.0, 0.0, 0.0, 0.0 );
        glClearDepth( 1.0 );

        /* Thickness specification */
        glPointSize( er_render->re_thick );
        glLineWidth( er_render->re_thick + 1 );

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

    void er_render_cell( er_render_t * const er_render ) {

        /* Cell dimension variables */
        le_real_t er_cmid = er_render->re_size / 2.0;

        /* Line pointer variables */
        le_real_t * er_pose = NULL;
        le_data_t * er_data = NULL;

        /* Array pointer variables */
        le_byte_t * er_byte = le_array_get_byte( er_render->re_array );

        /* Primitive bloc */
        glBegin( GL_POINTS );

        /* Display points */
        for ( le_size_t er_parse = 0; er_parse < le_array_get_size( er_render->re_array ); er_parse += LE_ARRAY_64S_LEN ) {

            /* Compute line pointers */
            er_pose = ( le_real_t * ) ( er_byte + er_parse );
            er_data = ( le_data_t * ) ( er_pose + sizeof( le_time_t ) ); 

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
        glEnd();   

    }

    void er_render_bound( er_render_t * const er_render ) {

        /* Cell dimension variables */
        le_real_t er_cmid = er_render->re_size / 2.0;

        /* Push edge color */
        glColor4f( 0.2, 0.2, 0.2, 1.0 );

        /* Primitive bloc */
        glBegin( GL_LINES );

        /* Edge lines */
        glVertex3f( - er_cmid, - er_cmid, - er_cmid );
        glVertex3f( - er_cmid, - er_cmid, + er_cmid );
        glVertex3f( - er_cmid, - er_cmid, + er_cmid );
        glVertex3f( - er_cmid, + er_cmid, + er_cmid );
        glVertex3f( - er_cmid, + er_cmid, + er_cmid );
        glVertex3f( - er_cmid, + er_cmid, - er_cmid );
        glVertex3f( - er_cmid, + er_cmid, - er_cmid );
        glVertex3f( - er_cmid, - er_cmid, - er_cmid );

        /* Edge lines */
        glVertex3f( + er_cmid, - er_cmid, - er_cmid );
        glVertex3f( + er_cmid, - er_cmid, + er_cmid );
        glVertex3f( + er_cmid, - er_cmid, + er_cmid );
        glVertex3f( + er_cmid, + er_cmid, + er_cmid );
        glVertex3f( + er_cmid, + er_cmid, + er_cmid );
        glVertex3f( + er_cmid, + er_cmid, - er_cmid );
        glVertex3f( + er_cmid, + er_cmid, - er_cmid );
        glVertex3f( + er_cmid, - er_cmid, - er_cmid );

        /* Edge lines */
        glVertex3f( - er_cmid, - er_cmid, - er_cmid );
        glVertex3f( + er_cmid, - er_cmid, - er_cmid );
        glVertex3f( - er_cmid, - er_cmid, + er_cmid );
        glVertex3f( + er_cmid, - er_cmid, + er_cmid );
        glVertex3f( - er_cmid, + er_cmid, + er_cmid );
        glVertex3f( + er_cmid, + er_cmid, + er_cmid );
        glVertex3f( - er_cmid, + er_cmid, - er_cmid );
        glVertex3f( + er_cmid, + er_cmid, - er_cmid );

        /* Primitive bloc */
        glEnd();

    }

    le_enum_t er_render_save( er_render_t * const er_render ) {

        /* Buffer variables */
        le_byte_t * er_buffer = NULL;

        /* Allocate buffer memory */
        if ( ( er_buffer = ( le_byte_t * ) malloc( er_render->re_width * er_render->re_height * 4 ) ) == NULL ) {

            /* Send message */
            return( _LE_FALSE );

        }

        /* Wait primitives */
        glFinish();

        /* Read buffer pixels */
        glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, er_buffer );

        /* Export buffer */
        if ( lc_image_png_write( ( char * ) er_render->re_path, er_render->re_width, er_render->re_height, LC_IMAGE_RGBA, er_buffer ) == LC_FALSE ) {

            /* Unallocate buffer memory */
            free( er_buffer );

            /* Send message */
            return( _LE_FALSE );

        }

        /* Unallocate buffer memory */
        free( er_buffer );

        /* Send message */
        return( _LE_TRUE );

    }

/*
    source - auxiliary functions
 */

    le_real_t er_render_get_view( le_char_t const * const er_view ) {

        /* Check pointer validity */
        if ( er_view == NULL ) {

            /* Return default value */
            return( - 45.0 );

        }

        /* Check view parameter */
        if ( LC_STRC( er_view, "ne" ) == LC_TRUE ) {

            /* Return parameter value */
            return( + 45.0 );

        } else if ( LC_STRC( er_view, "nw" ) == LC_TRUE ) {

            /* Return parameter value */
            return( - 45.0 );

        } else if ( LC_STRC( er_view, "sw" ) == LC_TRUE ) {

            /* Return parameter value */
            return( +135.0 );

        } else if ( LC_STRC( er_view, "se" ) == LC_TRUE ) {

            /* Return parameter value */
            return( -135.0 );

        } else {

            /* Return default value */
            return( - 45.0 );
    
        }

    }

