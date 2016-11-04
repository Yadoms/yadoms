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
         ///\Brief		Class which adapt in single string, using a json like ouptut [[data],[data],....]
         //--------------------------------------------------------------
         class CHighchartValueAdapter : public IResultAdapterEx<std::string>
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		Constructor
            //--------------------------------------------------------------
            CHighchartValueAdapter()
            {
            }

            //--------------------------------------------------------------
            /// \Brief		Destructor
            //--------------------------------------------------------------
            virtual ~CHighchartValueAdapter()
            {
            }

            // ISQLiteResultAdapter implementation
            bool adapt(boost::shared_ptr<IResultHandler> resultHandler) override
            {
               auto nCols = resultHandler->getColumnCount();
               if (nCols == 1)
               {
                  m_internalValue = "[";
                  while (resultHandler->next_step())
                  {
                     m_internalValue += "[" + CSqlExtension::extractData<std::string>(resultHandler, 0) + "],";
                  }
                  //check if any data
                  if (m_internalValue.size() > 1)
                     m_internalValue[m_internalValue.size() - 1] = ']'; //replace the last ,
                  else
                     m_internalValue += "]"; //no data, just append closing bracket
                  return true;
               }
               return false;
            }

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


