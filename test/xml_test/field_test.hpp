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
// ===========================================================================
//
//  Created on: Nov 25, 2014
//      Author: Eugen Wintersberger
//
#pragma once

#include <boost/current_function.hpp>

#include <pni/io/nx/nx.hpp>
#include <pni/io/nx/xml/field.hpp>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


using namespace pni::core;
using namespace pni::io::nx;

class field_test : public CppUnit::TestFixture
{
    private:
        CPPUNIT_TEST_SUITE(field_test);
        CPPUNIT_TEST(test_name);
        CPPUNIT_TEST(test_size);
        CPPUNIT_TEST(test_rank);
        CPPUNIT_TEST(test_shape);
        CPPUNIT_TEST(test_long_name);
        CPPUNIT_TEST(test_unit);
        CPPUNIT_TEST(test_from_xml_1);
        CPPUNIT_TEST(test_from_xml_2);
        CPPUNIT_TEST(test_from_xml_3);
        CPPUNIT_TEST(test_from_xml_4);
        CPPUNIT_TEST(test_from_xml_5);
        CPPUNIT_TEST(test_to_xml_1);
        CPPUNIT_TEST(test_to_xml_2);
        CPPUNIT_TEST(test_to_xml_3);
        CPPUNIT_TEST(test_read_data_scalar);
        CPPUNIT_TEST(test_read_data_array);
        CPPUNIT_TEST(test_write_data_scalar);
        CPPUNIT_TEST(test_write_data_array);
        CPPUNIT_TEST_SUITE_END();

        xml::node root,child;

        h5::nxfile file;
        h5::nxgroup root_group;
        h5::nxfield field;
        string buffer;

        void setup_xml(const string &fname);

    public:
        void setUp();
        void tearDown();
    
        void test_name();
        void test_size();
        void test_rank();
        void test_shape();
        void test_long_name();
        void test_unit();
        void test_read_data_scalar();
        void test_read_data_array();
        void test_write_data_scalar();
        void test_write_data_array();
        void test_from_xml_1();
        void test_from_xml_2();
        void test_from_xml_3();
        void test_from_xml_4();
        void test_from_xml_5();
        void test_to_xml_1();
        void test_to_xml_2();
        void test_to_xml_3();
};