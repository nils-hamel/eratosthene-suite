/*
 *  eratosthene-suite - client
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

    /*! \file   eratosthene-client-times.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - client - times module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_TIMES__
    # define __ER_CLIENT_TIMES__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

    # include "eratosthene-client-common.h"
    # include "eratosthene-client-font.h"
    # include "eratosthene-client-view.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_TIMES_C           { 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, ER_FONT_ERATOSTHENE, _LE_TRUE }

    /* define string justification */
    # define ER_TIMES_LEFT        ( 0 )
    # define ER_TIMES_RIGHT       ( 1 )
    # define ER_TIMES_CENTER      ( 2 )

    /* define interface string */
    # define ER_TIMES_MODES       { ( le_char_t * ) "< ONLY  ", ( le_char_t * ) "  ONLY >", ( le_char_t * ) "< OR >", ( le_char_t * ) "< AND >", ( le_char_t * ) "< XOR >" }

/*
    header - preprocessor macros
 */

    /* define rounding macro */
    # define er_times_rnd( t, r ) ( ( ( ( t ) / ( r ) ) * ( r ) ) + ( r ) )

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct er_times_struct
     *  \brief Times structure
     *
     *  This structure holds the required information for the display of the
     *  time interface. The time interface is used by the graphical clients to
     *  drive the display of earth model according to time.
     *
     *  Through this interface, the user can move two separated time values to
     *  tell the graphical clients what to display. In addition, the interface
     *  allows the user to change the times comparison mode.
     *
     *  The structure contains fields storing configuration of the interface
     *  buffer displayed over the earth model that are the buffer sizes and
     *  position. The structure also holds the interface buffer bytes array
     *  in which the interface is drawn.
     *
     *  The structure also contains fields storing precomputed vertical and
     *  horizontal positions that are used to display interface texts and to
     *  draw the interface temporal graduation.
     *
     *  \var er_times_struct::tm_width
     *  Width of the time interface buffer, in pixels
     *  \var er_times_struct::tm_height
     *  Height of the time interface buffer, in pixels
     *  \var er_times_struct::tm_length
     *  Size, in bytes, of the time interface buffer
     *  \var er_times_struct::tm_offset
     *  Vertical position of the time interface buffer - screen position
     *  \var er_times_struct::tm_buffer
     *  Time interface buffer bytes
     *  \var er_times_struct::tm_sh1
     *  Time interface texts vertical position - screen position
     *  \var er_times_struct::tm_sh2
     *  Time interface texts vertical position - screen position
     *  \var er_times_struct::tm_sh3
     *  Time interface texts vertical position - screen position
     *  \var er_times_struct::tm_bh1
     *  Time interface graduation vertical position - buffer position
     *  \var er_times_struct::tm_bh2
     *  Time interface graduation vertical position - buffer position
     *  \var er_times_struct::tm_middle
     *  Time interface horizontal middle position - buffer/screen position
     */

    typedef struct er_times_struct {

        le_size_t   tm_width;
        le_size_t   tm_height;
        le_size_t   tm_length;
        le_size_t   tm_offset;

        le_byte_t * tm_buffer;

        le_size_t   tm_sh1;
        le_size_t   tm_sh2;
        le_size_t   tm_bh1;
        le_size_t   tm_bh2;
        le_size_t   tm_middle;

        er_font_t   tm_font;

    le_enum_t _status; } er_times_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods (revoked)
     *
     *  This function creates and returns a times interface structure. It gets
     *  information about the screen sizes and precomputes the vertical and
     *  horizontal positions used to display interface texts and graphical
     *  elements.
     *
     *  The function also allocate the memory of the time interface buffer and
     *  initialise its content. As the elements of the time interface are drawn
     *  through the alpha components of the buffer, the initialisation of the
     *  RGB components made by the function stands during the whole execution.
     *
     *  \return Returns the constructed time interface structure
     */

    er_times_t er_times_create( le_size_t const er_width, le_size_t const er_height );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided time interface structure. It releases
     *  the interface buffer memory allocation and clears the structure fields.
     *
     *  \param er_times Time interface structure
     */

    le_void_t er_times_delete( er_times_t * const er_times );

    /*! \brief display methods (revoked)
     *
     *  This function is responsible of the time interface drawing and display
     *  in the opengl colour buffer. It is also responsible of the display of
     *  the time interface texts.
     *
     *  The function starts by clearing the alpha channel of the time interface
     *  buffer as it only performs rendering through this channel. It then draws
     *  the interface time graduation and graduation date texts. The other texts
     *  of the interface are then drawn : the text giving the time comparison
     *  mode and the current state of the two interface times.
     *
     *  The time interface buffer is then drawn on the colour buffer using the
     *  standard OpenGL routines. As the interface is drawn over the earth model
     *  scene, this function has to be called after earth model rendering.
     *
     *  \param er_times Time interface structure
     *  \param er_view  View structure
     */

    le_void_t er_times_display( er_times_t const * const er_times, er_view_t const * const er_view );

    /*! \brief display methods (revoked)
     *
     *  This function displays the time provided as parameter in the OpenGL
     *  colour buffer.
     *
     *  The function starts by converting the provided date in a text string
     *  that is written in the colour buffer using \b er_times_display_text()
     *  function. The date is converted using the libcommon date conversion
     *  function that follows the format : YYYY-MM-DD-hh-mm-ss.
     *
     *  \param er_time    Time value
     *  \param er_x       Time value text x-position - screen position
     *  \param er_y       Time value text y-position - screen position
     *  \param er_justify Time value text justification
     */

    le_void_t er_times_display_date( er_times_t const * const er_times, le_time_t const er_date, le_size_t const er_x, le_size_t const er_y, le_byte_t const er_value, le_enum_t const er_justify );

    /*! \brief display methods (revoked)
     *
     *  This function writes in the OpenGL colour buffer the text provided as
     *  parameter.
     *
     *  It uses the provided coordinates to set the position of the text. The
     *  justification parameter is used to set text alignment. The three usual
     *  modes are available : \b ER_TIMES_LEFT, \b ER_TIMES_RIGHT and the mode
     *  \b ER_TIMES_CENTER.
     *
     *  \param er_text    Text string
     *  \param er_x       Text x-position - screen position
     *  \param er_y       Text y-position - screen position
     *  \param er_justify Text justification
     */

    le_void_t er_times_display_text( er_times_t const * const er_times, le_char_t const * const er_text, le_size_t er_x, le_size_t er_y, le_byte_t const er_value, le_enum_t const er_justify );

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    header - inclusion guard
 */

    # endif

