/*
 *  eratosthene-suite - geodetic system
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

    # include "eratosthene-client-model.h"

/*
    source - constructor/destructor methods
 */

    er_model_t er_model_create( le_void_t ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Model variables */
        er_model_t er_model = { 1, { ER_CELL_C } };

        /* Parsing cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Create cell */
            ( er_model.md_cell )[er_parse] = er_cell_create( er_parse );

        }

        /* Return constructed model */
        return( er_model );

    }

    le_void_t er_model_delete( er_model_t * const er_model ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Parsing cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Delete cell */
            er_cell_delete( ( er_model->md_cell ) + er_parse );

        }

    }

/*
    source - accessor methods
 */

    le_size_t er_model_get_psiz( er_model_t const * const er_model ) {

        /* Return model point size */
        return( er_model->md_psiz );

    }

/*
    source - mutator methods
 */

    le_void_t er_model_set_psiz( er_model_t * const er_model, le_size_t er_psiz ) {

        /* Apply limitation */
        if ( ( er_psiz > 0 ) && ( er_psiz <= 8 ) ) {

            /* Assign model point size */
            er_model->md_psiz = er_psiz;

        }

    }

/*
    source - model display
 */

    void er_model_main( er_model_t * const er_model ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Display cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Check cell size */
            if ( er_cell_get_size( er_model->md_cell + er_parse ) > 0 ) {

                /* Check cell state */
                if ( er_cell_get_state( er_model->md_cell + er_parse ) == ER_CELL_IDLE ) {

                    /* Vertex and color array configuration */
                    glVertexPointer( 3, GL_DOUBLE, 0, er_cell_get_pose( er_model->md_cell + er_parse ) );
                    glColorPointer ( 3, GL_FLOAT , 0, er_cell_get_data( er_model->md_cell + er_parse ) );

                    /* Trigger cell primitive drawing */
                    glDrawArrays( GL_POINTS, 0, er_cell_get_size( er_model->md_cell + er_parse) / 3 );

                }

            }

        }

        /* Earth frame - orientation */
        glRotatef( 90.0, 1.0, 0.0, 0.0 );

        /* Earth frame - color */
        glColor3f( 0.3, 0.32, 0.4 );

        /* Earth model variables */
        GLUquadricObj * er_earth = gluNewQuadric();

        /* Configure quadric */
        gluQuadricDrawStyle( er_earth, GLU_LINE );

        /* Draw quadric */
        gluSphere( er_earth, ER_ERA, 360, 180 );

        /* Delete quadric */
        gluDeleteQuadric( er_earth );

    }

