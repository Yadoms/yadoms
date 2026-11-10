#include "stdafx.h"
#include "HugeDataVectorForKeywordDataAdapterV1.h"
#include <shared/exception/NotSupported.hpp>

namespace database
{
   namespace common
   {
      namespace adapters
      {
         bool CHugeDataVectorForKeywordDataAdapterV1::adapt(boost::shared_ptr<IResultHandler> resultHandler)
         {
            const auto nCols = resultHandler->getColumnCount();

            m_internalValue.reserve(100 * nCols);
            m_internalValue = R"({"result":true,"message":"","data":{"data":[)";

            long size = 0;
            while (resultHandler->nextStep())
            {
               m_internalValue += R"({"date":")" + resultHandler->extractValueAsString(0);
               m_internalValue += R"(","key":)" + resultHandler->extractValueAsString(1) + "},";
               ++size;
            }
            //check if any data
            if (m_internalValue.size() > 1)
            {
               if (size > 0)
                  m_internalValue[m_internalValue.size() - 1] = ']'; //replace the last ,
               else
                  m_internalValue += "]";
            }
            m_internalValue += "}}"; //no data, just append closing bracket
            return true;
         }


         std::vector<std::string> CHugeDataVectorForKeywordDataAdapterV1::getResults()
         {
            //getRawResults should be used
            throw shared::exception::CNotSupported("CHugeDataVectorForKeywordDataAdapterV1::getResults");
         }

         std::string CHugeDataVectorForKeywordDataAdapterV1::getRawResults() const
         {
            return m_internalValue;
         }
      } //namespace adapters
   } //namespace common
} //namespace database 
