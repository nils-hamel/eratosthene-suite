/*
 *  liberatosthene - geodetic system
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

    # include "eratosthene-client-render.h"

/*
    source - ugly global variable (GLUT callbacks)
 */

    er_engine_t er_handle = ER_ENGINE_C;

/*
    source - rendering engine
 */

    void er_engine( le_sock_t const er_client ) {

        /* Windows handle variables */
        int er_window = 0;

        /* Assign client socket to handle */
        er_handle.eg_client = er_client;

        /* Setting windows parameteres */
        glutInitWindowSize( glutGet( GLUT_SCREEN_WIDTH ), glutGet( GLUT_SCREEN_HEIGHT ) );

        /* Create rendering engine window */
        er_window = glutCreateWindow( "eratosthene-client" );

        /* Display rendering engine window in fullscreen */
        glutFullScreen();

        /* Hide mouse cursor */
        glutSetCursor( GLUT_CURSOR_NONE );

        /* Rendering engine display configuration */
        glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL );

        /* Assign callback functions */
        glutDisplayFunc      ( er_engine_render  );
        glutReshapeFunc      ( er_engine_reshape );
        glutKeyboardFunc     ( er_engine_keybd   );
        glutMouseFunc        ( er_engine_mouse   );
        glutMotionFunc       ( er_engine_move    );
        glutPassiveMotionFunc( er_engine_move    );

        /* Setting color clear value */
        glClearColor( 0.0, 0.0, 0.0, 0.0 );

        /* Setting depth clear value */
        glClearDepth( 1.0 );

        /* Setting depth configuration */
        glEnable( GL_DEPTH_TEST );
        glDepthFunc( GL_LEQUAL );
        glDepthMask( GL_TRUE );

        /* Shade model configuration */
        glShadeModel( GL_SMOOTH );

        /* GLUT event management loop */
        glutMainLoop();

        /* Delete rendering engine window */
        glutDestroyWindow( er_window );

    }

/*
    source - rendering engine - render callback
 */

    void er_engine_render( void ) {

        /* Clear color and depth buffers */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* Matrix mode to model view */
        glMatrixMode( GL_MODELVIEW );

        /* Set model view matrix to identity */
        glLoadIdentity();

        /* Push matrix */
        glPushMatrix(); {

            glTranslatef( 0, 0, -er_handle.eg_valt );

            glRotatef( er_handle.eg_vlon, 0.0, 1.0, 0.0 );
            glRotatef( er_handle.eg_vlat, 1.0, 0.0, 0.0 );

            er_model();

        /* Pop matrix */
        } glPopMatrix();

        /* Force primitives trigger */
        glFlush();

        /* Wait primitives */
        glFinish();

    }

/*
    source - rendering engine - reshape callback
 */

    void er_engine_reshape( int er_width, int er_height ) {

        /* Reset viewport */
        glViewport( 0, 0, er_width, er_height );

        /* Matrix mode to projection */
        glMatrixMode( GL_PROJECTION );

        /* Set projection matrix to identity */
        glLoadIdentity();

        /* Compute projectio matrix */
        gluPerspective( 45, ( float ) er_width / er_height, 1.0, 1000.0 );

    }

/*
    source - rendering engine - keyboard callback
 */

    void er_engine_keybd( unsigned char er_keycode, int er_x, int er_y ) {

        /* Switch on keycode */
        switch( er_keycode ) {

            /* Escape */
            case ( 27 ) : {

                /* Leave GLUT events management loop */
                glutLeaveMainLoop();

            } break;

            case ( 'q' ) : er_handle.eg_valt *= 1.1; break;
            case ( 'e' ) : er_handle.eg_valt *= 0.9; break;
            case ( 'd' ) : er_handle.eg_vlon += 0.1; break;
            case ( 'a' ) : er_handle.eg_vlon -= 0.1; break;
            case ( 'w' ) : er_handle.eg_vlat += 0.1; break;
            case ( 's' ) : er_handle.eg_vlat -= 0.1; break;

        };

        fprintf( stderr, "%u %i\n", er_keycode, GLUT_KEY_UP );

        /* Schedule render callback */
        glutPostRedisplay();

    }

/*
    source - rendering engine - mouse callback
 */

    void er_engine_mouse( int er_button, int er_state, int er_x, int er_y ) {

        /* Update engine handle */
        er_handle.eg_button = er_button;
        er_handle.eg_state  = er_state;
        er_handle.eg_x      = er_x;
        er_handle.eg_y      = er_y;
        er_handle.eg_u      = er_x;
        er_handle.eg_v      = er_y;

        /* Schedule render callback */
        glutPostRedisplay();

    }

    void er_engine_move( int er_x, int er_y ) {

        /* Check mouse state */
        if ( er_handle.eg_state == GLUT_DOWN ) {

            /* Update engine handle */
            er_handle.eg_u = er_x;
            er_handle.eg_v = er_y;

        }

        /* Update engine handle */
        er_handle.eg_s = er_x;
        er_handle.eg_t = er_y;

        /* Schedule render callback */
        glutPostRedisplay();

    }

