#include "st7735.h"

#define ST7735_MADCTL_MY  				0x80
#define ST7735_MADCTL_MX  				0x40
#define ST7735_MADCTL_MV  				0x20
#define ST7735_MADCTL_ML  				0x10
#define ST7735_MADCTL_RGB 				0x00
#define ST7735_MADCTL_BGR 				0x08
#define ST7735_MADCTL_MH  				0x04

#define ST7735_XSTART 					2
#define ST7735_YSTART 					3

#define ST7735_NOP     					0x00
#define ST7735_SWRESET 					0x01
#define ST7735_RDDID   					0x04
#define ST7735_RDDST   					0x09

#define ST7735_SLPIN   					0x10
#define ST7735_SLPOUT  					0x11
#define ST7735_PTLON   					0x12
#define ST7735_NORON   					0x13

#define ST7735_INVOFF  					0x20
#define ST7735_INVON   					0x21
#define ST7735_DISPOFF 					0x28
#define ST7735_DISPON  					0x29
#define ST7735_SET_COLUMN_ADDR   		0x2A
#define ST7735_SET_ROW_ADDR   			0x2B
#define ST7735_RAMWR   					0x2C
#define ST7735_RAMRD   					0x2E

#define ST7735_PTLAR   					0x30
#define ST7735_COLMOD  					0x3A
#define ST7735_MADCTL  					0x36

#define ST7735_FRMCTR1 					0xB1
#define ST7735_FRMCTR2 					0xB2
#define ST7735_FRMCTR3 					0xB3
#define ST7735_INVCTR  					0xB4
#define ST7735_DISSET5 					0xB6

#define ST7735_PWCTR1  					0xC0
#define ST7735_PWCTR2  					0xC1
#define ST7735_PWCTR3  					0xC2
#define ST7735_PWCTR4  					0xC3
#define ST7735_PWCTR5  					0xC4
#define ST7735_VMCTR1  					0xC5

#define ST7735_RDID1   					0xDA
#define ST7735_RDID2   					0xDB
#define ST7735_RDID3   					0xDC
#define ST7735_RDID4   					0xDD

#define ST7735_PWCTR6  					0xFC

#define ST7735_GMCTRP1 					0xE0
#define ST7735_GMCTRN1 					0xE1

#define ST7735_ROTATION 				(ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR)
#define ST7735_COLOR565(r, g, b) 		(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

static uint8_t init_cmds1[] =
{
	/*!< 15 commands in list */
	15,

	/*!< Software reset: 150 ms delay */
	ST7735_SWRESET,
	0x80,
	150,

	/*!< Out of sleep mode: 500 ms delay */
	ST7735_SLPOUT,
	0x80,
	255,

	/*!< Frame rate control: normal mode */
	ST7735_FRMCTR1,
	3,
	0x01,
	0x2C,
	0x2D,

	/*!< Frame rate control: idle mode */
	ST7735_FRMCTR2,
	3,
	0x01,
	0x2C,
	0x2D,

	/*!< Frame rate control: partial mode */
	ST7735_FRMCTR3,
	6,
	0x01,
	0x2C,
	0x2D,
	0x01,
	0x2C,
	0x2D,

	/*!< Display inverse control: no inverse */
	ST7735_INVCTR,
	1,
	0x07,

	/*!< Power control: -4.6V, mode auto */
	ST7735_PWCTR1,
	3,
	0xA2,
	0x02,
	0x84,

	/*!< Power control: VGH25 = 2.4C, VGSEL = -10, VGH = 3 * AVDD */
	ST7735_PWCTR2,
	1,
	0xC5,

	/*!< Power control: Opamp current small, boost frequency */
	ST7735_PWCTR3,
	2,
	0x0A,
	0x00,

	/*!< Power control: BCLK/2, Opamp current small & Medium low */
	ST7735_PWCTR4,
	2,
	0x8A,
	0x2A,

	/*!< Power control */
	ST7735_PWCTR5,
	2,
	0x8A,
	0xEE,

	/*!< Power control */
	ST7735_VMCTR1,
	1,
	0x0E,

	/*!< No display inverse */
	ST7735_INVOFF,
	0,

	/*!< Memori access control: row_addr/col_addr, bottom to top refresh */
	ST7735_MADCTL,
	1,
	ST7735_ROTATION,

	/*!< Column mode: 16bit color */
	ST7735_COLMOD,
	1,
	0x05
};

