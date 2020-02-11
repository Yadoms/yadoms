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

#ifndef LIBUSBPP_CONFIGURATION_HPP
#define LIBUSBPP_CONFIGURATION_HPP

#include <memory>
#include <string>
#include <stdint.h>

#include <libusbpp/Interface.hpp>


namespace LibUSB
{



	/// Configuration Implementation
	class ConfigurationImpl;

	class Configuration
	{
	public:
		Configuration(std::shared_ptr<ConfigurationImpl> pInit);
		~Configuration();

	// Configuration Descriptor Details.

		/// Returns the string descriptor describing this configuration
		std::wstring DescriptorString()const;

		/// Returns the identifier value of this configuration
		uint8_t Value()const;

		/// Returns the maximum amount of power this device will consume while fully operational. (mA)
		int MaxPower()const;

		/// Sets this configuration as the active configuration.
		void SetAsActive();

		/// Returns TRUE if the device is self powered.
		bool isSelfPowered()const;

		/// Returns TRUE if the device supports remote wakeup.
		bool supportsRemoteWakeup()const;

		/// Returns TRUE if there are extra descriptors present
		bool hasExtraDescriptors()const;

		/// Returns a pointer the the extra descriptors
		const unsigned char * getExtraDescriptors()const;

		/// Returns the size of the extra descriptors, in bytes.
		int getExtraDescriptorSize()const;

	// Interfaces

		/// Returns the number of interfaces supported by this configuration.
		int NumInterfaces()const;

		/// Returns the specified interface by index.
		std::shared_ptr<Interface> getInterfaceByIndex(int index)const;

		/// Returns the specified interface by Interface number.
		std::shared_ptr<Interface> getInterface(int InterfaceNumber)const;

	protected:


	private:

		/// Configuration Implementation
		std::shared_ptr<ConfigurationImpl> m_pConfigImpl_;
	};

}

#endif // LIBUSBPP_CONFIGURATION_HPP
