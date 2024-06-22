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
  * Interface to communicate whether the point vector at a given location
  * contains valid data.
  */

#ifndef _OPENGPS_POINT_VALIDITY_PROVIDER_HXX
#define _OPENGPS_POINT_VALIDITY_PROVIDER_HXX

#include <opengps/cxx/opengps.hxx>
#include <memory>

namespace OpenGPS
{
	class PointBuffer;

	/*!
	 * Interface to communicate the validity of a point vector at a given location.
	 */
	class PointValidityProvider
	{
	public:
		/*!
		 * Creates a new instance.
		 * @param value The point buffer of the Z axis.
		 * The Z axis is used to communicate validity because the Z
		 * axis is the only axis which cannot have values implicitly and
		 * therefore must always provide a valid measurement.
		 */
		PointValidityProvider(std::shared_ptr<PointBuffer> value);

		/*! Destroys this instance. */
		virtual ~PointValidityProvider();

		/*!
		 * Sets the validity of a point vector at a given location.
		 *
		 * A specific implementation may throw an OpenGPS::Exception if this operation
		 * is not permitted due to the current state of the object instance.
		 *
		 * @param index The location of the point data the validity is updated.
		 * @param value The value of the current validity of the point vector at the given position.
		 */
		virtual void SetValid(size_t index, bool value) = 0;

		/*!
		 * Gets the validity of a point vector at a given location.
		 *
		 * A specific implementation may throw an OpenGPS::Exception if this operation
		 * is not permitted due to the current state of the object instance.
		 *
		 * @param index The location of the point data the validity is checked.
		 * @returns Returns true if valid, false otherwise.
		 */
		virtual bool IsValid(size_t index) const = 0;

	protected:
		/*! Returns the point buffer of the Z axis. */
		PointBuffer* GetPointBuffer() const;

	private:
		/*! Point buffer of the Z axis. */
		std::shared_ptr<PointBuffer> m_PointBuffer;
	};
}

#endif
