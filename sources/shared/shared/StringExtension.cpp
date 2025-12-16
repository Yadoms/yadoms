#include "stdafx.h"
#include "StringExtension.h"

using namespace shared;


bool CStringExtension::startsWith(const std::string& ref,
                                  const std::string& token)
{
   // TODO : when switch to C++20, use std::starts_with
   if (ref.size() < token.size())
      return false;

   return ref.rfind(token, 0) == 0;
}

bool CStringExtension::startsWith(const std::string& ref,
                                  const char& token)
{
   return startsWith(ref, std::string(1, token));
}

bool CStringExtension::endsWith(const std::string& ref,
                                const std::string& token)
{
   // TODO : when switch to C++20, use std::ends_with
   if (ref.size() < token.size())
      return false;

   return std::equal(token.rbegin(), token.rend(), ref.rbegin());
}

bool CStringExtension::endsWith(const std::string& ref,
                                const char& token)
{
   return endsWith(ref, std::string(1, token));
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
