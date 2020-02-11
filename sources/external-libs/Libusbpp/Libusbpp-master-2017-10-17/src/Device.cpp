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

#include <libusbpp/Device.hpp>

#include "DeviceImpl.hpp"


LibUSB::Device::Device( std::shared_ptr<DeviceImpl> pInit )
{

	m_pDeviceImpl_ = pInit;

	// Obtain the descriptor
	m_pDeviceImpl_->getDeviceDescriptor();

}

LibUSB::Device::~Device()
{



}

uint16_t LibUSB::Device::USBSpecification()
{

	return m_pDeviceImpl_->getDeviceDescriptor()->bcdUSB;

}

uint8_t LibUSB::Device::DeviceClass()
{
	return m_pDeviceImpl_->getDeviceDescriptor()->bDeviceClass;
}


uint8_t LibUSB::Device::DeviceSubclass()
{
	return m_pDeviceImpl_->getDeviceDescriptor()->bDeviceSubClass;
}

uint8_t LibUSB::Device::DeviceProtocol()
{
	return m_pDeviceImpl_->getDeviceDescriptor()->bDeviceProtocol;
}

uint16_t LibUSB::Device::vendorID()
{
	return m_pDeviceImpl_->getDeviceDescriptor()->idVendor;
}

uint16_t LibUSB::Device::productID()
{
	return m_pDeviceImpl_->getDeviceDescriptor()->idProduct;
}

uint8_t LibUSB::Device::NumConfigurations()
{
	return m_pDeviceImpl_->getDeviceDescriptor()->bNumConfigurations;
}

std::wstring LibUSB::Device::ProductString()
{

	// Device must be open
	if (!isOpen())
	{

		throw std::runtime_error("LibUSB::Device::ProductString(): Device must be open!");

	}

	// Validate the descriptor index
	if(m_pDeviceImpl_->getDeviceDescriptor()->iProduct == 0)
	{
		// throw std::runtime_error("Product string is not supported on this device.");
		return L"Not supported.";
	}

	// Return an product string.
	return m_pDeviceImpl_->getStringDescriptorW(m_pDeviceImpl_->getDeviceDescriptor()->iProduct);

}

std::wstring LibUSB::Device::ManufacturerString()
{

	// Device must be open
	if (!isOpen())
	{

		throw std::runtime_error("LibUSB::Device::ManufacturerString(): Device must be open!");

	}

	// Validate the descriptor index
	if(m_pDeviceImpl_->getDeviceDescriptor()->iManufacturer == 0)
	{
		// throw std::runtime_error("Manufacturer string is not supported on this device.");
		return L"Not supported.";
	}

	// Return a manufacturer string.
	std::wstring resultStr = m_pDeviceImpl_->getStringDescriptorW(m_pDeviceImpl_->getDeviceDescriptor()->iManufacturer);

	return resultStr;

}

std::wstring LibUSB::Device::SerialString()
{

	// Device must be open
	if (!isOpen())
	{

		throw std::runtime_error("LibUSB::Device::SerialString(): Device must be open!");

	}

	// Validate the descriptor index
	if(m_pDeviceImpl_->getDeviceDescriptor()->iSerialNumber == 0)
	{
		// throw std::runtime_error("Serial number is not supported on this device.");
		return L"Not supported.";
	}

	// Return a serial string.
	return m_pDeviceImpl_->getStringDescriptorW(m_pDeviceImpl_->getDeviceDescriptor()->iSerialNumber);

}

void LibUSB::Device::Open()
{

	m_pDeviceImpl_->Open();

}

bool LibUSB::Device::isOpen()
{
	return m_pDeviceImpl_->isOpen();

}

std::shared_ptr<LibUSB::Configuration> LibUSB::Device::getConfiguration( uint8_t ConfigValue )
{

	// Check the active configuration object
	if ((m_pActiveConfiguration.get() != nullptr) && (m_pActiveConfiguration->Value() == ConfigValue))
	{
		return m_pActiveConfiguration;
	}

	// Create a configuration object
	return m_pDeviceImpl_->getConfiguration(ConfigValue);
}

std::shared_ptr<LibUSB::Configuration> LibUSB::Device::getActiveConfiguration()
{

	// Get the index of the active configuration
	uint8_t index = 0;

	if (m_pDeviceImpl_->getActiveConfiguration(index))
	{
		// Check the active configuration object
		if ((m_pActiveConfiguration.get() == nullptr) || (m_pActiveConfiguration->Value() != index))
		{
			// Create a new object
			m_pActiveConfiguration = m_pDeviceImpl_->getConfiguration(index);
		}
	}
	else
	{
		// The device is unconfigured - return a nullptr.
		m_pActiveConfiguration.reset();
	}

	return m_pActiveConfiguration;

}

void LibUSB::Device::TransferEventNotification( std::shared_ptr<Transfer> pCompletedTransfer )
{

	// By default: Do nothing.

}

void LibUSB::Device::Init()
{

	m_pDeviceImpl_->setParentDevice(shared_from_this());

}

std::shared_ptr<LibUSB::Endpoint> LibUSB::Device::getControlEndpoint()
{

	return m_pDeviceImpl_->getControlEndpoint();

}




