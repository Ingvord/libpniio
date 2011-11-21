/*
 * Declaration of the NXGroup template
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
 * Declaration of the NXGroup template
 *
 * Created on: Jul 1, 2011
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */

#ifndef NXGROUP_HPP_
#define NXGROUP_HPP_

#include <boost/shared_ptr.hpp>
#include <pni/utils/PNITypes.hpp>
#include <pni/utils/ArrayShape.hpp>

#include "NXObject.hpp"
#include "NXImpMap.hpp"
#include "NXField.hpp"
#include "NXFilter.hpp"
#include "NXExceptions.hpp"

using namespace pni::utils;

namespace pni{
namespace nx{

template<typename Imp>
class NXGroup:public NXObject<Imp> {
protected:
public:
	typedef typename NXImpMap<Imp::IMPCODE>::ObjectImplementation OImp;
	typedef typename NXImpMap<Imp::IMPCODE>::GroupImplementation GImp;
	typedef typename NXImpMap<Imp::IMPCODE>::FieldImplementation FImp;
	typedef boost::shared_ptr<NXGroup<Imp> > sptr;
	//! default constructor
	NXGroup();
	//! copy constructor

	//! The copy constructor is doing exactly the
	//! same as the assignment operator. Thus it can be used to assign
	//! an object directly at its construction.
	NXGroup(const NXGroup<Imp> &);
	//! destructor
	virtual ~NXGroup();

	//! assignment operator
	NXGroup<Imp> &operator=(const NXGroup<Imp> &);

	virtual NXGroup<GImp> createGroup(const String &n) const;
	virtual NXGroup<GImp> createGroup(const String &n,const String &type) const;
	virtual NXGroup<GImp> openGroup(const String &n) const;

	//! create a field for array data using an ArrayShape

	//! Here a reference to an ArrayShape object is used to create a data field
	//! \param n name of the field as String object
	//! \param tid PNI type code of the data type
	//! \param s reference to an ArrayShape object
	NXField<FImp> createNumericField(const String &n, PNITypeID tid,
			 	 	 	 	  const ArrayShape &s,const String &unit,
			 	 	 	 	  const String &desc) const;
	template<typename Filter>
	NXField<FImp> createNumericField(const String &n, PNITypeID tid,
				              const ArrayShape &s,const String &unit,
				              const String &desc,
				              NXFilter<Filter> &f) const;

	//! create a field for array data from an ArrayObject

	//! This method uses a reference to an ArrayObject instance. All parameters
	//! relevant to the
	//! field are derived from the ArrayObject object.
	//! \param n name of the field
	//! \param a reference to the ArrayObject instance
	NXField<FImp> createNumericField(const ArrayObject &a) const;
	template<typename Filter>
	NXField<FImp> createNumericField(const ArrayObject &a,NXFilter<Filter> &f) const;


	//! create a field for scalar data - simplest approach

	//! A data field for a single scalar datum will be created.
	//! \param n name of the data field
	//! \param tid ID of the PNI type for the field
	NXField<FImp> createNumericField(const String &n,PNITypeID tid,const String &unit,const String &desc) const;
	//! create a field for scalar data from ScalarObject

	//! A reference to an instance of ScalarObject is used to derive all
	//! parameters for data field creation.
	//! \param n name of the data field
	//! \parma s reference to the ScalarObject instance
	NXField<FImp> createNumericField(const ScalarObject &s) const;

	//! create a field for string data

	//! Create a field for String data encoded in UTF8.
	//! \param n name of the field
	//! \param s string for which the field should be created
	NXField<FImp> createStringField(const String &n) const;

	//! open a field object
	NXField<FImp> openField(const String &n) const;
	//! open an arbitrary object
	NXObject<OImp> open(const String &n);


	//! close the group object
	virtual void close();

	virtual bool exists(const String &n) const{
		return this->getImplementation().exists(n);
	}