uint8_t init_cmds2[] =
{
	/*!<  2 commands in list */
	2,

	/*!< Set column address: START = 0, END = 127 */
	ST7735_SET_COLUMN_ADDR,
	4,
	0x00,
	0x00,
	0x00,
	0x7F,

	/*!< Set row address: START = 0, END = 127 */
	ST7735_SET_ROW_ADDR,
	4,
	0x00,
	0x00,
	0x00,
	0x7F
};

uint8_t init_cmds3[] =
{
	/*!<  4 commands in list */
	4,

	/*!<  Magical unicorn dust */
	ST7735_GMCTRP1,
	16,
	0x02,
	0x1c,
	0x07,
	0x12,
	0x37,
	0x32,
	0x29,
	0x2d,
	0x29,
	0x25,
	0x2B,
	0x39,
	0x00,
	0x01,
	0x03,
	0x10,

	/*!<  Sparkles and rainbows */
	ST7735_GMCTRN1,
	16,
	0x03,
	0x1d,
	0x07,
	0x06,
	0x2E,
	0x2C,
	0x29,
	0x2D,
	0x2E,
	0x2E,
	0x37,
	0x3F,
	0x00,
	0x00,
	0x02,
	0x10,

	/*!<  Normal display on; 10 ms delay */
	ST7735_NORON,
	0x80,
	10,

	/*!<  Main screen turn on; 100 ms delay */
	ST7735_DISPON,
	0x80,
	100
};

typedef struct st7735 {
	uint16_t  				width;			/*!< Screen width */
	uint16_t 				height;			/*!< Screen height */
	st7735_func_set_gpio 	set_cs;			/*!< Function set CS. Used in SPI mode */
	st7735_func_set_gpio 	set_dc;			/*!< Function set DC. Used in SPI mode */
	st7735_func_set_gpio 	set_rst;		/*!< Function set RST. Used in SPI mode */
	st7735_func_spi_send 	spi_send;		/*!< Function send SPI data */
	st7735_func_delay 		delay; 			/*!< Function delay */
	uint16_t 				pos_x;			/*!< Position x */
	uint16_t 				pos_y;			/*!< Position y */
} st7735_t;

static err_code_t st7735_write_cmd(st7735_handle_t handle, uint8_t cmd)
{
	handle->set_dc(0);
	handle->spi_send(&cmd, 1);

	return ERR_CODE_SUCCESS;
}

static err_code_t st7735_write_data(st7735_handle_t handle, uint8_t *data, uint16_t len)
{
	handle->set_dc(1);
	handle->spi_send(data, len);

	return ERR_CODE_SUCCESS;
}

static err_code_t st7735_write_list_cmd(st7735_handle_t handle, uint8_t *list_cmd)
{
	uint8_t num_cmd, num_arg;
	uint16_t delay_ms;

	num_cmd = *list_cmd++;
	while (num_cmd--)
	{
		uint8_t cmd = *list_cmd++;
		if (st7735_write_cmd(handle, cmd))
		{
			return ERR_CODE_FAIL;
		}

		num_arg = *list_cmd++;
		// If high bit set, delay follows args
		delay_ms = num_arg & 0x80;
		num_arg &= ~0x80;
		if (num_arg)
		{
			if (st7735_write_data(handle, list_cmd, num_arg))
			{
				return ERR_CODE_FAIL;
			}
			list_cmd += num_arg;
		}

		if (delay_ms)
		{
			delay_ms = *list_cmd++;
			if (delay_ms == 255) delay_ms = 500;
			handle->delay(delay_ms);
		}
	}

	return ERR_CODE_SUCCESS;
}

static err_code_t st7735_reset(st7735_handle_t handle)
{
	if (handle->set_rst != NULL)
	{
		handle->set_rst(0);
		handle->delay(5);
		handle->set_rst(1);
	}

	return ERR_CODE_SUCCESS;
}

static err_code_t set_addr(st7735_handle_t handle, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	uint8_t data[4];

	data[0] = 0x00;
	data[1] = x0 + ST7735_XSTART;
	data[2] = 0x00;
	data[3] = x1 + ST7735_XSTART;

	st7735_write_cmd(handle, ST7735_SET_COLUMN_ADDR);
	st7735_write_data(handle, data, 4);

	data[1] = y0 + ST7735_YSTART;
	data[3] = y1 + ST7735_YSTART;
	st7735_write_cmd(handle, ST7735_SET_ROW_ADDR);
	st7735_write_data(handle, data, 4);

	st7735_write_cmd(handle, ST7735_RAMWR);

	return ERR_CODE_SUCCESS;
}

static err_code_t draw_pixel(st7735_handle_t handle, uint8_t x, uint8_t y, uint16_t color)
{
	set_addr(handle, x, y, x + 1, y + 1);

	uint8_t data[2] = { color >> 8, color & 0xFF };
	st7735_write_data(handle, data, 2);

	return ERR_CODE_SUCCESS;

}

