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
//  Created on: Jul 3, 2013
//      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//

#include <boost/current_function.hpp>
#include<cppunit/extensions/HelperMacros.h>

#include "as_group_test.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(as_group_test);

//-----------------------------------------------------------------------------
void as_group_test::setUp()
{
    field_shape = shape_t{0,10,10};
    attr_shape  = shape_t{4,4};

    file = h5::nxfile::create_file("is_valid.nx",true,0);
    root = file.root();
    group = root.create_group("group","NXentry");
    group.create_group("instrument","NXinstrument");
    field = root.create_field<uint32>("data",field_shape);
    field.attr<float32>("temp",attr_shape);
}

//-----------------------------------------------------------------------------
void as_group_test::tearDown() 
{ 
    field.close();
    group.close();
    root.close();
    file.close();
}


//-----------------------------------------------------------------------------
void as_group_test::test_group()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
        
    h5::nxobject object = root;
    h5::nxgroup g;
    //must work - the stored object is an instance of nxgroup
    CPPUNIT_ASSERT_NO_THROW(g = as_group(object));
    CPPUNIT_ASSERT(g.is_valid());
    CPPUNIT_ASSERT(g.name() == "/");
}

//-----------------------------------------------------------------------------
void as_group_test::test_field()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    h5::nxobject object = field;
    
    //must throw as the stored object is an instance of nxfield
    CPPUNIT_ASSERT_THROW(as_group(object),nxgroup_error);
}

//-----------------------------------------------------------------------------
void as_group_test::test_attribute()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    h5::nxobject object = field.attr("temp");
    //must throw as the stored object is an instance of nxattribute
    CPPUNIT_ASSERT_THROW(as_group(object),nxgroup_error);
    
}

