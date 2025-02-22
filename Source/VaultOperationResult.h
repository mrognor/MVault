#pragma once

#include <string>
#include <cstdint>
#include <typeinfo>
#include <iostream>
#include <typeindex>

#include "BackTrace/BackTrace.h"

namespace mvlt
{
    /**
        \defgroup VaultOperationResultCode VaultOperationResultCode
        \brief Enum with all possible vault operation results

        @{
    */
    
    /// \brief Enum with all error handling codes
    enum class VaultOperationResultCode : std::int8_t
    {
        OtherParentVaultNotValid,                      ///< This code is returned when working with VaultRecordSet when another object has an invalid parent Vault
        SameVaultRecordSet,                            ///< This code is returned when trying to call operations on the VaultRecordSet object itself
        TryToAddUniqueKeyInNonEmptyVaultWithoutLambda, ///< This code is returned when trying to add unique key without lamda in non-empty vault
        DuplicateKey,                                  ///< This code is returned when trying to add key which alredy in vault
        UniqueKey,                                     ///< This code is returned when trying to update default value in an unique key
        RecordAlredyInSet,                             ///< This code is returned when trying to add an record to the set when it is already in it
        ParentVaultNotMatch,                           ///< This code is returned during operations on VaultRecordSet when objects depend on different Vaults
        ParentVaultNotValid,                           ///< This code is returned when calling VaultRecordSet operations when the parent Vault is not valid
        DataRecordNotValid,                            ///< This code is returned when data is requested from the VaultRecordRef and the record it refers to is not valid
        WrongKey,                                      ///< This code is returned when the requested key has not been found
        WrongType,                                     ///< This code is returned when the requested type does not match the saved type
        Idle,                                          ///< The default state
        WrongValue,                                    ///< This value is returned when the key was found, the requested type and the saved type are equal, but the requested value was not found
        UniqueKeyValueAlredyInSet,                     ///< This value is returned when the key was found, the requested type and the saved type are equal, but the new value alredy in vault
        Success                                        ///< This value is returned when the request was completely successful
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

        /**
            \brief A method for setting the result of an operation

            \param [in] resultCode operation code
        */
        void SetOpResult(const VaultOperationResultCode& resultCode) noexcept;

        /**
            \brief Function for getting a text description of the operation result
            \return Operation description string
        */
        std::string ResultCodeString() const noexcept;

        /// \brief Method for printing operation result
        void Print() const noexcept;

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