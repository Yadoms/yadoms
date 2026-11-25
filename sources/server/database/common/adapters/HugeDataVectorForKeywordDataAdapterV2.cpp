#include "stdafx.h"
#include "HugeDataVectorForKeywordDataAdapterV2.h"
#include <regex>
#include <shared/exception/NotSupported.hpp>

using namespace database::common::adapters;

namespace
{
   bool isJsonNumber(const std::string& value)
   {
      static const std::regex JsonNumberPattern(R"(-?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][+-]?[0-9]+)?)");
      return std::regex_match(value, JsonNumberPattern);
   }

   std::string toJsonString(const std::string& value)
   {
      std::string result = "\"";
      for (const auto c : value)
      {
         switch (c)
         {
         case '"': result += R"(\")";
            break;
         case '\\': result += R"(\\)";
            break;
         case '\b': result += R"(\b)";
            break;
         case '\f': result += R"(\f)";
            break;
         case '\n': result += R"(\n)";
            break;
         case '\r': result += R"(\r)";
            break;
         case '\t': result += R"(\t)";
            break;
         default:
            if (static_cast<unsigned char>(c) < 0x20)
            {
               char buffer[7];
               snprintf(buffer, sizeof(buffer), "\\u%04x", static_cast<unsigned char>(c));
               result += buffer;
            }
            else
            {
               result += c;
            }
         }
      }
      return result + "\"";
   }
}

bool CHugeDataVectorForKeywordDataAdapterV2::adapt(boost::shared_ptr<IResultHandler> resultHandler)
{
   const auto nCols = resultHandler->getColumnCount();

   m_internalValue.reserve(100 * nCols);
   m_internalValue = R"([)";

   long size = 0;
   while (resultHandler->nextStep())
   {
      const auto value = resultHandler->extractValueAsString(1);
      m_internalValue += R"({"date":")" + resultHandler->extractValueAsString(0);
      m_internalValue += R"(","value":)" + (isJsonNumber(value) ? value : toJsonString(value)) + "},";
      ++size;
   }

   if (size > 0)
      m_internalValue[m_internalValue.size() - 1] = ']'; //replace the last ,
   else
      m_internalValue += "]";

   return true;
}


std::vector<std::string> CHugeDataVectorForKeywordDataAdapterV2::getResults()
{
   //getRawResults should be used
   throw shared::exception::CNotSupported("CHugeDataVectorForKeywordDataAdapterV2::getResults");
}

std::string CHugeDataVectorForKeywordDataAdapterV2::getRawResults() const
{
   return m_internalValue;
}
