/***************************************************************************//**
 * @file
 * @brief Board Control API
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "sl_board_control.h"
#include "sl_board_control_config.h"

#include "em_gpio.h"

sl_status_t sl_board_configure_vcom(void)
{
#if defined(SL_BOARD_ENABLE_VCOM) && SL_BOARD_ENABLE_VCOM
	return sl_board_enable_vcom();
#else
  return SL_STATUS_NOT_SUPPORTED;
#endif
}

sl_status_t sl_board_enable_vcom(void)
{
#if defined(SL_BOARD_ENABLE_VCOM_PORT)
	GPIO_PinModeSet(SL_BOARD_ENABLE_VCOM_PORT, SL_BOARD_ENABLE_VCOM_PIN,
			gpioModePushPull, 1);
	return SL_STATUS_OK;
#else
  return SL_STATUS_NOT_AVAILABLE;
#endif
}

sl_status_t sl_board_disable_vcom(void)
{
#if defined(SL_BOARD_ENABLE_VCOM_PORT)
	GPIO_PinModeSet(SL_BOARD_ENABLE_VCOM_PORT, SL_BOARD_ENABLE_VCOM_PIN,
			gpioModePushPull, 0);
	return SL_STATUS_OK;
#else
  return SL_STATUS_NOT_AVAILABLE;
#endif
}

sl_status_t sl_board_enable_display(void)
{
#if defined(SL_BOARD_ENABLE_DISPLAY_PORT)
	GPIO_PinModeSet(SL_BOARD_ENABLE_DISPLAY_PORT, SL_BOARD_ENABLE_DISPLAY_PIN,
			gpioModePushPull, 1);
#if defined(SL_BOARD_SELECT_DISPLAY_PORT)
  GPIO_PinModeSet(SL_BOARD_SELECT_DISPLAY_PORT, SL_BOARD_SELECT_DISPLAY_PIN, gpioModePushPull, 1);
#endif
	return SL_STATUS_OK;
#else
  return SL_STATUS_NOT_AVAILABLE;
#endif
}

sl_status_t sl_board_disable_display(void)
{
#if defined(SL_BOARD_ENABLE_DISPLAY_PORT)
	GPIO_PinModeSet(SL_BOARD_ENABLE_DISPLAY_PORT, SL_BOARD_ENABLE_DISPLAY_PIN,
			gpioModePushPull, 0);
#if defined(SL_BOARD_SELECT_DISPLAY_PORT)
  GPIO_PinModeSet(SL_BOARD_SELECT_DISPLAY_PORT, SL_BOARD_SELECT_DISPLAY_PIN, gpioModePushPull, 0);
#endif
	return SL_STATUS_OK;
#else
  return SL_STATUS_NOT_AVAILABLE;
#endif
}

sl_status_t sl_board_enable_sensor(sl_board_sensor_t sensor)
{
	sl_status_t status = SL_STATUS_NOT_AVAILABLE;

	switch (sensor)
	{
	case SL_BOARD_SENSOR_RHT:
#if defined(SL_BOARD_ENABLE_SENSOR_RHT_PORT)
		GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_RHT_PORT,
		SL_BOARD_ENABLE_SENSOR_RHT_PIN, gpioModePushPull, 1);
		status = SL_STATUS_OK;
#else
      EFM_ASSERT(false);
    #endif
		break;

	case SL_BOARD_SENSOR_LIGHT:
#if defined(SL_BOARD_ENABLE_SENSOR_LIGHT_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_LIGHT_PORT, SL_BOARD_ENABLE_SENSOR_LIGHT_PIN, gpioModePushPull, 1);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_SENSOR_PRESSURE:
#if defined(SL_BOARD_ENABLE_SENSOR_PRESSURE_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_PRESSURE_PORT, SL_BOARD_ENABLE_SENSOR_PRESSURE_PIN, gpioModePushPull, 1);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_SENSOR_HALL:
#if defined(SL_BOARD_ENABLE_SENSOR_HALL_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_HALL_PORT, SL_BOARD_ENABLE_SENSOR_HALL_PIN, gpioModePushPull, 1);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_SENSOR_GAS:
#if defined(SL_BOARD_ENABLE_SENSOR_GAS_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_GAS_PORT, SL_BOARD_ENABLE_SENSOR_GAS_PIN, gpioModePushPull, 1);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_SENSOR_IMU:
#if defined(SL_BOARD_ENABLE_SENSOR_IMU_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_IMU_PORT, SL_BOARD_ENABLE_SENSOR_IMU_PIN, gpioModePushPull, 1);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_SENSOR_MICROPHONE:
#if defined(SL_BOARD_ENABLE_SENSOR_MICROPHONE_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_MICROPHONE_PORT, SL_BOARD_ENABLE_SENSOR_MICROPHONE_PIN, gpioModePushPull, 1);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	default:
		EFM_ASSERT(false);  // Should not happen
		break;
	}

	return status;
}

sl_status_t sl_board_disable_sensor(sl_board_sensor_t sensor)
{
	sl_status_t status = SL_STATUS_NOT_AVAILABLE;

	switch (sensor)
	{
	case SL_BOARD_SENSOR_RHT:
#if defined(SL_BOARD_ENABLE_SENSOR_RHT_PORT)
		GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_RHT_PORT,
		SL_BOARD_ENABLE_SENSOR_RHT_PIN, gpioModePushPull, 0);
		status = SL_STATUS_OK;
#else
      EFM_ASSERT(false);
    #endif
		break;

	case SL_BOARD_SENSOR_LIGHT:
#if defined(SL_BOARD_ENABLE_SENSOR_LIGHT_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_LIGHT_PORT, SL_BOARD_ENABLE_SENSOR_LIGHT_PIN, gpioModePushPull, 0);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_SENSOR_PRESSURE:
#if defined(SL_BOARD_ENABLE_SENSOR_PRESSURE_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_PRESSURE_PORT, SL_BOARD_ENABLE_SENSOR_PRESSURE_PIN, gpioModePushPull, 0);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_SENSOR_HALL:
#if defined(SL_BOARD_ENABLE_SENSOR_HALL_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_HALL_PORT, SL_BOARD_ENABLE_SENSOR_HALL_PIN, gpioModePushPull, 0);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_SENSOR_GAS:
#if defined(SL_BOARD_ENABLE_SENSOR_GAS_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_GAS_PORT, SL_BOARD_ENABLE_SENSOR_GAS_PIN, gpioModePushPull, 0);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_SENSOR_IMU:
#if defined(SL_BOARD_ENABLE_SENSOR_IMU_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_IMU_PORT, SL_BOARD_ENABLE_SENSOR_IMU_PIN, gpioModePushPull, 0);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_SENSOR_MICROPHONE:
#if defined(SL_BOARD_ENABLE_SENSOR_MICROPHONE_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_SENSOR_MICROPHONE_PORT, SL_BOARD_ENABLE_SENSOR_MICROPHONE_PIN, gpioModePushPull, 0);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	default:
		EFM_ASSERT(false);  // Should not happen
		break;
	}

	return status;
}

sl_status_t sl_board_enable_memory(sl_board_memory_t memory)
{
	sl_status_t status = SL_STATUS_NOT_AVAILABLE;

	switch (memory)
	{
	case SL_BOARD_MEMORY_SDCARD:
#if defined(SL_BOARD_ENABLE_MEMORY_SDCARD_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_MEMORY_SDCARD_PORT, SL_BOARD_ENABLE_MEMORY_SDCARD_PIN, gpioModePushPull, 1);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_MEMORY_QSPI:
#if defined(SL_BOARD_ENABLE_MEMORY_QSPI_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_MEMORY_QSPI_PORT, SL_BOARD_ENABLE_MEMORY_QSPI_PIN, gpioModePushPull, 1);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	default:
		EFM_ASSERT(false); // Should not happen
		break;
	}

	return status;
}

sl_status_t sl_board_disable_memory(sl_board_memory_t memory)
{
	sl_status_t status = SL_STATUS_NOT_AVAILABLE;

	switch (memory)
	{
	case SL_BOARD_MEMORY_SDCARD:
#if defined(SL_BOARD_ENABLE_MEMORY_SDCARD_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_MEMORY_SDCARD_PORT, SL_BOARD_ENABLE_MEMORY_SDCARD_PIN, gpioModePushPull, 0);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	case SL_BOARD_MEMORY_QSPI:
#if defined(SL_BOARD_ENABLE_MEMORY_QSPI_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_MEMORY_QSPI_PORT, SL_BOARD_ENABLE_MEMORY_QSPI_PIN, gpioModePushPull, 0);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	default:
		EFM_ASSERT(false);  // Should not happen
		break;
	}

	return status;
}

sl_status_t sl_board_enable_oscillator(sl_board_oscillator_t oscillator)
{
	sl_status_t status = SL_STATUS_NOT_AVAILABLE;

	switch (oscillator)
	{
	case SL_BOARD_OSCILLATOR_TCXO:
#if defined(SL_BOARD_ENABLE_OSCILLATOR_TCXO_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_OSCILLATOR_TCXO_PORT, SL_BOARD_ENABLE_OSCILLATOR_TCXO_PIN, gpioModePushPull, 1);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	default:
		EFM_ASSERT(false);  // Should not happen
		break;
	}

	return status;
}

sl_status_t sl_board_disable_oscillator(sl_board_oscillator_t oscillator)
{
	sl_status_t status = SL_STATUS_NOT_AVAILABLE;

	switch (oscillator)
	{
	case SL_BOARD_OSCILLATOR_TCXO:
#if defined(SL_BOARD_ENABLE_OSCILLATOR_TCXO_PORT)
      GPIO_PinModeSet(SL_BOARD_ENABLE_OSCILLATOR_TCXO_PORT, SL_BOARD_ENABLE_OSCILLATOR_TCXO_PIN, gpioModePushPull, 0);
      status = SL_STATUS_OK;
    #else
		EFM_ASSERT(false);
#endif
		break;

	default:
		EFM_ASSERT(false); // Should not happen
		break;
	}

	return status;
}
