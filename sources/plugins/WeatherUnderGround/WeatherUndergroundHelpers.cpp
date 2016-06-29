#include "stdafx.h"
#include "WeatherUndergroundHelpers.h"

std::string trimAll(const std::string& s)
{
   auto sOut(s);
   if (sOut.size() == 0)
   {
      return sOut;
   }

   auto val = 0;
   for (size_t cur = 0; cur < sOut.size(); cur++)
   {
      if (sOut[cur] != ' ' && std::isalnum(sOut[cur]))
      {
         sOut[val] = sOut[cur];
         val++;
      }
   }
   sOut.resize(val);
   return sOut;
}

