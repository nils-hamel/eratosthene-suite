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

    er_model_t er_model_create( le_size_t er_cells ) {

        /* Model variables */
        er_model_t er_model = ER_MODEL_C;

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Allocate cell array memory */
        if ( ( er_model.md_cell = ( er_cell_t * ) malloc( er_cells * sizeof( er_cell_t ) ) ) != NULL ) {

            /* Assign cell array size */
            er_model.md_size = er_cells;

            /* Parsing cells array */
            for ( ; er_parse < er_model.md_size; er_parse ++ ) {

                /* Initialise cell */
                ( er_model.md_cell )[er_parse] = er_cell_create();

            }

        }

        /* Return constructed model */
        return( er_model );

    }

    le_void_t er_model_delete( er_model_t * const er_model ) {

        /* Model variables */
        er_model_t er_reset = ER_MODEL_C;

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Check cells array state */
        if ( er_model->md_size > 0 ) {

            /* Parsing cells array */
            for ( ; er_parse < er_model->md_size; er_parse ++ ) {

                /* Delete cell */
                er_cell_delete( ( er_model->md_cell ) + er_parse );

            }

            /* Unallocate cells array memory */
            free( er_model->md_cell );

        }

        /* Clear model fields */
        * ( er_model ) = er_reset;

    }

/*
    source - accessor methods
 */

    le_size_t er_model_get_sdisc( er_model_t const * const er_model ) {

        /* Return model space discretisation */
        return( er_model->md_sdis );

    }

    le_size_t er_model_get_tdisc( er_model_t const * const er_model ) {

        /* Return model time discretisation */
        return( er_model->md_tdis );

    }

