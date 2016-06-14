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

    # include "eratosthene-isometric-render.h"

/*
    source - exchange variables (GLUT callback communication)
 */

    er_render_t er_render = ER_RENDER_C;

/*
    source - isometry main functions
 */

    void er_render_isometry( le_char_t * const er_path, le_char_t * const er_query, le_array_t * const er_array, le_size_t const er_width ) {

        /* Create render structure */
        er_render.re_path   = er_path;
        er_render.re_array  = er_array;
        er_render.re_width  = er_width;
        er_render.re_height = er_width * ( 2.0 / sqrt( 3.0 ) );
        er_render.re_query  = er_query;

        /* Initialise rendering */
        er_render_isometry_init();

        /* Compute rendering parameters */
        er_render_isometry_cell();

        /* Compute rendering */
        glutMainLoop();

    }

/*
    source - isometry step functions
 */

    void er_render_isometry_init( void ) {

        /* Initialise windows size */
        glutInitWindowSize( er_render.re_width, er_render.re_height );

        /* Initialise display mode */
        glutInitDisplayMode( GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH );

        /* Create display window */
        glutCreateWindow( "eratosthene-isometry" );

        /* Configure main loop behavior */
        glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

        /* Initialise buffers clear values */
        glClearColor( 0.0, 0.0, 0.2, 0.0 );
        glClearDepth( 1.0 );

        /* Enable depth management */
        glEnable( GL_DEPTH_TEST );

        /* Initialise callback functions */
        glutDisplayFunc( & er_render_isometry_disp );

    }

    void er_render_isometry_cell( void ) {

        /* Convert query string to address structure */
        le_address_cvsa( & er_render.re_addr, er_render.re_query );

        /* Compute cell edge position */
        le_address_get_pose( & er_render.re_addr, er_render.re_edge );

        /* Compute cell size */
        er_render.re_size = ( LE_2P * LE_GEODESY_WGS84_A ) / pow( 2.0, le_address_get_size( & er_render.re_addr ) );

    }

    void er_render_isometry_disp( void ) {

        /* Initialise viewport */
        glViewport( 0, 0, er_render.re_width, er_render.re_height );

        /* Matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* Clear projection matrix */
        glLoadIdentity();

        /* Compute porjection matrix */
        glOrtho( 

            - ( er_render.re_size / 2.0 ) * sqrt( 2.0 ), ( er_render.re_size / 2.0 ) * sqrt( 2.0 ),
            - ( er_render.re_size / 2.0 ) * sqrt( 2.0 ) * ( 2.0 / sqrt( 3.0 ) ), ( er_render.re_size / 2.0 ) * sqrt( 2.0 ) * ( 2.0 / sqrt( 3.0 ) ),
            - ( er_render.re_size / 2.0 ) * sqrt( 2.0 ) * ( 2.0 / sqrt( 3.0 ) ), ( er_render.re_size / 2.0 ) * sqrt( 2.0 ) * ( 2.0 / sqrt( 3.0 ) )

        );

        /* Matrix mode to modelview */
        glMatrixMode( GL_MODELVIEW );

        /* Clear modelview matrix */
        glLoadIdentity();

        /* Clear buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* Point size specification */
        glPointSize( 1.0 );

        /* Apply isometric rotation */
        glRotatef( - 55.0, 1.0, 0.0, 0.0 );
        glRotatef( +135.0, 0.0, 0.0, 1.0 );

        /* Display cell elements */
        er_render_isometry_disp_elem();

        /* Display cell edge */
        er_render_isometry_disp_edge();

        /* Wait primitives render */
        glFlush();
        glFinish();

        /* Export rendered scene */
        er_render_isometry_disp_save();

        /* Exit main loop */
        glutPostRedisplay();
        glutLeaveMainLoop();

    }

/*
    source - isometry step sub-functions
 */

    void er_render_isometry_disp_elem( void ) {

        /* Line pointer variables */
        le_real_t * er_pose = NULL;
        le_data_t * er_data = NULL;

        /* Array pointer variables */
        le_byte_t * er_byte = le_array_get_byte( er_render.re_array );

        /* Cell dimension variables */
        le_real_t er_cmid = er_render.re_size / 2.0;

        /* Primitive bloc */
        glBegin( GL_POINTS );

        /* Display points */
        for ( le_size_t er_parse = 0; er_parse < le_array_get_size( er_render.re_array ); er_parse += LE_ARRAY_LINE ) {

            /* Compute line pointers */
            er_pose = ( le_real_t * ) ( er_byte + er_parse );
            er_data = ( le_data_t * ) ( er_byte + er_parse + sizeof( le_time_t ) + sizeof( le_real_t ) * 3 ); 

            /* Push element color */
            glColor4f( 

                er_data[0] / 255.0, 
                er_data[1] / 255.0, 
                er_data[2] / 255.0,

            1.0 );

            /* Push element vertex */
            glVertex3f(

                - er_cmid + ( er_pose[0] - er_render.re_edge[0] ) * LE_GEODESY_WGS84_A,
                - er_cmid + ( er_pose[1] - er_render.re_edge[1] ) * LE_GEODESY_WGS84_A,
                - er_cmid + ( er_pose[2] - er_render.re_edge[2] )

            );

        }

        /* Primitive bloc */
        glEnd();   

    }

    void er_render_isometry_disp_edge( void ) {

        /* Cell dimension variables */
        le_real_t er_cmid = er_render.re_size / 2.0;

        /* Push edge color */
        glColor4f( 1.0, 0.8, 0.3, 1.0 );

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

    void er_render_isometry_disp_save( void ) {

        /* Buffer variables */
        le_byte_t * er_buffer = NULL;

        /* Allocate buffer memory */
        if ( ( er_buffer = ( le_byte_t * ) malloc( er_render.re_width * er_render.re_height * 4 ) ) == NULL ) {

            /* Display message */
            fprintf( stderr, "eratosthene-suite : error : unable to allocate buffer memory\n" );

        } else {

            /* Read buffer pixel */
            glReadPixels( 0, 0, er_render.re_width, er_render.re_height, GL_RGBA, GL_UNSIGNED_BYTE, er_buffer );

            /* Export buffer */
            if ( er_png_write( ( char * ) er_render.re_path, er_render.re_width, er_render.re_height, er_buffer ) == 1 ) {

                /* Display message */
                fprintf( stderr, "eratosthene-suite : error : unable to export buffer to file\n" );

            }

            /* Unallocate buffer memory */
            free( er_buffer );

        }

    }

