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

#include <libusb.h>

#include <libusbpp/Interface.hpp>

#include "InterfaceImpl.hpp"


LibUSB::Interface::Interface( std::shared_ptr<InterfaceImpl> pImpl )
	:m_pInterfaceImpl(pImpl)
{

}


LibUSB::Interface::~Interface()
{

	// Destroy Implementation.
	m_pInterfaceImpl.reset();

}


int LibUSB::Interface::Number() const
{
	return m_pInterfaceImpl->Number();
}

int LibUSB::Interface::AlternateSetting() const
{
	return m_pInterfaceImpl->AlternateSetting();
}

int LibUSB::Interface::Class() const
{
	return m_pInterfaceImpl->Class();
}

int LibUSB::Interface::SubClass() const
{
	return m_pInterfaceImpl->SubClass();
}

int LibUSB::Interface::Protocol() const
{
	return m_pInterfaceImpl->Protocol();
}

std::wstring LibUSB::Interface::DescriptorString() const
{
	return m_pInterfaceImpl->DescriptorString();
}

int LibUSB::Interface::NumAlternateSettings() const
{
	return m_pInterfaceImpl->NumAlternateSettings();
}

void LibUSB::Interface::Claim()
{

	if (isClaimed())
	{
		return;
	}

	m_pInterfaceImpl->Claim();

}

void LibUSB::Interface::Release()
{
	m_pInterfaceImpl->Release();
}

void LibUSB::Interface::SetAlternate( uint8_t AlternateSetting /*= 0*/ )
{
	m_pInterfaceImpl->SetAlternate(AlternateSetting);
}

int LibUSB::Interface::NumEndpoints() const
{
	return m_pInterfaceImpl->NumEndpoints();
}

int LibUSB::Interface::getEPNumberByIndex( int index ) const
{
	return m_pInterfaceImpl->getEPNumberByIndex(index);
}

LibUSB::Interface::EndpointNumbers_t LibUSB::Interface::getEndpointNumbers() const
{
	EndpointNumbers_t mEndpointNumbers;

	// Create each endpoint number
	for (int i = 1; i <= NumEndpoints(); i++)
	{
		mEndpointNumbers.insert(std::make_pair(i,getEPNumberByIndex(i)));
	}

	return mEndpointNumbers;
}

std::shared_ptr<LibUSB::Endpoint> LibUSB::Interface::getEndpoint( int number )
{
	return m_pInterfaceImpl->getEndpoint(number);
}

bool LibUSB::Interface::isClaimed() const
{

	return m_pInterfaceImpl->isClaimed();
}

