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

/*
    source - specific function
 */

    le_void_t le_storage( le_unit_t * const le_unit, le_char_t const * const le_root, le_time_t const le_time ) {

        /* scale parser variable */
        le_size_t le_scale = 0;

        /* stream variable */
        le_file_t le_line = NULL;
        le_file_t le_data = NULL;
        le_file_t le_temp = NULL;

        /* string variable */
        le_char_t le_real[_LE_USE_STRING] = { 0 };

        /* string variable */
        le_char_t le_proc[_LE_USE_STRING] = { 0 };

        /* compose temporary path */
        sprintf( ( char * ) le_proc, "%s/%" _LE_TIME_P "/optimise.bin", le_root, le_time );

        /* parsing scales */
        while ( le_scale < _LE_USE_DEPTH ) {

            /* extract and check stream */
            if ( ( le_data = le_unit_get_stream( le_unit, le_scale + 1 ) ) != NULL ) {

                /* extract stream */
                le_line = le_unit_get_stream( le_unit, le_scale );

                /* create temporary stream */
                le_temp = fopen( ( char * ) le_proc, "w+" );

                /* process scale */
                le_storage_move( le_line, le_data, le_temp );

                /* close scale stream */
                fclose( le_data );

                /* swap stream - encapsulation fault */
                le_unit->un_pile[le_scale + 1] = ( le_data = le_temp );
                
                /* compose stream name */
                sprintf( ( char * ) le_real, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_root, le_time, le_scale + 1 );

                /* replace file */
                rename( ( char * ) le_proc, ( char * ) le_real );

            /* abort process */
            } else { return; }

            /* update index */
            le_scale ++;

        }

    }

    le_void_t le_storage_move( le_file_t const le_line, le_file_t const le_data, le_file_t const le_temp ) {

        /* offset variable */
        le_size_t le_offset = 0;

        /* offset variable */
        le_size_t le_tracker = 0;

        /* offset variable */
        le_size_t le_target = 0;

        /* length variable */
        le_size_t le_length = 0;

        /* class variable */
        le_class_t le_class_l = LE_CLASS_C;

        /* class variable */
        le_class_t le_class_m = LE_CLASS_C;

        /* search end of stream */
        fseek( le_line, 0, SEEK_END );

        /* retrieve stream size */
        le_length = ftell( le_line );

        /* parsing line stream */
        while ( le_offset < le_length ) {

            /* read class */
            le_class_io_read( & le_class_l, le_offset, le_line );

            /* parsing digits */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* check offset */
                if ( ( le_target = le_class_get_offset( & le_class_l, le_digit ) ) != _LE_OFFS_NULL ) {

                    /* reset offset */
                    le_class_set_offset( & le_class_l, le_digit, le_tracker );

                    /* import target class */
                    le_class_io_read( & le_class_m, le_target, le_data );

                    /* export target class */
                    le_class_io_write( & le_class_m, le_tracker, le_temp );

                    /* update tracker */
                    le_tracker += LE_CLASS_ARRAY;

                }

            }

            /* write class */
            le_class_io_write( & le_class_l, le_offset, le_line );

            /* update offset */
            le_offset += LE_CLASS_ARRAY;

        }

    }

/*
    source - i/o function
 */

    le_size_t le_storage_io_server( le_char_t * le_root ) {

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
        le_size_t le_scfg = le_storage_io_server( le_path );

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
        if ( le_get_status( le_unit = le_unit_create( le_path, le_time, LE_UNIT_READ, le_scfg, LE_UNIT_BDEF ) ) != LE_ERROR_SUCCESS ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to accoss storage unit\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* process storage unit */
        le_storage( & le_unit, le_path, le_time );

        /* delete unit */
        le_unit_delete( & le_unit );

        /* send message */
        return( EXIT_SUCCESS );

    }

