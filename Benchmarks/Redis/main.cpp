#include <MVault.h>

#include "RedisDb.h"
#include "../Helpers/Measure.h"


const std::string Key = "key";
const std::string Value = "value";
constexpr int MeasureCount = 10;

void Fill(RedisDbHelper& redisDb, const std::uint32_t& recordsCounter) noexcept
{
    for (std::uint32_t i = 0; i < recordsCounter; ++i)
    {
        std::string command = "SET " + Key + std::to_string(i) + " %s";
        std::string value = Value + std::to_string(i);

        redisAppendCommand(redisDb.GetContext(), command.c_str(), value.c_str());
    }

    for (std::uint32_t i = 0; i < recordsCounter; ++i)
    {
        redisGetReply(redisDb.GetContext(), nullptr);
    }
}

std::int64_t TestFill(const std::uint32_t& recordsCounter) noexcept
{
    std::int64_t res = 0;

    RedisDbHelper redisDb;

    for (uint32_t i = 0; i < MeasureCount; ++i)
    {
        Measure measure(res);
        Fill(redisDb, recordsCounter);
        redisDb.Flush();
    }

    return res / MeasureCount;
}

std::int64_t TestGetRecord(const std::uint32_t& recordsCounter) noexcept
{
    std::int64_t res = 0;

    RedisDbHelper redisDb;
    Fill(redisDb, recordsCounter);

    for (uint32_t i = 0; i < MeasureCount; ++i)
    {
        Measure measure(res, std::chrono::nanoseconds(0));

        std::string command = "GET " + Key + "1";
        redisCommand(redisDb.GetContext(), command.c_str());
        // redisReply* reply = static_cast<redisReply*>(redisCommand(redisDb.GetContext(), command.c_str()));
        // std::cout << std::string(reply->str, reply->len) << std::endl;
    }

    redisDb.Flush();

    return res / MeasureCount;
}

int main() 
{
    // Prepare vault to save bench data
    mvlt::Vault vlt;
    vlt.AddKey<std::uint32_t>("Records", 0);
    vlt.AddKey<std::int64_t>("Time", 0);

    const std::vector<std::uint32_t> sizes = {1, 10, 100, 1000, 10000, 100000, 1000000};

    // Measure fill
    for (const auto& size : sizes)
    {
        std::int64_t measured = TestFill(size);
        vlt.CreateRecord({{"Records", size}, {"Time", measured}});
    }

    vlt.Print();
    vlt.SaveToFile("RedisBenchFill.csv");
    vlt.DropData();

    // Measure get
    for (const auto& size : sizes)
    {
        std::int64_t measured = TestGetRecord(size);
        vlt.CreateRecord({{"Records", size}, {"Time", measured}});
    }

    vlt.Print();
}
