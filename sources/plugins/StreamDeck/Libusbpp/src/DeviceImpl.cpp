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

#include <iosfwd>
#include <sstream>
#include <cstring>

#include <libusb.h>

#include <libusbpp/Configuration.hpp>
#include <libusbpp/Endpoint.hpp>
#include <libusbpp/Exception.hpp>

#include "DeviceImpl.hpp"
#include "ConfigurationImpl.hpp"
#include "EndpointImpl.hpp"


LibUSB::DeviceImpl::DeviceImpl( libusb_device* device )
	: languageId(0)
{

	// Add a reference to pDevice and save it.
	m_pDevice.reset(libusb_ref_device(device), DeviceDeleter());

}


LibUSB::DeviceImpl::~DeviceImpl()
{

	// Ensure that all Configuration objects have been released/destroyed.

		/// \todo Release all claimed interfaces here.

}

std::shared_ptr<libusb_device_descriptor> LibUSB::DeviceImpl::getDeviceDescriptor()
{

	if (m_pDeviceDescriptor.get() == nullptr)
	{

		m_pDeviceDescriptor.reset(new libusb_device_descriptor);

		// Obtain the usb device descriptors
		int Result = libusb_get_device_descriptor(m_pDevice.get(), m_pDeviceDescriptor.get());

		if (Result != LIBUSB_SUCCESS)
		{
			throw std::runtime_error("LibUSB::DeviceImpl::getDeviceDescriptor(): libusb_get_device_descriptor() failed.");
		}

	}

	return m_pDeviceDescriptor;

}


bool LibUSB::DeviceImpl::isOpen() const
{

	return (m_pHandle.get() != nullptr);

}

void LibUSB::DeviceImpl::Open()
{

	if (m_pDevice.get() == nullptr)
	{
		throw std::runtime_error("LibUSB::DeviceImpl::Open(): Failed, there is no device!");
	}

	if (m_pHandle.get() == nullptr)
	{

		// Open the device
		libusb_device_handle* pHandle;
		int Result = libusb_open(m_pDevice.get(), &pHandle);

		if (Result != LIBUSB_SUCCESS)
		{

			throw LibUSBException("LibUSB::DeviceImpl::Open(): libusb_open() failed: ", Result);

		}

		m_pHandle.reset(pHandle, DeviceHandleDeleter());

	}
}

std::string LibUSB::DeviceImpl::getStringDescriptor( uint8_t index )
{


	unsigned char descStr[512];
	memset(descStr, 0, sizeof(descStr));

	int Result = libusb_get_string_descriptor_ascii(m_pHandle.get(), index, descStr, sizeof(descStr));

	if (Result < LIBUSB_SUCCESS)
	{
		throw LibUSBException("LibUSB::DeviceImpl::getStringDescriptor(): libusb_get_string_descriptor_ascii() failed: ", Result);
	}

	std::string strResult;
	strResult.assign((const char*)descStr);

	return strResult;
}

std::wstring LibUSB::DeviceImpl::getStringDescriptorW( uint8_t index )
{
	unsigned char descStr[128];
	wmemset((wchar_t*)descStr, L'\0', sizeof(descStr)/sizeof(wchar_t));

	int Result = libusb_get_string_descriptor(m_pHandle.get(), index, getLangId(), (unsigned char*)descStr, sizeof(descStr));

	if (Result < LIBUSB_SUCCESS)
	{
		throw LibUSBException("LibUSB::DeviceImpl::getStringDescriptorW(): libusb_get_string_descriptor() failed: ", Result);
	}

	// First character is the size of the string descriptor, in bytes
	uint8_t descSize = descStr[0];

	// First character no zero size, second character is 0x03, always
	if (descSize < 2 || descStr[1] != LIBUSB_DT_STRING)
	{
		throw std::runtime_error("LibUSB::DeviceImpl::getStringDescriptorW(): USB string descriptor returned from device is invalid.");
	}



	std::wstring strResult;
	strResult.resize((descSize-2)/2);

	for (size_t i = 0; i < (descSize-2)/2; ++i) {
		unsigned char chr1 = (unsigned char)descStr[2 * i + 2];
		unsigned char chr2 = (unsigned char)descStr[2 * i + 3];

		// Note: this line suppose that you use `UTF-16-BE` both for
		// the std::string and the std::wstring. You'll have to swap
		// chr1 & chr2 if this is not the case.
		unsigned short val = (chr2 << 8)|(chr1);
		strResult[i] = (wchar_t)(val);
	}

	return strResult;
}

uint16_t LibUSB::DeviceImpl::getLangId()
{

	/// \note This descriptor is described here: http://www.beyondlogic.org/usbnutshell/usb5.shtml
	///       and here: http://www.usb.org/developers/docs/USB_LANGIDs.pdf

	if((languageId == 0) && isOpen())
	{
		uint8_t data[255];
		memset(data, '\0', sizeof(data));
		int Result = libusb_get_string_descriptor(m_pHandle.get(), 0, 0, data, sizeof(data));
		if (Result < LIBUSB_SUCCESS)
		{
			throw LibUSBException("LibUSB::DeviceImpl::getLangId(): libusb_get_string_descriptor() failed: ", Result);
		}

		// First element is the size of the descriptor, in bytes
		uint8_t descriptorSize = data[0];

		// Second element should be 0x03
		if (data[1] != 0x03)
		{
			throw std::runtime_error("LibUSB::DeviceImpl::getLangId(): USB language string descriptor (index 0) is invalid.");
		}

		// Grab the first/default language.
		languageId = data[2] | data[3]<<8;

	}

	return languageId;

}

