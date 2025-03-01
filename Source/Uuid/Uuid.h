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
}