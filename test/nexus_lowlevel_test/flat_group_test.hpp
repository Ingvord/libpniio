//
// Declaration of Nexus object template.
//
// (c) Copyright 2014 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
// ==========================================================================
//
//  Created on: Sept 10, 2014
//      Author: Eugen Wintersberger
//

#pragma once

#include <pni/io/nx/nx.hpp>
#include <pni/io/nx/flat_group.hpp>

#include<cppunit/TestFixture.h>
#include<cppunit/extensions/HelperMacros.h>


using namespace pni::core;
using namespace pni::io::nx;

class flat_group_test: public CppUnit::TestFixture  
{
	CPPUNIT_TEST_SUITE(flat_group_test);
	CPPUNIT_TEST(test_creation);
	CPPUNIT_TEST_SUITE_END();
private:
    h5::nxfile _f;

public:
	void setUp();
	void tearDown();
	void test_creation();
};
