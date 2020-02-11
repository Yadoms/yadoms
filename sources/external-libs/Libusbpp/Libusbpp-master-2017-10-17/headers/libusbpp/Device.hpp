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

#ifndef LIBUSBPP_DEVICE_HPP
#define LIBUSBPP_DEVICE_HPP

#include <stdint.h>
#include <string>
#include <memory>

#include <libusbpp/Configuration.hpp>
#include <libusbpp/Endpoint.hpp>


namespace LibUSB
{

	/// Libusb Device Implementation Forward Declaration
	class DeviceImpl;

	/// Libusb device interface
	class Device : public std::enable_shared_from_this<Device>
	{
	public:

		Device(std::shared_ptr<DeviceImpl> pInit);

		virtual ~Device();

		void Init();

	// Device State

		/// Returns TRUE if the device is open.
		bool isOpen();

		/// Opens the device.
		void Open();

	// Device Descriptor

		/// USB specification release number
		uint16_t USBSpecification();

		/// USB-IF class code
		uint8_t DeviceClass();

		/// USB-IF subclass code for the device, qualified by the bDeviceClass value.
		uint8_t DeviceSubclass();

		/// USB-IF protocol code for the device, qualified by the bDeviceClass and bDeviceSubClass values.
		uint8_t DeviceProtocol();

		/// USB-IF vendor ID
		uint16_t vendorID();

		/// USB-IF product ID
		uint16_t productID();

		/// Returns a string describing the product.
		std::wstring ProductString();

		/// Returns a string describing the manufacturer.
		std::wstring ManufacturerString();

		/// Returns the serial number string of the device.
		std::wstring SerialString();

		/// Returns the number of possible configurations for this device.
		uint8_t NumConfigurations();

	// Configurations

		/// Returns the requested configuration
		std::shared_ptr<Configuration> getConfiguration(uint8_t ConfigValue);

		/// Returns the activeConfiguration
		std::shared_ptr<Configuration> getActiveConfiguration();

	// Control endpoint

		/// Returns the control endpoint (Endpoint 0)
		std::shared_ptr<Endpoint> getControlEndpoint();



	protected:

		/// Transfers need access to the transfer event notification method of their target device.
		friend class TransferImpl;

		/// Notification of a completed transfer
		/// \warning This function can be called from other threads when using asynchronous transfers!
		virtual void TransferEventNotification(std::shared_ptr<Transfer> pCompletedTransfer);


	private:

		/// Cached Active Configuration
		std::shared_ptr<Configuration> m_pActiveConfiguration;

		/// Device Implementation Object
		std::shared_ptr<DeviceImpl> m_pDeviceImpl_;



	};

}

#endif // LIBUSBPP_DEVICE_HPP
