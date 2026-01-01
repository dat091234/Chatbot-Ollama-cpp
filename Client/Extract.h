#ifndef _EXTRACT_H_
#define _EXTRACT_H_
#include <string>

using std::string;

string clean_and_format(string text);
string extract_response(string json_text);

#endif