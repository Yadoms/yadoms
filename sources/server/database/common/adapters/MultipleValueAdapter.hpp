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
         template <class T0,
                   class T1= boost::tuples::null_type,
                   class T2= boost::tuples::null_type,
                   class T3= boost::tuples::null_type,
                   class T4= boost::tuples::null_type,
                   class T5= boost::tuples::null_type,
                   class T6= boost::tuples::null_type,
                   class T7= boost::tuples::null_type,
                   class T8= boost::tuples::null_type,
                   class T9 = boost::tuples::null_type>
         class CMultipleValueAdapter : public IResultAdapterEx<boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>>
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
            bool adapt(boost::shared_ptr<IResultHandler> resultHandler) override
            {
               auto returnValue = false;
               auto nCols = resultHandler->getColumnCount();
               if (nCols)
               {
                  while (resultHandler->nextStep())
                  {
                     //check only one column is returned by query
                     //for(int i=0; i<column ; i++)
                     T0 t0;
                     T1 t1;
                     T2 t2;
                     T3 t3;
                     T4 t4;
                     T5 t5;
                     T6 t6;
                     T7 t7;
                     T8 t8;
                     T9 t9;

                     if (nCols > 0)
                        t0 = CSqlExtension::extractData<T0>(resultHandler, 0);
                     if (nCols > 1)
                        t1 = CSqlExtension::extractData<T1>(resultHandler, 1);
                     if (nCols > 2)
                        t2 = CSqlExtension::extractData<T2>(resultHandler, 2);
                     if (nCols > 3)
                        t3 = CSqlExtension::extractData<T3>(resultHandler, 3);
                     if (nCols > 4)
                        t4 = CSqlExtension::extractData<T4>(resultHandler, 4);
                     if (nCols > 5)
                        t5 = CSqlExtension::extractData<T5>(resultHandler, 5);
                     if (nCols > 6)
                        t6 = CSqlExtension::extractData<T6>(resultHandler, 6);
                     if (nCols > 7)
                        t7 = CSqlExtension::extractData<T7>(resultHandler, 7);
                     if (nCols > 8)
                        t8 = CSqlExtension::extractData<T8>(resultHandler, 8);
                     if (nCols > 9)
                        t9 = CSqlExtension::extractData<T9>(resultHandler, 9);

                     m_results.push_back(boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9));
                  }
                  returnValue = true;
               }
               return returnValue;
            }

            std::vector<boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>> getResults() override
            {
               return m_results;
            }

            // [END] ISQLiteResultAdapter implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		Contains the list of results entities
            //--------------------------------------------------------------
            std::vector<boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>> m_results;
         };
      } //namespace adapters
   } //namespace common
} //namespace database 


