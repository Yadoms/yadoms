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

#ifndef __LIBUSBPP_CONFIGURATION_IMPL_HPP
#define __LIBUSBPP_CONFIGURATION_IMPL_HPP

#include <memory>
#include <string>
#include <map>

#include <libusb.h>

#include <libusbpp/Interface.hpp>


namespace LibUSB
{
	/// Allows shared ptr to delete the configuration descriptor object.
	class ConfigDescriptorDeleter
	{
	public:
		void operator()(libusb_config_descriptor* configdescriptor) { libusb_free_config_descriptor(configdescriptor); };

	};

	/// DeviceImpl Forward Declaration
	class DeviceImpl;

	/// Configuration Implementation
	class ConfigurationImpl
	{
	public:

		/// Constructor - initializes
		ConfigurationImpl(libusb_config_descriptor* pConfigDescriptor, std::weak_ptr<DeviceImpl> pParentDeviceImpl);
		~ConfigurationImpl();

		/// Returns the string descriptor describing this configuration
		std::wstring DescriptorString(void)const;

		/// Returns the identifier value of this configuration
		uint8_t getValue()const;

		/// Returns the maximum amount of power this device will consume while fully operational. (units of 2mA)
		uint8_t getMaxPower()const;

		/// Sets this configuration as the active configuration.
		void SetAsActive();

		/// Returns TRUE if the device is self powered.
		bool isSelfPowered()const;

		/// Returns TRUE if the device supports remote wakeup.
		bool supportsRemoteWakeup()const;

		/// Returns TRUE if there are extra descriptors present
		bool hasExtraDescriptors()const;

		/// Returns a pointer the the extra descriptors
		const unsigned char* getExtraDescriptors()const;

		/// Returns the size of the extra descriptors, in bytes.
		int getExtraDescriptorSize()const;

		/// Returns the number of interfaces this configuration has.
		int NumInterfaces()const;

		/// Returns the requested interface (by index)
		std::shared_ptr<Interface> getInterfaceByIndex( int index )const;

		/// Returns the requested interface (by bInterfaceNumber)
		std::shared_ptr<Interface> getInterface(int InterfaceNumber)const;
	protected:

		/// Create all interfaces
		void CreateInterfaces();


	private:

		/// Interface Objects
		typedef std::map<int, std::shared_ptr<LibUSB::Interface>> InterfaceMap_t;
		InterfaceMap_t m_InterfaceMap;

		/// Configuration Descriptor
		std::shared_ptr<libusb_config_descriptor> m_pConfigDescriptor;

		/// Parent Device
		std::weak_ptr<DeviceImpl> m_pDeviceImpl;
	};

}

#endif // __LIBUSBPP_CONFIGURATION_IMPL_HPP
