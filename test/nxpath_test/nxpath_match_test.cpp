//
// (c) Copyright 2015 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
//
// This file is part of libpniio.
//
// libpniio is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// libpniio is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libpniio.  If not, see <http://www.gnu.org/licenses/>.
// ===========================================================================
//
//  Created on: Mar 26, 2015
//      Author: Eugen Wintersberger
//

#include <boost/test/unit_test.hpp>
#include <pni/core/types.hpp>
#include <pni/io/nx/nxpath.hpp>

using namespace pni::core;
using namespace pni::io::nx;

BOOST_AUTO_TEST_SUITE(nxpath_match_test)

    //-------------------------------------------------------------------------
    BOOST_AUTO_TEST_CASE(test_1)
    {
        nxpath p1 = nxpath::from_string(":NXentry/:NXinstrument/:NXdetector");
        nxpath p2 = nxpath::from_string("entry:NXentry/instrument:NXinstrument/detector:NXdetector");
        BOOST_CHECK(match(p1,p2));

        p2 = nxpath::from_string("scan_1:NXentry/instrument/detector:NXdetector");
        BOOST_CHECK(!match(p1,p2));
    }

    //-------------------------------------------------------------------------
    BOOST_AUTO_TEST_CASE(test_2)
    {
        nxpath p1 = nxpath::from_string("entry/:NXinstrument/detector_1");
        nxpath p2 = nxpath::from_string("entry/beamline:NXinstrument/detector_1");
        BOOST_CHECK(match(p1,p2));
    }

BOOST_AUTO_TEST_SUITE_END()