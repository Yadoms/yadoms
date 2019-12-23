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

#ifndef LIBUSBPP_ENDPOINT_HPP
#define LIBUSBPP_ENDPOINT_HPP

#include <memory>

#include <libusbpp/Transfer.hpp>
#include <libusbpp/EndpointDefs.hpp>


namespace LibUSB
{

	/// Endpoint implementation
	class EndpointImpl;

	/// USB endpoint class
	class Endpoint
	{
	public:

		Endpoint(std::shared_ptr<EndpointImpl> pImpl);
		~Endpoint();

		/// Raw Endpoint Address
		uint8_t Address()const;

		/// Endpoint Number
		int Number()const;

		/// Returns the direction of this endpoint
		Direction_t Direction()const;

		/// Returns the transfer type of this endpoint
		Transfer_t TransferType()const;

		/// Returns the synchronization type (ISOCHRONOUS MODE ONLY!)
		Synchronization_t SynchronizationType()const;

		/// Returns the usage type (ISOCHRONOUS MODE ONLY!)
		Usage_t UsageType()const;

		/// Returns the maximum packet size for this endpoint
		int MaxPacketSize()const;

		/// Returns the polling interval (in frames, 1f = 1mS @ low/full speed, 125uS at high), ignore for bulk/control, 1 for iso, 1-255 for interrupt.
		int PollingInterval()const;

	// Transfer

		/// Creates a new data transfer object
		std::shared_ptr<Transfer> CreateTransfer();



	protected:
	private:

		/// Endpoint Implementation
		std::shared_ptr<EndpointImpl> m_pEndpointImpl;
	};

}

#endif // LIBUSBPP_ENDPOINT_HPP
