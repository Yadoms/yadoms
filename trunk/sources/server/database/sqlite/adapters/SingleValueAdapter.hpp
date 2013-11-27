#pragma once

#include "tools/Converters.hpp"
#include "boost/tuple/tuple.hpp"

/*
template<class T1, class T2, class T3, class T4, class T5>
class CMultipleValueAdapter : public ISQLiteResultAdapter<boost::tuple<T1, T2, T3, T4, T5>>
{
public:
   //--------------------------------------------------------------
   /// \Brief		Constructor
   //--------------------------------------------------------------
   CMultipleValueAdapter()
   {
   }
   
   //--------------------------------------------------------------
   /// \Brief		Destructor
   //--------------------------------------------------------------
   virtual ~CMultipleValueAdapter()
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
         //for(int i=0; i<column ; i++)
         boost::tuple<T1, T2, T3, T4, T5> rowToAdd;
            //m_results.push_back(CConverters::getConverter<TValue>()(columValues[i]));

         returnValue = true;
      }
      return returnValue;
   }

   std::vector<TValue> getResults()
   {
      return m_results;
   }
   // [END] ISQLiteResultAdapter implementation

   static TValue convert(std::string v)
   {
      return boost::lexical_cast<TValue>(v);
   }
private:
   //--------------------------------------------------------------
   /// \Brief		Contains the list of results entities
   //--------------------------------------------------------------
    std::vector<TValue> m_results;
};
*/
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
            m_results.push_back(CConverters::getConverter<TValue>()(columValues[i]));

         returnValue = true;
      }
      return returnValue;
   }

   std::vector<TValue> getResults()
   {
      return m_results;
   }
   // [END] ISQLiteResultAdapter implementation

   static TValue convert(std::string v)
   {
      return boost::lexical_cast<TValue>(v);
   }
private:
   //--------------------------------------------------------------
   /// \Brief		Contains the list of results entities
   //--------------------------------------------------------------
    std::vector<TValue> m_results;
};