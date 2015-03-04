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
//

#include<iostream>
#include<string>
#include<vector>

#include<cppunit/extensions/HelperMacros.h>
#include<cppunit/TestCaller.h>
#include<cppunit/TestResult.h>
#include<cppunit/TestRunner.h>
#include<cppunit/XmlOutputter.h>
#include<cppunit/TextTestProgressListener.h>
#include<cppunit/ui/text/TextTestRunner.h>
#include<cppunit/extensions/TestFactoryRegistry.h>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc,char **argv)
{

    //setup the test runner
    CppUnit::TextTestRunner runner;
    CppUnit::TextTestProgressListener progress;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    
    runner.addTest(registry.makeTest());
    runner.eventManager().addListener(&progress);
    
    std::ofstream os("nxfile_tests.xml");
    runner.setOutputter(new CppUnit::XmlOutputter(&runner.result(),os));
    
    bool result = runner.run();

    return result ? 0 : 1;
}
#pragma GCC diagnostic pop