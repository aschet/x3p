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

/*! @file
 * An interface to provide access to the environment of different
 * architectures and operating systems.
 */

#ifndef _OPENGPS_ENVIRONMENT_HXX
#define _OPENGPS_ENVIRONMENT_HXX

#include <opengps/cxx/opengps.hxx>
#include <memory>
#include <filesystem>

namespace OpenGPS
{
	class String;

	/*!
	 * Interface for communicating with the operating system and related subjects.
	 * This encapsulates the differences between hardware architectures, compilers
	 * and operation systems this software library will run upon.
	 */
	class Environment
	{
	public:
		/*! Destroys this instance. */
		virtual ~Environment();

		/*!
		 * Creates a new instance of the environment if needed.
		 *
		 * @remarks You must call Environment::Reset somewhere within the scope
		 * this method has been accessed, e.g. the end of object lifetime within a class scope
		 * or at least once for all on termination of the program.
		 * Otherwise resources obtained will not get released correctly.
		 */
		static const Environment* GetInstance();

		/*!
		 * Frees resources obtained through Environment::GetInstance.
		 *
		 * @remarks This must be called at least once at the end of some scope in your program
		 * when Environment::GetInstance has been called.
		 * @see Environment::GetInstance
		 */
		static void Reset();

		/*!
		 * Gets a random sequence of numeric characters.
		 */
		virtual std::filesystem::path GetUniqueName() const = 0;

		/*!
		 * Gets the value of a named environment variable.
		 *
		 * @param varName The name of the environment variable.
		 * @param value Gets the value of the given environment variable.
		 * @returns Returns true if the value of the environment variable could be obtained, false otherwise.
		 */
		virtual bool GetVariable(const String& varName, String& value) const = 0;

		/*!
		 * Returns the last error message of a failed system API call.
		 */
		virtual String GetLastErrorMessage() const = 0;

		/*!
		 * Returns true on a system with little endian byte order, false on a system using big endian byte order.
		 */
		static bool IsLittleEndian();

		/*!
		 * Swaps endianness.
		 *
		 * @param value The bytes to be swapped as typed data.
		 * @param dst The array of bytes in swapped byte order.
		 * @returns Pointer to the array of bytes in swapped byte order.
		 */
		static unsigned char* ByteSwap(OGPS_Int16 cvalue, unsigned char* dst);

		/*!
		 * Swaps endianness.
		 *
		 * @param src The array of bytes to be swapped.
		 * @param value The swapped bytes as typed data.
		 */
		static void ByteSwap(const unsigned char* src, OGPS_Int16& value);

		/*!
		 * Swaps endianness.
		 *
		 * @param value The bytes to be swapped as typed data.
		 * @param dst The array of bytes in swapped byte order.
		 * @returns Pointer to the array of bytes in swapped byte order.
		 */
		static unsigned char* ByteSwap(OGPS_Int32 value, unsigned char* dst);

		/*!
		 * Swaps endianness.
		 *
		 * @param src The array of bytes to be swapped.
		 * @param value The swapped bytes as typed data.
		 */
		static void ByteSwap(const unsigned char* src, OGPS_Int32& value);

		/*!
		 * Swaps endianness.
		 *
		 * @param value The bytes to be swapped as typed data.
		 * @param dst The array of bytes in swapped byte order.
		 * @returns Pointer to the array of bytes in swapped byte order.
		 */
		static unsigned char* ByteSwap(OGPS_Float value, unsigned char* dst);

		/*!
		 * Swaps endianness.
		 *
		 * @param src The array of bytes to be swapped.
		 * @param value The swapped bytes as typed data.
		 */
		static void ByteSwap(const unsigned char* src, OGPS_Float& value);

		/*!
		 * Swaps endianness.
		 *
		 * @param value The bytes to be swapped as typed data.
		 * @param dst The array of bytes in swapped byte order.
		 * @returns Pointer to the array of bytes in swapped byte order.
		 */
		static unsigned char* ByteSwap(OGPS_Double value, unsigned char* dst);

		/*!
		 * Swaps endianness.
		 *
		 * @param src The array of bytes to be swapped.
		 * @param value The swapped bytes as typed data.
		 */
		static void ByteSwap(const unsigned char* src, OGPS_Double& value);

	protected:
		/*! Creates a new instance. */
		Environment();

		/*! Creates the appropriate instance for the current system. */
		static std::unique_ptr<Environment> CreateInstance();

	private:
		/*! Pointer to the environment. */
		static std::unique_ptr<Environment> m_Instance;
	};
}

#endif
