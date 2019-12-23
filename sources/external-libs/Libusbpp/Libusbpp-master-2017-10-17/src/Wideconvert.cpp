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

#include <algorithm>

/*
 * NOTE: from http://en.cppreference.com/w/c/string/multibyte/mbstowcs
 *   and from http://en.cppreference.com/w/c/string/multibyte/wcstombs
 *
 * As all bounds-checked functions, mbstowcs_s (wcstombs_s) is only
 * guaranteed to be available if __STDC_LIB_EXT1__ is defined by the
 * implementation and if the user defines __STDC_WANT_LIB_EXT1__ to
 * the integer constant 1 before including stdlib.h.
 */
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdlib.h>

#include "Wideconvert.hpp"

// FIXME: use the C++11
// see: http://stackoverflow.com/a/18374698
//      http://stackoverflow.com/q/15615136
//      http://stackoverflow.com/q/24802133
//      http://en.cppreference.com/w/cpp/header/codecvt

std::wstring LibUSB::Util::StringToWString( const std::string& ns )
{
#if defined(__STDC_LIB_EXT1__) || defined(_WIN32)
	size_t bufferSize;

	// first call to mbstowcs_s to get the target buffer size
	mbstowcs_s(&bufferSize, NULL, 0, ns.c_str(), ns.size());

	// create target buffer with required size
	wchar_t* buffer = new wchar_t[bufferSize];

	// second call to do the actual conversion
	mbstowcs_s(&bufferSize, buffer, bufferSize, ns.c_str(), ns.size());

	std::wstring result(buffer, bufferSize);
	delete[] buffer;
	return result;
#else
	// first call to wcstombs to get the target buffer size
	size_t bufferSize = mbstowcs (NULL, ns.c_str(), 0);

	// create target buffer with required size
	wchar_t* buffer = new wchar_t[bufferSize];

	// second call to do the actual conversion
	mbstowcs(buffer, ns.c_str(), bufferSize);

	std::wstring result(buffer, bufferSize);
	delete[] buffer;
	return result;
#endif
}

std::string LibUSB::Util::WStringToString( const std::wstring& ws )
{
#if defined(__STDC_LIB_EXT1__) || defined(_WIN32)
	size_t bufferSize;

	// first call to wcstombs_s to get the target buffer size
	wcstombs_s(&bufferSize, NULL, 0, ws.c_str(), ws.size());

	// create target buffer with required size
	char* buffer = new char[bufferSize];

	// second call to do the actual conversion
	wcstombs_s(&bufferSize, buffer, bufferSize, ws.c_str(), ws.size());

	std::string result(buffer, bufferSize);
	delete[] buffer;
	return result;
#else
	// first call to wcstombs to get the target buffer size
	size_t bufferSize = wcstombs(NULL, ws.c_str(), 0);

	// create target buffer with required size
	char* buffer = new char[bufferSize];

	// second call to do the actual conversion
	wcstombs(buffer, ws.c_str(), bufferSize);

	std::string result(buffer, bufferSize);
	delete[] buffer;
	return result;
#endif
}
