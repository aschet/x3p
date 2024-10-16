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
  * The internal memory structure that stores all point measurement data.
  */

#ifndef _OPENGPS_VECTOR_BUFFER_HXX
#define _OPENGPS_VECTOR_BUFFER_HXX

#include <opengps/data_point_type.h>
#include <opengps/cxx/point_vector_base.hxx>

#include "valid_buffer.hxx"

namespace OpenGPS
{
	class PointBuffer;
	class PointVectorProxyContext;

	/*!
	 * Implements the memory structure of point measurement data.
	 *
	 * For every three coordinate axis distinct memory blocks capable of
	 * serving all point data belonging to that single axis are managed.
	 * If no explicit measurement data is needed for a special
	 * axis because point data can be calculated implicitly by means of
	 * indexing then no OpenGPS::PointBuffer is provided
	 * and a nullptr pointer is saved instead.
	 *
	 * Direct manipulation of vector data
	 * is enabled by the usage of proxied point vectors.
	 *
	 * Also information
	 * about the validity of point vectors is provided based on indexes.
	 */
	class VectorBuffer
	{
	public:
		/*!
		 * Sets the buffer for point measurement data of the X component of a vector.
		 * @remarks The pointer passed will become managed by this instance.
		 * @param value The instance of an object that manages the data point
		 * measurements or nullptr if this axis does not need explicit point values.
		 */
		void SetX(std::shared_ptr<PointBuffer> value);

		/*!
		 * Sets the buffer for point measurement data of the Y component of a vector.
		 * @remarks The pointer passed will become managed by this instance.
		 * @param value The instance of an object that manages the data point
		 * measurements or nullptr if this axis does not need explicit point values.
		 */
		void SetY(std::shared_ptr<PointBuffer> value);

		/*!
		 * Sets the buffer for point measurement data of the Z component of a vector.
		 * @remarks The pointer passed will become managed by this instance.
		 * @param value The instance of an object that manages the data point
		 * measurements or nullptr if this axis does not need explicit point values.
		 */
		void SetZ(std::shared_ptr<PointBuffer> value);

		/*! Gets the buffer of point measurement data of the X components of all vectors. */
		std::shared_ptr<PointBuffer> GetX() const;

		/*! Gets the buffer of point measurement data of the Y components of all vectors. */
		std::shared_ptr<PointBuffer> GetY() const;

		/*! Gets the buffer of point measurement data of the Z components of all vectors. */
		std::shared_ptr<PointBuffer> GetZ() const;

		/*!
		 * Sets the instance of the object that manages validity information for all point vectors.
		 * @param value Provides information about the validity of every point vector.
		 * @param buffer Optionally specifies an object that enables direct access to the
		 * external file that may exist as a backend of the current OpenGPS::PointValidityProvider instance
		 * to serve point validity requests.
		 */
		void SetValidityProvider(std::shared_ptr<PointValidityProvider> value, std::shared_ptr<ValidBuffer> buffer);

		/*!
		 * Gets the object that tracks the validity of point vectors.
		 * @see VectorBuffer::HasValidityBuffer
		 */
		PointValidityProvider* GetValidityProvider() const;

		/*!
		 * Gets the object representing an external point validity file. May return nullptr, if point
		 * validity information is not aquired through an external file but inline by the usage of
		 * special point values for invalid points.
		 * @see VectorBuffer::HasValidityBuffer
		 */
		ValidBuffer* GetValidityBuffer() const;

		/*!
		 * Returns true if point validity information is processed by usage of an external file.
		 * It must be taken responsibility to read/write this file then before/after usage of
		 * OpenGPS::PointValidityProvider. Returns false if there is no such file handle instance
		 * which must be taken care of.
		 * @see VectorBuffer::GetValidityBuffer, VectorBuffer::GetValidityProvider,
		 * VectorBuffer::SetValidityProvider
		 */
		bool HasValidityBuffer() const;

		/*!
		 * Creates an object which proxies this object instance through a point vector.
		 * This OpenGPS::VectorBuffer instance manages static memory for all point vector data.
		 * A proxied point vector gives the impression of manipulating a single
		 * vector although every operation on that object is made on the data structure
		 * implemented by this OpenGPS::VectorBuffer instance directly.
		 * @param context From here the point vector proxy object gets information which row
		 * index of the internally stacked point measurement data is actually processed.
		 */
		std::shared_ptr<PointVectorBase> CreatePointVectorProxy(std::shared_ptr<PointVectorProxyContext> context);

	private:
		/*! Buffer for measurements of the X component of all point vectors. */
		std::shared_ptr<PointBuffer> m_X;

		/*! Buffer for measurements of the Y component of all point vectors. */
		std::shared_ptr<PointBuffer> m_Y;

		/*! Buffer for measurements of the Z component of all point vectors. */
		std::shared_ptr<PointBuffer> m_Z;

		/*! Buffer for validity information of all point vector indexes. */
		std::shared_ptr<ValidBuffer> m_ValidBuffer;

		/*! Interface to retrieve/provide point validity information.  */
		std::shared_ptr<PointValidityProvider> m_ValidityProvider;
	};
}

#endif
