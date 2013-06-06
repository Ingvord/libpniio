/*
 * (c) Copyright 2011 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
 * Implementation of an HDF5 attribute object.
 *
 * Created on: Jan 10, 2012
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */

#include "H5Attribute.hpp"


namespace pni{
namespace io{
namespace nx{
namespace h5{
    //===============private methods===========================================
    void H5Attribute::__set_space_type()
    {
        hid_t dspace_id = 0;
        hid_t dtype_id = 0;

        if(is_valid())
        {
            //obtain the IDs of the dataspace and the datatype used 
            //to create the attribute.
            dspace_id = H5Aget_space(id());
            dtype_id  = H5Aget_type(id());

            //we can use this Ids to create new H5Objects from which 
            //the datatype and the dataspace can be constructed.
            _dspace = H5Dataspace(dspace_id);
            _dtype  = H5Datatype(dtype_id);
        }
        else
        {
            _dspace = H5Dataspace();
            _dtype = H5Datatype();
        }
    }

    //===========constructors and destructors==================================
    //! implementation of the default constructor
    H5Attribute::H5Attribute():H5NamedObject(){ }

    //-------------------------------------------------------------------------
    //implementation of the copy constructor
    H5Attribute::H5Attribute(const H5Attribute &o):H5NamedObject(o)
    {
        //once the ID is set we can obtain the datatype and 
        //the data space
        __set_space_type();
    }

    //--------------------------------------------------------------------------
    //implementation of the move constructor
    H5Attribute::H5Attribute(H5Attribute &&o):
        H5NamedObject(std::move(o))
    {
        _dspace = std::move(o._dspace);
        _dtype  = std::move(o._dtype);
    }
    //-------------------------------------------------------------------------
    H5Attribute::H5Attribute(const hid_t &id):
        H5NamedObject(id)
    {
        __set_space_type();
    }

    //-----------------------------------------------------------------
    //implementation of the destructor
    H5Attribute::~H5Attribute()
    {
        _dspace.close();
        _dtype.close();

        if(is_valid()) H5Aclose(id());
        H5Object::reset_id();
    }

    //===============implementation of assignment operators====================
    // implementation of copy assignment operator
    H5Attribute &H5Attribute::operator=(const H5Attribute &a)
    {
       if(this != &a)
       {
           H5NamedObject::operator=(a);
           __set_space_type();
       }
       return *this;
    }
    
    //-------------------------------------------------------------------------
    //implementation of move assignment operator
    H5Attribute &H5Attribute::operator=(H5Attribute &&o)
    {
        if(this != &o)
        {
            H5NamedObject::operator=(std::move(o));
            _dspace = std::move(o._dspace);
            _dtype  = std::move(o._dtype);
        }
        return *this;
    }

    //=========implementation of inquery methods===============================
    type_id_t H5Attribute::type_id() const
    {
        return _dtype.type_id();
    }

    //-------------------------------------------------------------------------
    void H5Attribute::close()
    {
        _dspace.close();
        _dtype.close();

        if(is_valid()) H5Aclose(id());
        H5Object::reset_id();
    }

    //-------------------------------------------------------------------------
    string H5Attribute::name() const
    {
        char name[1024];
        H5Aget_name(id(),1024,name);
        return string(name);
    }

    //-------------------------------------------------------------------------
    string H5Attribute::base() const { return string(""); }

    //-------------------------------------------------------------------------
    string H5Attribute::path() const { return string(""); }
    
    //-------------------------------------------------------------------------
    //implementation of write from String
    void H5Attribute::write(const string *s) const
    {
        typedef const char * char_ptr_t;
        
        char_ptr_t *ptr = new char_ptr_t[size()];
        for(size_t i=0;i<size();i++) ptr[i] = s[i].c_str();

        //get element type
        hid_t element_type = H5Aget_type(id()); 

        herr_t err = H5Awrite(id(),element_type,ptr);
        delete [] ptr;

        if(err < 0)
            throw pni::io::nx::nxattribute_error(EXCEPTION_RECORD, 
                    "Error writing attribute ["+name()+"]!\n\n"+
                    get_h5_error_string());

        //close the data type
        H5Tclose(element_type);
    }

    //-------------------------------------------------------------------------
    //implementation to read to string
    void H5Attribute::read(string *s) const
    {
        typedef char * char_ptr_t;

        hid_t element_type = H5Aget_type(id());

        char_ptr_t *ptr = new char_ptr_t[size()];

        herr_t err = H5Aread(id(),element_type,ptr);
        if(err<0)
        {
            delete [] ptr;
            throw pni::io::nx::nxattribute_error(EXCEPTION_RECORD, 
                    "Error reading attribute ["+name()+"]!\n\n"+
                    get_h5_error_string());
        }

        //close the data type
        H5Tclose(element_type);

        for(size_t i = 0;i<size();i++)
        {
            s[i] = string(ptr[i]);
        }
        delete [] ptr;
    }
           

//end of namespace
}
}
}
}