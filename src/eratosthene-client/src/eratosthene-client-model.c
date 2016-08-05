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

    # include "eratosthene-client-model.h"

/*
    source - constructor/destructor methods
 */

    er_model_t er_model_create( le_char_t * const er_ip, le_sock_t const er_port ) {

        /* Created structure variables */
        er_model_t er_model = ER_MODEL_C;

        /* Array variables */
        le_array_t er_array = LE_ARRAY_C;

        /* Array access variables */
        le_array_cf_t er_access = LE_ARRAY_CF_C;

        /* Assign server network configuration */
        er_model.md_svip = er_ip;
        er_model.md_port = er_port;

        /* Query server configuration array */
        if ( le_client_array( er_ip, er_port, LE_NETWORK_MODE_CMOD, & er_array ) != LE_ERROR_SUCCESS ) {

            /* Send message */
            return( er_model._status = _LE_FALSE, er_model );

        } else {

            /* Compute configuration array mapping */
            le_array_cf( le_array_get_byte( & er_array ), 0, er_access );

            /* Retrieve configuration */
            er_model.md_sparam = er_access.as_size[0];
            er_model.md_tparam = er_access.as_time[0];

        }

        /* Allocate cells stack memory */
        if ( ( er_model.md_cell = ( er_cell_t * ) malloc( ( er_model.md_size = ER_MODEL_STACK ) * sizeof( er_cell_t ) ) ) == NULL ) {

            /* Send message */
            return( er_model._status = _LE_FALSE, er_model );

        } else {

            /* Create model cells */
            for ( le_size_t er_parse = 0; er_parse < er_model.md_size; er_parse ++ ) {

                /* Initialise cell */
                er_model.md_cell[er_parse] = er_cell_create();

            }

        }

        /* Return created model */
        return( er_model );

    }

    le_void_t er_model_delete( er_model_t * const er_model ) {

        /* Deleted structure variables */
        er_model_t er_reset = ER_MODEL_C;

        /* Check stack state */
        if ( er_model->md_cell != NULL ) {

            /* Delete model cells */
            for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

                /* Delete cell */
                er_cell_delete( er_model->md_cell + er_parse );

            }

            /* Unallocate stack memory */
            free( er_model->md_cell );

        }

        /* Delete structure */
        * ( er_model ) = er_reset;

    }

/*
    source - accessor methods
 */

    le_size_t er_model_get_cell( er_model_t const * const er_model ) {

        /* Parsing cells stack */
        for ( le_size_t er_parse = 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* Check cell state - return index */
            if ( er_cell_get_flag( er_model->md_cell + er_parse ) == _LE_FALSE ) return( er_parse );

        }

        /* Return index */
        return( er_model->md_size );

    }

    le_enum_t er_model_get_update( er_model_t * const er_model, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt ) {

        /* Static position memory variables */
        static le_real_t er_pose[3] = { 0.0, 0.0, 0.0 };

        /* Returned value variables */
        le_enum_t er_return = _LE_FALSE;

        /* Check update necessities - update return */
        if ( er_pose[0] != er_lon ) er_pose[0] = er_lon, er_return = _LE_TRUE;
        if ( er_pose[1] != er_lat ) er_pose[1] = er_lat, er_return = _LE_TRUE;
        if ( er_pose[2] != er_alt ) er_pose[2] = er_alt, er_return = _LE_TRUE;

        /* Send answer */
        return( er_return );

    }