/*
    source - mutator methods
 */

    le_void_t er_model_set_ip( er_model_t * const er_model, le_char_t const * const er_ip ) {

        /* Assign server ip */
        strcpy( ( char * ) er_model->md_svip, ( char * ) er_ip );

    }

    le_void_t er_model_set_port( er_model_t * const er_model, le_sock_t const er_port ) {

        /* Assign server port */
        er_model->md_port = er_port;

    }

    le_enum_t er_model_set_sdisc( er_model_t * const er_model ) {

        /* Socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Returned value variables */
        le_enum_t er_return = LE_ERROR_SUCCESS;

        /* Establish server connexion */
        if ( ( er_socket = le_client_create( er_model->md_svip, er_model->md_port ) ) != _LE_SOCK_NULL ) {

            /* Client/server query handshake */
            if ( le_client_handshake_mode( er_socket, LE_NETWORK_MODE_SMOD ) == LE_ERROR_SUCCESS ) {

                /* Read parameter in socket */
                if ( read( er_socket, & ( er_model->md_sdis ), sizeof( le_size_t ) ) != sizeof( le_size_t ) ) {

                    /* Push message */
                    er_return = LE_ERROR_IO_READ;

                }

            } else {

                /* Push message */
                er_return = LE_ERROR_IO_SOCKET;

            }

            /* Close server connexion */
            er_socket = le_client_delete( er_socket );

        }

        /* Send message */
        return( er_return );

    }

    le_enum_t er_model_set_tdisc( er_model_t * const er_model ) {

        /* Socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Returned value variables */
        le_enum_t er_return = LE_ERROR_SUCCESS;

        /* Establish server connexion */
        if ( ( er_socket = le_client_create( er_model->md_svip, er_model->md_port ) ) != _LE_SOCK_NULL ) {

            /* Client/server query handshake */
            if ( le_client_handshake_mode( er_socket, LE_NETWORK_MODE_TMOD ) == LE_ERROR_SUCCESS ) {

                /* Read parameter in socket */
                if ( read( er_socket, & ( er_model->md_tdis ), sizeof( le_size_t ) ) != sizeof( le_size_t ) ) {

                    /* Push message */
                    er_return = LE_ERROR_IO_READ;

                }

            } else {

                /* Push message */
                er_return = LE_ERROR_IO_SOCKET;

            }

            /* Close server connexion */
            er_socket = le_client_delete( er_socket );

        }

        /* Send message */
        return( er_return );

    }

    le_void_t er_model_set_push( er_model_t * const er_model ) {

        /* Swapping variables */
        er_cell_t er_cell = ER_CELL_C;

        /* Check zero cell state */
        if ( er_cell_get_size( er_model->md_cell ) > 0 ) {

            /* Update push value */
            er_model->md_push ++;

            /* Check limitations */
            if ( er_model->md_push < er_model->md_size ) {

                /* Backup pushed cell */
                er_cell = ( er_model->md_cell )[er_model->md_push];

                /* Assign zero cell */
                ( er_model->md_cell )[er_model->md_push] = ( er_model->md_cell )[0];

                /* Assign cell to zero */
                ( er_model->md_cell )[0] = er_cell;

            }

        }

    }

    le_void_t er_model_set_model( er_model_t * const er_model, le_time_t const er_time, le_real_t const er_lon, le_real_t const er_lat, le_real_t er_alt ) {

        /* Parsing variables */
        le_size_t er_plon = 0;
        le_size_t er_plat = 0;

        /* Position vector variable */
        le_real_t er_pose[3] = { 0.0, 0.0, er_alt };

        /* Scale width variables */
        le_real_t er_scale = LE_2P / pow( 2, LE_GEODESY_ASYA - 1 );

        /* Address variables */
        le_address_t er_addr = LE_ADDRESS_C_SIZE( LE_GEODESY_ASYA - 1 );

        /* Assign address time */
        le_address_set_time( & er_addr, er_time );

        /* Clear push stack */
        er_model->md_push = 0;

        /* Parsing neighbour central cells - longitude */
        for ( er_plon = 0; er_plon < 9; er_plon ++ ) {

            /* Parsing neighbour central cells - latitude */
            for ( er_plat = 0; er_plat < 5; er_plat ++ ) {

                /* Compose position vector */
                er_pose[0] = er_lon + er_scale * ( ( le_real_t ) er_plon - 4.0 );
                er_pose[1] = er_lat + er_scale * ( ( le_real_t ) er_plat - 2.0 );

                /* Assign address depth */
                le_address_set_depth( & er_addr, ER_MODEL_DPT );

                /* Assign address size */
                le_address_set_size( & er_addr, LE_GEODESY_ASYA - 1 );

                /* Assign address position */
                le_address_set_pose( & er_addr, er_pose );

                /* Recursive address push */
                er_model_set_addr( er_model, & er_addr, er_lon, er_lat, er_alt );

            }

        }

        /* Clear remaining cells */
        while ( ( ++ er_model->md_push ) < er_model->md_size ) {

            /* Clear cell */
            er_cell_set_empty( ( er_model->md_cell ) + er_model->md_push );

        }

    }

    le_void_t er_model_set_addr( er_model_t * const er_model, le_address_t * const er_addr, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt ) {

        /* Socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Distance computation */
        le_real_t er_dist = 0.0;
        le_real_t er_curr = 0.0;
        le_size_t er_dept = 0.0;

        /* Position vector variables */
        le_real_t er_pose[3] = { 0.0 };
        le_real_t er_view[3] = { er_lon, er_lat, er_alt - ER_ERA };

        /* Get cell position */
        le_address_get_pose( er_addr, er_pose );

        /* Compute cell center */
        er_pose[0] += ( LE_GEODESY_LMAX - LE_GEODESY_LMIN ) / pow( 2, le_address_get_size( er_addr ) + 1 );
        er_pose[1] += ( LE_GEODESY_LMAX - LE_GEODESY_LMIN ) / pow( 2, le_address_get_size( er_addr ) + 1 );
        er_pose[2] += ( LE_GEODESY_LMAX - LE_GEODESY_LMIN ) / pow( 2, le_address_get_size( er_addr ) + 1 );

        /* Convert to cartesian */
        er_geodesy_cartesian( er_pose, 1 );
        er_geodesy_cartesian( er_view, 1 );

        /* Compute distance */
        er_dist = sqrt( ( er_pose[0] - er_view[0] ) * ( er_pose[0] - er_view[0] ) + ( er_pose[1] - er_view[1] ) * ( er_pose[1] - er_view[1] ) + ( er_pose[2] - er_view[2] ) * ( er_pose[2] - er_view[2] ) );

        /* Compute geodetic scale */
        er_curr = er_geodesy_distance( er_dist, LE_GEODESY_ASYA - 1, er_model->md_sdis - 8 ); //er_model->md_sdis - 12 );
        er_dept = er_geodesy_depth( er_dist, 6, 8 );

        /* Check geodetic scale value */
        if ( ( fabs( er_curr - le_address_get_size( er_addr ) ) < 1 ) ) {

            /* Set address depth */
            le_address_set_depth( er_addr, er_dept );

            /* Set cell address */
            er_cell_set_addr( er_model->md_cell, er_addr );

            /* Establish server connexion */
            if ( ( er_socket = le_client_create( er_model->md_svip, er_model->md_port ) ) != _LE_SOCK_NULL ) {
                
                /* Update cell through server query */
                er_cell_set_query( er_model->md_cell, er_socket );

                /* Push queried cell */
                er_model_set_push( er_model );

                /* Close server connexion */
                er_socket = le_client_delete( er_socket );

            }

        } else {

            /* Increase address size */
            le_size_t er_addr_s = le_address_get_size( er_addr );

            /* Set address depth */
            le_address_set_depth( er_addr, 0 );

            /* Set cell address */
            er_cell_set_addr( er_model->md_cell, er_addr );

            /* Establish server connexion */
            if ( ( er_socket = le_client_create( er_model->md_svip, er_model->md_port ) ) != _LE_SOCK_NULL ) {
                
                /* Update cell through server query */
                er_cell_set_query( er_model->md_cell, er_socket );

                /* Close server connexion */
                er_socket = le_client_delete( er_socket );

                /* Check cell size */
                if ( er_cell_get_size( er_model->md_cell ) > 0 ) {

                    /* Set address depth */
                    le_address_set_depth( er_addr, ER_MODEL_DPT );

                    /* Parsing sub-cells */
                    for ( er_parse = 0; er_parse < _LE_USE_BASE; er_parse ++ ) {

                        /* Set address size */
                        le_address_set_size( er_addr, er_addr_s + 1 );

                        /* Set address digit */
                        le_address_set_digit( er_addr, er_addr_s, er_parse );

                        /* Recursive cell searching */
                        er_model_set_addr( er_model, er_addr, er_lon, er_lat, er_alt );

                    }

                }

            }

        }

    }

/*
    source - model display methods
 */

    le_void_t er_model_display_cell( er_model_t * const er_model ) {

        /* Parsing and count variables */
        le_size_t er_parse = 0;
        le_size_t er_count = 0;

        /* Display cells content */
        for ( er_parse = 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* Check cell content */
            if ( ( er_count = er_cell_get_size( ( er_model->md_cell ) + er_parse ) ) > 0 ) {

                /* Vertex and color pointer to cell arrays */
                glVertexPointer( 3, ER_MODEL_VA, 0, er_cell_get_pose( ( er_model->md_cell ) + er_parse ) );
                glColorPointer ( 3, ER_MODEL_CA, 0, er_cell_get_data( ( er_model->md_cell ) + er_parse ) );

                /* Display graphical primitives */
                glDrawArrays( GL_POINTS, 0, er_count / 3 );

            }

        }

    }

    le_void_t er_model_display_earth( le_void_t ) {

        /* Earth frame - orientation */
        glRotated( 90.0, 1.0, 0.0, 0.0 );

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

