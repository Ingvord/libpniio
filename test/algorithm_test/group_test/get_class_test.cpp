//
// (c) Copyright 2013 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
//  Created on: Jun 28, 2013
//      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//

#include <pni/io/nx/algorithms/create_group.hpp>
#include <pni/io/nx/algorithms/get_class.hpp>
#include <boost/current_function.hpp>
#include <cppunit/extensions/HelperMacros.h>
#include <pni/io/exceptions.hpp>

#include "get_class_test.hpp"

using pni::io::invalid_object_error;

CPPUNIT_TEST_SUITE_REGISTRATION(get_class_test);

//-----------------------------------------------------------------------------
void get_class_test::setUp()
{
    file = h5::nxfile::create_file("is_valid.nx",true);
    root = file.root();
    group = root.create_group("group","NXentry");
    field = root.create_field<uint32>("data");
}

//-----------------------------------------------------------------------------
void get_class_test::tearDown() 
{ 
    field.close();
    group.close();
    root.close();
    file.close();
}


//-----------------------------------------------------------------------------
void get_class_test::test_group()
{
    std::cerr<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    object_type object = group;
    CPPUNIT_ASSERT(get_class(object)=="NXentry");
    CPPUNIT_ASSERT(get_class(group) =="NXentry");
    CPPUNIT_ASSERT_THROW(get_class(object_type(h5::nxgroup())),
                         invalid_object_error);
    CPPUNIT_ASSERT_THROW(get_class(h5::nxgroup()),invalid_object_error);
}

//-----------------------------------------------------------------------------
void get_class_test::test_not_exists()
{
    std::cerr<<BOOST_CURRENT_FUNCTION<<std::endl;

    auto o = create_group(object_type(group),"log");
    CPPUNIT_ASSERT(get_class(o)=="");
}

//-----------------------------------------------------------------------------
void get_class_test::test_field()
{
    std::cerr<<BOOST_CURRENT_FUNCTION<<std::endl;

    object_type object = field;
    CPPUNIT_ASSERT_THROW(get_class(object),type_error);
}

//-----------------------------------------------------------------------------
void get_class_test::test_attribute()
{
    std::cerr<<BOOST_CURRENT_FUNCTION<<std::endl;

    object_type object = group.attributes["NX_class"];
    CPPUNIT_ASSERT_THROW(get_class(object),type_error);
   
}

