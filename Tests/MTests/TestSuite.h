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
} \
\
class TmpObj_##className##_##methodName \
{ \
public: \
    TmpObj_##className##_##methodName() \
    { \
        Tests.emplace_back([]() {className::methodName();}); \
    } \
}; \
 \
TmpObj_##className##_##methodName tmpObj_##className##_##methodName;
