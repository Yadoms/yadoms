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

#ifndef LIBUSBPP_TRANSFER_DEFS_HPP
#define LIBUSBPP_TRANSFER_DEFS_HPP

namespace LibUSB
{

	/// Control Request Direction
	typedef enum { HOST_TO_DEVICE, DEVICE_TO_HOST } DataTransferDirection_t;

	/// Control Request Type
	typedef enum { REQ_STANDARD, REQ_CLASS, REQ_VENDOR, REQ_RESERVED } RequestType_t;

	/// Control request recipient Type
	typedef enum { REC_DEVICE, REC_INTERFACE, REC_ENDPOINT, REC_OTHER } RequestRecipient_t;

	/// Transfer Results
	typedef enum { COMPLETED, FAILED, TIMED_OUT, CANCELLED, STALLED, NO_DEVICE, DATA_OVERFLOW} TransferResult_t;

}

#endif // LIBUSBPP_TRANSFER_DEFS_HPP
