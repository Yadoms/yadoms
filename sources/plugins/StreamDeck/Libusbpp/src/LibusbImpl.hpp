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

#ifndef __LIBUSBPP_LIBUSB_IMPL_HPP
#define __LIBUSBPP_LIBUSB_IMPL_HPP

#include <memory>
#include <thread>
#include <atomic>
#include <condition_variable>

#include <libusb.h>


namespace LibUSB
{


	class LibUSBImpl
	{
	public:

		LibUSBImpl(bool debug = false);
		~LibUSBImpl();

		static std::shared_ptr<libusb_context> m_pLibusb_context;


	protected:

		const enum libusb_log_level m_LibUSBLogLevel;

	};

}

#endif // __LIBUSBPP_LIBUSB_IMPL_HPP
