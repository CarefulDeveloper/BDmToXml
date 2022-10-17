#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
#include <sstream>

using namespace std;

vector<string> String_Split(const string &str, const string &splits);

string& String_Replace_All(string& src, const string& old_value, const string& new_value);

string String_TO_UTF8(const string& str);

string UTF8_To_String(const string& s);

bool String_Is_Number(const string& s);

string& String_Trim(string &s);