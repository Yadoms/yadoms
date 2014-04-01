#pragma once

#include "Json.h"

namespace web { namespace rest { namespace json {

   class CJsonGenericSerializer
   {
   public:
      CJsonGenericSerializer();
      virtual ~CJsonGenericSerializer();

      CJson serialize(const std::vector<std::map<std::string, std::string> > & object);
   };


      //--------------------------------------------------------------
   /// \Brief		Adapter for multiple columns resultset (up to 10 values in a boost::tuple)
   ///\template   T0 : the type of the column n°0
   ///\template   T1 : the type of the column n°1
   ///\template   T2 : the type of the column n°2
   ///\template   T3 : the type of the column n°3
   ///\template   T4 : the type of the column n°4
   ///\template   T5 : the type of the column n°5
   ///\template   T6 : the type of the column n°6
   ///\template   T7 : the type of the column n°7
   ///\template   T8 : the type of the column n°8
   ///\template   T9 : the type of the column n°9
   ///\example    CMultipleValueAdapter<int, double, std::string> will provide std::vector<boost::tuple<int, double, std::string> >
   //--------------------------------------------------------------
   /*
   template<class T0, class T1= boost::tuples::null_type, class T2= boost::tuples::null_type, class T3= boost::tuples::null_type, class T4= boost::tuples::null_type, class T5= boost::tuples::null_type, class T6= boost::tuples::null_type, class T7= boost::tuples::null_type, class T8= boost::tuples::null_type, class T9 = boost::tuples::null_type>
   class CJsonMultipleValueSerializer
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		Constructor
      //--------------------------------------------------------------
      CJsonMultipleValueSerializer(std::string n0, std::string n1= "", std::string n2= "", std::string n3= "", std::string n4= "", std::string n5= "", std::string n6= "", std::string n7= "", std::string n8= "", std::string n9= "")
         :m_columnNames(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)
      {
      }
   
      //--------------------------------------------------------------
      /// \Brief		Destructor
      //--------------------------------------------------------------
      virtual ~CMultipleValueAdapter()
      {
      }

      

      CJson serialize(const std::vector< boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > & objects)
      {
         CJson objectList;

         std::vector< boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::const_iterator i;

         for(i=objects.begin(); i!=objects.end(); i++)
         {
            CJson result;

            if(m_columnNames.get<0>() != "")
               result.put(m_columnNames.get<0>(), i->get<0>());
            
            objectList.push_back(std::make_pair("", result));
         }
         return objectList;
      }
   /*

      // ISQLiteResultAdapter implementation
      bool adapt(sqlite3_stmt * pStatement)
      {
         int nCols = sqlite3_column_count(pStatement);
         if (nCols) 
         {
            int rc;
            while ((rc = sqlite3_step(pStatement)) == SQLITE_ROW) 
            {
               //check only one column is returned by query
               //for(int i=0; i<column ; i++)
               T0 t0; T1 t1; T2 t2; T3 t3; T4 t4;
               T5 t5; T6 t6; T7 t7; T8 t8; T9 t9;

               if(column > 0)
                  t0 = CSQLite3Extension::extractData<T0 >(pStatement, 0);
               if(column > 1)
                  t1 = CSQLite3Extension::extractData<T1 >(columValues[1]);
               if(column > 2)
                  t2 = CSQLite3Extension::extractData<T2 >(columValues[2]);
               if(column > 3)
                  t3 = CSQLite3Extension::extractData<T3 >(columValues[3]);
               if(column > 4)
                  t4 = CSQLite3Extension::extractData<T4 >(columValues[4]);
               if(column > 5)
                  t5 = CSQLite3Extension::extractData<T5 >(columValues[5]);
               if(column > 6)
                  t6 = CSQLite3Extension::extractData<T6 >(columValues[6]);
               if(column > 7)
                  t7 = CSQLite3Extension::extractData<T7 >(columValues[7]);
               if(column > 8)
                  t8 = CSQLite3Extension::extractData<T8 >(columValues[8]);
               if(column > 9)
                  t9 = CSQLite3Extension::extractData<T9 >(columValues[9]);

               m_results.push_back(boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9));

            }
            returnValue = true;
         }
         return returnValue;
      }
    
   private:
      boost::tuple<std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string> m_columnNames;
   };
  */
} //namespace json
} //namespace rest
} //namespace web 

