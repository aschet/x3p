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

#ifndef _WIN32
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

#include "linux_environment.hxx"

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

using namespace OpenGPS;


int copyFile(const char* src, const char* dst)
{
	FILE* in_fd = nullptr, * out_fd = nullptr;
	size_t n_chars;
	const size_t BUFSIZE = 4096;
	char buf[BUFSIZE];


	// open files
	if ((in_fd = fopen(src, "r")) == nullptr)
	{
		return -1;
	}


	if ((out_fd = fopen(dst, "w")) == nullptr)
	{
		return -1;
	}

	// copy files
	while ((n_chars = fread(buf, 1, BUFSIZE, in_fd)) > 0)
	{
		if (fwrite(buf, 1, n_chars, out_fd) != n_chars)
		{
			// write error
			return -1;
		}
	}


	// close files
	if (fclose(in_fd) == -1 || fclose(out_fd) == -1)
	{
		// error closing file
		return -1;
	}

	return 0;
}

LinuxEnvironment::LinuxEnvironment()
	: Environment()
{
}

LinuxEnvironment::~LinuxEnvironment()
{
}

bool LinuxEnvironment::m_InitRandom = true;

#define GETRANDOM(min, max) ((rand()%(int)(((max) + 1)-(min)))+ (min))

std::filesystem::path LinuxEnvironment::GetUniqueName() const
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

bool LinuxEnvironment::GetVariable(const String& varName, String& value) const
{
	ResetLastErrorCode();

	String tempVarName(varName.c_str());
	char* varbuf = getenv(tempVarName.ToChar());

	if (!varbuf)
	{
		assert(varbuf == nullptr);

		return false;
	}

	value.FromChar(varbuf);

	return true;
}

String LinuxEnvironment::GetLastErrorMessage() const
{
	String message;
	message.FromChar(strerror(errno));

	return message;
}

void LinuxEnvironment::ResetLastErrorCode() const
{
	errno = 0;
}

std::unique_ptr<Environment> Environment::CreateInstance()
{
	return std::make_unique<LinuxEnvironment>();
}

#endif
