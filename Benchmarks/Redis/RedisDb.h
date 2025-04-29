#pragma once

#include <hiredis/hiredis.h>
#include <string>
#include <cstdint>
#include <iostream>
#include <thread>
#include <chrono>

constexpr std::uint16_t redisPipeSize = 1452;

class RedisDbHelper
{
protected:
    bool IsConnected = false;
    redisContext* Context = nullptr;
    redisReply* Reply = nullptr;
public:
    RedisDbHelper(const std::string& ip = "127.0.0.1", const std::uint16_t& port = 6379) noexcept;

    redisContext* GetContext() noexcept;

    bool Connect(const std::string& ip = "127.0.0.1", const std::uint16_t& port = 6379) noexcept;

    void ExecCommand(const std::string& formattedCommand, const std::string& value) noexcept;

    void Flush() noexcept;

    ~RedisDbHelper() noexcept;
};
