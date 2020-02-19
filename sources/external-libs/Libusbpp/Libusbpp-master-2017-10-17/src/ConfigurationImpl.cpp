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
#include <algorithm>

#include <libusbpp/Interface.hpp>
#include <libusbpp/Exception.hpp>

#include "ConfigurationImpl.hpp"
#include "DeviceImpl.hpp"
#include "InterfaceImpl.hpp"
#include "Wideconvert.hpp"


LibUSB::ConfigurationImpl::ConfigurationImpl( libusb_config_descriptor* pConfigDescriptor, std::weak_ptr<DeviceImpl> pDeviceImpl)
{

	// Store the config descriptor.
	m_pConfigDescriptor.reset(pConfigDescriptor, ConfigDescriptorDeleter());

	// Store parent device implementation.
	if(pDeviceImpl.expired())
	{
		throw std::runtime_error("LibUSB::ConfigurationImpl::ConfigurationImpl(): Constructor has an expired DeviceImpl.");
	}

	m_pDeviceImpl = pDeviceImpl;

	CreateInterfaces();

}

LibUSB::ConfigurationImpl::~ConfigurationImpl()
{

	// Ensure that all Interface objects have been released/destroyed.

}

std::wstring LibUSB::ConfigurationImpl::DescriptorString( void ) const
{

	if(m_pDeviceImpl.expired())
	{
		throw std::logic_error("LibUSB::ConfigurationImpl::DescriptorString(): Has an expired DeviceImpl.");
	}

	if (m_pConfigDescriptor->iConfiguration == 0)
	{
		// There is no string descriptor
		return L"[No descriptor for this configuration available.]";
	}


	std::wstring resultStr;

	try
	{
		resultStr = m_pDeviceImpl.lock()->getStringDescriptorW(m_pConfigDescriptor->iConfiguration);
	}
	catch(LibUSB::LibUSBException &e)
	{
		std::string resultnStr = e.what();
		resultStr = Util::StringToWString(resultnStr);
	}

	return resultStr;

}

uint8_t LibUSB::ConfigurationImpl::getValue() const
{

	return m_pConfigDescriptor->bConfigurationValue;

}

uint8_t LibUSB::ConfigurationImpl::getMaxPower() const
{

	return m_pConfigDescriptor->MaxPower;

}

bool LibUSB::ConfigurationImpl::isSelfPowered() const
{
	// Return bit 6 of bmAttributes.
	return (1 == ((m_pConfigDescriptor->bmAttributes & (1 << 6)) >> 6));
}

bool LibUSB::ConfigurationImpl::supportsRemoteWakeup() const
{
	// Return bit 5 of bmAttributes
	return (1 == ((m_pConfigDescriptor->bmAttributes & (1 << 5)) >> 5));
}

bool LibUSB::ConfigurationImpl::hasExtraDescriptors() const
{
	return (m_pConfigDescriptor->extra_length > 0);
}

const unsigned char* LibUSB::ConfigurationImpl::getExtraDescriptors() const
{
	return m_pConfigDescriptor->extra;
}

int LibUSB::ConfigurationImpl::getExtraDescriptorSize() const
{
	return m_pConfigDescriptor->extra_length;
}

void LibUSB::ConfigurationImpl::SetAsActive()
{

	if(m_pDeviceImpl.expired())
	{
		throw std::runtime_error("LibUSB::ConfigurationImpl::SetAsActive(): Has an expired DeviceImpl.");
	}

	m_pDeviceImpl.lock()->setActiveConfiguration(m_pConfigDescriptor->bConfigurationValue);


}

int LibUSB::ConfigurationImpl::NumInterfaces() const
{

	return m_pConfigDescriptor->bNumInterfaces;

}

std::shared_ptr<LibUSB::Interface> LibUSB::ConfigurationImpl::getInterfaceByIndex( int index ) const
{
	/// \note I'm at a loss determining if the (array) index is required to correspond to the interface number.

	// Locate the indicated interface
	if (index >= m_pConfigDescriptor->bNumInterfaces)
	{

		/// \note #1 Possibly iterate through the interfaces and check bInterfaceNumber instead???

		throw std::logic_error("LibUSB::ConfigurationImpl::getInterfaceByIndex(): Invalid index. [Perhaps you meant to use getInterface(int interfacenumber)?]");
	}


	// Create the InterfaceImpl object
	const libusb_interface *pInterface = &(m_pConfigDescriptor->interface[index]);
	std::shared_ptr<InterfaceImpl> pInterfaceImpl = std::make_shared<InterfaceImpl>(pInterface, m_pDeviceImpl);

	// Create the interface object.
	return std::make_shared<Interface>(pInterfaceImpl);

}

void LibUSB::ConfigurationImpl::CreateInterfaces()
{

	if (!m_InterfaceMap.empty())
	{
		throw std::logic_error("LibUSB::ConfigurationImpl::CreateInterfaces(): Interfaces already present.");
	}

	for (int i = 0; i < NumInterfaces(); i++)
	{
		std::shared_ptr<Interface> pInterface = getInterfaceByIndex(i);
		m_InterfaceMap.insert(std::make_pair(pInterface->Number(), pInterface));
	}

}

std::shared_ptr<LibUSB::Interface> LibUSB::ConfigurationImpl::getInterface( int InterfaceNumber ) const
{

	// From what I can tell, interface number 0 isn't exactly invalid/disallowed - so no range check.

	InterfaceMap_t::const_iterator itInterface = m_InterfaceMap.find(InterfaceNumber);

	if (itInterface == m_InterfaceMap.end())
	{
		throw std::logic_error("LibUSB::ConfigurationImpl::getInterface(): Interface number not found.");
	}

	return itInterface->second;

}

