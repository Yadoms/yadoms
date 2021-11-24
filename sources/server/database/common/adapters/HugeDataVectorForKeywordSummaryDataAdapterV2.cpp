#include "stdafx.h"
#include "HugeDataVectorForKeywordSummaryDataAdapterV2.h"
#include <shared/exception/NotSupported.hpp>

namespace database
{
   namespace common
   {
      namespace adapters
      {
         CHugeDataVectorForKeywordSummaryDataAdapterV2::CHugeDataVectorForKeywordSummaryDataAdapterV2(bool withAverage,
            bool withMin,
            bool withMax,
            bool withCount)
            : m_withAverage(withAverage),
              m_withMin(withMin),
              m_withMax(withMax),
              m_withCount(withCount)
         {
         }

         bool CHugeDataVectorForKeywordSummaryDataAdapterV2::adapt(boost::shared_ptr<IResultHandler> resultHandler)
         {
            const auto nCols = resultHandler->getColumnCount();

            m_internalValue.reserve(100 * nCols);
            m_internalValue = "[";

            long size = 0;
            while (resultHandler->nextStep())
            {
               m_internalValue += R"({"date":")" + resultHandler->extractValueAsString(0) + R"(")";

               int optionalColumn = 0;
               if (m_withAverage)
                  m_internalValue += R"(,"avg":)" + resultHandler->extractValueAsString(++optionalColumn);
               if (m_withMin)
                  m_internalValue += R"(,"min":)" + resultHandler->extractValueAsString(++optionalColumn);
               if (m_withMax)
                  m_internalValue += R"(,"max":)" + resultHandler->extractValueAsString(++optionalColumn);
               if (m_withCount)
                  m_internalValue += R"(,"count":)" + resultHandler->extractValueAsString(++optionalColumn);

               m_internalValue += "},";

               ++size;
            }
            //check if any data
            if (size > 0)
               m_internalValue[m_internalValue.size() - 1] = ']'; //replace the last ,
            else
               m_internalValue += "]";
            return true;
         }


         std::vector<std::string> CHugeDataVectorForKeywordSummaryDataAdapterV2::getResults()
         {
            //getRawResults should be used
            throw shared::exception::CNotSupported("CHugeDataVectorForKeywordSummaryDataAdapterV2::getResults");
         }

         std::string CHugeDataVectorForKeywordSummaryDataAdapterV2::getRawResults() const
         {
            return m_internalValue;
         }
      } //namespace adapters
   } //namespace common
} //namespace database 
