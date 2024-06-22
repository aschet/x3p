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
  * Allocate static memory to store point data.
  */

#ifndef _OPENGPS_POINT_BUFFER_HXX
#define _OPENGPS_POINT_BUFFER_HXX

#include <opengps/cxx/opengps.hxx>
#include <opengps/data_point_type.h>
#include <memory>
#include <cstring>
#include "stdafx.hxx"

namespace OpenGPS
{
	/*!
	 * A static memory buffer that stores all point data belonging to a single axis.
	 * Internally point vectors are not stored as a sequence of values of their three
	 * components (as one might have naturally thought). But rather there are three memory
	 * blocks. One for each axis that stores all its data point values in a single array.
	 * The three point buffer instances managing typesafe access to their memory blocks
	 * are assembled by the OpenGPS::VectorBufferBuilder and combined within an instance
	 * of OpenGPS::VectorBuffer. To make OpenGPS::VectorBuffer accessible as an instance
	 * of OpenGPS::PointVector one needs to obtain a OpenGPS::PointVectorProxy object by using
	 * OpenGPS::VectorBuffer::GetPointVectorProxy and providing a OpenGPS::PointVectorProxyContext
	 * object which holds the array index of the row which - expanded over the three internal
	 * memory blocks - constraints OpenGPS::VectorBuffer to be seen as one single OpenGPS::PointVector.
	 */
	class PointBuffer
	{
	public:
		/*! Destroys this instance. */
		virtual ~PointBuffer() = default;

		/*!
		 * Sets the value of the inernal memory at the given position.
		 *
		 * An exception might be thrown when this typed operation is
		 * not valid for a particular instance.
		 *
		 * @param index Where to set the new value.
		 * @param value The new value to be set.
		 */
		virtual void Set(size_t index, OGPS_Int16 value);

		/*!
		 * Sets the value of the inernal memory at the given position.
		 *
		 * An exception might be thrown when this typed operation is
		 * not valid for a particular instance.
		 *
		 * @param index Where to set the new value.
		 * @param value The new value to be set.
		 */
		virtual void Set(size_t index, OGPS_Int32 value);

		/*!
		 * Sets the value of the inernal memory at the given position.
		 *
		 * An exception might be thrown when this typed operation is
		 * not valid for a particular instance.
		 *
		 * @param index Where to set the new value.
		 * @param value The new value to be set.
		 */
		virtual void Set(size_t index, OGPS_Float value);

		/*!
		 * Sets the value of the inernal memory at the given position.
		 *
		 * An exception might be thrown when this typed operation is
		 * not valid for a particular instance.
		 *
		 * @param index Where to set the new value.
		 * @param value The new value to be set.
		 */
		virtual void Set(size_t index, OGPS_Double value);

		/*!
		 * Gets the value of the inernal memory at the given position.
		 *
		 * An exception might be thrown when this typed operation is
		 * not valid for a particular instance.
		 *
		 * @param index Get the value from this position.
		 * @param value Stores the value.
		 */
		virtual void Get(size_t index, OGPS_Int16& value) const;

		/*!
		 * Gets the value of the inernal memory at the given position.
		 *
		 * An exception might be thrown when this typed operation is
		 * not valid for a particular instance.
		 *
		 * @param index Get the value from this position.
		 * @param value Stores the value.
		 */
		virtual void Get(size_t index, OGPS_Int32& value) const;

		/*!
		 * Gets the value of the inernal memory at the given position.
		 *
		 * An exception might be thrown when this typed operation is
		 * not valid for a particular instance.
		 *
		 * @param index Get the value from this position.
		 * @param value Stores the value.
		 */
		virtual void Get(size_t index, OGPS_Float& value) const;

		/*!
		 * Gets the value of the inernal memory at the given position.
		 *
		 * An exception might be thrown when this typed operation is
		 * not valid for a particular instance.
		 *
		 * @param index Get the value from this position.
		 * @param value Stores the value.
		 */
		virtual void Get(size_t index, OGPS_Double& value) const;

		/*!
		 * Allocates internal memory.
		 *
		 * Throws an exception on failure.
		 *
		 * @param size Amount of point data to be stored.
		 */
		virtual void Allocate(size_t size);

		/*!
		 * Gets the maximum amount of point data that can be stored.
		 */
		size_t GetSize() const;

		/*!
		 * Gets the type of point data that can be stored within this instance.
		 */
		virtual OGPS_DataPointType GetPointType() const;

	protected:
		/*!
		 * Allocates internal memory.
		 *
		 * @remarks Allocated memory must be free by PointBuffer::Free manually.
		 *
		 * Throws an exception on failure.
		 *
		 * @param size Amount of point data to be stored.
		 * @param typeSize The size of the data type of the point data stored herein in bytes.
		 * @returns Returns a pointer to allocated memory or nullptr.
		 */
		template<typename T> std::unique_ptr<T[]> AllocateT(size_t size);

	private:
		/*! Logical size or amount of point data that can be stored. */
		size_t m_Size{};
	};

	template<typename T>
	std::unique_ptr<T[]> PointBuffer::AllocateT(size_t size)
	{
		assert(m_Size == 0);

		auto buffer{ std::make_unique<T[]>(size) };
		memset(buffer.get(), 0, size * sizeof(T));
		m_Size = size;
		return buffer;
	}
}

#endif
