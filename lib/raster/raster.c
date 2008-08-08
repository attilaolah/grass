
#include <grass/config.h>

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <grass/gis.h>
#include <grass/glocale.h>
#include <grass/raster.h>

#include "driver.h"


extern const struct driver *PNG_Driver(void);
extern const struct driver *PS_Driver(void);
extern const struct driver *HTML_Driver(void);
#ifdef USE_CAIRO
extern const struct driver *Cairo_Driver(void);
#endif

static void init(void)
{
    const char *fenc = getenv("GRASS_ENCODING");
    const char *font = getenv("GRASS_FONT");
    int t = R_screen_top();
    int b = R_screen_bot();
    int l = R_screen_left();
    int r = R_screen_rite();

    R_font(font ? font : "romans");

    if (fenc)
	R_charset(fenc);

    R_set_window(t, b, l, r);
}

int R_open_driver(void)
{
    const char *p = getenv("GRASS_RENDER_IMMEDIATE");
    const struct driver *drv =
	(p && G_strcasecmp(p, "PS") == 0) ? PS_Driver() :
	(p && G_strcasecmp(p, "HTML") == 0) ? HTML_Driver() :
#ifdef USE_CAIRO
	(p && G_strcasecmp(p, "cairo") == 0) ? Cairo_Driver() :
#endif
	PNG_Driver();

    LIB_init(drv, 0, NULL);

    init();

    return 0;
}

void R_flush(void)
{
    COM_Respond();
}

void R_close_driver(void)
{
    char *cmd = getenv("GRASS_NOTIFY");

    COM_Respond();
    COM_Graph_close();

    if (cmd)
	system(cmd);
}

/*!
 * \brief screen left edge
 *
 * Returns the coordinate of the left edge of the screen.
 *
 *  \param void
 *  \return int
 */

int R_screen_left(void)
{
    int l;

    COM_Screen_left(&l);

    return l;
}

/*!
 * \brief screen right edge
 *
 * Returns the coordinate of the right edge of the screen.
 *
 *  \param void
 *  \return int
 */

int R_screen_rite(void)
{
    int r;

    COM_Screen_rite(&r);

    return r;
}

/*!
 * \brief bottom of screen
 *
 * Returns the coordinate of the bottom of the screen.
 *
 *  \param void
 *  \return int
 */

int R_screen_bot(void)
{
    int b;

    COM_Screen_bot(&b);

    return b;
}


/*!
 * \brief top of screen
 *
 * Returns the coordinate of the top of the screen.
 *
 *  \param void
 *  \return int
 */

int R_screen_top(void)
{
    int t;

    COM_Screen_top(&t);

    return t;
}

int R_get_num_colors(void)
{
    int c;

    COM_Number_of_colors(&c);

    return c;
}

/*!
 * \brief select standard color
 *
 * Selects the
 * standard <b>color</b> to be used in subsequent draw commands.  The
 * <b>color</b> value is best retrieved using <i>D_translate_color.</i>
 * See Display_Graphics_Library.
 *
 *  \param index
 *  \return int
 */

void R_standard_color(int index)
{
    COM_Standard_color(index);
}

/*!
 * \brief select color
 *
 * When in
 * float mode (see <i>R_color_table_float</i>), this call selects the color
 * most closely matched to the <b>red, grn</b>, and <b>blue</b> intensities
 * requested. These values must be in the range of 0-255.
 *
 *  \param red
 *  \param grn
 *  \param blue
 *  \return int
 */

void R_RGB_color(unsigned char red, unsigned char grn, unsigned char blu)
{
    COM_Color_RGB(red, grn, blu);
}

/*!
 * \brief change the width of line
 *
 * Changes the <b>width</b> of line to be used in subsequent draw commands.
 *
 *  \param width
 *  \return int
 */

void R_line_width(int width)
{
    COM_Line_width(width);
}

/*!
 * \brief erase screen
 *
 * Erases the entire screen to black.
 *
 *  \param void
 *  \return int
 */

void R_erase(void)
{
    COM_Erase();
}

/*!
 * \brief move current location
 *
 * Move the current location to the absolute screen coordinate <b>x,y.</b>
 * Nothing is drawn on the screen.
 *
 *  \param x
 *  \param y
 *  \return int
 */

