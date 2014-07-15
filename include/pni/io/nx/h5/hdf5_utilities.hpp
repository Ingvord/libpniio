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
//
// Created on: Aug 20, 2013
//     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//

extern "C" { 
#include <hdf5.h>
}

#include <pni/core/types.hpp>

namespace pni{
namespace io{
namespace nx{
namespace h5{

    class h5object;

    using namespace pni::core;

    //============name methods=========================================
    //! 
    //! \ingroup nxh5_classes
    //! \brief return object name
    //!
    //! Returns the name of an object. Within an HDF5 file objects can 
    //! be addressed by a UNIX like path.  This method returns the 
    //! last part of the path.  So if an object would have the 
    //! following path /scan_1/detector/data this method would return 
    //! data.  The behavior of this method is slightly different for 
    //! a file object.
    //!
    //! \throws invalid_object_error if obj is not valid
    //! \throws io_error in case of data retrieval issues
    //!
    //! \return name of the object
    //!
    string get_name(const h5object &obj);

    //------------------------------------------------------------------------
    //!
    //! \ingroup nxh5_classes
    //! \brief get object filename
    //! 
    //! Returns the name of a file a dataset, attribute, or group object 
    //! belongs to.  If the filename cannot be obtained an empty string 
    //! is returned. This function assumes that id referes to a valid HDF5 
    //! object. In case of errors an io_error exception is thrown. 
    //!
    //! \throw io_error in case of errors
    //! \param id HDF5 id of the object
    //! \return string with the file name
    //!
    string get_filename(hid_t id);

    //-------------------------------------------------------------------------
    //!
    //! \ingroup nxh5_classes
    //! \brief get object path
    //!
    //! Return the path of an HDF5 object as a string. The object can be 
    //! either an attribute, a field, or a group.  This function assumes that 
    //! id referens to a valid HDF5 object. In case of errors an io_error 
    //! exception is thrown.
    //!
    //! \throws io_error in case of errors.
    //! \param id of the object
    //! \return path as string
    //!
    string get_object_path(hid_t id);

    //------------------------------------------------------------------------
    //! 
    //! \ingroup nxh5_classes
    //! \brief get parent path
    //! 
    //! Return the path to the parent of a particular object. 
    //! 
    //! \throws io_error  in case of errors
    //! \param id the HDF5 ID of the object
    //! \return path to the parent
    //! 
    string get_parent_path(hid_t id);
        
    //----------------------------------------------------------------
    //!
    //! \ingroup nxh5_classes
    //! \brief return the parent object
    //!
    //! Returns the parent object. This might be either a group (in 
    //! the case of a field or group) or a field.
    //!
    //! \throws object_error in case of any error
    //! \return object instance of the parent
    //!
    h5object parent(const h5object &obj);


//end of namespace
}
}
}
}
