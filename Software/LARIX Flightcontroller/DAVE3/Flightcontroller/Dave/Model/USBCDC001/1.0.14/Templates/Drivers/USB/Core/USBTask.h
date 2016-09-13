/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/* THIS FILE IS PROVIDED BY DAVE.
 * THIS FILE IS GENERATED.
 */

/*******************************************************************************
 *
 * Copyright (C) 2011 Infineon Technologies AG. All rights reserved.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with
 * Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are
 * supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
********************************************************************************
**                                                                            **
**                                                                            **
** PLATFORM : Infineon XMC4000 Series                                         **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** AUTHOR : App Developer                                                     **
**                                                                            **
** MAY BE CHANGED BY USER [yes/no]: Yes                                       **
**                                                                            **
** MODIFICATION DATE : Nov 19, 2012                                            **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                       Author(s) Identity                                   **
********************************************************************************
**                                                                            **
** Initials    Name                                                           **
** ---------------------------------------------------------------------------**
** CM          App Developer                                                  **
*******************************************************************************/


/** \file
 *  \brief Main USB service task management.
 *
 *  This file contains the function definitions required for the main USB service task, which must be called
 *  from the user application to ensure that the USB connection to or from a connected USB device is maintained.
 *
 *  \note This file should not be included directly. It is automatically included as needed by the USB driver
 *        dispatch header located in LUFA/Drivers/USB/USB.h.
 */

