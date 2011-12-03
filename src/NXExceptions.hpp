/*
 * Declaration of Nexus specific exceptions
 *
 * (c) Copyright 2011 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
 *
 * This file is part of libpninx.
 *
 * libpninx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libpninx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libpninx.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************
 *
 * Declaration of Nexus specific exceptions
 *
 * Created on: Aug 5, 2011
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */

#ifndef NXEXCEPTIONS_HPP_
#define NXEXCEPTIONS_HPP_

#include <iostream>
#include <pni/utils/Exceptions.hpp>

using namespace pni::utils;

namespace pni{
namespace nx{

//! \defgroup Exceptions

//! \ingroup Exceptions
//! field exception

//! Raised if an error occured in one of the NXField methods.
class NXFieldError:public Exception{
public:
	//! default constructor
	NXFieldError();
	//! constructor
	NXFieldError(const String &i,const String &d);
	//! destructor
	virtual ~NXFieldError();
	
	//! output operator
	friend std::ostream &operator<<(std::ostream &o,const NXFieldError &e);
};

//! \ingroup Exceptions
//! Attribute Error

//! Raised in cases where reading or writing an attribute failed.
class NXAttributeError:public Exception{
public:
	//! default constructor
	NXAttributeError();
	//! constructor
	NXAttributeError(const String &i,const String &d);
	//! destructor
	virtual ~NXAttributeError();
	
	//! output operator
	friend std::ostream &operator<<(std::ostream &o,const NXAttributeError &e);
};

//! \ingroup Exceptions
//! Group Error

//! Error in case of group operations.
class NXGroupError:public Exception{
public:
	//! default constructor
	NXGroupError();
	//! constructor
	NXGroupError(const String &i,const String &d);
	//! destructor
	virtual ~NXGroupError();
	//! output operator
	friend std::ostream &operator<<(std::ostream &o,const NXGroupError &e);
};

//! \ingroup Exceptions
//! File Error

//! File exception class.
class NXFileError:public Exception {
public:
	//! default constructor
	NXFileError():Exception("NXFileError"){

	}
	//! constructor
	NXFileError(const String &i,const String &d):Exception("NXFileError",i,d){

	}
	//! destructor
	virtual ~NXFileError(){

	}
	//! output operator
	friend std::ostream &operator<<(std::ostream &o,const NXFileError &e){
		return e.print(o);
	}
};


//end of namespace
}
}


#endif /* NXEXCEPTIONS_HPP_ */
