#pragma once
#include "IResultAdapter.h"
#include "SqlExtension.hpp"

namespace database
{
   namespace common
   {
      namespace adapters
      {
         //--------------------------------------------------------------
         ///\Brief		Class which adapt in single column resultset
         ///\template   TValue : the type of value
         ///\example    CSingleValueAdapter<int> will provide std::vector<int>
         //--------------------------------------------------------------
         template <class TValue>
         class CSingleValueAdapter: public IResultAdapterEx<TValue>
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		Constructor
            //--------------------------------------------------------------
            CSingleValueAdapter()
            {
            }

            //--------------------------------------------------------------
            /// \Brief		Destructor
            //--------------------------------------------------------------
            virtual ~CSingleValueAdapter()
            {
            }

            // ISQLiteResultAdapter implementation
            bool adapt(boost::shared_ptr<IResultHandler> resultHandler) override
            {
               auto nCols = resultHandler->getColumnCount();
               if (nCols == 1)
               {
                  while (resultHandler->next_step())
                  {
                     m_results.push_back(CSqlExtension::extractData<TValue>(resultHandler, 0));
                  }
                  return true;
               }
               return false;
            }

            std::vector<TValue> getResults() override
            {
               return m_results;
            }

            // [END] ISQLiteResultAdapter implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		Contains the list of results entities
            //--------------------------------------------------------------
            std::vector<TValue> m_results;
         };

         //--------------------------------------------------------------
         ///\Brief		Class which adapt in single column resultset
         ///\template   TValue : the type of value
         ///\example    CSingleValueAdapter<int> will provide std::vector<int>
         //--------------------------------------------------------------
         template <class TValue>
         class CSingleValueAdapterWithContainer: public IResultAdapterEx<TValue>
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		Constructor
            //--------------------------------------------------------------
            explicit CSingleValueAdapterWithContainer(std::vector<TValue>& c)
               : m_results(c)
            {
            }

            //--------------------------------------------------------------
            /// \Brief		Destructor
            //--------------------------------------------------------------
            virtual ~CSingleValueAdapterWithContainer()
            {
            }

            // ISQLiteResultAdapter implementation
            bool adapt(boost::shared_ptr<IResultHandler> resultHandler) override
            {
               auto nCols = resultHandler->getColumnCount();
               if (nCols == 1)
               {
                  while (resultHandler->next_step())
                  {
                     m_results.push_back(CSqlExtension::extractData<TValue>(resultHandler, 0));
                  }
                  return true;
               }
               return false;
            }

            std::vector<TValue> getResults() override
            {
               return m_results;
            }

            // [END] ISQLiteResultAdapter implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		Contains the list of results entities
            //--------------------------------------------------------------
            std::vector<TValue>& m_results;
         };
      } //namespace adapters
   } //namespace common
} //namespace database 


