//
// (c) Copyright 2011 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
// Created on: Jan 10, 2012
//     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//


#include <sstream>
extern "C"{
#include<hdf5.h>
}

#include <pni/io/nx/h5/h5_error_stack.hpp>
#include <pni/io/nx/nxexceptions.hpp>
#include <pni/io/nx/nxpath.hpp>

namespace pni{
namespace io{
namespace nx{
namespace h5{

    //============implementation of constructors and destructors===============
    //implementation of the default constructor
    group_imp::group_imp() noexcept
        :_object()
    {}

    //-------------------------------------------------------------------------
    group_imp::group_imp(h5object &&o)
        :_object(o)
    {
        if(get_hdf5_type(_object)!=h5object_type::GROUP)
            throw type_error(EXCEPTION_RECORD,
                    "Object is not a group instance!");
    }

    //-------------------------------------------------------------------------
    //implementation of the copy constructor
    group_imp::group_imp(const group_imp &o)
        :_object(o._object)
    {}

    //-------------------------------------------------------------------------
    //implementation of the move constructor
    group_imp::gruop_imp(group_imp &&o) noexcept
        :_object(std::move(o._object))
    {}

    //-------------------------------------------------------------------------
    //implementation of the standard constructor
    H5Group::H5Group(const string &name,const H5Group &parent)
    {
        hid_t link_pl = 0; //link creation property list
        hid_t cr_pl   = 0; //creation property list

        //create link property list
        link_pl = H5Pcreate(H5P_LINK_CREATE);
        if(link_pl < 0) 
        {
            string estr = "Creation of link property list failed"
                "for new group ["+name+"] under ["
                +get_object_path(parent.id())+
                "]!\n\n"+get_h5_error_string();
            throw pni::io::nx::nxgroup_error(EXCEPTION_RECORD,estr);
        }

        hid_t gid = H5Gcreate2(parent.id(),name.c_str(),
                link_pl,cr_pl,H5P_DEFAULT);
        if(gid<0)
        {
            string estr = "Error creating group ["+name+
                          "] under ["+get_object_path(parent.id())
                          +"!\n\n"+get_h5_error_string();
            throw pni::io::nx::nxgroup_error(EXCEPTION_RECORD,estr);
        }

        *this = H5Group(gid); 
        H5Pclose(link_pl);
    }


    //==============implementation of assignment operators=====================
    //implementation of copy assignment
    H5Group &H5Group::operator=(const H5Group &o)
    {
        if(this == &o) return *this;

        H5AttributeObject::operator=(o);

        return *this;
    }
    
    //-------------------------------------------------------------------------
    //implementation of copy conversion assignment
    H5Group &H5Group::operator=(const H5Object &o)
    {

        if(o.object_type() != H5ObjectType::GROUP)
            throw pni::io::nx::nxgroup_error(EXCEPTION_RECORD,
                    "Object is not a group object!\n\n"+
                    get_h5_error_string());

        if(this == &o) return *this;

        H5AttributeObject::operator=(o);

        return *this;
    }

    //-------------------------------------------------------------------------
    //implementation of move assignment
    H5Group &H5Group::operator=(H5Group &&o)
    {
        if(this == &o) return *this;

        H5AttributeObject::operator=(std::move(o));

        return *this;
    }
    
    //-------------------------------------------------------------------------
    //implementation of move conversion assignment
    H5Group &H5Group::operator=(H5Object &&o)
    {

        if(o.object_type() != H5ObjectType::GROUP)
            throw pni::io::nx::nxgroup_error(EXCEPTION_RECORD,
                    "Object is not a group object!\n\n"+
                    get_h5_error_string());

        if(this == &o) return *this;
        H5AttributeObject::operator=(std::move(o));

        return *this;
    }


    //-------------------------------------------------------------------------
    H5Object H5Group::open(const string &n) const
    {
        hid_t oid = H5Oopen(id(),n.c_str(),H5P_DEFAULT);
        if(oid<0)
            throw pni::io::nx::nxobject_error(EXCEPTION_RECORD, 
                    "Error opening object ["+n+"]!\n\n"+
                    get_h5_error_string());

        //determine the object type
        H5I_type_t tid = H5Iget_type(oid);
        if(tid==H5I_BADID)
            throw pni::io::nx::nxbackend_error(EXCEPTION_RECORD,
                    "Error obtaining object type for ["+n+"]!\n\n"+
                    get_h5_error_string());

        H5Object o;
        if(tid == H5I_GROUP) o = std::move(H5Group(oid)); 
        else if(tid == H5I_DATASET) o = std::move(H5Dataset(oid));
        
        //H5Oclose(oid);
        return o;
    }

    //-------------------------------------------------------------------------
    H5Object H5Group::operator[](const string &n) const
    {
        return this->open(n);
    }

    //-------------------------------------------------------------------------
    H5Object H5Group::open(size_t i) const
    {
        if(i >= nchildren())
        {
            std::stringstream sstream;
            sstream<<"Index ("<<i<<") exceeds number of child nodes ("
                <<nchildren()<<")!";
            throw index_error(EXCEPTION_RECORD,sstream.str());
        }

        char name[1024];
        ssize_t size = H5Lget_name_by_idx(id(),".",H5_INDEX_NAME,H5_ITER_INC,
                i,name,1024,H5P_DEFAULT);

        if(size < 0)
            throw pni::io::nx::nxgroup_error(EXCEPTION_RECORD, 
                    "Error obtaining object name!\n\n"+
                    get_h5_error_string());

        return open(string(name));
    }

    //-------------------------------------------------------------------------
    H5Object H5Group::operator[](size_t i) const
    {
        return open(i);
    }


    //-------------------------------------------------------------------------
    bool H5Group::exists(const string &n) const
    {
        htri_t retval;
        std::vector<string> plist;
        std::vector<string>::iterator iter;
        size_t spos1,spos2;
        string path;
        bool is_abs = false;

        //remove a trailing slash
        if(n[n.size()-1]=='/') path = n.substr(0,n.size()-1);
        else path = n;

        if(n[0] == '/') is_abs = true;

        //have to split the string
        spos1 = path.find_first_of("/");
        if(spos1 != n.npos)
        {
            if(spos1 != 0) plist.push_back(path.substr(0,spos1));

            do
            {
                spos2 = path.find_first_of("/",spos1+1);
                plist.push_back(path.substr(spos1+1,(spos2-spos1)-1));
                spos1 = spos2;
            }while(spos2 != path.npos);
        }
        else plist.push_back(path);

        //clear the path
        path.clear();
        if(is_abs) path += "/";

        for(iter = plist.begin(); iter != plist.end(); ++iter)
        {
            path += *iter;

            //do check
            //std::cout<<path<<std::endl;
            retval = H5Lexists(id(),path.c_str(),H5P_DEFAULT);
            if(retval<0) 
                throw pni::io::nx::nxgroup_error(EXCEPTION_RECORD, 
                        "Cannot check existance of objec ["+n+"]!\n\n"+
                        get_h5_error_string());

            if(!retval) return false;

            //append a trailing /
            path += "/";
        }

        return true;

    }



    //-------------------------------------------------------------------------
    size_t H5Group::nchildren() const
    {
        H5G_info_t ginfo;
        herr_t err = H5Gget_info(id(),&ginfo);
        if(err < 0)
            throw pni::io::nx::nxgroup_error(EXCEPTION_RECORD,
                    "Cannot obtain group information!\n\n"+
                    get_h5_error_string());

        return ginfo.nlinks;
    }            

//end of namespace
}
}
}
}

