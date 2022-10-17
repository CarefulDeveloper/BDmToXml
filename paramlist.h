#pragma once

#include <string>
#include "stringutil.h"
#include "result.hpp"

using namespace std;

// 定义命令行中可选参数有哪些
namespace Param
{
	const string CHAT_ID = "chat_id";
	const string OFFSET = "offset";
}

class Params
{
public:
	// 在处理这些参数的实际变量名和默认值
	string chatID = "";
	long long offset = 0;

	Params() {}

	Params(string chatID, long long offset) : chatID(chatID), offset(offset)
	{
	}
};

Result<Params> Check_Parm(string args);
static Result<string> Check_Chat_ID(string value);
static Result<long long> Check_Offset(string value);