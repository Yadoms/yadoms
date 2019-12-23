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

#ifndef LIBUSBPP_INTERFACE_HPP
#define LIBUSBPP_INTERFACE_HPP

#include <string>
#include <memory>
#include <map>

#include <stdint.h>

#include <libusbpp/Endpoint.hpp>


namespace LibUSB
{

	/// Interface implementation
	class InterfaceImpl;

	/// USB Interface class
	class Interface
	{
	public:
		Interface(std::shared_ptr<InterfaceImpl> pImpl);
		~Interface();

		/// Returns the interface number
		int Number()const;

		/// Returns the alternate setting for this interface
		int AlternateSetting()const;

		/// Returns the interface class
		int Class()const;

		/// Returns the interface Subclass
		int SubClass()const;

		/// Returns the interface Protocol
		int Protocol()const;

		/// Returns a string descriptor for this interface.
		std::wstring DescriptorString()const;

		/// Returns the number of available alternate settings for this interface.
		int NumAlternateSettings()const;

		/// Claims this interface for use.
		void Claim();

		/// Returns TRUE if the interface is claimed.
		bool isClaimed()const;

		/// Releases this interface
		void Release();

		/// Sets the requested alternate setting
		void SetAlternate(uint8_t AlternateSetting = 0);

		/// Returns the number of endpoints this interface has.
		int NumEndpoints()const;

		/// Returns the number of a specified endpoint
		int getEPNumberByIndex(int index)const;

		/// Endpoint number container type (index -> number)
		typedef std::map<int, int> EndpointNumbers_t;

		/// Returns the number container of all endpoint indices
		EndpointNumbers_t getEndpointNumbers()const;

		/// Returns the specified endpoint
		std::shared_ptr<Endpoint> getEndpoint(int number);

	protected:
	private:


		/// Interface implementation object.
		std::shared_ptr<InterfaceImpl> m_pInterfaceImpl;
	};

}

#endif // LIBUSBPP_INTERFACE_HPP
