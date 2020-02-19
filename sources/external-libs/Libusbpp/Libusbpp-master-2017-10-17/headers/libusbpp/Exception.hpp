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

#ifndef LIBUSBPP_EXCEPTION_HPP
#define LIBUSBPP_EXCEPTION_HPP

#include <exception>
#include <stdexcept>


namespace LibUSB
{

	class LibUSBException : public std::runtime_error
	{
	public:
		LibUSBException(std::string text, int errorCode);
		std::string translateError(int ErrorCode);

		/// Returns the raw libusb error code.
		int getLibusbErrorCode()const { return m_ErrorCode; };

		virtual const char* what();
	protected:

		int m_ErrorCode;
	private:
	};

}

#endif // LIBUSBPP_EXCEPTION_HPP
