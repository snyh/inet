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

#ifndef __INET_OSGGEOGRAPHICCOORDINATESYSTEM_H
#define __INET_OSGGEOGRAPHICCOORDINATESYSTEM_H

#include "inet/common/INETDefs.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/common/geometry/common/EulerAngles.h"
#include "inet/common/geometry/common/CoordinateSystem.h"

#ifdef WITH_OSG
#include <osgEarth/MapNode>

namespace inet {
namespace osg {

class INET_API OsgGeographicCoordinateSystem : public cSimpleModule, public IGeographicCoordinateSystem
{
  protected:
    GeoCoord playgroundPosition = GeoCoord::NIL;
    EulerAngles playgroundOrientation = EulerAngles::NIL;
    osgEarth::MapNode *mapNode = nullptr;
    ::osg::Matrixd locatorMatrix;
    ::osg::Matrixd inverseLocatorMatrix;

  protected:
    virtual void initialize(int stage) override;

  public:
    virtual GeoCoord getPlaygroundPosition() const override { return playgroundPosition; }
    virtual EulerAngles getPlaygroundOrientation() const override { return playgroundOrientation; }

    virtual Coord computePlaygroundCoordinate(const GeoCoord& geographicCoordinate) const override;
    virtual GeoCoord computeGeographicCoordinate(const Coord& playgroundCoordinate) const override;
};

} // namespace osg
} // namespace inet

#endif // WITH_OSG

#endif // ifndef __INET_OSGGEOGRAPHICCOORDINATESYSTEM_H

