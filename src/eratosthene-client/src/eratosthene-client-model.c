/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
 *
 *  Contributors
 *
 *      Ciprian Tomoiaga - ciprian.tomoiaga@gmail.com
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

        /* array size variables */
        le_size_t er_size = er_model.md_size << 1;

        /* create cell arrays */
        if ( ( er_model.md_cell = ( er_cell_t * ) malloc( er_size * sizeof( er_cell_t ) ) ) == NULL ) {

            /* return created structure */
            return( er_model._status = _LE_FALSE, er_model );

        }

        /* initialise arrays cells */
        for ( le_size_t er_parse = 0; er_parse < er_size; er_parse ++ ) {

            /* initialise cell */
            er_model.md_cell[er_parse] = er_cell_create();

        }

        /* assign secondary cache */
        er_model.md_virt = er_model.md_cell + er_model.md_size;

        /* return created structure */
        return( er_model );

    }

    le_void_t er_model_delete( er_model_t * const er_model ) {

        /* deleted structure variables */
        er_model_t er_delete = ER_MODEL_C;

        /* check cell array */
        if ( er_model->md_cell != NULL ) {

            /* update array size */
            er_model->md_size <<= 1;

            /* delete arrays cells */
            for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

                /* delete cell */
                er_cell_delete( er_model->md_cell + er_parse );

            }

            /* release array memory */
            free( er_model->md_cell );

        }

        /* delete structure */
        ( * er_model ) = er_delete;

    }

/*
    source - accessor methods
 */

    le_enum_t er_model_get_drop( er_model_t const * const er_model, le_address_t const * const er_addr ) {

        /* parsing d-cell array */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* check cell state */
            if ( er_cell_get_record( er_model->md_cell + er_parse ) == 0 ) {

                /* analyse address and cell filiation */
                if ( er_cell_get_share( er_model->md_cell + er_parse, er_addr ) == _LE_TRUE ) {

                    /* send message */
                    return( _LE_TRUE );

                }

            }

        }

        /* send message */
        return( _LE_FALSE );

    }

/*
    source - mutator methods
 */

    le_void_t er_model_set_prep( er_model_t * const er_model ) {

        /* reset synchronisation index */
        er_model->md_sync = 0;

        /* reset search index */
        er_model->md_free = 0;

        /* reset target value */
        er_model->md_push = 0;

        /* reset cells synchronisation flag */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* reset v-cell synchronisation flag */
            er_cell_set_zero( er_model->md_cell + er_parse, ER_CELL_SYN );

            /* reset d-cell synchronisation flag */
            er_cell_set_zero( er_model->md_virt + er_parse, ER_CELL_SYN );

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

                        /* check target size */
                        if ( er_model->md_push < er_model->md_size ) {

                            /* filiation-driven cell selection */
                            if ( er_model_get_drop( er_model, er_enum ) == _LE_FALSE ) {

                                /* push address */
                                er_cell_set_push( er_model->md_virt + er_model->md_push, er_enum );

                                /* update push value */
                                er_model->md_push ++;

                            }

                        }

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

    le_void_t er_model_set_fast( er_model_t * const er_model ) {

        /* parsing variables */
        le_size_t er_index = 0;

        /* parsing v-cells array */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_push; er_index = 0, er_parse ++ ) {

            /* parsing d-cells array */
            while ( er_index < er_model->md_size ) {

                /* detect cell identity */
                if ( er_cell_get_equal( er_model->md_virt + er_parse, er_model->md_cell + er_index ) == _LE_TRUE ) {

                    /* update v-cell flag */
                    er_cell_set_flag( er_model->md_virt + er_parse, ER_CELL_SYN );

                    /* update d-cell flag */
                    er_cell_set_flag( er_model->md_cell + er_index, ER_CELL_SYN | ER_CELL_DIS );

                    /* resume search */
                    er_index = er_model->md_size;

                /* update index */
                } else { er_index ++; }

            }

        }

    }

    le_enum_t er_model_set_sync( er_model_t * const er_model ) {

        /* serialisation variables */
        le_size_t er_serial = 0;

        /* check synchronisation state */
        if ( er_model->md_sync >= er_model->md_push ) {

            /* send message */
            return( _LE_TRUE );

        }

        /* update socket-array size */
        le_array_set_size( & er_model->md_iosa, 0 );

        /* push synchronisation index */
        er_model->md_syna = er_model->md_sync;

        /* parsing v-cell array segment */
        while ( ( er_model->md_sync < er_model->md_push ) && ( er_serial < LE_ARRAY_ADDR * ER_COMMON_PACK ) ) {

            /* select unsynchronised v-cell */
            if ( er_cell_get_flag( er_model->md_virt + er_model->md_sync, ER_CELL_SYN ) != ER_CELL_SYN ) {

                /* update socket-array size */
                le_array_set( & er_model->md_iosa, LE_ARRAY_ADDR );

                /* serialise address */
                er_serial = er_cell_serial( er_model->md_virt + er_model->md_sync, & er_model->md_iosa, er_serial );

            }

            /* update synchronisation index */
            er_model->md_sync ++;

        }

        /* check socket-array state */
        if ( er_serial > 0 ) {

            /* write socket-array on socket */
            le_array_io_write( & er_model->md_iosa, LE_MODE_QUER, er_model->md_sock );

            /* push synchronisation boundary */
            er_model->md_synb = er_model->md_sync;

            /* reset synchronisation index */
            er_model->md_sync = er_model->md_syna;

            /* parsing v-cell array segment */
            while ( er_model->md_sync < er_model->md_synb ) {

                /* select unsynchronised v-cell */
                if ( er_cell_get_flag( er_model->md_virt + er_model->md_sync, ER_CELL_SYN ) != ER_CELL_SYN ) {

                    /* search usable d-cell */
                    while ( er_cell_get_flag( er_model->md_cell + er_model->md_free, ER_CELL_SYN ) == ER_CELL_SYN ) {

                        /* update search index */
                        er_model->md_free ++;

                    }

                    /* update d-cell state */
                    er_cell_set_zero( er_model->md_cell + er_model->md_free, ER_CELL_DIS );

                    /* read socket-array */
                    le_array_io_read( & er_model->md_iosa, er_model->md_sock );

                    /* sychronise address cell */
                    er_cell_set_sync( er_model->md_cell + er_model->md_free, er_model->md_virt + er_model->md_sync );

                    /* check socket-array state */
                    if ( le_array_get_size( & er_model->md_iosa ) == 0 ) {

                        /* update d-cell content */
                        er_cell_set_empty( er_model->md_cell + er_model->md_free );

                        /* update d-cell state */
                        er_cell_set_flag( er_model->md_cell + er_model->md_free, ER_CELL_SYN );

                    } else {

                        /* update d-cell content */
                        er_cell_set_data( er_model->md_cell + er_model->md_free, & er_model->md_iosa );

                        /* update d-cell state */
                        er_cell_set_flag( er_model->md_cell + er_model->md_free, ER_CELL_SYN | ER_CELL_DIS );

                    }

                    /* update v-cell state */
                    er_cell_set_flag( er_model->md_virt + er_model->md_sync, ER_CELL_SYN );

                }

                /* update synchronisation index */
                er_model->md_sync ++;

            }

        }

        /* check synchronisation index */
        if ( er_model->md_sync >= er_model->md_push ) {

            /* parsing d-cells array */
            for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

                /* check d-cell state */
                if ( er_cell_get_flag( er_model->md_cell + er_parse, ER_CELL_SYN | ER_CELL_DIS ) == ER_CELL_DIS ) {

                    /* update d-cell flag */
                    er_cell_set_zero( er_model->md_cell + er_parse, ER_CELL_DIS );

                }

            }

        }

        /* send message */
        return( _LE_FALSE );

    }