void R_move_abs(int x, int y)
{
    COM_Move_abs(x, y);
}

/*!
 * \brief move current location
 *
 * Shift the current screen location by the values in <b>dx</b> and <b>dy</b>:
 \code
 Newx = Oldx + dx;
 Newy = Oldy + dy;
 \endcode
 * Nothing is drawn on the screen.
 *
 *  \param x dx
 *  \param y dy
 *  \return int
 */

void R_move_rel(int x, int y)
{
    COM_Move_rel(x, y);
}

/*!
 * \brief draw line
 *
 * Draw a line using the current color, selected via <i>R_color</i>, from the 
 * current location to the location specified by <b>x,y.</b> The current location
 * is updated to <b>x,y.</b>
 *
 *  \param x
 *  \param y
 *  \return int
 */

void R_cont_abs(int x, int y)
{
    COM_Cont_abs(x, y);
}

/*!
 * \brief draw line
 *
 * Draw a line using the
 * current color, selected via <i>R_color</i>, from the current location to
 * the relative location specified by <b>x</b> and <b>y.</b> The current
 * location is updated:
 \code
 Newx = Oldx + x;
 Newy = Oldy + y;
 \endcode
 *
 *  \param x
 *  \param y
 *  \return int
 */

void R_cont_rel(int x, int y)
{
    COM_Cont_rel(x, y);
}

/*!
 * \brief draw a series of dots
 *
 * Pixels at the <b>num</b> absolute positions in the <b>x</b> and
 * <b>y</b> arrays are turned to the current color. The current location is
 * left updated to the position of the last dot.
 *
 *  \param xarray x
 *  \param yarray y
 *  \param number
 *  \return int
 */

void R_polydots_abs(const int *xarray, const int *yarray, int number)
{
    COM_Polydots_abs(xarray, yarray, number);
}

/*!
 * \brief draw a series of dots
 *
 * Pixels at the <b>number</b> relative positions in the <b>x</b> and
 * <b>y</b> arrays are turned to the current color. The first position is
 * relative to the starting current location; the succeeding positions are then
 * relative to the previous position. The current location is updated to the
 * position of the last dot.
 *
 *  \param xarray x
 *  \param yarray y
 *  \param number
 *  \return int
 */

void R_polydots_rel(const int *xarray, const int *yarray, int number)
{
    COM_Polydots_rel(xarray, yarray, number);
}

/*!
 * \brief draw an open polygon
 *
 * The <b>number</b> absolute positions in the <b>x</b> and <b>y</b>
 * arrays are used to generate a multisegment line (often curved). This line is
 * drawn with the current color. The current location is left updated to the
 * position of the last point.
 * <b>Note.</b> It is not assumed that the line is closed, i.e., no line is
 * drawn from the last point to the first point.
 *
 *  \param xarray x
 *  \param yarray y
 *  \param number
 *  \return int
 */

void R_polyline_abs(const int *xarray, const int *yarray, int number)
{
    COM_Polyline_abs(xarray, yarray, number);
}

/*!
 * \brief draw an open polygon
 *
 * The <b>number</b> relative positions in the <b>x</b> and <b>y</b>
 * arrays are used to generate a multisegment line (often curved). The first
 * position is relative to the starting current location; the succeeding
 * positions are then relative to the previous position. The current location is
 * updated to the position of the last point. This line is drawn with the current
 * color.
 * <b>Note.</b> No line is drawn between the last point and the first point.
 *
 *  \param xarray x
 *  \param yarray y
 *  \param number
 *  \return int
 */

void R_polyline_rel(const int *xarray, const int *yarray, int number)
{
    COM_Polyline_rel(xarray, yarray, number);
}

/*!
 * \brief draw a closed polygon
 *
 * The <b>number</b> absolute positions in the <b>x</b> and <b>y</b> arrays
 * outline a closed polygon which is filled with the current color. The current
 * location is undefined afterwards.
 *
 *  \param xarray x
 *  \param yarray y
 *  \param number
 *  \return int
 */

void R_polygon_abs(const int *xarray, const int *yarray, int number)
{
    COM_Polygon_abs(xarray, yarray, number);
}

