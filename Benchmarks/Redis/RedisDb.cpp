#include "RedisDb.h"

RedisDbHelper::RedisDbHelper(const std::string& ip, const std::uint16_t& port) noexcept
{
    Connect(ip, port);
}

redisContext* RedisDbHelper::GetContext() noexcept
{
    return Context;
}

bool RedisDbHelper::Connect(const std::string& ip, const std::uint16_t& port) noexcept
{
    if (IsConnected)
    {
        std::cout << "Already connected to db!" << std::endl;
        return false;
    }

    Context = redisConnect(ip.c_str(), port);

    if (Context == nullptr || Context->err) 
    {
        if (Context) 
        {
            std::cerr << "Error: " << Context->errstr << std::endl;
            redisFree(Context);
        } 
        else 
        {
            std::cerr << "Can't allocate redis context" << std::endl;
        }

        std::cerr << "\x1B[31m" << "Failed connect to DB! Is Redis launched? " << "\033[0m" << std::endl;
        return false;
    }

    IsConnected = true;
    return true;
}

void RedisDbHelper::ExecCommand(const std::string& formattedCommand, const std::string& value) noexcept
{
    Reply = static_cast<redisReply*>(redisCommand(Context, formattedCommand.c_str(), value.c_str()));

    if (Reply == NULL) 
    {
        printf("redisCommand reply is NULL: %s\n", Context->errstr);
    }
    if (Reply->type == REDIS_REPLY_ERROR) 
    {
         printf("Command Error: %s\n", Reply->str);
    }

    freeReplyObject(Reply);
}

void RedisDbHelper::Flush() noexcept
{
    ExecCommand("flushall", "");
}

RedisDbHelper::~RedisDbHelper() noexcept
{
    if (IsConnected)
    {
        redisFree(Context);
    }
}