#pragma once

#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

string& String_Replace_All(string& src, const string& old_value, const string& new_value);

string String_TO_UTF8(string str);

string UTF8_To_String(const string& s);
