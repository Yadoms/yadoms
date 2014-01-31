
#pragma once

#include "tools/StringExtension.h"
#include "tools/Log.h"

//--------------------------------------------------------------
/// \Brief		Adapter for multiple values (up to 10 values in a boost::tuple
//--------------------------------------------------------------
template<class T0, class T1= boost::tuples::null_type, class T2= boost::tuples::null_type, class T3= boost::tuples::null_type, class T4= boost::tuples::null_type, class T5= boost::tuples::null_type, class T6= boost::tuples::null_type, class T7= boost::tuples::null_type, class T8= boost::tuples::null_type, class T9 = boost::tuples::null_type>
class CMultipleValueAdapter : public ISQLiteResultAdapter<boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >
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
         YADOMS_LOG(warning) << "Invalid arguments";
         returnValue = false;
      }
      else
      {
         //check only one column is returned by query
         //for(int i=0; i<column ; i++)
         T0 t0; T1 t1; T2 t2; T3 t3; T4 t4;
         T5 t5; T6 t6; T7 t7; T8 t8; T9 t9;

         if(column > 0)
            t0 = CStringExtension::parse<T0>(columValues[0]);
         if(column > 1)
            t1 = CStringExtension::parse<T1>(columValues[1]);
         if(column > 2)
            t2 = CStringExtension::parse<T2>(columValues[2]);
         if(column > 3)
            t3 = CStringExtension::parse<T3>(columValues[3]);
         if(column > 4)
            t4 = CStringExtension::parse<T4>(columValues[4]);
         if(column > 5)
            t5 = CStringExtension::parse<T5>(columValues[5]);
         if(column > 6)
            t6 = CStringExtension::parse<T6>(columValues[6]);
         if(column > 7)
            t7 = CStringExtension::parse<T7>(columValues[7]);
         if(column > 8)
            t8 = CStringExtension::parse<T8>(columValues[8]);
         if(column > 9)
            t9 = CStringExtension::parse<T9>(columValues[9]);

         m_results.push_back(boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9));

         returnValue = true;
      }
      return returnValue;
   }

   std::vector<boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > getResults()
   {
      return m_results;
   }

   // [END] ISQLiteResultAdapter implementation

private:
   //--------------------------------------------------------------
   /// \Brief		Contains the list of results entities
   //--------------------------------------------------------------
    std::vector<boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > m_results;
};

