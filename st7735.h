// MIT License

// Copyright (c) 2024 phonght32

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __ST7735_H__
#define __ST7735_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "err_code.h"
#include "fonts.h"

typedef err_code_t (*st7735_func_set_gpio)(uint8_t level);
typedef err_code_t (*st7735_func_spi_send)(uint8_t *buf_send, uint16_t len);
typedef void (*st7735_func_delay)(uint32_t time_ms);

/**
 * @brief   Handle structure.
 */
typedef struct st7735 *st7735_handle_t;

/**
 * @brief   Configuration structure.
 */
typedef struct {
	uint16_t  				width;			/*!< Screen width */
	uint16_t 				height;			/*!< Screen height */
	st7735_func_set_gpio 	set_cs;			/*!< Function set CS. Used in SPI mode */
	st7735_func_set_gpio 	set_dc;			/*!< Function set DC. Used in SPI mode */
	st7735_func_set_gpio 	set_rst;		/*!< Function set RST. Used in SPI mode */
	st7735_func_spi_send 	spi_send;		/*!< Function send SPI data */
	st7735_func_delay 		delay; 			/*!< Function delay */
} st7735_cfg_t;

/*
 * @brief   Initialize ST7735 with default parameters.
 *
 * @note    This function must be called first.
 *
 * @param   None.
 *
 * @return
 *      - Handle structure: Success.
 *      - Others:           Fail.
 */
st7735_handle_t st7735_init(void);

/*
 * @brief   Set configuration parameters.
 *
 * @param 	handle Handle structure.
 * @param   config Configuration structure.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_set_config(st7735_handle_t handle, st7735_cfg_t config);

/*
 * @brief   Configure ST7735 to run.
 *
 * @param 	handle Handle structure.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_config(st7735_handle_t handle);

/*
 * @brief   Fill screen.
 *
 * @param   handle Handle structure.
 * @param 	color Color.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_fill(st7735_handle_t handle, uint16_t color);

/*
 * @brief   Write character.
 *
 * @param   handle Handle structure.
 * @param   font_size Font size.
 * @param   chr Character.
 * @param 	color Color.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_write_char(st7735_handle_t handle, font_size_t font_size, uint8_t chr, uint16_t color);

/*
 * @brief   Write string.
 *
 * @param   handle Handle structure.
 * @param   font_size Font size.
 * @param   str Pointer references to the data.
 * @param 	color Color.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_write_string(st7735_handle_t handle, font_size_t font_size, uint8_t *str, uint16_t color);

/*
 * @brief   Draw pixel.
 *
 * @param   handle Handle structure.
 * @param 	x Column index.
 * @param 	y Row index.
 * @param 	color Color.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_draw_pixel(st7735_handle_t handle, uint8_t x, uint8_t y, uint16_t color);

/*
 * @brief   Draw line.
 *
 * @param   handle Handle structure.
 * @param 	x_start x start position.
 * @param 	y_start y start position.
 * @param 	x_end x end position.
 * @param 	y_end y end position.
 * @param 	color Line color.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_draw_line(st7735_handle_t handle, uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, uint16_t color);

/*
 * @brief   Draw rectangle.
 *
 * @param   handle Handle structure.
 * @param 	x_origin x origin position.
 * @param 	y_origin y origin position.
 * @param 	width Width.
 * @param 	height Height.
 * @param 	color Line color.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_draw_rectangle(st7735_handle_t handle, uint8_t x_origin, uint8_t y_origin, uint8_t width, uint8_t height, uint16_t color);

/*
 * @brief   Draw circle.
 *
 * @param   handle Handle structure.
 * @param 	x_origin x origin position.
 * @param 	y_origin y origin position.
 * @param 	radius Radius.
 * @param 	color Line color.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_draw_circle(st7735_handle_t handle, uint8_t x_origin, uint8_t y_origin, uint8_t radius, uint16_t color);

/*
 * @brief   Draw image.
 *
 * @param   handle Handle structure.
 * @param 	x_origin x origin position.
 * @param 	y_origin y origin position.
 * @param 	width Image width in pixel.
 * @param 	height Image height in pixel.
 * @param 	image_src Image source.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_draw_image(st7735_handle_t handle, uint8_t x_origin, uint8_t y_origin, uint8_t width, uint8_t height, uint8_t *image_src);

/*
 * @brief   Set position.
 *
 * @param   handle Handle structure.
 * @param 	x Column index.
 * @param 	y Row index.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_set_position(st7735_handle_t handle, uint8_t x, uint8_t y);

/*
 * @brief   Get position.
 *
 * @param   handle Handle structure.
 * @param 	x Pointer to column index.
 * @param 	y Pointer to row index.
 *
 * @return
 *      - ERR_CODE_SUCCESS: Success.
 *      - Others:           Fail.
 */
err_code_t st7735_get_position(st7735_handle_t handle, uint8_t *x, uint8_t *y);

#ifdef __cplusplus
}
#endif

#endif /* __ST7735_H__ */