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
 * ColumnInfo class definition
 *
 * Created on: Apr 30, 2012
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 *
 */

#pragma once


#include "../types.hpp"
#include <vector>

using namespace pni::core;

namespace pni{
namespace io{

    /*! \ingroup io_classes
    \brief Spreadsheet column information

    This class holds the basic information about a column stored in a
    spreadsheet. This includes the following parameters: the name of the column,
    its TypeID and the shape of each cell. The latter parameter allows even the
    storage of multidimensional data in such a structure.
    */
    class column_info
    {
        private:
            string _name; //!< name of the columne
            type_id_t _tid;  //!< type ID of column type
            std::vector<size_t>  _shape; //!< shape of column data
        public:
            //====================constructors and destructor===================
            //! default constructor
            column_info();

            //-----------------------------------------------------------------
            //! copy constructor
            column_info(const column_info &ci);

            //-----------------------------------------------------------------
            //! move constructor
            column_info(column_info &&ci);

            //-----------------------------------------------------------------
            /*! 
            \brief constructor

            A column can hold a scalar or a multidimensional data entry. 
            In the former case the type id is that of the element stored in the
            latter case it is the id of a single element in the array stored in
            each column element.

            \param n name of the column
            \param tid ID of the type stored in the column
            \param s shape of the column 
            */
            column_info(const string &n,type_id_t tid,
                        const std::vector<size_t> &s);

            //-----------------------------------------------------------------
            //! destructor
            ~column_info();

            //===================assignment operators==========================
            //! copy assignment operator
            column_info &operator=(const column_info &ci);

            //! move assignment operator
            column_info &operator=(column_info &&ci);

            //======================public member methods======================
            /*! \brief get cell shape

            Returns the shape object from a column.
            \return shape object.
            */
            const std::vector<size_t> &shape() const { return _shape;}

            //-----------------------------------------------------------------
            /*! 
            \brief get cells type ID

            Return the TypeID of the column if available. This is only valid for
            systems where type information is stored in the file (like for FIO). 
            \return type id of the column data
            */
            type_id_t type_id() const { return _tid; }

            //-----------------------------------------------------------------
            /*! 
            \brief get name

            Return the name of the column.
            \return name 
            */
            string name() const { return _name; }

    };

    std::ostream &operator<<(std::ostream &o,const column_info &ci);


//end of namespace
}
}

