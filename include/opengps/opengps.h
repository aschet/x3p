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

/*! \addtogroup C
 *  @{
 */

/*! @file
 * Common define's and typedef's.
 * @see cxx/opengps.hxx
 */

#ifndef _OPENGPS_H
#define _OPENGPS_H

#if _UNICODE
   #include <wchar.h>
#endif

#include "cxx/version.h"

#ifdef _WIN32
#  ifdef BUILD_ISO5436_2_XML_DLL
/*!
 * Manages export and import of symbols when used as shared library.
 * @remarks If you use openGPS as a shared library within you project
 * you must set the SHARED_OPENGPS_LIBRARY flag when compiling your source code!
 */
#     define _OPENGPS_EXPORT __declspec(dllexport)
#  else
#     ifdef BUILD_ISO5436_2_XML
/*!
 * Manages export and import of symbols when used as shared library.
 * @remarks If you use openGPS as a shared library within you project
 * you must set the SHARED_OPENGPS_LIBRARY flag when compiling your source code!
 */
#        define _OPENGPS_EXPORT
#    else
/* SHARED_OPENGPS_LIBRARY must be specified by the
user of a shared library version of openGPS.
Otherwise no symbols will be imported. This is
needed to distinguish between includes for
static and shared library usage scenarios. */
#        ifdef SHARED_OPENGPS_LIBRARY
/*!
 * Manages export and import of symbols when used as shared library.
 * @remarks If you use openGPS as a shared library within you project
 * you must set the SHARED_OPENGPS_LIBRARY flag when compiling your source code!
 */
#          define _OPENGPS_EXPORT __declspec(dllimport)
#        else
/*!
 * Manages export and import of symbols when used as shared library.
 * @remarks If you use openGPS as a shared library within you project
 * you must set the SHARED_OPENGPS_LIBRARY flag when compiling your source code!
 */
#          define _OPENGPS_EXPORT
#        endif
#     endif
#  endif
#else
/*!
 * Manages export and import of symbols when used as shared library.
 * @remarks If you use openGPS as a shared library within you project
 * you must set the SHARED_OPENGPS_LIBRARY flag when compiling your source code!
 */
#  define _OPENGPS_EXPORT
#endif

#include <stdbool.h>

typedef bool OGPS_Boolean;

#ifdef _UNICODE
/*! The current type of characters. This is either unicode (wchar_t) or char. */
#ifndef _WIN32
    #define _T(xx) L ## xx
#endif
typedef wchar_t OGPS_Character;
#else
/*! The current type of characters. This is either unicode (wchar_t) or char. */
#ifndef _WIN32
    #define _T(xx) xx
#endif
typedef char OGPS_Character;
#endif

#include <stdint.h>

/*! Represents measurement data of type short. */
typedef int16_t OGPS_Int16;
/*! Represents measurement data of type int. */
typedef int32_t OGPS_Int32;
/*! Represents measurement data of type float. */
typedef float OGPS_Float;
/*! Represents measurement data of type double. */
typedef double OGPS_Double;
/*! Represents most index ranges, still defined for backwards comptatibility. */
typedef size_t OGPS_ULong;

/*! Comprises text used to describe raised exceptions, that might be localized. */
#define _EX_T(x) (x)

/*! Gives the revision name of an ISO 5436-2 document.
    The xerces tool does not generate an appropriate enum value here. */
#define OGPS_ISO5436_2000_REVISION_NAME _T("ISO5436 - 2000")

/*! Gives the name of the surface feature type in an ISO 5436-2 document.
    The xerces tool does not generate an appropriate enum value here. */
#define OGPS_FEATURE_TYPE_SURFACE_NAME _T("SUR")

/*! Gives the name of the profile feature type in an ISO 5436-2 document.
    The xerces tool does not generate an appropriate enum value here. */
#define OGPS_FEATURE_TYPE_PROFILE_NAME _T("PRF")

/*! Gives the name of the point cloud feature type in an ISO 5436-2 document.
    The xerces tool does not generate an appropriate enum value here. */
#define OGPS_FEATURE_TYPE_POINTCLOUD_NAME _T("PCL")

/// Build a version string from version numbers
// This macro is necessary to make a number to string conversion
#ifdef _UNICODE
#  define _OPENGPS_MSTR(x) L ## #x
#else
#  define _OPENGPS_MSTR(x) #x
#endif

#define _OPENGPS_VERSIONSTRING_M(ver,mver,build) _OPENGPS_MSTR(ver) _T(".") _OPENGPS_MSTR(mver) _T(".") _OPENGPS_MSTR(build)

/// Build a version string
#define _OPENGPS_VERSIONSTRING _OPENGPS_VERSIONSTRING_M(_OPENGPS_VERSION, _OPENGPS_MINVERSION, _OPENGPS_BUILD)

/// Define ID string with programm name and Version
#define _OPENGPS_ID _OPENGPS_NAME _T(" (V") _OPENGPS_VERSIONSTRING _T(")")

#define _OPENGPS_DELETE(x) if((x) != nullptr) { delete (x); (x) = nullptr; }
#define _OPENGPS_DELETE_ARRAY(x) if((x) != nullptr) { delete[] (x); (x) = nullptr; }
#define _OPENGPS_FREE(x) if((x) != NULL) { free(x); (x) = NULL; }

#define _OPENGPS_DBL_EPSILON 2.2204460492503131e-016 /* smallest such that 1.0 + _OPENGPS_DBL_EPSILON != 1.0 */
#define _OPENGPS_DBL_EQ(a, b) (fabs((a) - (b)) < (_OPENGPS_DBL_EPSILON)) /* a equals b */
#define _OPENGPS_DBL_EQN(a) _OPENGPS_DBL_EQ(a, 0.0) /* a equals 0.0 */

#endif
/*! @} */
