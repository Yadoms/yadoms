#include "stdafx.h"
#include "StringExtension.h"

using namespace shared;


void CStringExtension::leftTrim(std::string& s)
{
   s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                   std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void CStringExtension::rightTrim(std::string& s)
{
   s.erase(std::find_if(s.rbegin(), s.rend(),
                        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

void CStringExtension::trim(std::string& s)
{
   leftTrim(s);
   rightTrim(s);
}

std::string CStringExtension::replaceValues(const std::string& input,
                                            const std::map<std::string, std::string>& replacements,
                                            const std::string& keyEncapsulationStartToken,
                                            const std::string& keyEncapsulationendToken)
{
   std::string result = input;
   for (const auto& replacement : replacements)
   {
      std::string fullKey = keyEncapsulationStartToken;
      fullKey.append(replacement.first);
      fullKey.append(keyEncapsulationendToken);

      while (true)
      {
         const auto foundIt = result.find(fullKey);
         if (foundIt == std::string::npos)
            break;

         result.replace(foundIt,
                        fullKey.size(),
                        replacement.second);
      }
   }

   boost::smatch m;
   const boost::regex r(keyEncapsulationStartToken + "([^}]*)" + keyEncapsulationendToken);
   if (boost::regex_search(result, m, r))
      throw std::invalid_argument(std::string("Key \"") + std::string(m[1].first, m[1].second) + "\" not defined in \"" + input + "\"");

   return result;
}
