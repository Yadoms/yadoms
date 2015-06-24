#include "stdafx.h"
#include "WeatherUndergroundHelpers.h"

std::string &trimAll(std::string &s)
{   
    if(s.size() == 0)
    {
        return s;
    }

    int val = 0;
    for (size_t cur = 0; cur < s.size(); cur++)
    {
        if(s[cur] != ' ' && std::isalnum(s[cur]))
        {
            s[val] = s[cur];
            val++;
        }
    }
    s.resize(val);
    return s;
}