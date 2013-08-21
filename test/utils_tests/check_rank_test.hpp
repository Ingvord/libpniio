/*
 * (c) Copyright 2013 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
 *  Created on: Jul 10, 2013
 *      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */
#pragma once
#include <type_traits>
#include<cppunit/TestFixture.h>
#include<cppunit/extensions/HelperMacros.h>
#include<boost/current_function.hpp>

#include <pni/core/types.hpp>
#include <pni/core/arrays.hpp>
#include <pni/io/nx/nx.hpp>
#include <pni/io/nx/utils/types.hpp>
#include <pni/io/nx/nexus_utils.hpp>

using namespace pni::core;
using namespace pni::io::nx;


/*!
\brief test for size check

Should test the check_size function template between different in memory
container and Nexus objects as well as among the container objects themselves. 

As Nexus objects we consider fields and attributes where two of each are created
with a size of 25 and 36 elements. 

Here we assume that TA is one of our array types while TB can be everything that
has a size member function.

\tparam TA container type 1
\tparam TB container type 2
*/
template<typename TA,typename TB>
class check_rank_test : public CppUnit::TestFixture
{
        typedef check_rank_test<TA,TB> this_test_t;
        CPPUNIT_TEST_SUITE(this_test_t);
        CPPUNIT_TEST(test_throw);
        CPPUNIT_TEST(test_no_throw);
        CPPUNIT_TEST_SUITE_END();
        
        h5::nxfile file;
        h5::nxfield field1,field2;
        h5::nxattribute attribute1,attribute2;

        shape_t s1;
        shape_t s2;
        size_t size_1;
        size_t size_2;

        TA a1,a2;
        TB b1,b2;

        //need some special functions 
        template<typename T,typename STORAGE,typename IMAP>
        static void create_container(const shape_t &s,darray<T,STORAGE,IMAP> &a)
        {
            a = darray<T,STORAGE,IMAP>(s);
        }

        template<typename ATYPE>
        static void create_container(const shape_t &s,numarray<ATYPE> &a )
        {
            a = numarray<ATYPE>(s);
        }
        
    public:
        void setUp();
        void tearDown();
        
        void test_throw();
        void test_no_throw();
};

//-----------------------------------------------------------------------------
template<typename TA,typename TB> void check_rank_test<TA,TB>::setUp()
{
    s1 = shape_t{6,5};
    s2 = shape_t{6};
    size_1 = 25;
    size_2 = 36;

    file = h5::nxfile::create_file("check_size_test.nx",true);
    field1 = file.create_field<uint8>("field1",s1);
    field2 = file.create_field<float64>("field2",s2);
    attribute1 = field1.attr<float32>("attr",s1);
    attribute2 = field2.attr<int64>("attr",s2);

    create_container(s1,a1);
    create_container(s2,a2);
    create_container(s1,b1);
    create_container(s2,b2);

}

//-----------------------------------------------------------------------------
template<typename TA,typename TB> void check_rank_test<TA,TB>::tearDown()
{
    //close all Nexus objects
    attribute1.close();
    attribute2.close();
    field1.close();
    field2.close();

    file.close();


}

//-----------------------------------------------------------------------------
template<typename TA,typename TB> void check_rank_test<TA,TB>::test_throw()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
   
    //check the two container types among each other
    CPPUNIT_ASSERT(check_rank(a1,b1));
    CPPUNIT_ASSERT(!check_rank(a1,b2));
    //check with selection
    CPPUNIT_ASSERT(check_rank(a1(slice(0,6),0),b2));

    //check here for fields and attribute with an array type
    CPPUNIT_ASSERT(check_rank(a1,field1));
    CPPUNIT_ASSERT(check_rank(a1,attribute1));
    CPPUNIT_ASSERT(!check_rank(a2,field1));
    CPPUNIT_ASSERT(!check_rank(a2,attribute1));
    CPPUNIT_ASSERT(check_rank(a1(slice(0,6),1),field2));
    CPPUNIT_ASSERT(check_rank(a2,field1(slice(0,6),1)));

    //check here for fields and attribute for a standard container type
    CPPUNIT_ASSERT(check_rank(b2,field2));
    CPPUNIT_ASSERT(check_rank(b2,attribute2));
    CPPUNIT_ASSERT(!check_rank(b1,field2));
    CPPUNIT_ASSERT(!check_rank(b1,attribute2));

}

//-----------------------------------------------------------------------------
template<typename TA,typename TB> void check_rank_test<TA,TB>::test_no_throw()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    //check the two container types among each other
    CPPUNIT_ASSERT_NO_THROW(check_rank(a1,b1,EXCEPTION_RECORD));
    CPPUNIT_ASSERT_THROW(check_rank(a1,b2,EXCEPTION_RECORD),shape_mismatch_error);
    //check with selection
    CPPUNIT_ASSERT_NO_THROW(check_rank(a2,b1(slice(0,6),1),EXCEPTION_RECORD));

    //check here for fields and attribute with an array type
    CPPUNIT_ASSERT_NO_THROW(check_rank(a1,field1,EXCEPTION_RECORD));
    CPPUNIT_ASSERT_NO_THROW(check_rank(a1,attribute1,EXCEPTION_RECORD));
    CPPUNIT_ASSERT_THROW(check_rank(a2,field1,EXCEPTION_RECORD),shape_mismatch_error);
    CPPUNIT_ASSERT_THROW(check_rank(a2,attribute1,EXCEPTION_RECORD),shape_mismatch_error);
    CPPUNIT_ASSERT_NO_THROW(check_rank(a1(slice(0,6),0),field2,EXCEPTION_RECORD));
    CPPUNIT_ASSERT_NO_THROW(check_rank(a2,field1(slice(0,6),0),EXCEPTION_RECORD));

    //check here for fields and attribute for a standard container type
    CPPUNIT_ASSERT_NO_THROW(check_rank(b2,field2,EXCEPTION_RECORD));
    CPPUNIT_ASSERT_NO_THROW(check_rank(b2,attribute2,EXCEPTION_RECORD));
    CPPUNIT_ASSERT_THROW(check_rank(b1,field2,EXCEPTION_RECORD),shape_mismatch_error);
    CPPUNIT_ASSERT_THROW(check_rank(b1,attribute2,EXCEPTION_RECORD),shape_mismatch_error);
}

