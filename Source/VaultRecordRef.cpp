#include "VaultRecordRef.h"
#include "Vault.hpp"
#include "VaultRecordRef.hpp"

namespace mvlt 
{
    void VaultRecordRef::SetRecord(VaultRecord* vaultRecord, Vault* vlt) noexcept
    {
        DBG_LOG_ENTER();

        vlt->RecursiveReadWriteMtx.ReadLock();

        if (vaultRecord != nullptr) vaultRecord->AddRef();
        if (VaultRecordPtr != nullptr) VaultRecordPtr->RemoveRef();

        VaultRecordPtr = vaultRecord;
        Vlt = vlt;

        vlt->RecursiveReadWriteMtx.ReadUnlock();
    }

    VaultRecordRef::VaultRecordRef() noexcept 
    {
        DBG_LOG_ENTER();
    }

    VaultRecordRef::VaultRecordRef(VaultRecord* vaultRecord, Vault* vlt) noexcept
    {
        DBG_LOG_ENTER();

        SetRecord(vaultRecord, vlt);
    }

    VaultRecordRef::VaultRecordRef(const VaultRecordRef& other) noexcept
    {
        DBG_LOG_ENTER();

        *this = other;
    }

    VaultRecordRef::VaultRecordRef(VaultRecordRef&& other) noexcept
    {
        DBG_LOG_ENTER();

        *this = std::move(other);
    }

    VaultRecordRef& VaultRecordRef::operator=(const VaultRecordRef& other) noexcept
    {
        DBG_LOG_ENTER();

        if (&other != this)
        {
            if (other.VaultRecordPtr != nullptr)
            {
                if (VaultRecordPtr != nullptr) VaultRecordPtr->RemoveRef();

                other.VaultRecordPtr->AddRef();
                VaultRecordPtr = other.VaultRecordPtr;
                Vlt = other.Vlt;
            }
            else 
            {
                if (VaultRecordPtr != nullptr) VaultRecordPtr->RemoveRef();
                Vlt = nullptr;
                VaultRecordPtr = nullptr;
            }
        }
        
        return *this;
    }

    VaultRecordRef& VaultRecordRef::operator=(VaultRecordRef&& other) noexcept
    {
        DBG_LOG_ENTER();

        *this = other;
        
        return *this;
    }

    bool VaultRecordRef::operator==(const VaultRecordRef& other) const noexcept
    {
        DBG_LOG_ENTER();

        bool res;

        if (IsValid())
        {
            res = (VaultRecordPtr == other.VaultRecordPtr);
        }
        else
        {
            if (other.IsValid()) res = false;
            else res = true;
        }

        return res;
    }

    std::string VaultRecordRef::GetRecordUniqueId() const noexcept
    {
        DBG_LOG_ENTER();

        std::stringstream ss;

        if (IsValid())
            ss << VaultRecordPtr;
        else
            ss << "null";

        return ss.str();
    }

    VaultOperationResult VaultRecordRef::SetData(const std::vector<std::pair<std::string, VaultParamInput>>& params) noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;

        WriteLock<RecursiveReadWriteMutex> writeLock(Vlt->RecursiveReadWriteMtx);

        // Copy data from function parametrs
        for (const auto& paramsIt : params)
        {
            res = paramsIt.second.SetDataToRef(paramsIt.first, *this);
            if (!res.IsOperationSuccess) break;
        }

