#pragma once

#ifndef _WIN32
#include <link.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef NO_BACKTRACE_SUPPORT
#include <execinfo.h>
#endif
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
        Compact, ///< Function with error
        None     ///< None debug backtrace
    };

    /// Variable to store backtrace format
    extern BackTraceFormat TraceFormat;

    /**
        \brief A function for setting the format of a debug message

        \param [in] traceFormat debug message format
    */
    void SetBackTraceFormat(const BackTraceFormat& traceFormat) noexcept;

    /// \brief Backtrace printing function
    void PrintBackTrace() noexcept;

    /// \brief A class for logging inputs and outputs from functions
    class TraceLogger
    {
    private:
        // Function name
        std::string FuncName;
    public:
        /// \brief Default constructor
        TraceLogger() noexcept = delete;

        /**
            \brief Copy constructor

            \param [in] other object to copy
        */
        TraceLogger(const TraceLogger& other) noexcept = delete;

        /**
            \brief Move constructor

            \param [in] other object to move
        */
        TraceLogger(TraceLogger&& other) noexcept = delete;

        /**
            \brief Constructor to store function name

            \param [in] funcName the name of the function in which the class object was created
        */
        explicit TraceLogger(const std::string& funcName) noexcept;

        /**
            \brief Assignment operator

            \param [in] other object to copy
            \return returns a new object, with data from other
        */
        TraceLogger& operator=(const TraceLogger& other) noexcept = delete;

        /**
            \brief Move assignment operator

            \param [in] other object to move
            \return returns a new object, with data from other
        */
        TraceLogger& operator=(TraceLogger&& other) noexcept = delete;

 
        /// \brief Default destructor
        ~TraceLogger() noexcept;
    };


    /**@} */
}