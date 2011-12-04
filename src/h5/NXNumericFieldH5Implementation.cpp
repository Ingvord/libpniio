/*
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
 * NXNumericFieldH5Implementation.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: Eugen Wintersberger
 */

#include "NXNumericFieldH5Implementation.hpp"
#include "H5Utilities.hpp"
#include "H5Exceptions.hpp"

namespace pni {
namespace nx {
namespace h5 {
//===========================Constructor implementation=========================

//implementation of the default constructor
NXNumericFieldH5Implementation::NXNumericFieldH5Implementation()
:NXFieldH5Implementation(){

}

//------------------------------------------------------------------------------
//implementation of the copy constructor
NXNumericFieldH5Implementation::
NXNumericFieldH5Implementation(const NXNumericFieldH5Implementation &o):
NXFieldH5Implementation(o){

}

//------------------------------------------------------------------------------
//implementation of the move constructor
NXNumericFieldH5Implementation::
NXNumericFieldH5Implementation(NXNumericFieldH5Implementation &&o):
NXFieldH5Implementation(std::move(o)){

}

//------------------------------------------------------------------------------
//implementation of the copy conversion constructor
NXNumericFieldH5Implementation::
NXNumericFieldH5Implementation(const NXFieldH5Implementation &o):
NXFieldH5Implementation(o){

}

//------------------------------------------------------------------------------
//implementation of the copy conversion constructor
NXNumericFieldH5Implementation::
NXNumericFieldH5Implementation(const NXObjectH5Implementation &o):
NXFieldH5Implementation(o){

}

//------------------------------------------------------------------------------
//implementation of the copy conversion constructor
NXNumericFieldH5Implementation::
NXNumericFieldH5Implementation(NXFieldH5Implementation &&o):
NXFieldH5Implementation(std::move(o)){

}

//------------------------------------------------------------------------------
//implementation of the move conversion constructor
NXNumericFieldH5Implementation::
NXNumericFieldH5Implementation(NXObjectH5Implementation &&o):
NXFieldH5Implementation(std::move(o)){

}

//------------------------------------------------------------------------------
//destructor implementation
NXNumericFieldH5Implementation::~NXNumericFieldH5Implementation() {

}

//===============Assignment operator implementation=============================
//implementation of copy assignment operator
NXNumericFieldH5Implementation &
NXNumericFieldH5Implementation::operator=(const NXNumericFieldH5Implementation &o){
	(NXFieldH5Implementation &)(*this) = (NXFieldH5Implementation &)o;
	return *this;
}

//------------------------------------------------------------------------------
//implementation of move assignment operator
NXNumericFieldH5Implementation &
NXNumericFieldH5Implementation::operator=(NXNumericFieldH5Implementation &&o){
	(NXFieldH5Implementation &)(*this) = std::move((NXFieldH5Implementation &&)o);
	return *this;
}

//------------------------------------------------------------------------------
//implementation of copy conversion assignment
NXNumericFieldH5Implementation &
NXNumericFieldH5Implementation::operator=(const NXFieldH5Implementation &o){
	(NXFieldH5Implementation &)(*this) = o;
	return *this;
}

//------------------------------------------------------------------------------
//implementation of move conversion assignment
NXNumericFieldH5Implementation &
NXNumericFieldH5Implementation::operator=(NXFieldH5Implementation &&o){
	(NXFieldH5Implementation &)(*this) = std::move(o);
	return *this;
}

//------------------------------------------------------------------------------
//implementation of copy conversion assignment
NXNumericFieldH5Implementation &
NXNumericFieldH5Implementation::operator=(const NXObjectH5Implementation &o){
	(NXFieldH5Implementation &)(*this) = o;
	return *this;
}

//------------------------------------------------------------------------------
//implementation of move conversion assignment
NXNumericFieldH5Implementation &
NXNumericFieldH5Implementation::operator=(NXObjectH5Implementation &&o){
	(NXFieldH5Implementation &)(*this) = std::move(o);
	return *this;
}

//==============Implementation of IO methods====================================
//implementation of append data
void NXNumericFieldH5Implementation::append(const NumericObject &o,bool block){
	EXCEPTION_SETUP("void NXNumericFieldH5Implementation::"
					"append_element(const NumericObject &o,bool block)");
	hid_t memspace = 0;
	herr_t err;
	hid_t elem_type;

	//get the appropriate memory type
	elem_type = H5TFactory.getTypeFromID(o.getTypeID());

	//set the offset for the selection to the last index in the container
	_offset[0] = getShape().getDimension(0);

	if(block){
		ArrayObject &array = (ArrayObject &)o;
		H5Utilities::ArrayShape2DataSpace(array.getShape(),memspace);
		_count[0] = array.getShape().getDimension(0);
		_resize_dataset(_count[0]);
	}else{
		//if we append only a single element
		memspace = _elemspace;
		_count[0] = 1;
		_resize_dataset(1);
	}

	//set the selection and write data
	H5Sselect_hyperslab(_filespace,H5S_SELECT_SET,_offset,NULL,_count,NULL);
	err = H5Dwrite(getId(),elem_type,memspace,_filespace,H5P_DEFAULT,o.getVoidPtr());
	if(err<0){
		EXCEPTION_INIT(H5DataSetError,"Error writing data to field ["+getName()+"]!");
		EXCEPTION_THROW();
	}

	//cleanup after appending a block
	if(block){
		if(H5Iis_valid(memspace)) H5Sclose(memspace);
	}
}

//------------------------------------------------------------------------------
//implementation of set
void NXNumericFieldH5Implementation::set(const UInt64 &i,const NumericObject &o,bool block){
	EXCEPTION_SETUP("void NXNumericFieldH5Implementation::"
					"set(const UInt64 &i,const NumericObject &d,bool block)");

	hid_t memspace = 0;
	hid_t dataset = getId();
	herr_t err = 0;
	hid_t elem_type;

	//get the appropriate memory type
	elem_type = H5TFactory.getTypeFromID(o.getTypeID());

	//resize the array if i is exceeds the bounds
	if(i>=getShape().getDimension(0)){
		_resize_dataset(1+i-getShape().getDimension(0));
	}

	//set offset
	_offset[0] = i;

	if(block){
		ArrayObject &array = (ArrayObject &)o;
		_resize_dataset(array.getShape().getDimension(0)-1);
		H5Utilities::ArrayShape2DataSpace(array.getShape(),memspace);
		_count[0] = array.getShape().getDimension(0);
	}else{
		_count[0] = 1;
		memspace = _elemspace;
	}

	//set the selection and write data
	H5Sselect_hyperslab(_filespace,H5S_SELECT_SET,_offset,NULL,_count,NULL);
	err = H5Dwrite(dataset,elem_type,memspace,_filespace,H5P_DEFAULT,o.getVoidPtr());
	if(err<0){
		EXCEPTION_INIT(H5DataSetError,"Error writing data to field ["+getName()+"]!");
		EXCEPTION_THROW();
	}

	//cleanup after appending a block
	if(block){
		if(H5Iis_valid(memspace)) H5Sclose(memspace);
	}
}

//------------------------------------------------------------------------------
//implementation of get
void NXNumericFieldH5Implementation::get(const UInt64 &i,NumericObject &o,bool block){
	EXCEPTION_SETUP("void NXNumericFieldH5Implementation::"
					"get(const UInt64 &i,const NumericObject &d,bool block)");

	if(i>=getShape().getDimension(0)){
		EXCEPTION_INIT(IndexError,"Element index exceeds container size!");
		EXCEPTION_THROW();
	}

	hid_t memspace = 0;
	hid_t dataset = getId();
	herr_t err = 0;
	hid_t elem_type;

	//get the appropriate memory type
	elem_type = H5TFactory.getTypeFromID(o.getTypeID());

	//set the offset for the selection to the last index in the container
	_offset[0] = i;

	if(block){
		ArrayObject &array = (ArrayObject &)o;
		if((i+array.getShape().getDimension(0))>getShape().getDimension(0)){
			EXCEPTION_INIT(IndexError,"Index + block size exceeds container size!");
			EXCEPTION_THROW();
		}
		H5Utilities::ArrayShape2DataSpace(array.getShape(),memspace);
		_count[0] = array.getShape().getDimension(0);
	}else{
		memspace = _elemspace;
		_count[0] = 1;
	}

	//setup the selection
	H5Sselect_hyperslab(_filespace,H5S_SELECT_SET,_offset,NULL,_count,NULL);
	err = H5Dread(dataset,elem_type,memspace,_filespace,H5P_DEFAULT,o.getVoidPtr());
	if(err<0){
		EXCEPTION_INIT(H5DataSetError,"Error reading data from field ["+getName()+"]!");
		EXCEPTION_THROW();
	}

	if(block){
		if(H5Iis_valid(memspace)) H5Sclose(memspace);
	}
}

//-----------------------------------------------------------------------------
//implementation of get all
void NXNumericFieldH5Implementation::get(NumericObject &o){
	EXCEPTION_SETUP("void NXNumericFieldH5Implementation::get(NumericObject &o)");
	herr_t err;
	hid_t dataset = getId();
	hid_t elem_type;

	//get the appropriate memory type
	elem_type = H5TFactory.getTypeFromID(o.getTypeID());

	//clear all selection
	H5Sselect_none(_filespace);

	//read the data
	err = H5Dread(dataset,elem_type,H5S_ALL,H5S_ALL,H5P_DEFAULT,o.getVoidPtr());
	if(err<0){
		EXCEPTION_INIT(H5DataSetError,"Error reading data from field ["+getName()+"]!");
		EXCEPTION_THROW();
	}

}

} /* namespace h5 */
} /* namespace nx */
} /* namespace pni */