        return res;
    }
                
    VaultOperationResult VaultRecordRef::GetDataAsString(const std::string& key, std::string& str) const noexcept
    {
        DBG_LOG_ENTER();

        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(std::string);
        
        if (VaultRecordPtr == nullptr)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            return res;
        }

        ReadLock<RecursiveReadWriteMutex> readLock(Vlt->RecursiveReadWriteMtx);

        // Check if Vault still accessable
        if (!VaultRecordPtr->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;
            return res;
        }

        // If key not exist
        if(!Vlt->GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;
            return res;
        }

        VaultRecordPtr->GetDataAsString(key, str);
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        return res;
    }

    bool VaultRecordRef::IsValid() const noexcept
    {
        DBG_LOG_ENTER();

        bool res = false;

        if (VaultRecordPtr != nullptr) res = VaultRecordPtr->GetIsValid();

        return res;
    }

    bool VaultRecordRef::IsKeyExist(const std::string& key) const noexcept
    {
        DBG_LOG_ENTER();

        bool res = false;

        if (Vlt != nullptr) res = Vlt->IsKeyExist(key);

        return res;
    }

    std::vector<std::string> VaultRecordRef::GetKeys() const noexcept
    {
        DBG_LOG_ENTER();

        std::vector<std::string> res;

        if (VaultRecordPtr != nullptr)
        {
            // Check if Vault still accessable
            if (VaultRecordPtr->GetIsValid())
                res = Vlt->GetKeys();
        }

        return res;
    }

    void VaultRecordRef::PrintRecord() const noexcept
    {
        DBG_LOG_ENTER();

        if (VaultRecordPtr != nullptr)
        {
            VaultRecordPtr->VaultRecordMutex.lock();

            // Check if Vault still accessable
            if (VaultRecordPtr->GetIsValid())
            {
                Vlt->RecursiveReadWriteMtx.ReadLock();

                std::cout << "Vault record " << VaultRecordPtr << ":" << "\n";

                for (const auto& keyPair : Vlt->VaultHashMapStructure)
                {
                    DataSaver dataSaver;
                    VaultRecordPtr->GetDataSaver(keyPair.first, dataSaver);
                
                    std::cout << "\t" << keyPair.first << " = " << dataSaver.Str() << "\n";
                }

                Vlt->RecursiveReadWriteMtx.ReadUnlock();
            }
            else std::cout << "VaultRecordRef not valid!" << "\n";

            VaultRecordPtr->VaultRecordMutex.unlock();
        }
        else std::cout << "VaultRecordRef not valid!" << "\n";
    }

    void VaultRecordRef::Reset() noexcept
    {
        DBG_LOG_ENTER();

        if (VaultRecordPtr != nullptr)
            VaultRecordPtr->RemoveRef();
        
        VaultRecordPtr = nullptr;
        Vlt = nullptr;
    }

    std::string VaultRecordRef::ToJson(const bool& isFormat, const std::size_t& tabSize) const noexcept
    {
        DBG_LOG_ENTER();

        const std::string tab(tabSize, ' ');
        std::size_t keyCounter = 0;
        std::string res = "{";

        if (VaultRecordPtr != nullptr)
        {
            VaultRecordPtr->VaultRecordMutex.lock();

            // Check if Vault still accessable
            if (VaultRecordPtr->GetIsValid())
            {
                Vlt->RecursiveReadWriteMtx.ReadLock();

                for(const std::string& key : Vlt->KeysOrder)
                {
                    if (isFormat) res += "\n" + tab;

                    DataSaver dataSaver;
                    VaultRecordPtr->GetDataSaver(key, dataSaver);

                    res += "\"" + key + "\":";

                    if (isFormat) res += " ";
                    res += mvlt::ToJson(dataSaver);

                    ++keyCounter;
                    if (keyCounter != Vlt->KeysOrder.size()) res += ",";
                }

                Vlt->RecursiveReadWriteMtx.ReadUnlock();
            }

            VaultRecordPtr->VaultRecordMutex.unlock();
        }

        if (isFormat) res += "\n";
        res += "}";

        return res;
    }

    std::vector<std::pair<std::string, std::string>> VaultRecordRef::ToStrings() const noexcept
    {
        DBG_LOG_ENTER();

        std::vector<std::pair<std::string, std::string>> res;

        if (VaultRecordPtr != nullptr)
        {
            VaultRecordPtr->VaultRecordMutex.lock();

            // Check if Vault still accessable
            if (VaultRecordPtr->GetIsValid())
            {
                Vlt->RecursiveReadWriteMtx.ReadLock();

                for(const std::string& key : Vlt->KeysOrder)
                {
                    DataSaver dataSaver;
                    VaultRecordPtr->GetDataSaver(key, dataSaver);

                    res.emplace_back(std::pair<std::string, std::string>(key, dataSaver.Str()));
                }

                Vlt->RecursiveReadWriteMtx.ReadUnlock();
            }

            VaultRecordPtr->VaultRecordMutex.unlock();
        }

        return res;
    }

    VaultRecordRef::~VaultRecordRef() noexcept
    {
        DBG_LOG_ENTER();

        if (VaultRecordPtr != nullptr) VaultRecordPtr->RemoveRef();
    }
}