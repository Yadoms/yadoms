#include "stdafx.h"
#include "GenericAdapter.h"
#include "SqlExtension.hpp"


namespace database
{
   namespace common
   {
      namespace adapters
      {
         CGenericAdapter::CGenericAdapter()
         {
         }

         CGenericAdapter::~CGenericAdapter()
         {
         }

         bool CGenericAdapter::adapt(boost::shared_ptr<IResultHandler> resultHandler)
         {
            auto nCols = resultHandler->getColumnCount();
            if (nCols)
            {
               std::vector<std::string> cols;
               for (auto nCol = 0; nCol < nCols; nCol++)
                  cols.push_back(std::string(resultHandler->getColumnName(nCol)));

               while (resultHandler->next_step())
               {
                  std::map<std::string, std::string> newRow;
                  for (auto nCol = 0; nCol < nCols; nCol++)
                     newRow.insert(std::make_pair(cols[nCol], CSqlExtension::extractData<std::string>(resultHandler, nCol)));
                  m_results.push_back(newRow);
               }
               return true;
            }
            return false;
         }


         std::vector<std::map<std::string, std::string>> CGenericAdapter::getResults()
         {
            return m_results;
         }
      } //namespace adapters
   } //namespace common
} //namespace database 


