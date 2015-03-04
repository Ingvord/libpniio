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
//  Created on: Feb 27, 2015
//      Author: Eugen Wintersberger
//

#pragma once

#include <boost/current_function.hpp>
#include <pni/core/types.hpp>
#include <pni/core/arrays.hpp>
#include <pni/io/nx/nxpath.hpp>

#include<cppunit/TestFixture.h>
#include<cppunit/extensions/HelperMacros.h>

#include "../TestHelpers.hpp"

using namespace pni::core;
using namespace pni::io::nx;

//!
//! \ingroup test_code
//! \brief test nxpath
//! 
//! Test the nxpath class
//!
class nxpath_push_test:public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(nxpath_push_test);
    CPPUNIT_TEST(test_back);
    CPPUNIT_TEST(test_front);
    CPPUNIT_TEST(test_front_back);
    CPPUNIT_TEST(test_root_front);
    CPPUNIT_TEST(test_root_back);
	CPPUNIT_TEST_SUITE_END();

    nxpath p;
public:
	void setUp();
	void tearDown();
    
    void test_front();
    void test_back();
    void test_front_back();
    void test_root_front();
    void test_root_back();
};

