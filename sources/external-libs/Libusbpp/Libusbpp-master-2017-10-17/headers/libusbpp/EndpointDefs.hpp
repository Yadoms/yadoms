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

#ifndef LIBUSBPP_ENDPOINT_DEFS_HPP
#define LIBUSBPP_ENDPOINT_DEFS_HPP

namespace LibUSB
{

	/// Direction Type
	typedef enum { DIR_OUT, DIR_IN } Direction_t;

	/// Transfer Types
	typedef enum { CONTROL, ISOCHRONOUS, BULK, INTERRUPT } Transfer_t;

	/// Synchronization Types
	typedef enum { NONE, ASYNCRONOUS , ADAPTIVE, SYNCHRONOUS } Synchronization_t;

	/// Usage Type
	typedef enum { DATA, FEEDBACK, EXPLICITFEEDBACKDATA, RESERVED } Usage_t;


}

#endif // LIBUSBPP_ENDPOINT_DEFS_HPP
