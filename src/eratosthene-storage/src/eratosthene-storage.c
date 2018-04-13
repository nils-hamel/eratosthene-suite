/*
 *  eratosthene-suite - storage
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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

    # include "eratosthene-storage.h"

    /* temporary */
    le_size_t le_motion_counter = 0;

/*
    source - specific function
 */

    le_void_t le_storage( le_unit_t * const le_unit ) {

        /* parsing variable */
        le_size_t le_parse = 0;

        /* stream variable */
        le_file_t le_root = NULL;
        le_file_t le_work = NULL;

        /* offset variable */
        le_size_t le_offset = 0;

        /* offset variable */
        le_size_t le_enumerate = 0;

        /* locker variable */
        le_size_t le_locker = 0;

        /* stream size variable */
        le_size_t le_length = 0;

        /* flag variable */
        le_enum_t le_flag = _LE_FALSE;

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* parsing scales */
        while ( le_parse < _LE_USE_DEPTH ) {

            /* extract scale stream */
            le_root = le_unit_get_stream( le_unit, le_parse );

            /* extract and check scale stream */
            if ( ( le_work = le_unit_get_stream( le_unit, le_parse + 1 ) ) != NULL ) {

                /* temporary */ fprintf( stderr, "Process on scale %" _LE_SIZE_P ", current moves %" _LE_SIZE_P "\n", le_parse, le_motion_counter );

                /* reset offset */
                le_offset = 0;

                /* reset locker */
                le_locker = 0;

                /* search stream end */
                fseek( le_root, 0, SEEK_END );

                /* retrieve stream size */
                le_length = ftell( le_root );

                /* parsing stream classes */
                while ( le_offset < le_length ) {

                    /* read class */
                    le_class_io_read( & le_class, le_offset, le_root );

                    /* reset modification flag */
                    le_flag = _LE_FALSE;

                    /* parsing offsets */
                    for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                        /* extract offset */
                        if ( ( le_enumerate = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                            /* motion condition */
                            if ( le_enumerate != le_locker ) {

                                /* update class offset */
                                le_class_set_offset( & le_class, le_digit, le_locker );

                                /* set modification flag */
                                le_flag = _LE_TRUE;

                                /* replace near reference */
                                if ( le_storage_self( & le_class, le_digit, le_locker, le_enumerate ) == _LE_FALSE ) {

                                    /* replace distant reference */
                                    le_storage_move( le_root, le_offset, le_locker, le_enumerate, le_length );

                                }

                                /* swap classes */
                                le_storage_swap( le_work, le_enumerate, le_locker );

                            }

                            /* update locker */
                            le_locker += LE_CLASS_ARRAY;

                        }

                    }

                    /* check modification flag */
                    if ( le_flag == _LE_TRUE ) {

                        /* export modified class */
                        le_class_io_write( & le_class, le_offset, le_root );

                    }

                    /* update offset */
                    le_offset += LE_CLASS_ARRAY;

                }
                
                /* update index */
                le_parse ++;

            /* interrupt process */
            } else { le_parse = _LE_USE_DEPTH; }

        }
        

    }

    le_enum_t le_storage_self( le_class_t * const le_class, le_size_t const le_index, le_size_t const le_locker, le_size_t const le_replace ) {

        /* parsing digit */
        for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

            /* check index */
            if ( le_digit != le_index ) {

                /* compare reference */
                if ( le_class_get_offset( le_class, le_digit ) == le_locker ) {

                    /* swap reference */
                    le_class_set_offset( le_class, le_digit, le_replace );

                    /* send message */
                    return( _LE_TRUE );

                }

            }

        }

        /* send message */
        return( _LE_FALSE );

    }

    le_void_t le_storage_move( le_file_t const le_root, le_size_t le_offset, le_size_t const le_locker, le_size_t const le_replace, le_size_t const le_length ) {

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* parsing classes */
        while ( ( le_offset += LE_CLASS_ARRAY ) < le_length ) {

            /* read class */
            le_class_io_read( & le_class, le_offset, le_root );

            /* parsing digit */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* compare offset */
                if ( le_class_get_offset( & le_class, le_digit ) == le_locker ) {

                    /* set class offset */
                    le_class_set_offset( & le_class, le_digit, le_replace );

                    /* write class */
                    le_class_io_write( & le_class, le_offset, le_root );

                    /* abort process */
                    return;

                }

            }

        }

    }

    le_void_t le_storage_swap( le_file_t const le_work, le_size_t const le_offa, le_size_t const le_offb ) {

        /* swap variable */
        le_class_t le_ca = LE_CLASS_C;
        le_class_t le_cb = LE_CLASS_C;

        /* read class */
        le_class_io_read( & le_ca, le_offa, le_work );

        /* read class */
        le_class_io_read( & le_cb, le_offb, le_work );

        /* write class */
        le_class_io_write( & le_ca, le_offb, le_work );

        /* write class */
        le_class_io_write( & le_cb, le_offa, le_work );

        /* temporary */ le_motion_counter ++;

    }

/*
    source - i/o function
 */

    le_size_t le_storage_io_space( le_char_t * le_root ) {

        /* string variable */
        le_char_t le_path[_LE_USE_STRING] = { 0 };

        /* stream variable */
        le_file_t le_stream = NULL;

        /* returned value variable */
        le_size_t le_return = _LE_SIZE_NULL;

        /* compose configuration path */
        sprintf( ( char * ) le_path, "%s/system", le_root );

        /* create stream */
        if ( ( le_stream = fopen( ( char * ) le_path, "r" ) ) == NULL ) {

            /* send message */
            return( _LE_SIZE_NULL );

        }

        /* read server configuration */
        if ( fscanf( le_stream, "%" _LE_SIZE_S, & le_return ) != 1 ) {

            /* delete stream */
            fclose( le_stream );

            /* send message */
            return( _LE_SIZE_NULL );

        }

        /* delete stream */
        fclose( le_stream );

        /* return spatial parameter */
        return( le_return );

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* path variable */
        le_char_t * le_path = ( le_char_t * ) lc_read_string( argc, argv, "--path", "-p" );

        /* parameter variable */
        le_size_t le_scfg = le_storage_io_space( le_path );

        /* time variable */
        le_time_t le_time = ( le_time_t ) lc_read_signed( argc, argv, "--time", "-t", _LE_TIME_NULL );

        /* unit structure variable */
        le_unit_t le_unit = LE_UNIT_C;

        /* check spatial parameter */
        if ( le_scfg == _LE_SIZE_NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to access server configuration\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* check unit creation */
        if ( le_get_status( le_unit = le_unit_create( le_path, le_time, LE_UNIT_READ, le_scfg ) ) != LE_ERROR_SUCCESS ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to accoss storage unit\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* process storage unit */
        le_storage( & le_unit );

        /* temporary */ fprintf( stderr, "# move : %" _LE_SIZE_P "\n", le_motion_counter );

        /* delete unit */
        le_unit_delete( & le_unit );

        /* send message */
        return( EXIT_SUCCESS );

    }

