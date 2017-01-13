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

#include "inet/common/geometry/common/CoordinateSystem.h"

namespace inet {

const GeoCoord GeoCoord::NIL = GeoCoord(NaN, NaN, NaN);

Define_Module(SimpleGeographicCoordinateSystem);

void SimpleGeographicCoordinateSystem::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
        playgroundLatitude = par("playgroundLatitude");
        playgroundLongitude = par("playgroundLongitude");
        playgroundAltitude = par("playgroundAltitude");
    }
}

Coord SimpleGeographicCoordinateSystem::computePlaygroundCoordinate(const GeoCoord& geographicCoordinate) const
{
    double playgroundX = (geographicCoordinate.longitude - playgroundLongitude) * cos(fabs(playgroundLatitude / 180 * M_PI)) * metersPerDegree;
    double playgroundY = (playgroundLatitude - geographicCoordinate.latitude) * metersPerDegree;
    return Coord(playgroundX, playgroundY, geographicCoordinate.altitude + playgroundAltitude);
}

GeoCoord SimpleGeographicCoordinateSystem::computeGeographicCoordinate(const Coord& playgroundCoordinate) const
{
    double geograpicLatitude = playgroundLatitude - playgroundCoordinate.y / metersPerDegree;
    double geograpicLongitude = playgroundLongitude + playgroundCoordinate.x / metersPerDegree / cos(fabs(playgroundLatitude / 180 * M_PI));
    return GeoCoord(geograpicLatitude, geograpicLongitude, playgroundCoordinate.z - playgroundAltitude);
}


} // namespace inet

