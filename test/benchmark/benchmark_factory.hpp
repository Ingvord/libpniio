#pragma once

#include <memory>
#include <pni/core/types.hpp>

#include "file_io_benchmark.hpp"

using namespace pni::core;

class benchmark_factory
{
    private:
        //! number of frames along the first dimension
        size_t _nx;
        //! number of frames along the second dimension
        size_t _ny;
        //! total number of frames to write for each benchmark
        size_t _nframes;
        //! name of the output file
        string _fname;

        std::unique_ptr<file_io_benchmark> 
            create_pninx_benchmark(const string &tc) const;

        std::unique_ptr<file_io_benchmark>
            create_hdf5_benchmark(const string &tc) const;
    public:
        //===============constructors and destructor===========================
        //! default constructor
        benchmark_factory();

        //---------------------------------------------------------------------
        /*! 
        \brief constructor

        \param fname fname of the output file
        \param nx number of pixels in the first direction
        \param ny number of pixels in the second direction
        \param nframes number of frames to write
        */
        benchmark_factory(const string &fname,size_t nx,size_t ny,size_t nframes);

        //---------------------------------------------------------------------
        /*! 
        \brief create a benchmark

        Create a benchmark for a particular data type and backend
        \param type data type
        \param backend the backend to use
        \return unique pointer to the benchmark instance
        */
        std::unique_ptr<file_io_benchmark> create(const string &type,
                                                  const string &backend) const;


};

