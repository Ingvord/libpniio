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
// ===========================================================================
//
// Created on: Nov 26, 2014
//     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//

#pragma once

#include <pni/core/types.hpp>
#include <pni/core/arrays.hpp>
#include <pni/core/type_erasures.hpp>

#include "../algorithms/create_field.hpp"
#include "../algorithms/get_name.hpp"
#include "../algorithms/get_class.hpp"
#include "../algorithms/set_class.hpp"
#include "xml_node.hpp"
#include "attribute_data.hpp"


namespace pni{
namespace io{
namespace nx{
namespace xml{

    using namespace pni::core;
   
    //!
    //! \ingroup xml_lowlevel_utils
    //! \brief read and write groups
    struct group
    {

        //!
        //! \brief create group from XML
        //!
        //! \throws parser_error in case of parsing problems
        //!
        //! \throws invalid_object_error if the parent is not valid
        //! \throws parser_error if the content of the group node is illformed 
        //! and cannot be parsed
        //! \throws io_error if metadadata writing failed
        //! \throws key_error if the group already exists or the name is
        //! otherwise invalid. 
        //! \throws object_error in case of any other error
        //!
        //! \tparam GTYPE group type
        //! \param parent the parent group of the new group
        //! \param group_node the node storing the group data
        //! \return group instance
        //!
        template<typename GTYPE>
        static typename GTYPE::group_type 
        from_xml(const GTYPE &parent,const node &group_node)
        {
            typedef attribute_data<string> attr_data;
            typedef GTYPE group_type;
            typedef typename GTYPE::value_type object_type;

            auto name   = attr_data::read(group_node,"name");
            group_type group = create_group(object_type(parent),name);

            try
            {
                auto gclass = attr_data::read(group_node,"type");    
                set_class(group,gclass);
            }
            catch(...)
            {}

            
            return group;
        }

        //-----------------------------------------------------------------
        //!
        //! \brief write group to XML
        //!
        //! Create an XML group from a Nexus group. 
        //!
        //! \throws parser_error in case of problems parsing required XML
        //! attributes
        //! \throws type_error if the passed type is not a group type
        //! \throws io_error if retrievel of group properties failed 
        //! \throws invalid_object_error if the group is not valid
        //! \throws object_error in case of any other error
        //!
        //! \tparam GTYPE group type
        //! \param group instance of GTYPE
        //! \return node with group information
        //!
        template<typename GTYPE>
        static node to_xml(const GTYPE &group)
        {
            node group_node;

            //write name and type attributes
            group_node.put("<xmlattr>.name",get_name(group));

            string gclass = get_class(group);
            if(!gclass.empty())
                group_node.put("<xmlattr>.type",gclass);
            
            return group_node;
        }
    };

//end of namespace
}
}
}
}