/*
    source - display methods
 */

    le_void_t er_model_display_cell( er_model_t const * const er_model, er_view_t const * const er_view ) {

        /* view position variables */
        le_real_t er_lon = er_view_get_lon( er_view );
        le_real_t er_lat = er_view_get_lat( er_view );

        /* cell edge variables */
        le_real_t * er_edge = NULL;

        /* optimisation variables */
        le_real_t er_cosl = cos( - er_lon * ER_COMMON_D2R );
        le_real_t er_sinl = sin( - er_lon * ER_COMMON_D2R );
        le_real_t er_cosa = cos( + er_lat * ER_COMMON_D2R );
        le_real_t er_sina = sin( + er_lat * ER_COMMON_D2R );

        /* motion management - tilt rotation */
        glRotated( - er_view_get_gam( er_view ), 1.0, 0.0, 0.0 );

        /* motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_view_get_alt( er_view ) + LE_ADDRESS_WGS_A );

        /* motion management - azimuth rotation */
        glRotated( + er_view_get_azm( er_view ), 0.0, 0.0, 1.0 );

        /* parsing d-cells array */
        for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* check d-cell flag */
            if ( er_cell_get_flag( er_model->md_cell + er_parse, ER_CELL_DIS ) == ER_CELL_DIS ) {

                /* assign d-cell vertex and color pointers */
                glVertexPointer( 3, ER_MODEL_VERTEX, LE_ARRAY_UF3, er_cell_get_pose( er_model->md_cell + er_parse ) );
                glColorPointer ( 3, ER_MODEL_COLORS, LE_ARRAY_UF3, er_cell_get_data( er_model->md_cell + er_parse ) );

                /* retrieve d-cell edge array */
                er_edge = er_cell_get_edge( er_model->md_cell + er_parse );

                /* d-cell matrix */
                glPushMatrix();

                    /* compute cell translation */
                    er_edge[3] = er_sinl * er_edge[0];
                    er_edge[4] = er_cosl * er_edge[2];
                    er_edge[5] = er_cosa * er_edge[4] + er_sina * er_edge[1] - er_cosa * er_edge[3];
                    er_edge[4] = er_sina * er_edge[3] + er_cosa * er_edge[1] - er_sina * er_edge[4];
                    er_edge[3] = er_cosl * er_edge[0] + er_sinl * er_edge[2];

                    /* motion management - cell translation */
                    glTranslated( er_edge[3], er_edge[4], er_edge[5] - LE_ADDRESS_WGS_A );

                    /* motion management - planimetric rotation */
                    glRotated( + er_lat, 1.0, 0.0, 0.0 );
                    glRotated( - er_lon, 0.0, 1.0, 0.0 );

                    /* display graphical primitives */
                    glDrawArrays( GL_POINTS, 0, er_cell_get_record( er_model->md_cell + er_parse ) );

                /* d-cell matrix */
                glPopMatrix();

            }

        }

    }

    le_void_t er_model_display_earth( er_view_t const * const er_view ) {

        /* motion management - tilt rotation */
        glRotated( - er_view_get_gam( er_view ), 1.0, 0.0, 0.0 );

        /* motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_view_get_alt( er_view ) + LE_ADDRESS_WGS_A );

        /* motion management - azimuth rotation */
        glRotated( + er_view_get_azm( er_view ), 0.0, 0.0, 1.0 );

        /* earth wireframe - centering */
        glTranslated( 0.0, 0.0, - LE_ADDRESS_WGS_A );

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
        gluSphere( er_earth, LE_ADDRESS_WGS_A, 360, 180 );

        /* configure quadric */
        gluQuadricDrawStyle( er_earth, GLU_FILL );

        /* delete quadric */
        gluDeleteQuadric( er_earth );

    }

