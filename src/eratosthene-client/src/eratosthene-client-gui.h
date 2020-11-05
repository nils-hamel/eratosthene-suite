/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@alumni.epfl.ch
 *      Copyright (c) 2016-2020 DHLAB, EPFL
 *      Copyright (c) 2020 Republic and Canton of Geneva
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

    /*! \file   eratosthene-client-gui.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - client - graphical user interface
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_GUI__
    # define __ER_CLIENT_GUI__

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
    # define ER_GUI_C      { 0, 0, 0, 0, 0, NULL, { 0 }, ER_GUI_MODES, ER_GUI_QUERY, ER_GUI_TIME, ER_GUI_SPAN, ER_FONT_ERATOSTHENE, _LE_FALSE }

    /* define string justification */
    # define ER_GUI_LEFT   ( 0 )
    # define ER_GUI_RIGHT  ( 1 )
    # define ER_GUI_CENTER ( 2 )

    /* define interface string */
    # define ER_GUI_MODES  {    \
    ( le_char_t * ) "",         \
    ( le_char_t * ) "",         \
    ( le_char_t * ) "",         \
    ( le_char_t * ) "+",        \
    ( le_char_t * ) "DDH",      \
    ( le_char_t * ) "DDM"       \
    }

    /* define interface string */
    # define ER_GUI_QUERY  {    \
    ( le_char_t * ) "Q:NEAR",   \
    ( le_char_t * ) "Q:DEEP"    \
    }

    /* define interface string */
    # define ER_GUI_TIME   {    \
    ( le_char_t * ) "",         \
    ( le_char_t * ) "T:1",      \
    ( le_char_t * ) "T:2",      \
    ( le_char_t * ) "T:*",      \
    ( le_char_t * ) "T:*",      \
    ( le_char_t * ) "T:*"       \
    }

    /* define interface string */
    # define ER_GUI_SPAN   {    \
    ( le_char_t * ) "S:0",      \
    ( le_char_t * ) "S:1",      \
    ( le_char_t * ) "S:2",      \
    ( le_char_t * ) "S:3",      \
    ( le_char_t * ) "S:4",      \
    ( le_char_t * ) "S:5",      \
    ( le_char_t * ) "S:6",      \
    ( le_char_t * ) "S:7",      \
    ( le_char_t * ) "S:8",      \
    ( le_char_t * ) "S:9"       \
    }

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct er_gui_struct
     *  \brief GUI structure
     *
     *  This structure holds the required element needed to display the GUI
     *  information. The information are displayed through simple text on the
     *  screen on top of the model rendering.
     *
     *  The information include in the first place the position in time of the
     *  current point of view taking into account the query mode. If only the
     *  first or second times are active, their are displayed. In case both
     *  times are active for convolution rendering, they are both displayed
     *  along with the convolution mode.
     *
     *  On a other text line, the information display the index of the active
     *  time (in case only one time is active), the current query additional
     *  depth along with the query temporal mode.
     *
     *  On the last text line, the information display the temporal range of the
     *  current active time(s). This includes the lower and upper boundary times
     *  along with the temporal range expressed in human readable format.
     *
     *  The first five field are related to the information graphical buffer in
     *  which the information are rendered and that is stamped on top of the
     *  model rendering. This includes information about the buffer size, length
     *  and position on the overall render.
     *
     *  The next field is the information rendering buffer itself. It is a byte
     *  array with RGBA format. The alpha channel is used to only render the
     *  information text.
     *
     *  The next five field are used to store text elements that are used to
     *  compose and render the information text. These text elements are the
     *  predictable part of the information texts.
     *
     *  The last field holds the font structure that contains the font pixel
     *  description. The function of the font module are used to render the
     *  texts in the information buffer.
     *
     *  \var er_gui_struct::gu_width
     *  Width of the GUI buffer, in pixels
     *  \var er_gui_struct::gu_height
     *  Height of the GUI buffer, in pixels
     *  \var er_gui_struct::gu_length
     *  Size, in bytes, of the GUI buffer
     *  \var er_gui_struct::gu_left
     *  Horizontal position of the GUI buffer, in pixels
     *  \var er_gui_struct::gu_top
     *  Vertical position of the GUI buffer, in pixels
     *  \var er_gui_struct::gu_buffer
     *  GUI buffer bytes
     *  \var er_gui_struct::gu_text
     *  Information text composition string
     *  \var er_gui_struct::gu_mode
     *  Predefined text for convolution mode display
     *  \var er_gui_struct::gu_query
     *  Predefined text for query mode display
     *  \var er_gui_struct::gu_time
     *  Predefined text for time index display
     *  \var er_gui_struct::gu_span
     *  Predefined text for query additional depth
     *  \var er_gui_struct::gu_font
     *  Font structure
     *  \var er_gui_struct::_status
     *  Standard status field
     */

    typedef struct er_gui_struct {

        le_size_t   gu_width;
        le_size_t   gu_height;
        le_size_t   gu_length;
        le_size_t   gu_left;
        le_size_t   gu_top;

        le_byte_t * gu_buffer;

        le_char_t   gu_text[32];
        le_char_t * gu_mode[6];
        le_char_t * gu_query[2];
        le_char_t * gu_time[6];
        le_char_t * gu_span[10];

        er_font_t   gu_font;

    le_enum_t _status; } er_gui_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a GUI structure. The function prepares
     *  information about the size and position of the GUI information based on
     *  the resolution of the overall render.
     *
     *  The function also initialises the GUI graphical buffer memory in which
     *  the information are rendered. It also initialise the content of the
     *  buffer as the text rendering only occurs through the modification of the
     *  alpha channel.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field. On error, the
     *  \b _status field is returned with value \b _LE_FALSE and \b _LE_TRUE
     *  otherwise.
     *
     *  \param er_width  Overall render width, in pixels
     *  \param er_height Overall render height, in pixels
     *
     *  \return Returns the constructed GUI structure
     */

    er_gui_t er_gui_create( le_size_t const er_width, le_size_t const er_height );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided GUI structures. It releases the GUI
     *  graphical buffer and resets the structure fields using default values.
     *
     *  \param er_gui GUI structure
     */

    le_void_t er_gui_delete( er_gui_t * const er_gui );

    /*! \brief mutator methods
     *
     *  This function resets the value of the alpha channel component of the
     *  provided GUI structure graphical buffer. The component are all reset to
     *  zero.
     *
     *  \param er_gui GUI structure
     */

    le_void_t er_gui_set_buffer( er_gui_t * const er_gui );

    /*! \brief render methods
     *
     *  This function is responsible of rendering the provided GUI information
     *  in its graphical buffer based on the provided viewpoint structure.
     *
     *  The function draws the three text line that compose the information by
     *  reading the content of the provided viewpoint.
     *
     *  The function uses the font provided through the GUI structure along with
     *  the predefined text elements.
     *
     *  As the information rendering ends, the function stamps the GUI graphical
     *  buffer on the overall render using its byte content and the position
     *  information provided by the GUI structure.
     *
     *  \param er_gui  GUI structure
     *  \param er_view View structure
     */

    le_void_t er_gui_display( er_gui_t * const er_gui, er_view_t const * const er_view );

    /*! \brief render methods
     *
     *  This function allows to render in the provided GUI structure graphical
     *  buffer the provided time range in human readable format.
     *
     *  The time value is divided until its best representation, in terms of
     *  seconds, minutes, hours, days and years, is reached.
     *
     *  The text is then rendered using the provided GUI structure font in its
     *  graphical buffer. The position of the text (left and top) are given by
     *  the \b er_x and \b er_y parameter. The alpha value of the text is given
     *  through the \b er_value parameter. The text justification is provided by
     *  the \b er_justify parameter.
     *
     *  \param er_gui     GUI structure
     *  \param er_range   Time range value
     *  \param er_value   Rendering alpha value
     *  \param er_x       Rendering left position, in pixels
     *  \param er_y       Rendering top position, in pixels
     *  \param er_justify Text justification
     */

    le_void_t er_gui_range( er_gui_t * const er_gui, le_time_t er_range, le_byte_t const er_value, le_size_t const er_x, le_size_t const er_y, le_enum_t const er_justify );

    /*! \brief render methods
     *
     *  This function allows to render in the provided GUI structure graphical
     *  buffer the provided date (time) in human readable format.
     *
     *  The function converts the provided date (time) in a date string before
     *  to render it. The date format is : YYYY-MM-DD+hh:mm:ss.
     *
     *  The text is then rendered using the provided GUI structure font in its
     *  graphical buffer. The position of the text (left and top) are given by
     *  the \b er_x and \b er_y parameter. The alpha value of the text is given
     *  through the \b er_value parameter. The text justification is provided by
     *  the \b er_justify parameter.
     *
     *  \param er_gui     GUI structure
     *  \param er_date    Date value
     *  \param er_value   Rendering alpha value
     *  \param er_x       Rendering left position, in pixels
     *  \param er_y       Rendering top position, in pixels
     *  \param er_justify Text justification
     */

    le_void_t er_gui_display_date( er_gui_t * const er_gui, le_time_t const er_date, le_byte_t const er_value, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify );

    /*! \brief render methods
     *
     *  This function allows to render in the provided GUI structure graphical
     *  buffer the provided text string.
     *
     *  The text is then rendered using the provided GUI structure font in its
     *  graphical buffer. The position of the text (left and top) are given by
     *  the \b er_x and \b er_y parameter. The alpha value of the text is given
     *  through the \b er_value parameter. The text justification is provided by
     *  the \b er_justify parameter.
     *
     *  \param er_gui     GUI structure
     *  \param er_text    Text string
     *  \param er_value   Rendering alpha value
     *  \param er_x       Rendering left position, in pixels
     *  \param er_y       Rendering top position, in pixels
     *  \param er_justify Text justification
     */

    le_void_t er_gui_display_text( er_gui_t * const er_gui, le_char_t const * const er_text, le_byte_t const er_value, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify );

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

