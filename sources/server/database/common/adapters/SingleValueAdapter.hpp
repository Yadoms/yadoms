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
         ///\Template   TValue : the type of value
         ///\Example    CSingleValueAdapter<int> will provide std::vector<int>
         //--------------------------------------------------------------
         template <class TValue>
         class CSingleValueAdapter final : public IResultAdapterEx<TValue>
         {
         public:
            CSingleValueAdapter() = default;
            ~CSingleValueAdapter() override = default;

            // ISQLiteResultAdapter implementation
            bool adapt(boost::shared_ptr<IResultHandler> resultHandler) override
            {
               const auto nCols = resultHandler->getColumnCount();
               if (nCols != 1)
                  return false;

               while (resultHandler->nextStep())
                  m_results.push_back(CSqlExtension::extractData<TValue>(resultHandler, 0));

               return true;
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
         ///\Template   TValue : the type of value
         ///\Example    CSingleValueAdapter<int> will provide std::vector<int>
         //--------------------------------------------------------------
         template <class TValue>
         class CSingleValueAdapterWithContainer final : public IResultAdapterEx<TValue>
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		Constructor
            //--------------------------------------------------------------
            explicit CSingleValueAdapterWithContainer(std::vector<TValue>& c)
               : m_results(c)
            {
            }

            ~CSingleValueAdapterWithContainer() override = default;

            // ISQLiteResultAdapter implementation
            bool adapt(boost::shared_ptr<IResultHandler> resultHandler) override
            {
               const auto nCols = resultHandler->getColumnCount();
               if (nCols != 1)
                  return false;

               while (resultHandler->nextStep())
                  m_results.push_back(CSqlExtension::extractData<TValue>(resultHandler, 0));

               return true;
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
