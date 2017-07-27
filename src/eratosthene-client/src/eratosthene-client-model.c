/*
 *  eratosthene-suite - client
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

    # include "eratosthene-client-model.h"

/*
    source - constructor/destructor methods
 */

    er_model_t er_model_create( le_sock_t const er_socket, le_size_t const er_scfg, le_time_t const er_tcfg ) {

        /* created structure variables */
        er_model_t er_model = ER_MODEL_I( er_socket, er_scfg, er_tcfg );

        /* create model cells */
        if ( ( er_model.md_cell = ( er_cell_t * ) malloc( er_model.md_size * sizeof( er_cell_t ) ) ) == NULL ) {

            /* return created structure */
            return( er_model._status = _LE_FALSE, er_model );

        }

        /* initialise model cells */
        for ( le_size_t er_parse = 0; er_parse < er_model.md_size; er_parse ++ ) {

            /* create cell structure */
            er_model.md_cell[er_parse] = er_cell_create();

        }

        /* return created structure */
        return( er_model );

    }

    er_model_t_ er_model_create_( le_sock_t const er_socket, le_size_t const er_scfg, le_time_t const er_tcfg ) {

        /* created structure variables */
        er_model_t_ er_model = ER_MODEL_I_( er_socket, er_scfg, er_tcfg );

        /* array size variables */
        le_size_t er_size = er_model.md_size << 1;

        /* create cell arrays */
        if ( ( er_model.md_cell = ( er_cell_t_ * ) malloc( er_size * sizeof( er_cell_t_ ) ) ) == NULL ) {

            /* return created structure */
            return( er_model._status = _LE_FALSE, er_model );

        }

        /* initialise arrays cells */
        for ( le_size_t er_parse = 0; er_parse < er_size; er_parse ++ ) {

            /* initialise cell */
            er_model.md_cell[er_parse] = er_cell_create_();

        }

        /* assign secondary cache */
        er_model.md_targ = er_model.md_cell + er_model.md_size;

        /* return created structure */
        return( er_model );

    }

    le_void_t er_model_delete( er_model_t * const er_model ) {

        /* deleted structure variables */
        er_model_t er_delete = ER_MODEL_C;

        /* model cells state */
        if ( er_model->md_cell != NULL ) {

            /* delete model cells */
            for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

                /* delete cell structure */
                er_cell_delete( er_model->md_cell + er_parse );

            }

            /* delete model cells */
            free( er_model->md_cell );

        }

        /* delete structure */
        ( * er_model ) = er_delete;

    }

    le_void_t er_model_delete_( er_model_t_ * const er_model ) {

        /* deleted structure variables */
        er_model_t_ er_delete = ER_MODEL_C_;

        /* check cell array */
        if ( er_model->md_cell != NULL ) {

            /* update array size */
            er_model->md_size <<= 1;

            /* delete arrays cells */
            for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

                /* delete cell */
                er_cell_delete_( er_model->md_cell + er_parse );

            }

            /* release array memory */
            free( er_model->md_cell );

        }

        /* delete structure */
        ( * er_model ) = er_delete;

    }

