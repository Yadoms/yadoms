#include "stdafx.h"
#include "HugeDataVectorForKeywordSummaryDataAdapter.h"
#include <shared/exception/NotSupported.hpp>

namespace database
{
	namespace common
	{
		namespace adapters
		{
			bool CHugeDataVectorForKeywordSummaryDataAdapter::adapt(boost::shared_ptr<IResultHandler> resultHandler)
			{
            const auto nCols = resultHandler->getColumnCount();

				if (nCols < 6) //since 2.3.0, summary table contains 7 columns
				{
               //ensure result string is json !!!
               m_internalValue = R"({"result":false,"message":"invalid query column count"})";
               return false;
            }

            m_internalValue.reserve(100 * nCols);
            m_internalValue = R"({"result":true,"message":"","data":{"data":[)";

            long size = 0;
            while (resultHandler->next_step())
            {
               m_internalValue += R"({"avg":)" + resultHandler->extractValueAsString(3);
               m_internalValue += R"(,"min":)" + resultHandler->extractValueAsString(4);
               m_internalValue += R"(,"date":")" + resultHandler->extractValueAsString(1);
               m_internalValue += R"(","max":)" + resultHandler->extractValueAsString(5) + "},";
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


			std::vector<std::string> CHugeDataVectorForKeywordSummaryDataAdapter::getResults()
			{
				//getRawResults should be used
				throw shared::exception::CNotSupported("CHugeDataVectorForKeywordSummaryDataAdapter::getResults");
			}

			std::string CHugeDataVectorForKeywordSummaryDataAdapter::getRawResults() const
			{
				return m_internalValue;
			}

		} //namespace adapters
	} //namespace common
} //namespace database 