/*
    source - mutator methods
 */

    le_void_t er_model_set_update_cell( er_model_t * const er_model, le_address_t * const er_enum, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt ) {

        /* Enumerator size variables */
        le_size_t er_scale = le_address_get_size( er_enum );

        /* Scale base variables */
        le_size_t er_base = LE_ADDRESS_BASE( er_scale );

        /* Distance variables */
        le_real_t er_dist = 0.0;

        /* Parsing scale digits */
        for ( le_size_t er_digit = 0; er_digit < er_base; er_digit ++ ) {

            /* Update enumerator size */
            le_address_set_size( er_enum, er_scale + 1 );

            /* Assign enumerator digit */
            le_address_set_digit( er_enum, er_scale, er_digit );

            /* Check enumeration constraint */
            if ( er_scale > ER_MODEL_ENUM ) {

                /* Compute distance */
                er_dist = er_geodesy_distance( er_enum, er_lon, er_lat, er_alt );

                /* Check selection criterion */
                if ( er_dist < er_geodesy_limit( er_alt ) ) {

                    /* Check depth criterion */
                    if ( fabs( er_geodesy_depth( er_dist, er_model->md_sparam, ER_MODEL_DEPTH ) - ( le_real_t ) er_scale ) < 1.0 ) {

                        /* Check cells stack */
                        if ( er_model->md_push < er_model->md_size ) {

                            /* Set address depth */
                            le_address_set_depth( er_enum, ER_MODEL_DEPTH );

                            /* Set cell address */
                            er_cell_set_push( er_model->md_cell + ( er_model->md_push ++ ), er_enum );

                        }

                    } else {

                        /* Check enumeration boundary */
                        if ( ( er_scale + ER_MODEL_DEPTH + 2 ) < er_model->md_sparam ) {

                            /* Set address depth */
                            le_address_set_depth( er_enum, 0 );

                            /* Set cell address */
                            er_cell_set_push( er_model->md_cell, er_enum );

                            /* Swap addresses */
                            er_cell_set_swap( er_model->md_cell, er_model->md_cell );

                            /* Check parent cell */
                            if ( er_cell_io_query( er_model->md_cell, er_model->md_svip, er_model->md_port ) > 0 ) {

                                /* Constrained enumeration */
                                er_model_set_update_cell( er_model, er_enum, er_lon, er_lat, er_alt ); 

                            }

                        }

                    }

                }

            /* Constraintless enumeration */
            } else { er_model_set_update_cell( er_model, er_enum, er_lon, er_lat, er_alt ); }

        }

    }

    le_void_t er_model_set_update_query( er_model_t * const er_model ) {

        /* Parsing variables */
        le_size_t er_parse = 0;
        le_size_t er_inner = 0;

        /* Searching variables */
        le_size_t er_found  = 0;

        /* Parsing model cells */
        for ( er_parse = 1; er_parse < er_model->md_push; er_parse ++ ) {

            /* Check pushed address */
            if ( er_cell_get_push( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                /* Reset cell search */
                er_inner = 1, er_found = er_model->md_size;

                /* Searching allocation */
                while ( ( er_inner < er_model->md_size ) && ( er_found == er_model->md_size ) ) {

                    /* Compare address and pushed address */
                    if ( er_cell_get_match( er_model->md_cell + er_inner, er_model->md_cell + er_parse ) == _LE_TRUE ) {

                        /* Reset pushed address */
                        er_cell_set_pop( er_model->md_cell + er_parse );

                        /* Update cell flag */
                        er_cell_set_flag( er_model->md_cell + er_inner, _LE_TRUE );

                        /* Update cell flag */
                        er_cell_set_draw( er_model->md_cell + er_inner, _LE_TRUE );

                    /* Update cell search */
                    er_found = er_inner; } else { er_inner ++; }

                }

            }

        }

        /* Parsing model cells */
        for ( er_parse = 1; er_parse < er_model->md_push; er_parse ++ ) {

            /* Check pushed address */
            if ( er_cell_get_push( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                /* Check search results */
                if ( ( er_found = er_model_get_cell( er_model ) ) != er_model->md_size ) {

                    /* Swap address and pushed address */
                    er_cell_set_swap( er_model->md_cell + er_found, er_model->md_cell + er_parse );

                    /* Update cell array */
                    er_cell_io_query( er_model->md_cell + er_found, er_model->md_svip, er_model->md_port );

                    /* Update cell flag */
                    er_cell_set_flag( er_model->md_cell + er_found, _LE_TRUE );

                    /* Update cell state */
                    er_cell_set_draw( er_model->md_cell + er_found, _LE_TRUE );

                }

                /* Reset pused address */
                er_cell_set_pop( er_model->md_cell + er_parse );

            }

        }

    }

    le_void_t er_model_set_update_terminate( er_model_t * const er_model ) {

        /* Parsing model cells */
        for ( le_size_t er_parse = 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* Check cell flag */
            if ( er_cell_get_flag( er_model->md_cell + er_parse ) == _LE_FALSE ) {

                /* Update cell state */
                er_cell_set_draw( er_model->md_cell + er_parse, _LE_FALSE );

            } else {

                /* Reset cell state */
                er_cell_set_flag( er_model->md_cell + er_parse, _LE_FALSE );

            }

        }

        /* Reset update index */
        er_model->md_push = 1;

    }

/*
    source - model display methods
 */

    le_void_t er_model_display_cell( er_model_t * const er_model, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt, le_real_t const er_azm, le_real_t const er_gam ) {

        /* Optimisation variables */
        le_real_t er_cosl = cos( - er_lon * ER_D2R );
        le_real_t er_sinl = sin( - er_lon * ER_D2R );
        le_real_t er_cosa = cos( + er_lat * ER_D2R );
        le_real_t er_sina = sin( + er_lat * ER_D2R );

        /* Edge array variables */
        le_real_t * er_edge = NULL;

        /* Motion management - tilt rotation */
        glRotated( - er_gam, 1.0, 0.0, 0.0 );

        /* Motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_alt + ER_ERA );

        /* Motion management - azimuth rotation */
        glRotated( + er_azm, 0.0, 0.0, 1.0 );

        /* Display model cells */
        for ( le_size_t er_parse = 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* Check cell state */
            if ( er_cell_get_draw( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                /* Vertex and color pointer assignation */
                glVertexPointer( 3, ER_MODEL_VERTEX, 0, er_cell_get_pose( er_model->md_cell + er_parse ) );
                glColorPointer ( 3, ER_MODEL_COLORS, 0, er_cell_get_data( er_model->md_cell + er_parse ) );

                /* Cell matrix */
                glPushMatrix(); {

                    /* Retrieve edge array */
                    er_edge = er_cell_get_edge( er_model->md_cell + er_parse );

                    /* Motion management - cell edge translation */
                    glTranslated( er_cosl * er_edge[0] + er_sinl * er_edge[2], er_sina * er_sinl * er_edge[0] + er_cosa * er_edge[1] - er_sina * er_cosl * er_edge[2], er_cosa * er_cosl * er_edge[2] + er_sina * er_edge[1] - er_cosa * er_sinl * er_edge[0] - ER_ERA );

                    /* Motion management - planimetric rotation */
                    glRotated( + er_lat, 1.0, 0.0, 0.0 );
                    glRotated( - er_lon, 0.0, 1.0, 0.0 );

                    /* Display graphical primitives */
                    glDrawArrays( GL_POINTS, 0, er_cell_get_size( er_model->md_cell + er_parse ) / 3 );

                /* Cell matrix */
                } glPopMatrix();

            }

        }

    }

    le_void_t er_model_display_earth( le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt, le_real_t const er_azm, le_real_t const er_gam ) {

        /* Motion management - tilt rotation */
        glRotated( - er_gam, 1.0, 0.0, 0.0 );

        /* Motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_alt + ER_ERA );

        /* Motion management - azimuth rotation */
        glRotated( + er_azm, 0.0, 0.0, 1.0 );

        /* Earth wireframe - centering */
        glTranslated( 0.0, 0.0, - ER_ERA );

        /* Motion management - planimetric rotation */
        glRotated( + er_lat, 1.0, 0.0, 0.0 );
        glRotated( - er_lon, 0.0, 1.0, 0.0 );

        /* Earth wireframe - orientation */
        glRotated( 90.0, 1.0, 0.0, 0.0 );

        /* Earth wireframe - color */
        glColor3f( 0.3, 0.32, 0.4 );

        /* Create quadric */
        GLUquadricObj * er_earth = gluNewQuadric();

        /* Configure quadric */
        gluQuadricDrawStyle( er_earth, GLU_LINE );

        /* Display quadric */
        gluSphere( er_earth, ER_ERA, 360, 180 );

        /* Delete quadric */
        gluDeleteQuadric( er_earth );

    }

