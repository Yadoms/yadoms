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

#include <stdexcept>

#include <libusbpp/Endpoint.hpp>

#include "EndpointImpl.hpp"


LibUSB::Endpoint::Endpoint( std::shared_ptr<EndpointImpl> pImpl )
	: m_pEndpointImpl(pImpl)
{


}

LibUSB::Endpoint::~Endpoint()
{

	// Abort all transfers.

}

int LibUSB::Endpoint::Number() const
{

	return m_pEndpointImpl->Number();
}

LibUSB::Direction_t LibUSB::Endpoint::Direction() const
{

	return m_pEndpointImpl->Direction();
}

LibUSB::Transfer_t LibUSB::Endpoint::TransferType() const
{

	return m_pEndpointImpl->TransferType();
}

LibUSB::Synchronization_t LibUSB::Endpoint::SynchronizationType() const
{

	return m_pEndpointImpl->SynchronizationType();
}

LibUSB::Usage_t LibUSB::Endpoint::UsageType() const
{

	return m_pEndpointImpl->UsageType();
}

int LibUSB::Endpoint::MaxPacketSize() const
{

	return m_pEndpointImpl->MaxPacketSize();
}

int LibUSB::Endpoint::PollingInterval() const
{

	return m_pEndpointImpl->PollingInterval();
}


std::shared_ptr<LibUSB::Transfer> LibUSB::Endpoint::CreateTransfer()
{
	return m_pEndpointImpl->CreateTransfer();
}

uint8_t LibUSB::Endpoint::Address() const
{

	return m_pEndpointImpl->Address();
}