bool LibUSB::DeviceImpl::getActiveConfiguration( uint8_t &ConfigValue )const
{
	// Obtain the currently active config
	libusb_config_descriptor * pConfig;

	int Result = libusb_get_active_config_descriptor(m_pDevice.get(), &pConfig);

	if (Result != LIBUSB_SUCCESS)
	{
		switch(Result)
		{
		case LIBUSB_ERROR_NOT_FOUND:

			// The device is in an unconfigured state.
			ConfigValue = 0;
			return false;
			break;

		default:
			throw LibUSBException("LibUSB::DeviceImpl::getActiveConfiguration(): libusb_get_active_config_descriptor() failed: ", Result);
			return false;
			break;
		}
	}

	// Obtain the index
	ConfigValue = pConfig->bConfigurationValue;

	return true;

}

std::shared_ptr<LibUSB::Configuration> LibUSB::DeviceImpl::getConfiguration( uint8_t ConfigValue )
{
	// Check for an existing object.
	if((m_ConfigurationMap.find(ConfigValue) != m_ConfigurationMap.end()) && (m_ConfigurationMap[ConfigValue].expired()))
	{

		return m_ConfigurationMap[ConfigValue].lock();

	}

	// Create a new configuration object
	libusb_config_descriptor* pConfig = nullptr;

	int Result = libusb_get_config_descriptor_by_value(m_pDevice.get(), ConfigValue, &pConfig);

	if (Result != LIBUSB_SUCCESS)
	{
		std::stringstream exceptionText;
		exceptionText << "LibUSB::DeviceImpl::getConfiguration(): libusb_get_config_descriptor() failed (index " << (int)ConfigValue << "). ";

		switch(Result)
		{
		case LIBUSB_ERROR_NOT_FOUND:

			exceptionText << "The requested configuration does not exist: ";
			break;

		default:

			exceptionText << "Uncategorized: ";
			break;
		}

		throw LibUSBException(exceptionText.str(), Result);

	}

	std::shared_ptr<ConfigurationImpl> pConfigImpl = std::make_shared<LibUSB::ConfigurationImpl>(pConfig, this->shared_from_this());

	std::shared_ptr<Configuration> pConfigurationObj = std::make_shared<LibUSB::Configuration>(pConfigImpl);

	// Save a weak_ptr to the configuration object to prevent duplication.
	m_ConfigurationMap.insert(std::make_pair(ConfigValue, pConfigurationObj));

	return pConfigurationObj;

}

void LibUSB::DeviceImpl::setActiveConfiguration( uint8_t ConfigValue )
{

	// Set the active configuration.
	int Result = libusb_set_configuration(m_pHandle.get(), ConfigValue);

	if (Result != LIBUSB_SUCCESS)
	{
		std::stringstream exceptionText;
		exceptionText << "LibUSB::DeviceImpl::setActiveConfiguration(): libusb_set_configuration() failed (index " << (int)ConfigValue << "). ";

		switch(Result)
		{
		case LIBUSB_ERROR_NOT_FOUND:

			exceptionText << "The requested configuration does not exist: ";
			break;

		case LIBUSB_ERROR_BUSY:

			exceptionText << "Interfaces are currently claimed: ";
			break;

		case LIBUSB_ERROR_NO_DEVICE:

			exceptionText << "Device has been disconnected: ";
			break;

		default:

			exceptionText << "Uncategorized: ";
			break;
		}

		throw LibUSBException(exceptionText.str(), Result);

	}

}

std::weak_ptr<LibUSB::Device> LibUSB::DeviceImpl::getDevice() const
{

	if (m_ParentDevice.expired())
	{
		throw std::logic_error("LibUSB::DeviceImpl::getDevice(): Expired pointer to parent LibUSB::Device object.");
	}

	return m_ParentDevice;

}

void LibUSB::DeviceImpl::setParentDevice( std::weak_ptr<Device> pParentDevice )
{

	if (pParentDevice.expired())
	{
		throw std::logic_error("LibUSB::DeviceImpl::setParentDevice(): New parent device pointer is expired.");
	}

	m_ParentDevice = pParentDevice;
}

std::shared_ptr<LibUSB::Endpoint> LibUSB::DeviceImpl::getControlEndpoint()
{

	if (m_pEndpointZero.get() == nullptr)
	{

		// Initialize Endpoint Zero Dummy Descriptor
		m_EndpointZeroDescriptor.bLength = 0x07;
		m_EndpointZeroDescriptor.bDescriptorType = libusb_descriptor_type::LIBUSB_DT_ENDPOINT;
		m_EndpointZeroDescriptor.bEndpointAddress = 0;
		m_EndpointZeroDescriptor.bmAttributes = 0;
		m_EndpointZeroDescriptor.wMaxPacketSize = getDeviceDescriptor()->bMaxPacketSize0;
		m_EndpointZeroDescriptor.bInterval = 0;

		// All your endpoints are belong to us.
		std::shared_ptr<LibUSB::EndpointImpl> pEPImpl = std::make_shared<LibUSB::EndpointImpl>(&m_EndpointZeroDescriptor, shared_from_this());
		m_pEndpointZero = std::make_shared<LibUSB::Endpoint>(pEPImpl);

	}

	return m_pEndpointZero;

}
