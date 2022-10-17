/**
 * 此文件用作包装各种返回值，为返回值添加状态。目前未测试模板类参数重载是否存在bug
*/

#ifndef _RESULT_HPP_
#define _RESULT_HPP_

#include <string>

using namespace std;

enum ResultType
{
#ifdef ERROR
#undef ERROR //取消宏定义
#endif
    SUCCESS,  //有结果，无信息
    ERROR,    //无结果，有信息
    WARN,     //有结果，有信息
    UNDEFINED //无结果，无信息
};

template <class T>
class Result
{
private:
    T data;
    string msg;

public:
    ResultType type;
    Result();
    Result(ResultType type);
    Result(string msg, ResultType type);
    Result(ResultType type, T data);
    Result(string msg, ResultType type, T data);
    void dataOrDefault(T &data);
    string message();
    ~Result();
};

template <class T>
Result<T>::Result(ResultType type)
{
    this->type = type;
}

template <class T>
Result<T>::Result(string msg, ResultType type)
{
    this->type = type;
    this->msg = msg;
}

template <class T>
Result<T>::Result(ResultType type, T data)
{
    this->type = type;
    this->data = data;
}

template <class T>
Result<T>::Result(string msg, ResultType type, T data)
{
    this->type = type;
    this->data = data;
    this->msg = msg;
}

template <class T>
void Result<T>::dataOrDefault(T &data)
{
    if (this->type == ResultType::SUCCESS)
        data = this->data;
}

template <class T>
string Result<T>::message()
{
    return this->msg;
}

template <class T>
Result<T>::~Result() {}
#endif