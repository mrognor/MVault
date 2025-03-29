#pragma once

#include <string>
#include <random>

namespace mvlt
{
    /**
        \brief Function to convert digit to hex

        \return string with hex representation of digit
    */
    std::string ToHex(std::uint64_t digit) noexcept;

    /**
        \brief Function to generate uuid string

        \return string with uuid
    */
    std::string Uuid() noexcept;

    /**
        \brief A method for generating the name of a temporary file

        The uuid of version 4 is used as the file name.
        The path for Linux is /tmp. The path for Windows: C:\Windows\Temp

        \param [in] prefix the line to add to the beginning of the file name

        \return full path with name to tmp file
    */
    std::string GenTmpFileName(const std::string& prefix = "") noexcept;
}