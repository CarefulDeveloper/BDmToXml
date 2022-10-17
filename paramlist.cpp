#include "paramlist.h"

static Result<string> Check_Chat_ID(string value)
{
    if (String_Is_Number(value))
    {
        return Result<string>(ResultType::SUCCESS, value);
    }
    else
    {
        return Result<string>(string("输入的\"" + value + "\"不是一个数字"), ResultType::ERROR);
    }
}

static Result<long long> Check_Offset(string value)
{
    if (String_Is_Number(value))
    {
        return Result<long long>(ResultType::SUCCESS, _atoi64(value.data()));
    }
    else
    {
        return Result<long long>(string("输入的\"" + value + "\"不是一个数字"), ResultType::ERROR);
    }
}

Result<Params> Check_Parm(string args)
{
    Params params;
    vector<string> paramList = String_Split(args, "|");

    for (int index = 0; index < paramList.size(); index++)
    {
        vector<string> item = String_Split(paramList[index], "=");
        // 如果不是键值对属性
        if (item.size() != 2)
            continue;
        string key = item[0];
        string value = item[1];
        key = String_Trim(key);
        value = String_Trim(value);
        if (key == Param::CHAT_ID)
        {
            Check_Chat_ID(value).dataOrDefault(params.chatID);
        }
        else if (key == Param::OFFSET)
        {
            Check_Offset(value).dataOrDefault(params.offset);
        }
    }
    return Result<Params>(ResultType::SUCCESS, params);
}