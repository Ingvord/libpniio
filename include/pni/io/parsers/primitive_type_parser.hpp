
//
// (c) Copyright 2015 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
// Created on: Jan 23, 2015
//     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//
//

#pragma once

#include<pni/core/types.hpp>
#include<vector>
#include<limits>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/mpl/map.hpp>

#include "../exceptions.hpp"
#include "element_rule_type.hpp"

namespace pni{
namespace io{

    using namespace boost::spirit;
    using namespace pni;

    //!
    //! \ingroup parser_classes
    //! \brief parser for primitive types
    //!
    //! This version of the parser structure provides a default parsing 
    //! implementation for primitive types. The aim of this class is 
    //! to hide all the complexity of the boost::spirit::qi parsers. 
    //! In addition it catches all spirit exceptions and wraps them 
    //! into a parser_error exception. 
    //! 
    //! Use this parser to parse a single primitive value from a string.
    //! The input data must be trimmed - so no leading or trailing 
    //! blanks are allowed. The string is supposed to end with the last 
    //! character assembling the value to parse.
    //! 
    //! \tparam ITERT iterator type
    //! \tparam T     primitive data type
    //! \tparam is_primitive is set to truen when T is a primitive type
    //!
    template<
             typename ITERT,
             typename T,
             bool     is_primitive = core::is_primitive_type<T>::value
            >
    class parser 
    {
        public:
            typedef ITERT iterator_type;
            typedef T     value_type;
            typedef qi::expectation_failure<iterator_type> expectation_error;
        private:
            typedef element_rule_type<iterator_type,value_type> rule_type ;
            typedef typename  rule_type::type parser_type;

            parser_type _parser_instance;
        public:
            //!
            //! \brief parser primitive type
            //! 
            //! Parses the input string and returns an instance of a primitive type.
            //! In case of errors parser_error is thrown. 
            //!
            //! \throws parser_error in case of any problems
            //! \param data the string with input data
            //! \return instance of the primitive type
            value_type parse(const string &data) const
            {
                iterator_type start = data.begin();
                iterator_type stop  = data.end();
                value_type result;

                try
                {
                    if(!qi::parse(start,stop,_parser_instance>qi::eoi,result))
                    {
                        throw parser_error(EXCEPTION_RECORD,
                                "Failure parsing primitive type!");
                    }

                    //need to check for the condition that the value becomes
                    //infinity and throw an exception
                    if((result == std::numeric_limits<value_type>::infinity()) 
                        ||
                       (result == -std::numeric_limits<value_type>::infinity()))
                        throw parser_error(EXCEPTION_RECORD,
                                "Value is infinity!");
                }
                catch(const expectation_error &error)
                {
                    throw parser_error(EXCEPTION_RECORD,
                            "Syntax error!");
                }

                return result;
            }
    };

//end of namespace
}
}