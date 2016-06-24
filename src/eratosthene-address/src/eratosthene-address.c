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

        /* Geodetic array variables */
        le_real_t er_pose[3] = { 0.0 };

        /* Address structure variables */
        le_address_t er_addr = LE_ADDRESS_C_SIZE( lc_read_uint( argc, argv, "--scale", "-s", LE_GEODESY_ASYA ) );

        /* Read position parameters */
        er_pose[0] = lc_read_double( argc, argv, "--longitude", "-l", 0.0 );
        er_pose[1] = lc_read_double( argc, argv, "--latitude" , "-a", 0.0 );
        er_pose[2] = lc_read_double( argc, argv, "--altitude" , "-h", 0.0 );

        /* Display original coordinates */
        fprintf( stdout, ER_ADDRESS_PFS, er_pose[0], er_pose[1], er_pose[2] );

        /* Convert to radians */
        er_pose[0] *= ER_ADDRESS_D2R;
        er_pose[1] *= ER_ADDRESS_D2R;

        /* Convert position into index */
        le_address_set_pose( & er_addr, er_pose );

        /* Convert index into position */
        le_address_get_pose( & er_addr, er_pose );

        /* Convert to degrees */
        er_pose[0] *= ER_ADDRESS_R2D;
        er_pose[1] *= ER_ADDRESS_R2D;

        /* Display reconverted coordinates */
        fprintf( stdout, ER_ADDRESS_PFS, er_pose[0], er_pose[1], er_pose[2] );

        /* Display address */
        for ( le_size_t er_parse = 0; er_parse < le_address_get_size( & er_addr ); er_parse ++ ) {

            /* Display address digit */
            fprintf( stdout, "%c", le_address_get_digit( & er_addr, er_parse ) + 48 ); 

        /* Output format */
        } fprintf( stdout, ", %" _LE_SIZE_P "\n", le_address_get_size( & er_addr ) );

        /* Return to system */
        return( EXIT_SUCCESS );

    }

