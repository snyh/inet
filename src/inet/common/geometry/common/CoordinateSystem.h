//
// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_COORDINATESYSTEM_H
#define __INET_COORDINATESYSTEM_H

#include "inet/common/INETDefs.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/common/geometry/common/EulerAngles.h"

namespace inet {

class INET_API GeoCoord
{
  public:
    static const GeoCoord NIL;

  public:
    /** @name latitude, longitude and altitude coordinate of the position. */
    /*@{*/
    double latitude;
    double longitude;
    double altitude;
    /*@}*/

  public:
    GeoCoord(double latitude, double longitude, double altitude) : latitude(latitude), longitude(longitude), altitude(altitude) { }
};

class INET_API IGeographicCoordinateSystem
{
  public:
    virtual GeoCoord getPlaygroundPosition() const = 0;
    virtual EulerAngles getPlaygroundOrientation() const = 0;

    virtual Coord computePlaygroundCoordinate(const GeoCoord& geographicCoordinate) const = 0;
    virtual GeoCoord computeGeographicCoordinate(const Coord& playgroundCoordinate) const = 0;
};

class INET_API SimpleGeographicCoordinateSystem : public cSimpleModule, public IGeographicCoordinateSystem
{
  protected:
    double metersPerDegree = 111320;
    double playgroundLatitude = NaN;
    double playgroundLongitude = NaN;
    double playgroundAltitude = NaN;

  protected:
    virtual void initialize(int stage) override;

  public:
    virtual GeoCoord getPlaygroundPosition() const override { return GeoCoord(playgroundLatitude, playgroundLongitude, playgroundAltitude); }
    virtual EulerAngles getPlaygroundOrientation() const override { return EulerAngles::ZERO; }

    virtual Coord computePlaygroundCoordinate(const GeoCoord& geographicCoordinate) const override;
    virtual GeoCoord computeGeographicCoordinate(const Coord& playgroundCoordinate) const override;
};


} // namespace inet

#endif // ifndef __INET_COORDINATESYSTEM_H