static err_code_t draw_line(st7735_handle_t handle, uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, uint16_t color)
{
	int32_t deltaX = abs(x_end - x_start);
	int32_t deltaY = abs(y_end - y_start);
	int32_t signX = ((x_start < x_end) ? 1 : -1);
	int32_t signY = ((y_start < y_end) ? 1 : -1);
	int32_t error = deltaX - deltaY;
	int32_t error2;

	draw_pixel(handle, x_end, y_end, color);

	while ((x_start != x_end) || (y_start != y_end))
	{
		draw_pixel(handle, x_start, y_start, color);

		error2 = error * 2;
		if (error2 > -deltaY) {
			error -= deltaY;
			x_start += signX;
		} else {
			/*nothing to do*/
		}

		if (error2 < deltaX) {
			error += deltaX;
			y_start += signY;
		} else {
			/*nothing to do*/
		}
	}

	return ERR_CODE_SUCCESS;
}

st7735_handle_t st7735_init(void)
{
	st7735_handle_t handle = calloc(1, sizeof(st7735_t));
	if (handle == NULL)
	{
		return NULL;
	}

	return handle;
}

err_code_t st7735_set_config(st7735_handle_t handle, st7735_cfg_t config)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	handle->width = config.width;
	handle->height = config.height;
	handle->set_cs = config.set_cs;
	handle->set_dc = config.set_dc;
	handle->set_rst = config.set_rst;
	handle->spi_send = config.spi_send;
	handle->delay = config.delay;
	handle->pos_x = 0;
	handle->pos_y = 0;

	return ERR_CODE_SUCCESS;
}

err_code_t st7735_config(st7735_handle_t handle)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	if (handle->set_cs != NULL)
	{
		handle->set_cs(0);
	}

	st7735_reset(handle);

	st7735_write_list_cmd(handle, init_cmds1);
	st7735_write_list_cmd(handle, init_cmds2);
	st7735_write_list_cmd(handle, init_cmds3);

	if (handle->set_cs != NULL)
	{
		handle->set_cs(1);
	}

	return ERR_CODE_SUCCESS;
}

err_code_t st7735_fill(st7735_handle_t handle, uint16_t color)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	if (handle->set_cs != NULL)
	{
		handle->set_cs(0);
	}

	set_addr(handle, 0, 0, handle->width - 1, handle->height - 1);

	uint8_t data[2] = { color >> 8, color & 0xFF };
	for (uint8_t heigt_idx = 0; heigt_idx < handle->height; heigt_idx++)
	{
		for (uint8_t width_idx = 0; width_idx < handle->width; width_idx++)
		{
			st7735_write_data(handle, data, 2);
		}
	}

	if (handle->set_cs != NULL)
	{
		handle->set_cs(1);
	}

	return ERR_CODE_SUCCESS;
}

err_code_t st7735_write_char(st7735_handle_t handle, font_size_t font_size, uint8_t chr, uint16_t color)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	if (handle->set_cs != NULL)
	{
		handle->set_cs(0);
	}

	font_t font;
	get_font(chr, font_size, &font);

	uint8_t num_byte_per_row = font.data_len / font.height;
	for (uint8_t height_idx = 0; height_idx < font.height; height_idx ++)
	{
		for ( uint8_t byte_idx = 0; byte_idx < num_byte_per_row; byte_idx++)
		{
			for (uint8_t width_idx = 0; width_idx < 8; width_idx++)
			{
				uint8_t x = handle->pos_x + width_idx + byte_idx * 8;
				uint8_t y = handle->pos_y + height_idx;
				if (((font.data[height_idx * num_byte_per_row + byte_idx] << width_idx) & 0x80) == 0x80)
				{
					draw_pixel(handle, x, y, color);
				}
			}
		}
	}

	handle->pos_x += font.width + num_byte_per_row;

	if (handle->set_cs != NULL)
	{
		handle->set_cs(1);
	}

	return ERR_CODE_SUCCESS;
}

