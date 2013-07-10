/*
 * (c) Copyright 2012 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
 *
 * This file is part of libpniio.
 *
 * libpniio is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libpniio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libpniio.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************
 *
 * Created on: Feb 10, 2012
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */

#include <boost/current_function.hpp>
#include "H5LinkTest.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(H5LinkTest);

//-----------------------------------------------------------------------------
void H5LinkTest::setUp(){
    _file1 = H5File::create_file("H5LinkTest1.h5",true,0);
    _file2 = H5File::create_file("H5LinkTest2.h5",true,0);
}

//------------------------------------------------------------------------------
void H5LinkTest::tearDown(){
    _file1.close();
    _file2.close();
}

//------------------------------------------------------------------------------
void H5LinkTest::test_internal()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    H5Datatype type = H5DatatypeFactory::create_type<float64>();
    H5Dataspace space;
    H5Dataset ds("data",_file1,type,space);
    float64 value = 100.1234;
    CPPUNIT_ASSERT_NO_THROW(ds.write(&value));

    CPPUNIT_ASSERT_NO_THROW(H5Link::create("/data",_file1,"data_1_link_1"));
    CPPUNIT_ASSERT_NO_THROW(H5Link::create("data",_file1,"data_1_link_2"));
    CPPUNIT_ASSERT_NO_THROW(H5Link::create(ds,_file1,"data_1_link_3"));

    H5Dataset dread;
    CPPUNIT_ASSERT_NO_THROW(dread = _file1.open("data_1_link_1"));
    float64 read;
    CPPUNIT_ASSERT_NO_THROW(dread.read(&read));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(value,read,1.e-6);
    
    CPPUNIT_ASSERT_NO_THROW(dread = _file1.open("data_1_link_2"));
    CPPUNIT_ASSERT_NO_THROW(dread.read(&read));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(value,read,1.e-6);

    CPPUNIT_ASSERT_NO_THROW(dread = _file1.open("data_1_link_3"));
    CPPUNIT_ASSERT_NO_THROW(dread.read(&read));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(value,read,1.e-6);

    //links can also be created in advance
    CPPUNIT_ASSERT_NO_THROW(H5Link::create("/detector/counts",_file1,"detdat"));
    type = H5DatatypeFactory::create_type<uint16>();
    CPPUNIT_ASSERT_NO_THROW(ds=H5Dataset("/detector/counts",_file1,type,space));
    uint16 scalar = 1024;
    uint16 sread = 0;
    CPPUNIT_ASSERT_NO_THROW(ds.write(&scalar));
    dread = _file1.open("detdat");
    CPPUNIT_ASSERT_NO_THROW(dread.read(&sread));
    CPPUNIT_ASSERT(sread == scalar);
}

//------------------------------------------------------------------------------
void H5LinkTest::test_external()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    H5Datatype type = H5DatatypeFactory::create_type<float64>();
    H5Dataspace space;

    //create target in the first file
    H5Dataset dwrite("data",_file1,type,space);
    //write some data
    float64 wval = -1.25091;
    CPPUNIT_ASSERT_NO_THROW(dwrite.write(&wval));

    //create the external link in the second file

    CPPUNIT_ASSERT_NO_THROW(H5Link::create("H5LinkTest1.h5:/data",
                _file2,"external/data"));

    float64 read;
    H5Dataset dread = _file2.open("/external/data");
    CPPUNIT_ASSERT_NO_THROW(dread.read(&read));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(wval,read,1.e-6);

}