/*!
 * \brief draw a closed polygon
 *
 * The <b>number</b> relative positions in the <b>x</b> and <b>y</b>
 * arrays outline a closed polygon which is filled with the current color. The
 * first position is relative to the starting current location; the succeeding
 * positions are then relative to the previous position. The current location is
 * undefined afterwards.
 *
 *  \param xarray x
 *  \param yarray y
 *  \param number
 *  \return int
 */

void R_polygon_rel(const int *xarray, const int *yarray, int number)
{
    COM_Polygon_rel(xarray, yarray, number);
}

/*!
 * \brief fill a box
 *
 * A box is drawn in the current color using the coordinates <b>x1,y1</b> and
 * <b>x2,y2</b> as opposite corners of the box. The current location is undefined
 * afterwards
 *
 *  \param x1
 *  \param y1
 *  \param x2
 *  \param y2
 *  \return int
 */

void R_box_abs(int x1, int y1, int x2, int y2)
{
    COM_Box_abs(x1, y1, x2, y2);
}


/*!
 * \brief fill a box
 *
 * A box is drawn in the current color using the current location as one corner 
 * and the current location plus <b>x</b> and <b>y</b> as the opposite corner 
 * of the box. The current location is undefined afterwards.
 *
 *  \param x
 *  \param y
 *  \return int
 */

void R_box_rel(int x, int y)
{
    COM_Box_rel(x, y);
}

/*!
 * \brief set text size
 *
 * Sets text pixel width and height to <b>width</b> and <b>height.</b>
 *
 *  \param width
 *  \param height
 *  \return int
 */

void R_text_size(int width, int height)
{
    COM_Text_size(width, height);
}

void R_text_rotation(float rotation)
{
    COM_Text_rotation(rotation);
}

/*!
 * \brief set text clipping frame
 *
 * Subsequent calls to <i>R_text</i> will have text strings
 * clipped to the screen frame defined by <b>top, bottom, left, right.</b>
 *
 *  \param t top
 *  \param b bottom
 *  \param l left
 *  \param r right
 *  \return int
 */

void R_set_window(int t, int b, int l, int r)
{
    COM_Set_window(t, b, l, r);
}

/*!
 * \brief write text
 *
 * Writes <b>text</b> in the current color and font, at the current text
 * width and height, starting at the current screen location.
 *
 *  \param sometext
 *  \return int
 */

void R_text(const char *text)
{
    COM_Text(text);
}

/*!
 * \brief get text extents
 *
 * The extent of the area enclosing the <b>text</b>
 * is returned in the integer pointers <b>top, bottom, left</b>, and
 * <b>right.</b> No text is actually drawn. This is useful for capturing the
 * text extent so that the text location can be prepared with proper background
 * or border.
 *
 *  \param sometext
 *  \param t top
 *  \param b bottom
 *  \param l left
 *  \param r right
 *  \return int
 */

void R_get_text_box(const char *text, int *t, int *b, int *l, int *r)
{
    COM_Get_text_box(text, t, b, l, r);
}

/*!
 * \brief choose font
 *
 * Set current font to <b>font name</b>.
 * 
 *  \param name
 *  \return int
 */

void R_font(const char *name)
{
    COM_Font_get(name);
}

void R_charset(const char *name)
{
    COM_Font_init_charset(name);
}

void R_font_list(char ***list, int *count)
{
    COM_Font_list(list, count);
}

void R_font_info(char ***list, int *count)
{
    COM_Font_info(list, count);
}

void R_begin_scaled_raster(int mask, int src[2][2], int dst[2][2])
{
    COM_begin_scaled_raster(mask, src, dst);
}

int R_scaled_raster(int n, int row,
		      const unsigned char *red, const unsigned char *grn,
		      const unsigned char *blu, const unsigned char *nul)
{
    return COM_scaled_raster(n, row, red, grn, blu, nul);
}

void R_end_scaled_raster(void)
{
    COM_end_scaled_raster();
}

void R_bitmap(int ncols, int nrows, int threshold, const unsigned char *buf)
{
    COM_Bitmap(ncols, nrows, threshold, buf);
}