/*
    source - mutator methods
 */

    le_void_t er_model_set_prep_( er_model_t_ * const er_model ) {

        /* reset push value */
        er_model->md_push = 0;

        /* reset synchronisation value */
        er_model->md_sync = 1;

        /* reset d-cells synchronisation flag */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* clear d-cell/t-cell synchronisation flag */
            er_cell_set_clear_( er_model->md_cell + er_parse, ER_CELL_SYN_ );
            er_cell_set_clear_( er_model->md_targ + er_parse, ER_CELL_SYN_ );

        }

    }

    le_void_t er_model_set_enum_( er_model_t_ * const er_model, le_address_t * const er_enum, le_size_t const er_scale, er_view_t const * const er_view ) {

        /* scale base variables */
        le_size_t er_base = le_address_base( er_scale );

        /* distance variables */
        le_real_t er_line = 0.0;

        /* scale digit enumeration */
        for ( le_size_t er_digit = 0; er_digit < er_base; er_digit ++ ) {

            /* update address size */
            le_address_set_size( er_enum, er_scale + 1 );

            /* assign address digit */
            le_address_set_digit( er_enum, er_scale, er_digit );

            /* enumeration constraint */
            if ( er_scale < ER_COMMON_ENUM ) {

                /* continue enumeration */
                er_model_set_enum_( er_model, er_enum, er_scale + 1, er_view );

            } else {

                /* compute and check distance */
                if ( ( er_line = er_geodesy_distance( er_enum, er_view ) ) < er_geodesy_face( er_view_get_alt( er_view ) ) ) {

                    /* check selection criterion */
                    if ( er_geodesy_select( er_line, er_model->md_scfg, er_scale ) == _LE_TRUE ) {

                        /* check target size */
                        if ( er_model->md_push < er_model->md_size ) {

                            /* push address */
                            er_cell_set_push_( er_model->md_targ + er_model->md_push, er_enum );

                            /* update push value */
                            er_model->md_push ++;

                        }

                    } else {

                        /* check enumeration boundary */
                        if ( ( er_scale + 2 + ER_COMMON_SPAN ) < er_model->md_scfg ) {

                            /* continue enumeration */
                            er_model_set_enum_( er_model, er_enum, er_scale + 1, er_view );

                        }

                    }

                }

            }

        }

    }

    le_void_t er_model_set_fast_( er_model_t_ * const er_model ) {

        /* parsing d-cells array */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* check d-cell address length */
            if ( er_cell_get_size_( er_model->md_cell + er_parse ) > 0 ) {

                /* parsing t-cell array */
                for ( le_size_t er_index = 0; er_index < er_model->md_push; er_index ++ ) {

                    /* check address identity */
                    if ( er_cell_get_equal_( er_model->md_cell + er_parse, er_model->md_targ + er_index ) == _LE_TRUE ) {

                        /* update cells flag */
                        er_cell_set_flag_( er_model->md_cell + er_parse, ER_CELL_SYN_ | ER_CELL_DIS_ );
                        er_cell_set_flag_( er_model->md_targ + er_index, ER_CELL_SYN_ );

                    }

                }

            }

        }

    }

    le_void_t er_model_set_sync_( er_model_t_ * const er_model ) {

        /* serialisation variables */
        le_size_t er_serial = 0;

        /* array index variables */
        le_size_t er_search = 0;

        /* socket-array variables */
        le_array_t er_array = LE_ARRAY_C;

        /* check synchronisation */
        if ( er_model->md_sync == er_model->md_scfg ) {

            /* parsing d-cells array */
            for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

                /* check d-cell state */
                if ( er_cell_get_flag_( er_model->md_cell + er_parse, ER_CELL_DIS_ ) == ER_CELL_DIS_ ) {

                    /* check d-cell state */
                    if ( er_cell_get_flag_( er_model->md_cell + er_parse, ER_CELL_SYN_ ) != ER_CELL_SYN_ ) {

                        /* update d-cell display state */
                        er_cell_set_clear_( er_model->md_cell + er_parse, ER_CELL_DIS_ );

                    }

                }

            }

        } else {

            /* parsing t-cells array */
            for ( le_size_t er_parse = 0; er_parse < er_model->md_push; er_parse ++ ) {

                /* select unsynchronised t-cell */
                if ( er_cell_get_flag_( er_model->md_targ + er_parse, ER_CELL_SYN_ ) != ER_CELL_SYN_ ) {

                    /* select t-cell by length */
                    if ( er_cell_get_size_( er_model->md_targ + er_parse ) == er_model->md_sync ) {

                        /* update array size */
                        le_array_set( & er_array, LE_ARRAY_ADDR );

                        /* serialise address */
                        er_serial = er_cell_serial_( er_model->md_targ + er_parse, & er_array, er_serial );

                    }

                }

            }

            /* check array size */
            if ( le_array_get_size( & er_array ) > 0 ) {

                /* write socket-array */
                le_array_io_write( & er_array, LE_MODE_QUER, er_model->md_sock );

                /* parsing t-cells array */
                for ( le_size_t er_parse = 0; er_parse < er_model->md_push; er_parse ++ ) {

                    /* select unsynchronised t-cell */
                    if ( er_cell_get_flag_( er_model->md_targ + er_parse, ER_CELL_SYN_ ) != ER_CELL_SYN_ ) {

                        /* select t-cell by length */
                        if ( er_cell_get_size_( er_model->md_targ + er_parse ) == er_model->md_sync ) {

                            /* search usable d-cell */
                            while ( er_cell_get_flag_( er_model->md_cell + er_search, ER_CELL_SYN_ ) == ER_CELL_SYN_ ) er_search ++;

                            /* update d-cell state */
                            er_cell_set_clear_( er_model->md_cell + er_search, ER_CELL_DIS_ );

                            /* read socket-array */
                            le_array_io_read( & er_model->md_read, er_model->md_sock );

                            /* process socket-array */
                            er_cell_io_read_( er_model->md_cell + er_search, & er_model->md_read );

                            /* sychronise address cell */
                            er_cell_set_sync_( er_model->md_cell + er_search, er_model->md_targ + er_parse );

                            /* update d-cell state */
                            er_cell_set_flag_( er_model->md_cell + er_search, ER_CELL_DIS_ | ER_CELL_SYN_ );

                            /* update t-cell state */
                            er_cell_set_flag_( er_model->md_targ + er_parse, ER_CELL_SYN_ );

                        }

                    }

                }

            }

            /* update synchronisation level */
            er_model->md_sync = er_model->md_sync + 1;

        }

    }

    le_void_t er_model_set_enum( er_model_t * const er_model, le_address_t * const er_enum, le_size_t const er_scale, er_view_t const * const er_view ) {

        /* scale base variables */
        le_size_t er_base = le_address_base( er_scale );

        /* distance variables */
        le_real_t er_line = 0.0;

        /* scale digit enumeration */
        for ( le_size_t er_digit = 0; er_digit < er_base; er_digit ++ ) {

            /* update address size */
            le_address_set_size( er_enum, er_scale + 1 );

            /* assign address digit */
            le_address_set_digit( er_enum, er_scale, er_digit );

            /* enumeration constraint */
            if ( er_scale < ER_COMMON_ENUM ) {

                /* continue enumeration */
                er_model_set_enum( er_model, er_enum, er_scale + 1, er_view );

            } else {

                /* compute and check distance */
                if ( ( er_line = er_geodesy_distance( er_enum, er_view ) ) < er_geodesy_face( er_view_get_alt( er_view ) ) ) {

                    /* check selection criterion */
                    if ( er_geodesy_select( er_line, er_model->md_scfg, er_scale ) == _LE_TRUE ) {

                        /* push address */
                        er_model_set_push( er_model, er_enum );

                    } else {

                        /* check enumeration boundary */
                        if ( ( er_scale + 2 + ER_COMMON_SPAN ) < er_model->md_scfg ) {

                            /* continue enumeration */
                            er_model_set_enum( er_model, er_enum, er_scale + 1, er_view );

                        }

                    }

                }

            }

        }

    }

    le_void_t er_model_set_push( er_model_t * const er_model, le_address_t * const er_addr ) {

        /* parsing variables */
        le_size_t er_parse = er_model->md_tail + er_model->md_size - 1;

        /* congruence variables */
        le_size_t er_cycle = 0;

        /* search address */
        while ( er_parse >= er_model->md_head ) {

            /* compute congruence */
            er_cycle = er_parse % er_model->md_size;

            /* check address matching */
            if ( er_cell_get_match( er_model->md_cell + er_cycle, er_addr ) == _LE_TRUE ) {

                /* update states */
                er_cell_set_flag( er_model->md_cell + er_cycle, ER_CELL_QRY | ER_CELL_DIS );

            /* abort or continue push */
            return; } else { er_parse --; }

        }

        /* compute stack limit */
        er_parse = er_model->md_tail + er_model->md_size;

        /* searching pushable cell */
        while ( er_model->md_head < er_parse ) {

            /* compute congruence */
            er_cycle = er_model->md_head % er_model->md_size;

            /* check state */
            if ( er_cell_get_flag( er_model->md_cell + er_cycle, ER_CELL_QRY ) == 0 ) {

                /* push address */
                er_cell_set_push( er_model->md_cell + er_cycle, er_addr );

                /* update head */
                er_model->md_head ++;

            /* abort or continue push */
            return; } else { er_model->md_head ++; }

        }

    }

    le_void_t er_model_set_query( er_model_t * const er_model ) {

        /* congruence variables */
        le_size_t er_cycle = 0;

        /* serialisation variables */
        le_size_t er_serial = 0;

        /* array variables */
        le_array_t er_array = LE_ARRAY_C;

        /* parsing pushed cells */
        for ( le_size_t er_parse = er_model->md_head; er_parse >= er_model->md_tail; er_parse -- ) {

            /* compute congurence */
            er_cycle = er_parse % er_model->md_size;

            /* update array size */
            le_array_set( & er_array, LE_ARRAY_ADDR );

            /* serialise address */
            er_serial = er_cell_serial( er_model->md_cell + er_cycle, & er_array, er_serial );

        }

        /* write query pack */
        le_array_io_write( & er_array, LE_MODE_QUER, er_model->md_socket );

        /* parsing pushed cell */
        for ( le_size_t er_parse = er_model->md_head; er_parse >= er_model->md_tail; er_parse -- ) {

            /* compute congruence */
            er_cycle = er_parse % er_model->md_size;

            /* update state */
            er_cell_set_clear( er_model->md_cell + er_cycle, ER_CELL_DIS );

            /* read socket-array */
            le_array_io_read( & er_model->md_read, er_model->md_socket );

            /* receive cell content */
            er_cell_io_read( er_model->md_cell + er_cycle, & er_model->md_read );

            /* update state */
            er_cell_set_flag( er_model->md_cell + er_cycle, ER_CELL_QRY | ER_CELL_DIS );

        }

        /* parsing model */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* check states */
            if ( er_cell_get_flag( er_model->md_cell + er_parse, ER_CELL_QRY ) == 0 ) {

                /* update states */
                er_cell_set_clear( er_model->md_cell + er_parse, ER_CELL_DIS );

            } else {

                /* update states */
                er_cell_set_clear( er_model->md_cell + er_parse, ER_CELL_QRY );

            }

        }

        /* update model head and tail */
        er_model->md_tail = ( er_model->md_head %= er_model->md_size );

    }

