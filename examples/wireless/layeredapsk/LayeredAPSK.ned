//
// Copyright (C) 2014 OpenSim Ltd.
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

package inet.examples.wireless.layeredapsk;

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.inet.AdhocHost;
import inet.physicallayer.apskradio.bitlevel.APSKLayeredScalarRadioMedium;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;

network LayeredAPSK
{
    parameters:
        int numHosts;
    submodules:
        visualizer: IntegratedCanvasVisualizer {
            parameters:
                @display("p=100,250");
        }
        configurator: IPv4NetworkConfigurator {
            parameters:
                @display("p=100,150");
        }
        radioMedium: APSKLayeredScalarRadioMedium {
            parameters:
                @display("p=100,50");
        }
        host[numHosts]: AdhocHost {
            parameters:
                @display("r=,,#707070;p=250,58");
        }
}
