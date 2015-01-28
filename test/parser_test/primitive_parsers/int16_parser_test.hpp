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
//  Created on: Jan 21, 2015
//      Author: Eugen Wintersberger
//

#pragma once

#include <boost/current_function.hpp>
#include <pni/io/parsers/parser.hpp>
#include <pni/io/exceptions.hpp>
#include <pni/core/types.hpp>

#include<cppunit/TestFixture.h>
#include<cppunit/extensions/HelperMacros.h>

#include "../../TestHelpers.hpp"

using namespace pni::core;
using namespace pni::io;

class int16_parser_test:public CppUnit::TestFixture 
{
        CPPUNIT_TEST_SUITE(int16_parser_test);
        CPPUNIT_TEST(test_regular_value);
        CPPUNIT_TEST(test_overflow);
        CPPUNIT_TEST_SUITE_END();

        typedef int16                  result_type; 
        typedef string::const_iterator iterator_type;
    
        typedef parser<iterator_type,result_type> parser_type;

    public:
        void setUp();
        void tearDown();
        void test_regular_value();
        void test_overflow();
};

