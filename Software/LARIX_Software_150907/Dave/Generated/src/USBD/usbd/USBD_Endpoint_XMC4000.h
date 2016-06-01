/*
  Copyright 2013  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/
/*******************************************************************************
 Copyright (c) 2014, Infineon Technologies AG                                 **
 All rights reserved.                                                         **
                                                                              **
 Redistribution and use in source and binary forms, with or without           **
 modification,are permitted provided that the following conditions are met:   **
                                                                              **
 *Redistributions of source code must retain the above copyright notice,      **
 this list of conditions and the following disclaimer.                        **
 *Redistributions in binary form must reproduce the above copyright notice,   **
 this list of conditions and the following disclaimer in the documentation    **
 and/or other materials provided with the distribution.                       **
 *Neither the name of the copyright holders nor the names of its contributors **
 may be used to endorse or promote products derived from this software without**
 specific prior written permission.                                           **
                                                                              **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   **
 POSSIBILITY OF SUCH DAMAGE.                                                  **
                                                                              **
 To improve the quality of the software, users are encouraged to share        **
 modifications, enhancements or bug fixes with Infineon Technologies AG       **
 dave@infineon.com).                                                          **
                                                                              **
********************************************************************************
**                                                                            **
**                                                                            **
** PLATFORM : Infineon XMC4000 Series                                         **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** AUTHOR : App Developer                                                     **
**                                                                            **
** MAY BE CHANGED BY USER [yes/no]: No                                        **
**                                                                            **
** MODIFICATION DATE : Feb 21, 2014     		                              **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                       Author(s) Identity                                   **
********************************************************************************
**                                                                            **
** Initials    Name                                                           **
** ---------------------------------------------------------------------------**
** NSND          App Developer                                                **
*******************************************************************************/

/**
 * @file   USBD_Endpoint_XMC4000.h
 *
 * @brief Header file for Device end point access.
 *
 * It provides enums,function prototypes and inline function definitions to
 * access the USB end point.
 *
 */



#ifndef USBD_ENDPOINT_XMC4000_H_
#define USBD_ENDPOINT_XMC4000_H_

#include "../usb/Common/Common.h"
#include "../usb/Core/Endpoint.h"
#include "../usb/Core/USBController.h"
#include "usbd.h" /* IXF */


