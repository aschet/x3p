/***************************************************************************
 *   Copyright by Johannes Herwig (NanoFocus AG) 2007                      *
 *   Copyright by Georg Wiora (NanoFocus AG) 2007                          *
 *                                                                         *
 *   This file is part of the openGPS (R)[TM] software library.            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License (LGPL)    *
 *   as published by the Free Software Foundation; either version 3 of     *
 *   the License, or (at your option) any later version.                   *
 *   for detail see the files "licence_LGPL-3.0.txt" and                   *
 *   "licence_GPL-3.0.txt".                                                *
 *                                                                         *
 *   openGPS is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   The name "openGPS" and the logo are registered as                     *
 *   European trade mark No. 006178354 for                                 *
 *   Physikalisch Technische Bundesanstalt (PTB)                           *
 *   http://www.ptb.de/                                                    *
 *                                                                         *
 *   More information about openGPS can be found at                        *
 *   http://www.opengps.eu/                                                *
 ***************************************************************************/

#ifdef _WIN32

#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "win32_environment.hxx"
#include "stdafx.hxx"

#include <opengps/cxx/string.hxx>

#include <sstream>

#ifdef _UNICODE

namespace OpenGPS
{
	typedef std::wostringstream OutStringStream;
}

#else

namespace OpenGPS
{
	typedef std::ostringstream OutStringStream;
}

#endif

Win32Environment::Win32Environment()
	: Environment()
{
}

Win32Environment::~Win32Environment()
{
}

bool Win32Environment::m_InitRandom = true;

#define GETRANDOM(min, max) ((rand()%(int)(((max) + 1)-(min)))+ (min))

std::filesystem::path Win32Environment::GetUniqueName() const
{
	if (m_InitRandom)
	{
		srand((unsigned int)time(nullptr));
		m_InitRandom = false;
	}

	// random positive number with max. 8 digits
	int randNum = GETRANDOM(0, 10000000);

	OutStringStream os;
	os << randNum;

	return os.str();
}

bool Win32Environment::GetVariable(const String& varName, String& value) const
{
	ResetLastErrorCode();

	const size_t bufferLength = GetEnvironmentVariable(varName.c_str(), nullptr, 0);

	if (bufferLength == 0)
	{
		assert(GetLastError() == ERROR_ENVVAR_NOT_FOUND);

		return false;
	}

	value.resize(bufferLength - 1);
	assert(value.size() == bufferLength - 1);

	/* This readonly->read/write cast should be safe here. GetEnvironmentVariable is not supposed
	to vary buffer size nor its location. Only new content should be copied in. */
	if (GetEnvironmentVariable(varName.c_str(), (LPTSTR)value.c_str(), static_cast<DWORD>(bufferLength)) != bufferLength - 1)
	{
		value.erase();
		return false;
	}
	else
	{
		String unescaped;
		unescaped.assign(value);

		const size_t bufferLength2 = ExpandEnvironmentStrings(unescaped.c_str(), nullptr, 0);

		if (bufferLength2 == 0)
		{
			assert(GetLastError() == ERROR_ENVVAR_NOT_FOUND);

			return false;
		}

		value.resize(bufferLength2 - 1);

		/* Wired cast should be safe here. Same as above. */
		/* This assumes we use Unicode here. See API for ExpandEnvironmentStrings. */
		if (ExpandEnvironmentStrings(unescaped.c_str(), (LPTSTR)value.c_str(), static_cast<DWORD>(bufferLength2)) != bufferLength2)
		{
			value.erase();
			return false;
		}
	}

	return true;
}

String Win32Environment::GetLastErrorMessage() const
{
	const DWORD error_code = GetLastError();
	LPTSTR lpMsgBuf = nullptr;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		error_code,
		0,
		(LPTSTR)&lpMsgBuf,
		0,
		nullptr);
	String message;
	if (lpMsgBuf != nullptr)
	{
		message.assign(lpMsgBuf);
		LocalFree(lpMsgBuf);
	}
	return message;
}

void Win32Environment::ResetLastErrorCode() const
{
	SetLastError(0);
}

std::unique_ptr<Environment> Environment::CreateInstance()
{
	return std::make_unique<Win32Environment>();
}

#endif
