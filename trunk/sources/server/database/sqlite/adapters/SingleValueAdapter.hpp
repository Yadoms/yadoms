#pragma once

#include "tools/StringExtension.hpp"

template<class TValue>
class CSingleValueAdapter: public ISQLiteResultAdapter<TValue>
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
   bool adapt(int column, char** columValues, char** columnName)
   {
      bool returnValue = false;
      if(column == 0 || columValues == NULL || columnName == NULL)
      {
         BOOST_LOG_TRIVIAL(warning) << "Invalid arguments";
         returnValue = false;
      }
      else
      {
         //check only one column is returned by query
         BOOST_ASSERT(column == 1);

         for(int i=0; i<column ; i++)
            m_results.push_back(CStringExtension::parse<TValue>(columValues[i]));

         returnValue = true;
      }
      return returnValue;
   }

   std::vector<TValue> getResults()
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