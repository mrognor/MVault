#pragma once

#ifndef _WIN32
#include <link.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <execinfo.h>
#endif

#include <vector>
#include <cstdint>
#include <sstream>
#include <iostream>
#include <typeindex>


#define TRACE_MAX 1024

#ifndef NDEBUG
#define DBG_LOG_ENTER() TraceLogger LocalTraceLoggerMacro(__FUNCTION__);
#else
#define DBG_LOG_ENTER()
#endif

namespace mvlt
{
    /**
        \defgroup BackTraceFunctions BackTrace functions
        \brief All functions for debug BackTrace printing

        @{
    */


    /// \brief Debug backtrace format
    enum class BackTraceFormat : std::uint8_t
    {
        Full,    ///< Full backtrace
        Compact, ///< Only function with error
        None     ///< None debug backtrace
    };

    /// Variable to store backtrace format
    extern BackTraceFormat TraceFormat;

    /**
        \brief A function for setting the format of a debug message

        \param [in] traceFormat debug message format
    */
    void SetBackTraceFormat(const BackTraceFormat& traceFormat);

    /// \brief Backtrace printing function
    void PrintBackTrace();

    /// \brief A class for logging inputs and outputs from functions
    class TraceLogger
    {
    private:
        // Function name
        std::string FuncName;
    public:
        /// \brief Default constructor
        TraceLogger() = default;

        /**
            \brief Copy constructor

            \param [in] other object to copy
        */
        TraceLogger(const TraceLogger& other) = default;

        /**
            \brief Move constructor

            \param [in] other object to move
        */
        TraceLogger(TraceLogger&& other) = default;

        /**
            \brief Constructor to store function name

            \param [in] funcName the name of the function in which the class object was created
        */
        explicit TraceLogger(const std::string& funcName);

        /**
            \brief Assignment operator

            \param [in] other object to copy
            \return returns a new object, with data from other
        */
        TraceLogger& operator=(const TraceLogger& other) = default;

        /**
            \brief Move assignment operator

            \param [in] other object to move
            \return returns a new object, with data from other
        */
        TraceLogger& operator=(TraceLogger&& other) = default;

 
        /// \brief Default destructor
        ~TraceLogger();
    };


    /**@} */
}