#pragma once

#include "../Source/Vault.h"
#include "ColorizedPrint.h"

void ColorizedPrint(const std::string text, const ConsoleTextColor& color = ConsoleTextColor::Default);

void MultithreadingReadTest();

void MultithreadingKeysTest();

void MultithreadingRecordsTest();

void Bench();