/*
    source - model management
 */

    void er_model_update( er_model_t * const er_model, le_time_t const er_time, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt ) {

        /* Parsing variables */
        le_size_t er_parse = 0;
        le_size_t er_index = 0;
        le_size_t er_depth = 4;

        /* Address variables */
        le_address_t er_ref = LE_ADDRESS_C_SIZE( 24 );
        le_address_t er_qry = LE_ADDRESS_C_SIZE( 24 );

        /* Pose array variables */
        le_real_t er_pose[3] = { er_lon, er_lat, er_alt > LE_GEODESY_HMAX ? 0 : er_alt };

        /* Compute reference address */
        le_address_set_pose( & er_ref, er_pose );

        while ( ( er_parse < ER_MODEL_SEG ) && ( er_depth <= 17 ) ) {

            /* Copy address head */
            memcpy( er_qry.as_addr, er_ref.as_addr, er_depth );

            for ( er_index = 0; er_index < ( ( er_depth + 1 >= 10 ) ? 8 : 4 ); er_index ++ ) {

                if ( ( er_index != le_address_get_digit( & er_ref, er_depth ) ) || ( er_depth == 17 ) ) {

                    le_address_set_size ( & er_qry, er_depth + 1 );
                    le_address_set_digit( & er_qry, er_depth, er_index );
                    le_address_set_time ( & er_qry, er_time );
                    le_address_set_depth( & er_qry, 6 );

                    memset( ( er_model->md_cell + er_parse )->ce_push, 0, _LE_USE_DEPTH );

                    le_address_cvas( & er_qry, ( er_model->md_cell + er_parse )->ce_push );

                    fprintf( stderr, "%u : %s\n", er_index, ( er_model->md_cell + er_parse )->ce_push );

                    er_parse ++;

                }

            }

            er_depth ++;

        }

    }

    void er_model_client( le_char_t const * const er_ip, le_sock_t const er_port, er_model_t * const er_model ) {

        /* Parsing variables - static */
        le_size_t er_parse = 0;

        /* Socket handle variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Parsing cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Check update necessities */
            if ( er_cell_get_update( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                 fprintf( stderr, "Update : %s\n", ( er_model->md_cell + er_parse )->ce_push );

                /* Establish server connexion */
                if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {
                    
                    /* Update cell through server query */
                    er_cell_set_query( er_model->md_cell + er_parse, er_socket );

                    /* Close server connexion */
                    le_client_delete( er_socket );

                }

            }

        }

    }

    void er_model_update2( er_model_t * const er_model, le_time_t const er_time, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt ) {

        /* Parsing variables - static */
        static le_size_t er_parse = 0;

        /* Address variables */
        le_char_t er_addr[_LE_USE_DEPTH] = { 0 };

        /* Search requiered update */
        do {

            /* Update parser */
            er_parse = ( er_parse + 1 ) % ER_MODEL_SEG;

            /* Compute altimetric model */
            le_size_t er_malt = 12 - ( 10 * ( er_alt - ER_ERA ) ) / ER_ERA;

            /* Compose address */
            sprintf( ( char * ) er_addr, "/%" _LE_TIME_P "/%s/%" _LE_SIZE_P, er_time, ( er_model->md_cell + er_parse )->ce_base, er_malt );

            fprintf( stderr, "%s\n", er_addr );

        /* Check for update */
        } while ( ( er_cell_set_addr( er_model->md_cell + er_parse, er_addr ) == _LE_FALSE ) && ( er_parse != 0 ) );

        return;

        /* Parsing cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Compute altimetric model */
            le_size_t er_malt = 12 - ( 10 * ( er_alt - ER_ERA ) ) / ER_ERA;

            /* Push cell address */
            sprintf( ( char * ) ( er_model->md_cell + er_parse )->ce_push, "/%" _LE_TIME_P "/%s/%" _LE_SIZE_P, er_time, ( er_model->md_cell + er_parse )->ce_base, er_malt );

        }

    }

    void er_model_client2( le_char_t const * const er_ip, le_sock_t const er_port, er_model_t * const er_model ) {

        /* Parsing variables - static */
        static le_size_t er_parse = 0;

        le_enum_t er_flag = 0;

        /* Socket handle variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        do {

            /* Update parser */
            er_parse = ( er_parse + 1 ) % ER_MODEL_SEG;

            fprintf( stderr, "%u %u\n", er_flag, er_parse );

            if ( ( er_flag = er_cell_get_update( er_model->md_cell + er_parse ) ) == _LE_TRUE ) {

            /* Establish server connexion */
            if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {
                
                /* Update cell through server query */
                er_cell_set_query( er_model->md_cell + er_parse, er_socket );

                /* Close server connexion */
                le_client_delete( er_socket );

            }

            }

        } while ( ( er_flag  == _LE_FALSE ) && ( er_parse != 0 ) );

        return;

        /* Parsing variables - static */
        //le_size_t er_parse = 0;

        /* Socket handle variables */
        //le_sock_t er_socket = _LE_SOCK_NULL;

        /* Parsing cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Check update necessities */
            if ( er_cell_get_update( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                /* Establish server connexion */
                if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {
                    
                    /* Update cell through server query */
                    er_cell_set_query( er_model->md_cell + er_parse, er_socket );

                    /* Close server connexion */
                    le_client_delete( er_socket );

                }

            }

        }

    }

