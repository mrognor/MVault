#pragma once

#include <string>
#include <typeindex>
#include <typeinfo>

namespace mvlt
{
    /**
        \defgroup VaultOperationResultCode VaultOperationResultCode
        \brief Enum with all possible vault operation results

        @{
    */
    
    /// \brief Enum with all error handling codes
    enum class VaultOperationResultCode
    {
        Error = -3,     ///< This code is returned when any error has occurred that is not described
        WrongKey = -2,  ///< This code is returned when the requested key has not been found.
        WrongType = -1, ///< This code is returned when the requested type does not match the saved type.
        Idle = 0,       ///< The default state.
        WrongValue = 1, ///< This value is returned when the key was found, the requested type and the saved type are equal, but the requested value was not found
        Success = 2,    ///< This value is returned when the request was completely successful
    };
    
    /**@} */

    /// \brief Structure for storing the results of MVault operations
    struct VaultOperationResult
    {
        /// \brief A variable for determining whether the operation was successful
        bool IsOperationSuccess = true;
        
        /// \brief The result code of the operation
        VaultOperationResultCode ResultCode = VaultOperationResultCode::Idle;

        /// \brief Requested key
        std::string Key;

        /// \brief Requested key type
        std::type_index RequestedType = typeid(void);
        
        /// \brief Saved key type
        std::type_index SavedType = typeid(void);

        /// \brief Function for getting a text description of the operation result
        /// \return Operation description string
        std::string ResultCodeString() const;
    };
}