#ifndef __USBTASK_H__
#define __USBTASK_H__

	/* Includes: */
		#include "USBMode.h"		
		#include "StdRequestType.h"
		#include "StdDescriptors.h"

		#if defined(USB_CAN_BE_DEVICE)
			//#include "DeviceStandardReq.h"
		#endif

		#if defined(USB_CAN_BE_HOST)
			#include "HostStandardReq.h"
		#endif
	
		#ifdef USE_IFX_DEV 
			#ifdef USB_CAN_BE_HOST
				#include "../USBCORE001/usbcore001_host.h"
			#endif
			#ifdef USB_CAN_BE_DEVICE
				#include "../USBCORE001/usbcore001.h"
				#include "../USBCORE001/usbcore001_device.h"
			#endif
		#endif


	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Public Interface - May be used in end-application: */
		/* Global Variables: */
			/** Indicates if the USB interface is currently initialized but not necessarily connected to a host
			 *  or device (i.e. if USB_Init() has been run). If this is false, all other library globals related
			 *  to the USB driver are invalid.
			 *
			 *  \note This variable should be treated as read-only in the user application, and never manually
			 *        changed in value.
			 *
			 *  \ingroup Group_USBManagement
			 */
			extern volatile bool USB_IsInitialized;

			/** Structure containing the last received Control request when in Device mode (for use in user-applications
			 *  inside of the EVENT_USB_Device_ControlRequest() event, or for filling up with a control request to
			 *  issue when in Host mode before calling USB_Host_SendControlRequest().
			 *
			 *  \ingroup Group_USBManagement
			 */
			 extern USB_Request_Header_t USB_ControlRequest;

			#if defined(USB_CAN_BE_HOST) || defined(__DOXYGEN__)
				#if !defined(HOST_STATE_AS_GPIOR) || defined(__DOXYGEN__)
					/** Indicates the current host state machine state. When in host mode, this indicates the state
					 *  via one of the values of the USB_Host_States_t enum values.
					 *
					 *  This value may be altered by the user application to implement the HOST_STATE_Addressed,
					 *  \ref HOST_STATE_Configured and HOST_STATE_Suspended states which are not implemented by
					 *  the library internally.
					 *
					 *  To reduce program size and speed up checks of this global on the AVR8 architecture, it can be
					 *  placed into one of the AVR's \c GPIOR hardware registers instead of RAM by defining the
					 *  \c HOST_STATE_AS_GPIOR token to a value between 0 and 2 in the project makefile and passing it to
					 *  the compiler via the -D switch. When defined, the corresponding GPIOR register should not be used
					 *  in the user application except implicitly via the library APIs.
					 *
					 *  \note This global is only present if the user application can be a USB host.
					 *
					 *  \see \ref USB_Host_States_t for a list of possible device states.
					 *
					 *  \ingroup Group_Host
					 */
					extern volatile uint8_t USB_HostState;
				#else
					#define _GET_HOST_GPIOR_NAME2(y) GPIOR ## y
					#define _GET_HOST_GPIOR_NAME(x)  _GET_HOST_GPIOR_NAME2(x)
					#define USB_HostState            _GET_HOST_GPIOR_NAME(HOST_STATE_AS_GPIOR)
				#endif
			#endif

			#if defined(USB_CAN_BE_DEVICE) || defined(__DOXYGEN__)
				#if !defined(DEVICE_STATE_AS_GPIOR) || defined(__DOXYGEN__)
					/** Indicates the current device state machine state. When in device mode, this indicates the state
					 *  via one of the values of the \ref USB_Device_States_t enum values.
					 *
					 *  This value should not be altered by the user application as it is handled automatically by the
					 *  library. The only exception to this rule is if the NO_LIMITED_CONTROLLER_CONNECT token is used
					 *  (see \ref EVENT_USB_Device_Connect() and \ref EVENT_USB_Device_Disconnect() events).
					 *
					 *  To reduce program size and speed up checks of this global on the AVR8 architecture, it can be
					 *  placed into one of the AVR's \c GPIOR hardware registers instead of RAM by defining the
					 *  \c DEVICE_STATE_AS_GPIOR token to a value between 0 and 2 in the project makefile and passing it to
					 *  the compiler via the -D switch. When defined, the corresponding GPIOR register should not be used
					 *  in the user application except implicitly via the library APIs.
					 *
					 *  \note This global is only present if the user application can be a USB device.
					 *        \n\n
					 *
					 *  \note This variable should be treated as read-only in the user application, and never manually
					 *        changed in value except in the circumstances outlined above.
					 *
					 *  \see \ref USB_Device_States_t for a list of possible device states.
					 *
					 *  \ingroup Group_Device
					 */
					extern volatile uint8_t USB_DeviceState;
				#else
					#define _GET_DEVICE_GPIOR_NAME2(y) GPIOR ## y
					#define _GET_DEVICE_GPIOR_NAME(x)  _GET_DEVICE_GPIOR_NAME2(x)
					#define USB_DeviceState            _GET_DEVICE_GPIOR_NAME(DEVICE_STATE_AS_GPIOR)
				#endif
			#endif

			 /**
			 * @ingroup USBCDC001_apidoc
			 * @{
			 */
		/* Function Prototypes: */
			/** This is the main USB management task. The USB driver requires this task to be executed
			 *  continuously when the USB system is active (device attached in host mode, or attached to a host
			 *  in device mode) in order to manage USB communications. This task may be executed inside an RTOS,
			 *  fast timer ISR or the main user application loop.
			 *
			 *  The USB task must be serviced within 30ms while in device mode, or within 1ms while in host mode.
			 *  The task may be serviced at all times, or (for minimum CPU consumption):
			 *
			 *    - In device mode, it may be disabled at start-up, enabled on the firing of the EVENT_USB_Device_Connect()
			 *      event and disabled again on the firing of the EVENT_USB_Device_Disconnect() event.
			 *
			 *    - In host mode, it may be disabled at start-up, enabled on the firing of the EVENT_USB_Host_DeviceAttached()
			 *      event and disabled again on the firing of the EVENT_USB_Host_DeviceEnumerationComplete() or
			 *      EVENT_USB_Host_DeviceEnumerationFailed() events.
			 *
			 *  If in device mode (only), the control endpoint can instead be managed via interrupts entirely by the library
			 *  by defining the INTERRUPT_CONTROL_ENDPOINT token and passing it to the compiler via the -D switch.
			 *
			 *  \see Group_Events for more information on the USB events.
			 *
			 *  \ingroup Group_USBManagement
			 */
			void USB_USBTask(void);
			/**
			 * @}
			 */

	/* Private Interface - For use in library only: */
	#if !defined(__DOXYGEN__)
		/* Function Prototypes: */
			#if defined(__INCLUDE_FROM_USBTASK_C)
				#if defined(USB_CAN_BE_HOST)
					static void USB_HostTask(void);
				#endif

				#if defined(USB_CAN_BE_DEVICE)
					static void USB_DeviceTask(void);
				#endif
			#endif

		/* Macros: */
			#define HOST_TASK_NONBLOCK_WAIT(Duration, NextState) MACROS{ USB_HostState   = HOST_STATE_WaitForDevice; \
			                                                             WaitMSRemaining = (Duration);               \
			                                                             PostWaitState   = (NextState);              }MACROE
	#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif
