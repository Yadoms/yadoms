#pragma once

#include "IResultAdapter.h"
#include "SqlExtension.hpp"
#include <shared/exception/NotSupported.hpp>

namespace database
{
   namespace common
   {
      namespace adapters
      {
         //--------------------------------------------------------------
         ///\Brief		Class which adapt in single string, using a json like output [[data],[data],....]
         //--------------------------------------------------------------
         class CHugeDataVectorAdapter : public IResultAdapterEx<std::string>
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		Constructor
            //--------------------------------------------------------------
            CHugeDataVectorAdapter()
            {
            }

            //--------------------------------------------------------------
            /// \Brief		Destructor
            //--------------------------------------------------------------
            virtual ~CHugeDataVectorAdapter()
            {
            }

            // ISQLiteResultAdapter implementation
            bool adapt(boost::shared_ptr<IResultHandler> resultHandler) override
            {
               auto nCols = resultHandler->getColumnCount();
               m_internalValue.reserve(100 * nCols);
               m_internalValue = "{\"result\":\"true\",\"message\":\"\",\"data\":{\"data\":[";
               if (nCols == 6)
               {
                  long size = 0;
                  while (resultHandler->next_step())
                  {
                     m_internalValue += "{\"type\":\"";
                     m_internalValue += resultHandler->extractValueAsString(0);
                     m_internalValue += "\",\"keywordId\":\"";
                     m_internalValue += resultHandler->extractValueAsString(2); 
                     m_internalValue += "\",\"avg\":\"" + resultHandler->extractValueAsString(3);
                     m_internalValue += "\",\"min\":\"" + resultHandler->extractValueAsString(4) + "\",\"date\":\"";
                     m_internalValue += resultHandler->extractValueAsString(1);
                     m_internalValue += "\",\"max\":\"" + resultHandler->extractValueAsString(5) + "\"},";
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
               }
               return true;
            }
            /*
            bool adapt(std::vector<boost::shared_ptr<entities::CAcquisitionSummary>> resultSummary)
            {
               auto nCols = resultSummary.size();
               m_internalValue.reserve(100 * nCols);
               m_internalValue = "{\"data\":[";
               for (auto iterator = resultSummary.begin(); iterator != resultSummary.end(); ++iterator)
               {
                  m_internalValue += "{\"type\":\"Hour\",\"keywordId\":\"";
                  m_internalValue += std::to_string((*iterator)->KeywordId());
                  m_internalValue += "\",\"avg\":\"" + (*iterator)->Avg();
                  m_internalValue += "\",\"min\":\"" + (*iterator)->Min() + "\",\"date\":\"";
                  m_internalValue += boost::posix_time::to_iso_string((*iterator)->Date());
                  m_internalValue += "\",\"max\":\"" + (*iterator)->Max() + "\"},";
               }
               //check if any data
               if (m_internalValue.size() > 1)
                  m_internalValue[m_internalValue.size() - 1] = ']'; //replace the last ,

               m_internalValue += "}"; //no data, just append closing bracket
               return true;
            }*/

            std::vector<std::string> getResults() override
            {
               //getRawResults should be used
               throw shared::exception::CNotSupported("CHighchartValueAdapter::getResults");
            }

            // [END] ISQLiteResultAdapter implementation

            //--------------------------------------------------------------
            /// \Brief		Get the result (raw format)
            /// \return		The result (raw format) using a json like ouptut [[data],[data],....]
            //--------------------------------------------------------------
            std::string getRawResults() const
            {
               return m_internalValue;
            }

         private:
            std::string m_internalValue;
         };
      } //namespace adapters
   } //namespace common
} //namespace database 