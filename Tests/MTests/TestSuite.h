#pragma once

#define TEST_SUITE(className, ...) \
class className \
{ \
private: \
    static constexpr const char* ClassName = #className; \
public: \
    __VA_ARGS__ \
}; \

#define TEST_CASE(methodName) \
static void methodName();

#define TEST_BODY(className, methodName, ...) \
void className::methodName() \
{ \
    TEST_LOG(ClassName); \
    __VA_ARGS__ \
}