	virtual void remove(const String &n) const{
		this->getImplementation().remove(n);
	}


};

//========================constructors and destructors==========================
template<typename Imp>
NXGroup<Imp>::NXGroup():NXObject<Imp>(){
}

template<typename Imp> NXGroup<Imp>::NXGroup(const NXGroup &g){
	EXCEPTION_SETUP("template<typename Imp> NXGroup<Imp>::NXGroup(const NXGroup &g)");
	this->setImplementation(g.getImplementation());
}

template<typename Imp> NXGroup<Imp>::~NXGroup(){
	close();
}

//======================================variants of the assignment operator====

template<typename Imp> NXGroup<Imp> &NXGroup<Imp>::operator=(const NXGroup<Imp> &g){
	EXCEPTION_SETUP("template<typename Imp> NXGroup<Imp> &NXGroup<Imp>::operator=(const NXGroup<Imp> &g)");
	if( this != &g){
		this->setImplementation(g.getImplementation());
	}

	return *this;
}

//==============methods for creating and opening groups========================
template<typename Imp>
NXGroup<typename NXGroup<Imp>::GImp> NXGroup<Imp>::createGroup(const String &n) const{
	EXCEPTION_SETUP("template<typename Imp> NXGroup<typename NXGroup<Imp>::GImp> NXGroup<Imp>::createGroup(const String &n) const");

	NXGroup<typename NXGroup<Imp>::GImp> group;

	try{
		group.setImplementation(NXObject<Imp>::getImplementation().createGroup(n));
	}catch(...){
		EXCEPTION_INIT(NXGroupError,"Error creating group ["+n+"] below group ["+this->getName()+"]!");
		EXCEPTION_THROW();
	}
	return group;
}

template<typename Imp>
NXGroup<typename NXGroup<Imp>::GImp > NXGroup<Imp>::createGroup(const String &n,const String &type) const{
	EXCEPTION_SETUP("template<typename Imp> NXGroup<typename NXGroup<Imp>::GImp > NXGroup<Imp>::createGroup(const String &n) const");

	NXGroup<typename NXGroup<Imp>::GImp > group;

	//before creating the group we should first lookup if the group
	//really exists - this has to be implemented!!!!

	//create the group
	try{
		group.setImplementation(NXObject<Imp>::getImplementation().createGroup(n));
	}catch(...){
		EXCEPTION_INIT(NXGroupError,"Error creating group ["+n+"] below group ["+this->getName()+"]!");
		EXCEPTION_THROW();
	}

	//here we have to set the group type
	group.setAttribute("NX_class",type);

	return group;
}

template<typename Imp>
NXGroup<typename NXGroup<Imp>::GImp > NXGroup<Imp>::openGroup(const String &n) const{
	EXCEPTION_SETUP("template<typename Imp> NXGroup<typename NXGroup<Imp>::GImp > NXGroup<Imp>::openGroup(const String &n) const");

	NXGroup<GImp> group ;

	//open a group
	try{
		group.setImplementation(this->getImplementation().openGroup(n));
	}catch(...){
		EXCEPTION_INIT(NXGroupError,"Error opening group ["+n+" from group ["+this->getName()+"]!");
		EXCEPTION_THROW();
	}

	return group;
}


//================Methods for opening and creating fields======================

template<typename Imp>
NXField<typename NXGroup<Imp>::FImp >
NXGroup<Imp>::createNumericField(const String &n,PNITypeID tid,const ArrayShape &s,
		                  const String &unit,const String &desc) const{
	EXCEPTION_SETUP("template<typename Imp> NXField<typename NXGroup<Imp>::FImp >"
					"NXGroup<Imp>::createNumericField(const String &n,"
					"PNITypeID tid,const ArrayShape &s,const String &unit,"
					"const String &desc) const");

	NXField<FImp> field;
	try{
		field.setImplementation(this->getImplementation().createNumericField(n,tid,s));
		field.setAttribute("unit",unit);
		field.setAttribute("description",desc);
	}catch(...){
		EXCEPTION_INIT(NXGroupError,"Error creating array field ["+n+"] below group ["+this->getName()+"]!");
		EXCEPTION_THROW();
	}

	return field;
}

template<typename Imp>
template<typename Filter>
NXField<typename NXGroup<Imp>::FImp >
NXGroup<Imp>::createNumericField(const String &n,PNITypeID tid,
		                  const ArrayShape &s,const String &unit,const String &desc,
		                  NXFilter<Filter> &f) const{
	EXCEPTION_SETUP("template<typename Imp> template<typename Filter>"
				    " NXField<typename NXGroup<Imp>::FImp > "
					"NXGroup<Imp>::createNumericField(const String &n,"
					"PNITypeID tid,const ArrayShape &s,const String &unit,"
					"const String &desc,NXFilter<Filter> &f) const");

	NXField<FImp> field;

	try{
		field.setImplementation(this->_imp.createNumericField(n.c_str(),tid,s,f));
		field.setAttribute("unit",unit);
		field.setAttribute("description",desc);
	}catch(...){
		EXCEPTION_INIT(NXGroupError,"Error creating array field ["+n+"] below grou ["+this->getName()+"]!");
		EXCEPTION_THROW();
	}

	return field;
}

template<typename Imp>
NXField<typename NXGroup<Imp>::FImp >
NXGroup<Imp>::createNumericField(const ArrayObject &a) const{
	EXCEPTION_SETUP("template<typename Imp> NXField<typename NXGroup<Imp>::FImp >"
					" NXGroup<Imp>::createNumericField(const ArrayObject &a) const");

	NXField<typename NXGroup<Imp>::FImp > field;
	try{
		field = createNumericField(a.getName(),a.getTypeID(),a.getShape(),a.getUnit(),a.getDescription());
	}catch(...){
		EXCEPTION_INIT(NXGroupError,"Error creating array object ["+a.getName()+"] below group ["+this->getName()+"]!");
		EXCEPTION_THROW();
	}

	return field;
}

template<typename Imp>
template<typename Filter>
NXField<typename NXGroup<Imp>::FImp >
NXGroup<Imp>::createNumericField(const ArrayObject &a,NXFilter<Filter> &f) const{
	EXCEPTION_SETUP("template<typename Imp> template<typename Filter>"
					" NXField<typename NXGroup<Imp>::FImp > "
					"NXGroup<Imp>::createNumericField(const ArrayObject &a,"
					"NXFilter<Filter> &f) const");

	NXField<typename NXGroup<Imp>::FImp > field;
	try{
		field = createNumericField(a.getName(),a.getTypeID(),a.getShape(),a.getUnit(),a.getDescription(),f);
	}catch(...){
		EXCEPTION_INIT(NXGroupError,"Error creating array field ["+a.getName()+"] below group ["+this->getName()+"]!");
		EXCEPTION_THROW();
	}

	return field;
}


template<typename Imp>
NXField<typename NXGroup<Imp>::FImp > NXGroup<Imp>::openField(const String &n) const{
	EXCEPTION_SETUP("template<typename Imp> NXField<typename NXGroup<Imp>::FImp > NXGroup<Imp>::openField(const String &n) const");
	NXField<FImp> field;

	try{
		field.setImplementation(this->getImplementation().openField(n));
	}catch(...){
		EXCEPTION_INIT(NXGroupError,"Error opening field ["+n+"] in group ["+this->getName()+"]!");
		EXCEPTION_THROW();
	}

	return field;
}

template<typename Imp> NXField<typename NXGroup<Imp>::FImp >
NXGroup<Imp>::createNumericField(const String &n,PNITypeID tid,const String &unit,const String &desc) const{
	EXCEPTION_SETUP("template<typename Imp> NXField<typename NXGroup<Imp>::FImp > "
					"NXGroup<Imp>::createNumericField(const String &n,"
					"PNITypeID tid,const String &unit,const String &desc) const");
	NXField<FImp> field;

	try{
		field.setImplementation(this->getImplementation().createNumericField(n,tid));
		field.setAttribute("unit",unit);
		field.setAttribute("description",desc);
	}catch(...){
		EXCEPTION_INIT(NXGroupError,"Cannot create scalar field ["+n+"] below group ["+this->getName()+"]!");
		EXCEPTION_THROW();
	}

	return field;
}

template<typename Imp>
NXField<typename NXGroup<Imp>::FImp > NXGroup<Imp>::createNumericField(const ScalarObject &s) const{
	EXCEPTION_SETUP("template<typename Imp> NXField<typename NXGroup<Imp>::FImp > "
					"NXGroup<Imp>::createNumericField(const ScalarObject &s) const");
	NXField<typename NXGroup<Imp>::FImp > field;

	try{
		field =createNumericField(s.getName(),s.getTypeID(),s.getUnit(),s.getDescription());
	}catch(...){
		EXCEPTION_INIT(NXGroupError,"Cannot create scalar field ["+s.getName()+"] below group ["+this->getName()+"]!");
		EXCEPTION_THROW();
	}
	return field;
}


template<typename Imp>
NXField<typename NXGroup<Imp>::FImp > NXGroup<Imp>::createStringField(const String &n) const{
	EXCEPTION_SETUP("template<typename Imp> NXField<typename NXGroup<Imp>::FImp > NXGroup<Imp>::createStringField(const String &n,const String &s) const");
	NXField<FImp> field;

	try{
		field.setImplementation(this->getImplementation().createStringField(n,0));
	}catch(...){
		EXCEPTION_INIT(NXGroupError,"Error creating string field ["+n+"] below group ["+this->getName()+"]!");
		EXCEPTION_THROW();
	}

	return field;
}



template<typename Imp>
void NXGroup<Imp>::close(){
	this->getImplementation().close();
}

//end of namespace
}
}


#endif /* NXGROUP_HPP_ */
