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
//  Created on: Apr 24, 2015
//      Author: Eugen Wintersberger
//
#pragma once

#include <pni/io/nx/nx.hpp>
#include <pni/io/nx/xml.hpp>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


using namespace pni::core;
using namespace pni::io::nx;

static const string xml_file = "detector_with_transformation.xml";
static const string nxs_file = "detector_with_transformation.nxs";

class detector_with_transformation_test : public CppUnit::TestFixture
{
    private:
        CPPUNIT_TEST_SUITE(detector_with_transformation_test);
        CPPUNIT_TEST(test);
        CPPUNIT_TEST_SUITE_END();
        

        h5::nxobject root_group;        
        h5::nxfile   file;
        xml::node    root_node;        
    public:
        void setUp();
        void tearDown();
        
        void test();
};