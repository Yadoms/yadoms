#include "stdafx.h"
#include "StringExtension.h"

using namespace shared;

std::vector<std::string> CStringExtension::splitAnyOfAndCompress(std::string_view input,
                                                                 std::string_view seps)
{
   std::vector<std::string> result;
   std::string token;

   auto is_sep = [&](char c) {
      return seps.find(c) != std::string_view::npos;
      };

   for (char c : input)
   {
      if (is_sep(c))
      {
         if (!token.empty())
         {
            result.push_back(std::move(token));
            token.clear();
         }
      }
      else
      {
         token += c;
      }
   }

   if (!token.empty())
      result.push_back(std::move(token));

   return result;
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