/*
    source - display methods
 */

    le_void_t er_model_display_cell_( er_model_t_ const * const er_model, er_view_t const * const er_view ) {

        /* translation array variables */
        le_real_t er_tran[3] = { 0.0 };

        /* size variables */
        le_size_t er_size = 0;

        /* edge array variables */
        le_real_t * er_edge = NULL;

        /* view position variables */
        le_real_t er_lon = er_view_get_lon( er_view );
        le_real_t er_lat = er_view_get_lat( er_view );

        /* optimisation variables */
        le_real_t er_cosl = cos( - er_lon * ER_COMMON_D2R );
        le_real_t er_sinl = sin( - er_lon * ER_COMMON_D2R );
        le_real_t er_cosa = cos( + er_lat * ER_COMMON_D2R );
        le_real_t er_sina = sin( + er_lat * ER_COMMON_D2R );

        /* motion management - tilt rotation */
        glRotated( - er_view_get_gam( er_view ), 1.0, 0.0, 0.0 );

        /* motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_view_get_alt( er_view ) + LE_ADDRESS_WGSA );

        /* motion management - azimuth rotation */
        glRotated( + er_view_get_azm( er_view ), 0.0, 0.0, 1.0 );

        /* parsing model cells array */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* check cell drawing state - continue parsing */
            if ( er_cell_get_flag_( er_model->md_cell + er_parse, ER_CELL_DIS_ ) == 0 ) continue;

            /* check cell size - continue parsing */
            if ( ( er_size = er_cell_get_count_( er_model->md_cell + er_parse ) ) == 0 ) continue;

            /* vertex and color pointer assignation */
            glVertexPointer( 3, ER_MODEL_VERTEX, LE_ARRAY_UF3, er_cell_get_pose_( er_model->md_cell + er_parse ) );
            glColorPointer ( 3, ER_MODEL_COLORS, LE_ARRAY_UF3, er_cell_get_data_( er_model->md_cell + er_parse ) );

            /* retrieve cell edge coordinates */
            er_edge = er_cell_get_edge_( er_model->md_cell + er_parse );

            /* cell matrix */
            glPushMatrix(); {

                /* compute cell translation - step */
                er_tran[0] = er_sinl * er_edge[0];
                er_tran[1] = er_cosl * er_edge[2];

                /* compute cell translation */
                er_tran[2] = er_cosa * er_tran[1] + er_sina * er_edge[1] - er_cosa * er_tran[0];
                er_tran[1] = er_sina * er_tran[0] + er_cosa * er_edge[1] - er_sina * er_tran[1];
                er_tran[0] = er_cosl * er_edge[0] + er_sinl * er_edge[2];

                /* motion management - cell translation */
                glTranslated( er_tran[0], er_tran[1], er_tran[2] - LE_ADDRESS_WGSA );

                /* motion management - planimetric rotation */
                glRotated( + er_lat, 1.0, 0.0, 0.0 );
                glRotated( - er_lon, 0.0, 1.0, 0.0 );

                /* display graphical primitives */
                glDrawArrays( GL_POINTS, 0, er_size );

            /* cell matrix */
            } glPopMatrix();

        }

    }

    le_void_t er_model_display_cell( er_model_t const * const er_model, er_view_t const * const er_view ) {

        /* translation array variables */
        le_real_t er_tran[3] = { 0.0 };

        /* size variables */
        le_size_t er_size = 0;

        /* edge array variables */
        le_real_t * er_edge = NULL;

        /* view position variables */
        le_real_t er_lon = er_view_get_lon( er_view );
        le_real_t er_lat = er_view_get_lat( er_view );

        /* optimisation variables */
        le_real_t er_cosl = cos( - er_lon * ER_COMMON_D2R );
        le_real_t er_sinl = sin( - er_lon * ER_COMMON_D2R );
        le_real_t er_cosa = cos( + er_lat * ER_COMMON_D2R );
        le_real_t er_sina = sin( + er_lat * ER_COMMON_D2R );

        /* motion management - tilt rotation */
        glRotated( - er_view_get_gam( er_view ), 1.0, 0.0, 0.0 );

        /* motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_view_get_alt( er_view ) + LE_ADDRESS_WGSA );

        /* motion management - azimuth rotation */
        glRotated( + er_view_get_azm( er_view ), 0.0, 0.0, 1.0 );

        /* parsing model cells array */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* check cell drawing state - continue parsing */
            if ( er_cell_get_flag( er_model->md_cell + er_parse, ER_CELL_DIS ) == 0 ) continue;

            /* check cell size - continue parsing */
            if ( ( er_size = er_cell_get_count( er_model->md_cell + er_parse ) ) == 0 ) continue;

            /* vertex and color pointer assignation */
            glVertexPointer( 3, ER_MODEL_VERTEX, LE_ARRAY_UF3, er_cell_get_pose( er_model->md_cell + er_parse ) );
            glColorPointer ( 3, ER_MODEL_COLORS, LE_ARRAY_UF3, er_cell_get_data( er_model->md_cell + er_parse ) );

            /* retrieve cell edge coordinates */
            er_edge = er_cell_get_edge( er_model->md_cell + er_parse );

            /* cell matrix */
            glPushMatrix(); {

                /* compute cell translation - step */
                er_tran[0] = er_sinl * er_edge[0];
                er_tran[1] = er_cosl * er_edge[2];

                /* compute cell translation */
                er_tran[2] = er_cosa * er_tran[1] + er_sina * er_edge[1] - er_cosa * er_tran[0];
                er_tran[1] = er_sina * er_tran[0] + er_cosa * er_edge[1] - er_sina * er_tran[1];
                er_tran[0] = er_cosl * er_edge[0] + er_sinl * er_edge[2];

                /* motion management - cell translation */
                glTranslated( er_tran[0], er_tran[1], er_tran[2] - LE_ADDRESS_WGSA );

                /* motion management - planimetric rotation */
                glRotated( + er_lat, 1.0, 0.0, 0.0 );
                glRotated( - er_lon, 0.0, 1.0, 0.0 );

                /* display graphical primitives */
                glDrawArrays( GL_POINTS, 0, er_size );

            /* cell matrix */
            } glPopMatrix();

        }

    }

    le_void_t er_model_display_earth( er_view_t const * const er_view ) {

        /* motion management - tilt rotation */
        glRotated( - er_view_get_gam( er_view ), 1.0, 0.0, 0.0 );

        /* motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_view_get_alt( er_view ) + LE_ADDRESS_WGSA );

        /* motion management - azimuth rotation */
        glRotated( + er_view_get_azm( er_view ), 0.0, 0.0, 1.0 );

        /* earth wireframe - centering */
        glTranslated( 0.0, 0.0, - LE_ADDRESS_WGSA );

        /* motion management - planimetric rotation */
        glRotated( + er_view_get_lat( er_view ), 1.0, 0.0, 0.0 );
        glRotated( - er_view_get_lon( er_view ), 0.0, 1.0, 0.0 );

        /* earth wireframe - orientation */
        glRotated( 90.0, 1.0, 0.0, 0.0 );

        /* create quadric */
        GLUquadricObj * er_earth = gluNewQuadric();

        /* configure quadric */
        gluQuadricDrawStyle( er_earth, GLU_LINE );

        /* earth wireframe - color */
        glColor4f( 0.18, 0.22, 0.28, 1.00 );

        /* display quadric */
        gluSphere( er_earth, LE_ADDRESS_WGSA, 360, 180 );

        /* configure quadric */
        gluQuadricDrawStyle( er_earth, GLU_FILL );

        /* delete quadric */
        gluDeleteQuadric( er_earth );

    }

