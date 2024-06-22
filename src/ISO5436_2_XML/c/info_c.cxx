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

#include <opengps/info.h>
#include <opengps/cxx/info.hxx>
#include <opengps/cxx/string.hxx>
#include "messages_c.hxx"
#include "../cxx/stdafx.hxx"

size_t ogps_GetNameInfo(OGPS_Character* text, size_t size)
{
	return HandleExceptionRetval(0, [&]() {
		String src;
		Info::GetName(&src);
		return src.CopyTo(text, size);
	});
}

size_t ogps_GetAboutInfo(OGPS_Character* text, size_t size)
{
	return HandleExceptionRetval(0, [&]() {
		String src;
		Info::GetAbout(&src);
		return src.CopyTo(text, size);
	});
}

size_t ogps_GetVersionInfo(OGPS_Character* text, size_t size)
{
	return HandleExceptionRetval(0, [&]() {
		String src;
		Info::GetVersion(&src);
		return src.CopyTo(text, size);
	});
}

void ogps_PrintVersionInfo()
{
	HandleException([&]() {
		Info::PrintVersion();
	});
}

size_t ogps_GetCopyrightInfo(OGPS_Character* text, size_t size)
{
	return HandleExceptionRetval(0, [&]() {
		String src;
		Info::GetCopyright(&src);
		return src.CopyTo(text, size);
	});
}

void ogps_PrintCopyrightInfo()
{
	HandleException([&]() {
		Info::PrintCopyright();
	});
}

size_t ogps_GetLicenseInfo(OGPS_Character* text, size_t size)
{
	return HandleExceptionRetval(0, [&]() {
		String src;
		Info::GetLicense(&src);
		return src.CopyTo(text, size);
	});
}

void ogps_PrintLicenseInfo()
{
	HandleException([&]() {
		Info::PrintLicense();
	});
}
