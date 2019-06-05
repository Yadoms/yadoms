#include "stdafx.h"
#include "HugeDataVectorAdapter.h"
#include <shared/exception/NotSupported.hpp>

namespace database
{
	namespace common
	{
		namespace adapters
		{
			CHugeDataVectorAdapter::CHugeDataVectorAdapter()
			{
			}

			CHugeDataVectorAdapter::~CHugeDataVectorAdapter()
			{
			}

			bool CHugeDataVectorAdapter::adapt(boost::shared_ptr<IResultHandler> resultHandler)
			{
            const auto nCols = resultHandler->getColumnCount();

				if (nCols >= 6) //since 2.3.0, summary table contains 7 columns
				{
               m_internalValue.reserve(100 * nCols);
               m_internalValue = "{\"result\":true,\"message\":\"\",\"data\":{\"data\":[";

					long size = 0;
					while (resultHandler->next_step())
					{
						m_internalValue += "{\"avg\":" + resultHandler->extractValueAsString(3);
						m_internalValue += ",\"min\":" + resultHandler->extractValueAsString(4);
						m_internalValue += ",\"date\":\"" + resultHandler->extractValueAsString(1);
						m_internalValue += "\",\"max\":" + resultHandler->extractValueAsString(5) + "},";
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
            else
            {
               //ensure result string is json !!!
               m_internalValue = "{\"result\":false,\"message\":\"invalid query column count\"}";
               return false;
            }
			}


			std::vector<std::string> CHugeDataVectorAdapter::getResults()
			{
				//getRawResults should be used
				throw shared::exception::CNotSupported("CHighchartValueAdapter::getResults");
			}

			std::string CHugeDataVectorAdapter::getRawResults() const
			{
				return m_internalValue;
			}

		} //namespace adapters
	} //namespace common
} //namespace database 