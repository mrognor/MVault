#include "VaultRequest.h"

#include "VaultRequest.hpp"

namespace mvlt 
{
    // Set value to DefaultRequestPredicat global variable
    std::function<bool(const VaultRecordRef&)> DefaultRequestPredicat = [](const VaultRecordRef&){return true;};
}