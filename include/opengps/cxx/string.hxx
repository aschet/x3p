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

/*! @defgroup Cpp C++ Interface for X3P-library
   @addtogroup Cpp
   @{ */


/*! @file
 * An enhanced std::string string type.
 */

#ifndef _OPENGPS_CXX_STRING_HXX
#define _OPENGPS_CXX_STRING_HXX

#include <opengps/cxx/opengps.hxx>
#include <iostream>
#include <string>
#include <array>
#include <memory>

namespace OpenGPS
{
	/*!
	 * Stores an ::OGPS_Character sequence.
	 *
	 * @todo Add a streaming input operator
	 */
	class _OPENGPS_EXPORT String : public

#ifdef _UNICODE
		std::wstring
#else
		std::string
#endif

	{
#ifdef _UNICODE
		/*! The std::string<::OGPS_Character> type.*/
		typedef std::wstring BaseType;
#else
		/*! The std::string<::OGPS_Character> type.*/
		typedef std::string BaseType;
#endif

	public:
		/*! The ::OGPS_Character type.*/
		typedef OGPS_Character ElementType;

	public:
		/*! Creates a new instance. */
		String();

		/*!
		 * Creates a new instance.
		 *
		 * @param s Initialize the newly created object with the given character sequence.
		 */
		String(const BaseType& s);

		/*!
		 * Creates a new instance.
		 *
		 * @param s Initialize the newly created object with the given character sequence.
		 */
		String(const OGPS_Character* s);

		String(const String& s);

		String& operator=(const String& rhs);

		/*! Destructs an object. */
		~String();

		/*!
		 * Converts the unicode character sequence to ANSI char.
		 * @returns An ANSI char pointer or nullptr.
		 */
		const char* ToChar();

		/*!
		 * Stores an ANSI char squence.
		 * @param s An ANSI char squence to store as unicode internally.
		 */
		void FromChar(const char* s);

		/*!
		 * Stores an ANSI char squence.
		 * @param s An ANSI char squence to store as unicode internally.
		 * @param length Amount of characters to be copied.
		 */
		void FromChar(const char* s, size_t length);

		/*!
		 * Stores an MD5 sum as a character sequence in hexadecimal format.
		 *
		 * @param md5 The 128-Bit MD5 value to be storedin hexadecimal format.
		 * @returns Returns true on success, false otherwise.
		 */
		bool ConvertFromMd5(const std::array<unsigned char, 16>& md5);

		/*!
		 * Converts the current character squence representing a 128-Bit MD5 sum
		 * in hexadecimal format to the binary representation of that MD5 sum.
		 *
		 * @param md5 Gets the converted MD5 binary values.
		 * @returns Returns true on success, false otherwise.
		 */
		bool ConvertToMd5(std::array<unsigned char, 16>& md5) const;

		/*!
		 * Copies the current character sequence to an external buffer.
		 *
		 * @param target The external buffer.
		 * @param size The size of the external buffer in characters.
		 * @returns On success returns the number of chracters copied to the
		 * target buffer without the terminating null character. On failure
		 * returns the number of characters the target buffer must be able to store.
		 */
		size_t CopyTo(OGPS_Character* const target, const size_t size) const;

		/*!
		 * Replaces all occurances of a given string with another.
		 *
		 * @param old_str The string to be replaced.
		 * @param new_str The string inserted for a match.
		 * @returns This instance.
		 */
		String& ReplaceAll(const String& old_str, const String& new_str);

#ifdef _UNICODE
	private:
		std::unique_ptr<char[]> m_Chars;
#endif
	};

}


#ifdef _UNICODE
/*!
 * Streams unicode strings to ansi character streams.
 *
 * Takes care of the appropriate conversion, @see OpenGPS::String::ToChar.
 */
inline std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, const std::basic_string<wchar_t>& text)
{
	OpenGPS::String buffer(text);
	return os << buffer.ToChar();
}

/*!
 * Streams unicode strings to ansi character streams.
 *
 * Takes care of the appropriate conversion, @see OpenGPS::String::ToChar.
 */
inline std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, const OpenGPS::String& text)
{
	OpenGPS::String buffer(text);
	return os << buffer.ToChar();
}

/*!
 * Streams unicode strings to ansi character streams.
 *
 * Takes care of the appropriate conversion, @see OpenGPS::String::ToChar.
 */
inline std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, OpenGPS::String& text)
{
	return os << text.ToChar();
}

/*!
 * Streams OpenGPS::String type.
 */
inline std::basic_ostream<wchar_t>& operator<<(std::basic_ostream<wchar_t>& os, const OpenGPS::String& text)
{
	return os << text.c_str();
}
#else
/*!
 * Streams OpenGPS::String type.
 */
inline std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, const OpenGPS::String& text)
{
	return os << text.c_str();
}
#endif


#endif

/*! @} */
