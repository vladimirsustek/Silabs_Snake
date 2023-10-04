/***************************************************************************//**
 * @file
 * @brief iostream usart examples functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "em_chip.h"
#include "sl_iostream.h"
#include "sl_iostream_init_instances.h"
#include "sl_iostream_handles.h"

#include "glib.h"
#include "dmd.h"
#include "SnakeGame/snake_port.h"
#include "nvm3_default.h"

#ifdef ORIGINAL_DEMO_APPLICATION
/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#ifndef BUFSIZE
#define BUFSIZE    80
#endif

/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/

/* Input buffer */
static char buffer[BUFSIZE];

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * Initialize example.
 ******************************************************************************/

#ifndef LCD_MAX_LINES
#define LCD_MAX_LINES      11
#endif
#endif
/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/

void memlcd_app_init(void)
{

}

void app_iostream_eusart_init(void)
{
	/* Prevent buffering of output/input.*/
#if !defined(__CROSSWORKS_ARM) && defined(__GNUC__)
	setvbuf(stdout, NULL, _IONBF, 0); /*Set unbuffered mode for stdout (newlib)*/
	setvbuf(stdin, NULL, _IONBF, 0); /*Set unbuffered mode for stdin (newlib)*/
#endif

	/* Output on vcom usart instance */
	//const char str1[] = "IOstream EUSART example\r\n\r\n";
	//sl_iostream_write(sl_iostream_vcom_handle, str1, strlen(str1));
	/* Setting default stream */
	sl_iostream_set_default(sl_iostream_vcom_handle);
	//const char str2[] = "This is output on the default stream\r\n";
	//sl_iostream_write(SL_IOSTREAM_STDOUT, str2, strlen(str2));

	/* Using printf */
	/* Writing ASCII art to the VCOM iostream */
	printf("Snake game control upper-case: W A S D P.\r\n");

	memlcd_app_init();

	nvm3_initDefault();
}

/***************************************************************************//**
 * Example ticking function.
 ******************************************************************************/
void app_iostream_eusart_process_action(void)
{
	int8_t c = 0;
#ifdef ORIGINAL_DEMO_APPLICATION
  static uint8_t index = 0;
#endif
	static bool print_welcome = true;

	if (print_welcome)
	{
		printf("> ");
		print_welcome = false;
	}

	/* Retrieve characters, print local echo and full line back */
	c = getchar();
	if (c != -1)
	{
		platform_snake_set_control(c);
	}
#ifdef ORIGINAL_DEMO_APPLICATION
  if (c > 0) {
    if (c == '\r' || c == '\n') {
      buffer[index] = '\0';
      printf("\r\nYou wrote: %s\r\n> ", buffer);
      index = 0;
    } else {
      if (index < BUFSIZE - 1) {
        buffer[index] = c;
        index++;
      }
      /* Local echo */
      putchar(c);
    }
  }
#endif
}
