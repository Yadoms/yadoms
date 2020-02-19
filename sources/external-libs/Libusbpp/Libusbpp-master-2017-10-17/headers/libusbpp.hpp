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

#ifndef LIBUSBPP_HPP
#define LIBUSBPP_HPP

#include <memory>
#include <list>

#include <libusbpp/Device.hpp>
#include <libusbpp/Transfer.hpp>
#include <libusbpp/EndpointDefs.hpp>


namespace LibUSB
{

	/// LibUSBImpl forward declaration
	class LibUSBImpl;

	/// Contains static methods for enumerating devices
	class LibUSB
	{

	public:

		/// Function pointer to a LibUSB device object factory.
		/// \todo Replace with std::function?
		typedef std::shared_ptr<Device> (*DeviceFactory_t)(std::shared_ptr<DeviceImpl>);

		/*!
		 * \brief
		 *
		 * Returns a list of devices (that can be opened) that match the given vendor/product id.
		 *
		 * \param vendorID (uint16_t): USB-IF vendor id of the desired device.
		 * \param deviceID (uint16_t): USB-IF product id of the desired device.
		 * \param debugLibUSB (bool): Enable LibUSB debug output on standard error.
		 * \returns (std::list<std::shared_ptr<D>>): List of shared pointers to LibUSB::Device class objects.
		 * \sa
		 * \note
		 * \warning Multiple devices can be returned via this method, if attached.
		 */
		static std::list<std::shared_ptr<Device>> FindDevice(uint16_t vendorID, uint16_t productID, bool debugLibUSB = false, DeviceFactory_t factory = nullptr);

		/*!
		 * \brief
		 *
		 * Returns a list of devices (that can be opened) that match the given vendor/product id.
		 *
		 * \param vendorID (uint16_t): USB-IF vendor id of the desired device.
		 * \param deviceID (uint16_t): USB-IF product id of the desired device.
		 * \param serialStr (std::wstring): Device unique serial number
		 * \param debugLibUSB (bool): Enable LibUSB debug output on standard error.
		 * \returns (std::list<std::shared_ptr<D>>): List of shared pointers to LibUSB::Device class objects.
		 * \sa
		 * \note
		 * \warning Multiple devices can be returned via this method, if attached.
		 */
		static std::list<std::shared_ptr<Device>> FindDevice(uint16_t vendorID, uint16_t productID, std::wstring serialStr, bool debugLibUSB = false, DeviceFactory_t factory = nullptr);

		/// Returns all devices attached to the system.
		static std::list<std::shared_ptr<Device>> FindAllDevices(bool debugLibUSB = false, DeviceFactory_t factory = nullptr);

	private:

		friend class TransferImpl;

		static void Initialize(bool debug);


		/// LibUSBImpl Singleton object
		static std::shared_ptr<LibUSBImpl> Impl_;



	};


}

#endif // LIBUSBPP_HPP
