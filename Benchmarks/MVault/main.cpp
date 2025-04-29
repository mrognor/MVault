#include <MVault.h>

#include "../Helpers/Measure.h"

const std::string Key = "key";
const std::string Value = "value";
constexpr int MeasureCount = 10;

std::int64_t TestFill(const std::uint32_t& recordsCounter) noexcept
{
    std::int64_t res = 0;

    for (uint32_t i = 0; i < MeasureCount; ++i)
    {
        Measure measure(res);

        mvlt::Vault vlt;
        vlt.AddKey<std::string>(Key, "");
    
        for (std::uint32_t i = 0; i < recordsCounter; ++i)
            vlt.CreateRecord({{Key, std::string(Value + std::to_string(i))}});
    
        vlt.SaveToFile(std::to_string(recordsCounter));
        vlt.DropData();
    }

    return res / MeasureCount;
}

std::int64_t TestGetRecord(const std::uint32_t& recordsCounter) noexcept
{
    std::int64_t res = 0;

    mvlt::Vault vlt;
    vlt.AddKey<std::string>(Key, "");

    for (std::uint32_t i = 0; i < recordsCounter; ++i)
        vlt.CreateRecord({{Key, std::string(Value + std::to_string(i))}});

    for (uint32_t i = 0; i < MeasureCount; ++i)
    {
        Measure measure(res, std::chrono::nanoseconds(0));

        mvlt::VaultRecordRef vrr;
        vlt.GetRecord(Key, Value + "1", vrr);
    }

    return res / MeasureCount;
}

int main()
{
    // Prepare vault to save bench data
    mvlt::Vault vlt;
    vlt.AddKey<std::uint32_t>("Records", 0);
    vlt.AddKey<std::int64_t>("Time", 0);

    // Vault to bench
    mvlt::Vault benchVlt;
    benchVlt.AddKey<std::string>(Key, "");

    const std::vector<std::uint32_t> sizes = {1, 10, 100, 1000, 10000, 100000, 1000000};

    for (const auto& size : sizes)
    {
        std::int64_t measured = TestFill(size);
        vlt.CreateRecord({{"Records", size}, {"Time", measured}});
    }

    vlt.Print();
    vlt.SaveToFile("MVaultBenchFill.csv");
    vlt.DropData();

    // Measure get
    for (const auto& size : sizes)
    {
        std::int64_t measured = TestGetRecord(size);
        vlt.CreateRecord({{"Records", size}, {"Time", measured}});
    }

    vlt.Print();
}