/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	extern "C" {
#endif
	/**
	 * @ingroup USBD_enumerations
	 * @{
	 */

	/** Enum for the possible error return codes of the
	 * \ref Endpoint_WaitUntilReady() function.
	*
	*/
	enum Endpoint_WaitUntilReady_ErrorCodes_t
	{
		ENDPOINT_READYWAIT_NoError                 = 0, /**< Endpoint is ready for next packet, no error. */
		ENDPOINT_READYWAIT_EndpointStalled         = 1, /**< The endpoint was stalled during the stream
														 *   transfer by the host or device.
														 */
		ENDPOINT_READYWAIT_DeviceDisconnected      = 2,	/**< Device was disconnected from the host while
														 *   waiting for the endpoint to become ready.
														 */
		ENDPOINT_READYWAIT_BusSuspended            = 3, /**< The USB bus has been suspended by the host and
														 *   no USB endpoint traffic can occur until the bus
														 *   has resumed.
														 */
		ENDPOINT_READYWAIT_Timeout                 = 4, /**< The host failed to accept or send the next packet
														 *   within the software timeout period set by the
														 *   \ref USB_STREAM_TIMEOUT_MS macro.
														 */
	};
	/**
	 * @}
	 */

	/**
	 * @ingroup USBD_apidoc
	 * @{
	 */
	/** Get the endpoint address of the currently selected endpoint. This is
	 * typically used to save the currently selected endpoint so that it can be
	 * restored after another endpoint has been  manipulated.
	 *
	 *  \return Index of the currently selected endpoint.
	 */
	static inline uint8_t Endpoint_GetCurrentEndpoint(void);
	static inline uint8_t Endpoint_GetCurrentEndpoint(void)
	{
		return device.CurrentDirection | device.CurrentEndpoint;
	}

	/** Selects the given endpoint address.
	 *
	 *  Any endpoint operations which do not require the endpoint address to be
	 *  indicated will operate on
	 *  the currently selected endpoint.
	 *
	 *  \param[in] Address Endpoint address to select.
	 */
	static inline void Endpoint_SelectEndpoint(const uint8_t Address);
	static inline void Endpoint_SelectEndpoint(const uint8_t Address)
	{
		device.CurrentEndpoint = Address & ENDPOINT_EPNUM_MASK;
		device.CurrentDirection = (Address & ENDPOINT_DIR_MASK);
	}

	/** Determines if the current CONTROL type endpoint has received a SETUP
	 * packet.
	 *
	 *  \ingroup Group_EndpointPacketManagement_AVR8
	 *
	 *  \return Boolean \c true if the selected endpoint has received a SETUP
	 *  packet, \c false otherwise.
	 */
	static inline bool Endpoint_IsSETUPReceived(void);
	static inline bool Endpoint_IsSETUPReceived(void)
	{
		return device.IsSetupRecieved;
	}


	/** Clears a received SETUP packet on the currently selected CONTROL type
	 * endpoint, freeing up the
	 *  endpoint for the next packet.
	 *
	 *  \ingroup Group_EndpointPacketManagement_AVR8
	 *
	 *  \note This is not applicable for non CONTROL type endpoints.
	 */
	static inline void Endpoint_ClearSETUP(void);
	static inline void Endpoint_ClearSETUP(void)
	{
		device.IsSetupRecieved = 0;
	}

	/** Nothing done in this function
	 *
	 */
	static inline void Endpoint_ClearStatusStage(void);
	static inline void Endpoint_ClearStatusStage(void)
	{

	}

	/** Determines if the selected IN endpoint is ready for a new packet to be
	 * sent to the host.
	 *
	 *  \ingroup Group_EndpointPacketManagement_AVR8
	 *
	 *  \return Boolean \c true if the current endpoint is ready for an IN
	 *  packet, \c false otherwise.
	 */
	static inline bool Endpoint_IsINReady(void);
	static inline bool Endpoint_IsINReady(void)
	{
		USBD_Endpoint_t *ep = &device.Endpoints[device.CurrentEndpoint];
		return ep->InInUse == 0 && ep->IsEnabled;
	}

	/** Determines if the selected OUT endpoint has received new packet from the
	 *  host.
	 *
	 *  \ingroup Group_EndpointPacketManagement_AVR8
	 *
	 *  \return Boolean \c true if current endpoint is has received an OUT
	 *  packet, \c false otherwise.
	 */
	static inline bool Endpoint_IsOUTReceived(void);
	static inline bool Endpoint_IsOUTReceived(void)
	{
		USBD_Endpoint_t *ep = &device.Endpoints[device.CurrentEndpoint];
		return ep->IsOutRecieved;
	}

	/** Determines the currently selected endpoint's direction.
	 *
	 *  \return The currently selected endpoint's direction, as a
	 *  \c ENDPOINT_DIR_* mask.
	 */
	static inline uint8_t Endpoint_GetEndpointDirection(void);
	static inline uint8_t Endpoint_GetEndpointDirection(void)
	{
		USBD_Endpoint_t *ep = &device.Endpoints[device.CurrentEndpoint];
		return ep->Address & ENDPOINT_DIR_MASK;
	}

	/** Indicates the number of bytes currently stored in the current endpoint's
	 *  selected bank.
	 *
	 *  \ingroup Group_EndpointRW_AVR8
	 *
	 *  \return Total number of bytes in the currently selected Endpoint's FIFO
	 *  buffer.
	 */
	static inline uint16_t Endpoint_BytesInEndpoint(void);
	static inline uint16_t Endpoint_BytesInEndpoint(void)
	{
		USBD_Endpoint_t *ep = &device.Endpoints[device.CurrentEndpoint];
		if (ep->Direction)
			return ep->InBytesAvailable;
		else
			return ep->OutBytesAvailable;
	}

	/** Acknowledges an OUT packet to the host on the currently selected
	 * endpoint, freeing up the endpoint for the next packet and switching to
	 * the alternative endpoint bank if double banked.
	 *
	 *  \ingroup Group_EndpointPacketManagement_AVR8
	 */
	void Endpoint_ClearOUT(void);

	/** Sends an IN packet to the host on the currently selected endpoint,
	 * freeing up the endpoint for the next packet and switching to the
	 * alternative endpoint bank if double banked.
	 *
	 *  \ingroup Group_EndpointPacketManagement_AVR8
	 */
	void Endpoint_ClearIN(void);

	/** Determines if the currently selected endpoint may be read from (if data
	 * is waiting in the endpoint bank and the endpoint is an OUT direction, or
	 * if the bank is not yet full if the endpoint is an IN direction). This
	 * function will return false if an error has occurred in the endpoint, if
	 * the endpoint is an OUT direction and no packet (or an empty packet) has
	 * been received, or if the endpoint is an IN direction and the endpoint
	 * bank is full.
	 *
	 *  \ingroup Group_EndpointPacketManagement
	 *
	 *  \return Boolean \c true if the currently selected endpoint may be read
	 *  from or written to, depending on its direction.
	 */
	bool Endpoint_IsReadWriteAllowed(void);

	/** Stalls the current endpoint, indicating to the host that a logical
	 * problem occurred with the indicated endpoint and that the current
	 * transfer sequence should be aborted. This provides a way for devices to
	 * indicate invalid commands to the host so that the current transfer can be
	 *  aborted and the host can begin its own recovery sequence.
	 *
	 *  The currently selected endpoint remains stalled until either the
	 *  \ref Endpoint_ClearStall() macro is called, or the host issues a CLEAR
	 *  FEATURE request to the device for the currently selected endpoint.
	 *
	 *  \ingroup Group_EndpointPacketManagement
	 */
	static inline void Endpoint_StallTransaction(void);
	static inline void Endpoint_StallTransaction(void)
	{
		device.Endpoints[device.CurrentEndpoint].IsHalted = 1;
		device.Driver->EndpointStall(device.CurrentDirection |
										device.CurrentEndpoint,1);
	}

	/** Clears the STALL condition on the currently selected endpoint.
	 *
	 *  \ingroup Group_EndpointPacketManagement_AVR8
	 */
	static inline void Endpoint_ClearStall(void);
	static inline void Endpoint_ClearStall(void)
	{
		device.Endpoints[device.CurrentEndpoint].IsHalted = 0;
		device.Driver->EndpointStall(device.CurrentDirection |
											device.CurrentEndpoint,0);
	}

	/** Determines if the currently selected endpoint is stalled, \c false
	 * otherwise.
	 *
	 *  \ingroup Group_EndpointPacketManagement
	 *
	 *  \return Boolean \c true if the currently selected endpoint is stalled,
	 *  \c false otherwise.
	 */
	static inline bool Endpoint_IsStalled(void);
	static inline bool Endpoint_IsStalled(void)
	{
		bool status = false;
		if(device.Endpoints[device.CurrentEndpoint].IsHalted == 1)
		{
			status = true;
		}
		return status;
	}

	/** Resets the endpoint bank FIFO. This clears all the endpoint banks and
	 * resets the USB controller's data In and Out pointers to the bank's
	 * contents.
	 *
	 *  \param[in] Address  Endpoint address whose FIFO buffers are to be reset.
	 */
	static inline void Endpoint_ResetEndpoint(const uint8_t Address);
	static inline void Endpoint_ResetEndpoint(const uint8_t Address)
	{
		device.Driver->EndpointAbort(device.CurrentDirection |
												device.CurrentEndpoint);
		USBD_Endpoint_t *ep = &device.Endpoints[device.CurrentEndpoint];
		ep->IsHalted = 0;
		ep->IsOutRecieved = 0;
		ep->InBytesAvailable = 0;
		ep->InDataLeft = 0;
		ep->InInUse = 0;
		ep->OutBytesAvailable = 0;
		ep->OutOffset = 0;
		ep->OutInUse = 0;
	}

	/** Resets the data toggle of the currently selected endpoint. */
	static inline void Endpoint_ResetDataToggle(void);
	static inline void Endpoint_ResetDataToggle(void)
	{
		xmc_device.endpoint_in_register[device.CurrentEndpoint]->diepctl |=
																	(1<<28);
	}


	/** Configures a table of endpoint descriptions, in sequence. This function
	 * can be used to configure multiple endpoints at the same time.
	 *
	 *  \note Endpoints with a zero address will be ignored, thus this function
	 *  cannot be used to configure the control endpoint.
	 *
	 *  \param[in] Table   Pointer to a table of endpoint descriptions.
	 *  \param[in] Entries Number of entries in the endpoint table to configure.
	 *
	 *  \return Boolean \c true if all endpoints configured successfully,
	 *  \c false otherwise.
	 */
	bool Endpoint_ConfigureEndpointTable(
			const USB_Endpoint_Table_t* const Table,  const uint8_t Entries);

	/** Spin-loops until the currently selected non-control endpoint is ready
	 * for the next packet of data to be read or written to it.
	 *
	 *  \note This routine should not be called on CONTROL type endpoints.
	 *
	 *  \ingroup Group_EndpointRW
	 *
	 *  \return A value from the \ref Endpoint_WaitUntilReady_ErrorCodes_t enum.
	 */
	uint8_t Endpoint_WaitUntilReady(void);

	/** Reads one byte from the currently selected endpoint's bank, for OUT
	 * direction endpoints.
	 *
	 *  \ingroup Group_EndpointPrimitiveRW
	 *
	 *  \return Next byte in the currently selected endpoint's FIFO buffer.
	 */
	uint8_t Endpoint_Read_8(void);

	/** Writes one byte to the currently selected endpoint's bank, for IN
	 * direction endpoints.
	 *
	 *  \ingroup Group_EndpointPrimitiveRW
	 *
	 *  \param[in] Data  Data to write into the the currently selected
	 *  endpoint's FIFO buffer.
	 */
	void Endpoint_Write_8(const uint8_t data);

	/** Writes four bytes to the currently selected endpoint's bank in little
	 * endian format, for IN direction endpoints.
	 *
	 *  \ingroup Group_EndpointPrimitiveRW
	 *
	 *  \param[in] Data  Data to write to the currently selected endpoint's
	 *  FIFO buffer.
	 */
	void Endpoint_Write_32_LE(const uint32_t Data);

	/** Reads four bytes from the currently selected endpoint's bank in little
	 * endian format, for OUT  direction endpoints.
	 *
	 *  \ingroup Group_EndpointPrimitiveRW
	 *
	 *  \return Next four bytes in the currently selected endpoint's FIFO
	 *  buffer.
	 */
	uint32_t Endpoint_Read_32_LE(void);

/**
 * @}
 */

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif



#endif /* USBD_ENDPOINT_XMC4000_H_ */