err_code_t st7735_write_string(st7735_handle_t handle, font_size_t font_size, uint8_t *str, uint16_t color)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	if (handle->set_cs != NULL)
	{
		handle->set_cs(0);
	}

	while (*str)
	{
		font_t font;
		get_font(*str, font_size, &font);

		uint8_t num_byte_per_row = font.data_len / font.height;
		for (uint8_t height_idx = 0; height_idx < font.height; height_idx ++)
		{
			for ( uint8_t byte_idx = 0; byte_idx < num_byte_per_row; byte_idx++)
			{
				for (uint8_t width_idx = 0; width_idx < 8; width_idx++)
				{
					uint8_t x = handle->pos_x + width_idx + byte_idx * 8;
					uint8_t y = handle->pos_y + height_idx;
					if (((font.data[height_idx * num_byte_per_row + byte_idx] << width_idx) & 0x80) == 0x80)
					{
						draw_pixel(handle, x, y, color);
					}
				}
			}
		}
		handle->pos_x += font.width + num_byte_per_row;
		str++;
	}

	if (handle->set_cs != NULL)
	{
		handle->set_cs(1);
	}

	return ERR_CODE_SUCCESS;
}

err_code_t st7735_draw_pixel(st7735_handle_t handle, uint8_t x, uint8_t y, uint16_t color)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	if (handle->set_cs != NULL)
	{
		handle->set_cs(0);
	}

	draw_pixel(handle, x, y, color);

	if (handle->set_cs != NULL)
	{
		handle->set_cs(1);
	}

	return ERR_CODE_SUCCESS;
}

err_code_t st7735_draw_line(st7735_handle_t handle, uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, uint16_t color)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	if (handle->set_cs != NULL)
	{
		handle->set_cs(0);
	}

	draw_line(handle, x_start, y_start, x_end, y_end, color);

	if (handle->set_cs != NULL)
	{
		handle->set_cs(1);
	}

	return ERR_CODE_SUCCESS;
}

err_code_t st7735_draw_rectangle(st7735_handle_t handle, uint8_t x_origin, uint8_t y_origin, uint8_t width, uint8_t height, uint16_t color)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	if (handle->set_cs != NULL)
	{
		handle->set_cs(0);
	}

	draw_line(handle, x_origin, y_origin, x_origin + width, y_origin, color);
	draw_line(handle, x_origin + width, y_origin, x_origin + width, y_origin + height, color);
	draw_line(handle, x_origin + width, y_origin + height, x_origin, y_origin + height, color);
	draw_line(handle, x_origin, y_origin + height, x_origin, y_origin, color);

	if (handle->set_cs != NULL)
	{
		handle->set_cs(1);
	}

	return ERR_CODE_SUCCESS;
}

err_code_t st7735_draw_circle(st7735_handle_t handle, uint8_t x_origin, uint8_t y_origin, uint8_t radius, uint16_t color)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	if (handle->set_cs != NULL)
	{
		handle->set_cs(0);
	}

	int32_t x = -radius;
	int32_t y = 0;
	int32_t err = 2 - 2 * radius;
	int32_t e2;

	do {
		draw_pixel(handle, x_origin - x, y_origin + y, color);
		draw_pixel(handle, x_origin + x, y_origin + y, color);
		draw_pixel(handle, x_origin + x, y_origin - y, color);
		draw_pixel(handle, x_origin - x, y_origin - y, color);

		e2 = err;
		if (e2 <= y) {
			y++;
			err = err + (y * 2 + 1);
			if (-x == y && e2 <= x) {
				e2 = 0;
			}
			else {
				/*nothing to do*/
			}
		} else {
			/*nothing to do*/
		}

		if (e2 > x) {
			x++;
			err = err + (x * 2 + 1);
		} else {
			/*nothing to do*/
		}
	} while (x <= 0);

	if (handle->set_cs != NULL)
	{
		handle->set_cs(1);
	}

	return ERR_CODE_SUCCESS;
}

err_code_t st7735_draw_image(st7735_handle_t handle, uint8_t x_origin, uint8_t y_origin, uint8_t width, uint8_t height, uint8_t *image_src)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	if (handle->set_cs != NULL)
	{
		handle->set_cs(0);
	}

	set_addr(handle, x_origin, y_origin, x_origin + width - 1, y_origin + height - 1);
	st7735_write_data(handle, image_src, width * height * 2);

	if (handle->set_cs != NULL)
	{
		handle->set_cs(1);
	}

	return ERR_CODE_SUCCESS;
}

err_code_t st7735_set_position(st7735_handle_t handle, uint8_t x, uint8_t y)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	handle->pos_x = x;
	handle->pos_y = y;

	return ERR_CODE_SUCCESS;
}

err_code_t st7735_get_position(st7735_handle_t handle, uint8_t *x, uint8_t *y)
{
	/* Check if handle structure is NULL */
	if (handle == NULL)
	{
		return ERR_CODE_NULL_PTR;
	}

	*x = handle->pos_x;
	*y = handle->pos_y;

	return ERR_CODE_SUCCESS;
}
