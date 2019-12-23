/*
 * Copyright (C) 2012, Anthony Clay, ZarthCode LLC, all rights reserved.
 * Copyright (C) 2016, Stephan Linz, Li-Pro.Net, all rights reserved.
 *
 * This file is part of the LibUSB C++ wrapper library (libusbpp).
 *
 * libusbpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libusbpp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libusbpp.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LIBUSBPP_ENDPOINT_IMPL_HPP
#define __LIBUSBPP_ENDPOINT_IMPL_HPP

#include <memory>
#include <stdint.h>

#include <libusb.h>

#include <libusbpp/EndpointDefs.hpp>
#include <libusbpp/Transfer.hpp>


namespace LibUSB
{
	/// Device Implementation
	class DeviceImpl;

	/// Endpoint Implementation
	class EndpointImpl : public std::enable_shared_from_this<EndpointImpl>
	{
	public:

		/// Constructor
		EndpointImpl(const libusb_endpoint_descriptor* pDescriptor, std::weak_ptr<DeviceImpl> pDeviceImpl);

		/// Destructor
		~EndpointImpl();

		/// Endpoint Address
		uint8_t Address()const;

		/// Endpoint Number
		uint8_t Number()const;

		/// Returns the direction of this endpoint
		Direction_t Direction()const;

		/// Returns the transfer type of this endpoint
		Transfer_t TransferType()const;

		/// Returns the synchronization type (ISOCHRONOUS MODE ONLY!)
		Synchronization_t SynchronizationType()const;

		/// Returns the usage type (ISOCHRONOUS MODE ONLY!)
		Usage_t UsageType()const;

		/// Returns the maximum packet size for this endpoint
		uint16_t MaxPacketSize()const;

		/// Returns the polling interval (in frames, 1f = 1mS @ low/full speed, 125uS at high), ignore for bulk/control, 1 for iso, 1-255 for interrupt.
		uint8_t PollingInterval()const;

	// Transfer creation

		/// Creates a new transfer object
		std::shared_ptr<Transfer> CreateTransfer();

	// Utility methods

		std::weak_ptr<DeviceImpl> getDeviceImpl()const;


	private:

		/// Endpoint Descriptor
		const libusb_endpoint_descriptor* m_pEndpointDescriptor;

		/// Weak pointer to the deviceimpl/parent
		std::weak_ptr<DeviceImpl> m_pDeviceImpl;

	};

}

#endif // __LIBUSBPP_ENDPOINT_IMPL_HPP
