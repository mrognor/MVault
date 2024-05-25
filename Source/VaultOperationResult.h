#pragma once

#include <string>
#include <typeinfo>
#include <typeindex>

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
        ParentVaultNotValid = -4,///< This code is returned when calling VaultRecordSet operations when the parent Vault is not valid
        DataRecordNotValid = -3, ///< This code is returned when data is requested from the VaultRecordRef and the record it refers to is not valid
        WrongKey = -2,           ///< This code is returned when the requested key has not been found.
        WrongType = -1,          ///< This code is returned when the requested type does not match the saved type.
        Idle = 0,                ///< The default state.
        WrongValue = 1,          ///< This value is returned when the key was found, the requested type and the saved type are equal, but the requested value was not found
        Success = 2,             ///< This value is returned when the request was completely successful
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
        std::string ResultCodeString() const noexcept;

        /**
            \brief Friend comparison operator

            \param [in] resA first object to compare
            \param [in] resB second object to compare

            \return true if objects equal otherwise return false
        */
        friend bool operator== (const VaultOperationResult& resA, const VaultOperationResult& resB) noexcept;
    };

    /**
        \brief VaultOperationResult comparison operator

        \param [in] resA first object to compare
        \param [in] resB second object to compare

        \return true if objects equal otherwise return false
    */
    bool operator== (const VaultOperationResult& resA, const VaultOperationResult& resB) noexcept;
}