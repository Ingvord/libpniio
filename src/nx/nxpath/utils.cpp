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
// Created on: Jun 26, 2013
//     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//

#include <sstream>
#include <pni/io/nx/nxpath/utils.hpp>
#include <pni/core/error.hpp>


namespace pni{
namespace io{
namespace nx{

    void append(nxpath &p,const string &gname,const string &gclass)
    {
        p.push_back(nxpath::element_type{gname,gclass});
    }

    //--------------------------------------------------------------------------
    void prepend(nxpath &p,const string &gname,const string &gclass)
    {
        p.push_front(nxpath::element_type{gname,gclass});
    }

    //--------------------------------------------------------------------------
    void split_path(const nxpath &p,size_t s,nxpath &p1,nxpath &p2)
    {

        if(s>=p.size())
        {
            std::stringstream ss;
            ss<<"Split index "<<s<<" exceeds input path size ";
            ss<<p.size()<<"!";
            throw index_error(EXCEPTION_RECORD,ss.str());
        }
        auto split_iter = p.begin();
        std::advance(split_iter,s);

        nxpath::elements_type gp1(s),gp2(p.size()-s);
        std::copy(p.begin(),split_iter,gp1.begin());
        std::copy(split_iter,p.end(),gp2.begin());

        //if the original path was absolute also the first part of the two 
        //must be absolute
        p1 = nxpath(p.filename(),gp1,"");
        p2 = nxpath("",gp2,p.attribute());
    }

    //--------------------------------------------------------------------------
    void split_last(const nxpath &p,nxpath &gpath,nxpath &opath)
    {
        split_path(p,p.size()-1,gpath,opath);
    }

    //--------------------------------------------------------------------------
    string string_from_path(const nxpath &p)
    {
        string ostr;
        if(!p.filename().empty())
            ostr += p.filename()+"://";

        if(p.is_absolute()) ostr += "/";

        //dump groups
        auto slash_iter = p.begin();
        std::advance(slash_iter,p.size()-1);
        for(auto iter = p.begin();iter!=p.end();++iter)
        {
            if(!iter->first.empty()) ostr += iter->first;
            if(!iter->second.empty()) ostr += ":"+iter->second;
            if(iter!=slash_iter) ostr += "/";
        }

        if(!p.attribute().empty())
            ostr += "@"+p.attribute();

        return ostr;
    }

    //--------------------------------------------------------------------------
    bool has_name(const nxpath::element_type &e)
    {
        return !e.first.empty();
    }

    //--------------------------------------------------------------------------
    bool has_class(const nxpath::element_type &e)
    {
        return !e.second.empty();
    }

    //--------------------------------------------------------------------------
    bool is_complete(const nxpath::element_type &e)
    {
        return has_name(e)&&has_class(e);
    }


    //--------------------------------------------------------------------------
    std::istream &operator>>(std::istream &i,nxpath &p)
    {   
        string buffer;
        i>>buffer;
        p = nxpath::from_string(buffer);
        return i;
    }

    //--------------------------------------------------------------------------
    bool operator==(const nxpath &lhs,const nxpath &rhs)
    {
        if(lhs.filename()!=rhs.filename()) return false;

        if(rhs.size() != lhs.size()) return false;

        for(auto lhs_iter = lhs.begin(), rhs_iter = rhs.begin();
                 lhs_iter != lhs.end();
                 ++lhs_iter,++rhs_iter)
        {
            if((lhs_iter->first != rhs_iter->first)||
               (lhs_iter->second != rhs_iter->second)) return false;
        }

        if(lhs.attribute() != rhs.attribute()) return false;

        return true;
    }

    //--------------------------------------------------------------------------
    bool operator!=(const nxpath &lhs,const nxpath &rhs)
    {
        return !(lhs == rhs);
    }

//end of namespace
}
}
}