set(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cbf_reader.hpp 
                 ${CMAKE_CURRENT_SOURCE_DIR}/dectris_reader.hpp 
                 ${CMAKE_CURRENT_SOURCE_DIR}/types.hpp)
                
set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cbf_reader.cpp 
                 ${CMAKE_CURRENT_SOURCE_DIR}/dectris_reader.cpp)

install(FILES ${HEADER_FILES} 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pni/io/cbf)
  
set(PNIIO_HEADER_FILES ${PNIIO_HEADER_FILES} ${HEADER_FILES} PARENT_SCOPE)
set(PNIIO_SOURCE_FILES ${PNIIO_SOURCE_FILES} ${SOURCE_FILES} PARENT_SCOPE)
