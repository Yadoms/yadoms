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

#include <iostream>

#include <libusbpp/Exception.hpp>

#include "LibusbImpl.hpp"


/// Used by shared_ptr to delete a libusb context
class ContextDeleter
{
public:
	void operator()(libusb_context* ctx) { libusb_exit(ctx); };

};

LibUSB::LibUSBImpl::LibUSBImpl( bool debugLibUSB /*=false*/ )
    : m_LibUSBLogLevel ( debugLibUSB ? LIBUSB_LOG_LEVEL_DEBUG : LIBUSB_LOG_LEVEL_NONE )
{
	// Create the libusb context
	libusb_context* pContext = nullptr;
	int Result = libusb_init(&pContext);
	if (Result != LIBUSB_SUCCESS)
	{
		throw std::runtime_error("LibUSB::LibUSBImpl::LibUSBImpl(): libusb_init() failed.");
	}

	// Set log message verbosity
	libusb_set_debug(pContext, m_LibUSBLogLevel);

	// Store in a shared_ptr
	m_pLibusb_context.reset(pContext, ContextDeleter());


}

LibUSB::LibUSBImpl::~LibUSBImpl()
{

}


std::shared_ptr<libusb_context> LibUSB::LibUSBImpl::m_pLibusb_context;
