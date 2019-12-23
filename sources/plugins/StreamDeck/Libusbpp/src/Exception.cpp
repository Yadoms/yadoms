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

#include <sstream>

#include <libusb.h>

#include <libusbpp/Exception.hpp>


LibUSB::LibUSBException::LibUSBException( std::string text, int errorCode )
	: runtime_error(text.append(translateError(errorCode))), m_ErrorCode(errorCode)
{

}

std::string LibUSB::LibUSBException::translateError( int ErrorCode )
{
	std::string Result;

	switch(ErrorCode)
	{

	case LIBUSB_SUCCESS:
			Result = "LIBUSB_SUCCESS - Success (no error)";
			break;

	case LIBUSB_ERROR_IO:
			Result = "LIBUSB_ERROR_IO - Input/output error.";
			break;

	case LIBUSB_ERROR_INVALID_PARAM:
			Result = "LIBUSB_ERROR_INVALID_PARAM - Invalid parameter.";
			break;

	case LIBUSB_ERROR_ACCESS:
			Result = "LIBUSB_ERROR_ACCESS - Access denied (insufficient permissions)";
			break;

	case LIBUSB_ERROR_NO_DEVICE:
			Result = "LIBUSB_ERROR_NO_DEVICE - No such device (it may have been disconnected)";
			break;

	case LIBUSB_ERROR_NOT_FOUND:
			Result = "LIBUSB_ERROR_NOT_FOUND - Entity not found.";
			break;

	case LIBUSB_ERROR_BUSY:
			Result = "LIBUSB_ERROR_BUSY - Resource busy.";
			break;

	case LIBUSB_ERROR_TIMEOUT:
			Result = "LIBUSB_ERROR_TIMEOUT - Operation timed out.";
			break;

	case LIBUSB_ERROR_OVERFLOW:
			Result = "LIBUSB_ERROR_OVERFLOW - Overflow.";
			break;

	case LIBUSB_ERROR_PIPE:
			Result = "LIBUSB_ERROR_PIPE - Pipe error.";
			break;

	case LIBUSB_ERROR_INTERRUPTED:
			Result = "LIBUSB_ERROR_INTERRUPTED - System call interrupted (perhaps due to signal)";
			break;

	case LIBUSB_ERROR_NO_MEM:
			Result = "LIBUSB_ERROR_NO_MEM - Insufficient memory.";
			break;

	case LIBUSB_ERROR_NOT_SUPPORTED:
			Result = "LIBUSB_ERROR_NOT_SUPPORTED - Operation not supported or unimplemented on this platform.";
			break;

	case LIBUSB_ERROR_OTHER:
			Result = "LIBUSB_ERROR_OTHER - Other error.";
			break;
	}

	return Result;
}

const char* LibUSB::LibUSBException::what()
{

	std::stringstream errStr;
	errStr <<  exception::what() << this->translateError(m_ErrorCode);
	return errStr.str().c_str();
}
