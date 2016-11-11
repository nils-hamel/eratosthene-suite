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

    # include "eratosthene-address.h"

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        if ( lc_read_flag( argc, argv, "--time", "-t" ) == LC_TRUE ) {

            /* specialised method */
            er_address_time( argc, argv );

            /* send message */
            return( EXIT_SUCCESS );

        } else if ( lc_read_flag( argc, argv, "--space", "-s" ) == LC_TRUE ) {

            /* specialised method */
            er_address_space( argc, argv );

            /* send message */
            return( EXIT_SUCCESS );

        } else {

            /* send message */
            return( EXIT_FAILURE );

        }

    }

/*
    source - space conversion methods
 */

    le_void_t er_address_space( int argc, char ** argv ) {

        /* geodetic array variables */
        le_real_t er_pose[3] = { 0.0 };

        /* address structure variables */
        le_address_t er_addr = LE_ADDRESS_C_SIZE( lc_read_unsigned( argc, argv, "--scale", "-s", LE_GEODESY_ASYA ) );

        /* read position parameters */
        er_pose[0] = lc_read_double( argc, argv, "--longitude", "-l", 0.0 );
        er_pose[1] = lc_read_double( argc, argv, "--latitude" , "-a", 0.0 );
        er_pose[2] = lc_read_double( argc, argv, "--altitude" , "-h", 0.0 );

        /* display original coordinates */
        fprintf( stdout, ER_ADDRESS_PFS, er_pose[0], er_pose[1], er_pose[2] );

        /* convert to radians */
        er_pose[0] *= ER_ADDRESS_D2R;
        er_pose[1] *= ER_ADDRESS_D2R;

        /* convert position into index */
        le_address_set_pose( & er_addr, er_pose );

        /* convert index into position */
        le_address_get_pose( & er_addr, er_pose );

        /* convert to degrees */
        er_pose[0] *= ER_ADDRESS_R2D;
        er_pose[1] *= ER_ADDRESS_R2D;

        /* display reconverted coordinates */
        fprintf( stdout, ER_ADDRESS_PFS, er_pose[0], er_pose[1], er_pose[2] );

        /* display address */
        for ( le_size_t er_parse = 0; er_parse < le_address_get_size( & er_addr ); er_parse ++ ) {

            /* display address digit */
            fprintf( stdout, "%c", le_address_get_digit( & er_addr, er_parse ) + 48 ); 

        /* output format */
        } fprintf( stdout, ", %" _LE_SIZE_P "\n", le_address_get_size( & er_addr ) );

    }

/*
    source - time conversion methods
 */

    le_void_t er_address_time( int argc, char ** argv ) {

        /* timestamps variables */
        le_time_t er_time = _LE_TIME_NULL;

        /* date string variables */
        le_char_t * er_date = ( le_char_t * ) lc_read_string( argc, argv, "--date", "-d" );

        /* check consistency */
        if ( er_date == NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : date not specified" );

            /* abort process */
            return;

        }

        /* convert date to timestamps */
        er_time = ( le_time_t ) lc_time_from_string( er_date, ( unsigned char * ) "%Y-%m-%d-%H-%M-%S" );

        /* display result */
        fprintf( stdout, "%s\n%" _LE_TIME_P "\n", er_date, er_time );

    }

