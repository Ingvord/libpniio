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
// Created on: May 29, 2014
//     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//
#pragma once

#include <pni/core/types.hpp>
#include <pni/core/error.hpp>
#include <pni/io/nx/nxobject.hpp>
#include <pni/io/nx/nxobject_traits.hpp>
#include <pni/io/nx/algorithms/get_class.hpp>

namespace pni{
namespace io{
namespace nx{

    //!
    //! \ingroup algorithm_code
    //! \brief checks group type
    //!
    //! This function template checks whether or not a group is of a particular
    //! type. 
    //!
    //! \throws invalid_object_error if parent group is not valid
    //! \throws shape_mismatch_error the attribute is not a scalar
    //! \throws type_error the attribute type is not supported
    //! \throws io_error attribute data retrieval failed
    //! \throws object_error in case of any other error
    //!
    //! \tparam OTYPE nexus object template
    //! \tparam IMPID implementation ID of the object
    //!
    //! \param object the group to check
    //! \param type the type we want ot check for
    //! \return true if the group is of that type, false otherwise
    //!
    template<
             template<nximp_code> class OTYPE,
             nximp_code IMPID
            >
    bool is_class(const OTYPE<IMPID> &object,const pni::core::string &type)
    {
        using group_type = typename nxobject_trait<IMPID>::group_type;

        static_assert(std::is_same<group_type,OTYPE<IMPID>>::value,
                      "The object must be a group type!");

        return get_class(object)==type;
    }

    //!
    //! \ingroup algorithm_internal_code
    //! \brief is class visitor
    //!
    //! This visitor is used to check the class a group belongs too.
    //! 
    //! \tparam GTYPE group type
    //! \tparam FTYPE field type
    //! \tparam ATYPE attribute type
    //! \tparam LTYPE link type
    //!
    template<
             typename GTYPE,
             typename FTYPE,
             typename ATYPE,
             typename LTYPE
            > 
    class is_class_visitor : public boost::static_visitor<bool>
    {
        private:
            pni::core::string _class; //!< class type
        public:
            //! result type
            using result_type = bool;
            //! Nexus group type
            using group_type = GTYPE;
            //! Nexus field type
            using field_type = FTYPE;
            //! Nexus attribute type
            using attribute_type = ATYPE;
            //! NeXus link type
            using link_type = LTYPE;

            //-----------------------------------------------------------------
            //!
            //! \brief constructor
            //!
            //! \param s class type
            //!
            is_class_visitor(const pni::core::string &s):_class(s) {}
           
            //-----------------------------------------------------------------
            //!
            //! \brief process groups
            //!
            //! \throws invalid_object_error if parent group is not valid
            //! \throws shape_mismatch_error the attribute is not a scalar
            //! \throws type_error the attribute type is not supported
            //! \throws io_error attribute data retrieval failed
            //! \throws object_error in case of any other error
            //!
            //! \param g group instance
            //! \return true if class types match, false otherwise
            //!
            result_type operator()(const group_type &g) const
            {
                return is_class(g,_class);
            }

            //-----------------------------------------------------------------
            //!
            //! \brief process fields
            //! 
            //! \throws type_error fields do not have a class
            //! \param f field instance
            //! \return can be ignored 
            //!
            result_type operator()(const field_type &) const
            {
                using namespace pni::core;
                throw type_error(EXCEPTION_RECORD,
                        "Fields do not have a class!");
                return false;
            }

            //----------------------------------------------------------------
            //!
            //! \brief process attributes
            //!
            //! \throws type_error attributes have no class
            //! \param a attribute instance
            //! \return can be ignored
            //!
            result_type operator()(const attribute_type &) const
            {
                using namespace pni::core;
                throw type_error(EXCEPTION_RECORD,
                        "Attributes do not have a class!");
                return false;
            }
            
            //----------------------------------------------------------------
            //!
            //! \brief process links
            //!
            //! \throws type_error attributes have no class
            //! \param a attribute instance
            //! \return can be ignored
            //!
            result_type operator()(const link_type &) const
            {
                using namespace pni::core;
                throw type_error(EXCEPTION_RECORD,
                        "Links do not have a class!");
                return false;
            }
    };

    //!
    //! \ingroup algorithm_code
    //! \brief is class function
    //!
    //! Returns true if a group stored in an instance of nxobject is member
    //! of a particular class.
    //!
    //! \throws invalid_object_error if parent group is not valid
    //! \throws shape_mismatch_error the attribute is not a scalar
    //! \throws type_error the attribute type is not supported
    //! \throws io_error attribute data retrieval failed
    //! \throws object_error in case of any other error
    //!
    //! \tparam GTYPE group type
    //! \tparam FTYPE field type
    //! \tparam ATYPE attribute type
    //! \tparam LTYPE link type
    //!
    //! \param o instance of nxobject
    //! \param c Nexus class to check for
    //! \return true if group class and c match, false otherwise
    //!
    template<
             typename GTYPE,
             typename FTYPE,
             typename ATYPE,
             typename LTYPE
            > 
    bool is_class(const nxobject<GTYPE,FTYPE,ATYPE,LTYPE> &o,
                  const pni::core::string &c)
    {
        using visitor_type = is_class_visitor<GTYPE,FTYPE,ATYPE,LTYPE>;
        return boost::apply_visitor(visitor_type(c),o);
    }

//end of namespace
}
}
}