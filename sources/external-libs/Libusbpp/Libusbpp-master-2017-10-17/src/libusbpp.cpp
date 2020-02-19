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

#include <libusbpp.hpp>

#include "DeviceImpl.hpp"
#include "LibusbImpl.hpp"


std::list<std::shared_ptr<LibUSB::Device>> LibUSB::LibUSB::FindDevice( uint16_t vendorID, uint16_t productID, bool debugLibUSB /*=false*/, DeviceFactory_t factory /*= nullptr*/ )
{

	// Ensure libusb is initialized.
	Initialize(debugLibUSB);

	// Create a list of attached devices
	libusb_device **device_list = nullptr;

	ssize_t NumResults = libusb_get_device_list(Impl_->m_pLibusb_context.get(), &device_list);


	// Iterate each device.
	std::list<std::shared_ptr<Device>> deviceList;

	for (ssize_t i = 0; i < NumResults; i++)
	{

		// Create a device.
		std::shared_ptr<Device> pDevice;
		if(factory != nullptr)
		{

			pDevice = factory(std::make_shared<DeviceImpl>(device_list[i]));

		}
		else
		{
			pDevice.reset(new Device(std::make_shared<DeviceImpl>(device_list[i])));
		}

		pDevice->Init();

		// Check the device
		if ((pDevice->vendorID() == vendorID) && (pDevice->productID() == productID))
		{
			// Add device to the output list
			deviceList.push_back(pDevice);
		}

	}

	// Free the device list
	libusb_free_device_list(device_list, 1);

	return deviceList;

}

std::list<std::shared_ptr<LibUSB::Device>> LibUSB::LibUSB::FindDevice( uint16_t vendorID, uint16_t productID, std::wstring serialStr, bool debugLibUSB /*=false*/, DeviceFactory_t factory /*= nullptr*/ )
{

	// Get list of devices that match product/vendor id.
	std::list<std::shared_ptr<Device>> DeviceList = FindDevice(vendorID, productID, debugLibUSB, factory);

	std::list<std::shared_ptr<Device>> ResultList;

	// Open each device and locate a matching serial.
	for( std::shared_ptr<Device> &pDevice : DeviceList )
	{
		if(pDevice->SerialString() == serialStr)
		{

			ResultList.push_back(pDevice);
			break;
		}
	}

	return ResultList;
}

std::list<std::shared_ptr<LibUSB::Device>> LibUSB::LibUSB::FindAllDevices( bool debugLibUSB /*=false*/, DeviceFactory_t factory /*= nullptr*/ )
{

	// Ensure libusb is initialized.
	Initialize(debugLibUSB);

	// Create a list of attached devices
	libusb_device **device_list = nullptr;

	ssize_t NumResults = libusb_get_device_list(Impl_->m_pLibusb_context.get(), &device_list);


	// Iterate each device.
	std::list<std::shared_ptr<Device>> deviceList;

	for (ssize_t i = 0; i < NumResults; i++)
	{

		// Create a device.
		std::shared_ptr<Device> pDevice;
		if(factory != nullptr)
		{
			pDevice = factory(std::make_shared<DeviceImpl>(device_list[i]));
		}
		else
		{
			pDevice.reset(new Device(std::make_shared<DeviceImpl>(device_list[i])));
		}

		pDevice->Init();

		// Add device to the output list
		deviceList.push_back(pDevice);

	}

	// Free the device list
	libusb_free_device_list(device_list, 1);

	return deviceList;

}

void LibUSB::LibUSB::Initialize( bool debug )
{
	// Ensure libusb is initialized.
	if (Impl_.get() == nullptr)
	{
		Impl_.reset(new LibUSBImpl(debug));
	}
}

std::shared_ptr<LibUSB::LibUSBImpl> LibUSB::LibUSB::Impl_;
