#pragma once

#include <Poco/Types.h>
#include <Poco/DateTime.h>
#include <Poco/Timestamp.h>
#include <shared/enumeration/IExtendedEnum.h> 
#include <shared/Field.hpp>
#include "database/DatabaseException.hpp"
#include "DatabaseColumn.h"

namespace database
{
   namespace common
   {
#define EMPTY_STR ""

#define CQUERY_OP_EQUAL "="
#define CQUERY_OP_LIKE " LIKE "
#define CQUERY_OP_IN " IN "
#define CQUERY_OP_SUP ">"
#define CQUERY_OP_INF "<"
#define CQUERY_OP_NOT_EQUAL "<>"
#define CQUERY_OP_SUP_EQUAL ">="
#define CQUERY_OP_INF_EQUAL "<="

#define CQUERY_OP_PLUS "+"
#define CQUERY_OP_MINUS "-"
#define CQUERY_OP_DIVIDE "/"
#define CQUERY_OP_MUL "*"


      /*
      CQuery q;
   
      q. Select().
      From(CPluginTable::getTableName()).
      Where(CPluginTable::getIdColumnName(), "=", 2);
   
      q. InsertInto(CPluginTable::getTableName(), CPluginTable::getIdColumnName(), CPluginTable::getNameColumnName()).
      Values(1, "test");
   
      */

      //
      /// \brief  Class used to create queries
      //
      class CQuery
      {
      public:
         enum EQueryType
         {
            kNotYetDefined,
            kSelect,
            kInsert,
            kUpdate,
            kDelete,
            kDrop,
            kCreate,
            kAlter,
            kVacuum
         };

         enum E_OrderWay
         {
            kAsc = 0,
            kDesc
         };

         class CNotUsedTemplateField
         {
         public:
            CNotUsedTemplateField()
            {
            }

            virtual ~CNotUsedTemplateField()
            {
            }
         };

      protected:
         //
         /// \brief           Constructor
         //
         explicit CQuery(const bool fromWithClauseNeeded = false);

      public:
         //
         /// \brief           Copy constructor
         //
         CQuery(const CQuery& toCopy);

         CQuery& operator=(const CQuery&) = delete;


         //
         /// \brief           Destructor
         //
         virtual ~CQuery();

         //
         /// \brief              Clear current query
         /// \return             A reference to itself to allow method chaining
         //  
         CQuery& Clear();

         //
         /// \brief           Start a query with 'SELECT *'
         /// \return          A reference to itself to allow method chaining
         //
         CQuery& Select();

         //
         /// \brief           Start a query with 'SELECT field1 [,field2[, field3...]]'
         /// \param  field1   a field to append to the select fields
         /// \param  field2   a field to append to the select fields
         /// \param  field3   a field to append to the select fields
         /// \param  field4   a field to append to the select fields
         /// \param  field5   a field to append to the select fields
         /// \param  field6   a field to append to the select fields
         /// \param  field7   a field to append to the select fields
         /// \param  field8   a field to append to the select fields
         /// \param  field9   a field to append to the select fields
         /// \param  field10  a field to append to the select fields
         /// \return          A reference to itself to allow method chaining
         //
         template <class T1, class T2 = CNotUsedTemplateField, class T3 = CNotUsedTemplateField, class T4 = CNotUsedTemplateField, class T5 = CNotUsedTemplateField, class T6 = CNotUsedTemplateField, class T7 = CNotUsedTemplateField, class T8 = CNotUsedTemplateField, class T9 = CNotUsedTemplateField, class T10 = CNotUsedTemplateField>
         inline CQuery& Select(const T1& field1, const T2& field2 = T2(), const T3& field3 = T3(), const T4& field4 = T4(), const T5& field5 = T5(), const T6& field6 = T6(), const T7& field7 = T7(), const T8& field8 = T8(), const T9& field9 = T9(), const T10& field10 = T10());


         //
         /// \brief           Start a query with 'SELECT COUNT(*)'
         /// \return          A reference to itself to allow method chaining
         //
         CQuery& SelectCount();

         //
         /// \brief           Start a query with 'SELECT COUNT(field1 [,field2[, field3...]])'
         /// \param  field1   a field to append to the select fields
         /// \param  field2   a field to append to the select fields
         /// \param  field3   a field to append to the select fields
         /// \param  field4   a field to append to the select fields
         /// \param  field5   a field to append to the select fields
         /// \param  field6   a field to append to the select fields
         /// \param  field7   a field to append to the select fields
         /// \param  field8   a field to append to the select fields
         /// \param  field9   a field to append to the select fields
         /// \param  field10  a field to append to the select fields
         /// \return          A reference to itself to allow method chaining
         //
         template <class T1, class T2 = CNotUsedTemplateField, class T3 = CNotUsedTemplateField, class T4 = CNotUsedTemplateField, class T5 = CNotUsedTemplateField, class T6 = CNotUsedTemplateField, class T7 = CNotUsedTemplateField, class T8 = CNotUsedTemplateField, class T9 = CNotUsedTemplateField, class T10 = CNotUsedTemplateField>
         inline CQuery& SelectCount(const T1& field1, const T2& field2 = T2(), const T3& field3 = T3(), const T4& field4 = T4(), const T5& field5 = T5(), const T6& field6 = T6(), const T7& field7 = T7(), const T8& field8 = T8(), const T9& field9 = T9(), const T10& field10 = T10());

         //
         /// \brief           Append 'From table1 [,table2 [,table3.. ]]'
         /// \param  field1   a table name to append to the from clause
         /// \param  field2   a table name to append to the from clause
         /// \param  field3   a table name to append to the from clause
         /// \param  field4   a table name to append to the from clause
         /// \param  field5   a table name to append to the from clause
         /// \param  field6   a table name to append to the from clause
         /// \param  field7   a table name to append to the from clause
         /// \param  field8   a table name to append to the from clause
         /// \param  field9   a table name to append to the from clause
         /// \param  field10  a table name to append to the from clause
         /// \return          A reference to itself to allow method chaining
         //   
         template <class T1, class T2 = CNotUsedTemplateField, class T3 = CNotUsedTemplateField, class T4 = CNotUsedTemplateField, class T5 = CNotUsedTemplateField, class T6 = CNotUsedTemplateField, class T7 = CNotUsedTemplateField, class T8 = CNotUsedTemplateField, class T9 = CNotUsedTemplateField, class T10 = CNotUsedTemplateField>
         inline CQuery& From(const T1& table1, const T2& table2 = T2(), const T3& table3 = T3(), const T4& table4 = T4(), const T5& table5 = T5(), const T6& table6 = T6(), const T7& table7 = T7(), const T8& table8 = T8(), const T9& table9 = T9(), const T10& table10 = T10());

         //
         /// \brief           Append 'From subquery'
         /// \param  subquery   the subquery
         /// \return          A reference to itself to allow method chaining
         //   
         CQuery& From(const CQuery& subquery);

         //
         /// \brief           Append from to an update statement in case of using with WITH (needed in some db engines)
         ///                  This common implmentation do nothing, put db provider may override it
         /// \param  withResultName   the with clause result name
         /// \return          A reference to itself to allow method chaining
         //   
         CQuery& FromWith(const std::string& withResultName);

         //
         /// \brief           Append 'From (subquery)'
         /// \param  subquery   the subquery
         /// \return          A reference to itself to allow method chaining
         //   
         CQuery& FromParenthesis(const CQuery& subquery);

         //
         /// \brief           Append the where clause
         /// \param  condition the where condition
         /// \return          A reference to itself to allow method chaining
         //     
         CQuery& Where(const std::string& condition);

         //
         /// \brief           Append the where clause
         ///                  Where("id", "=", "12")
         /// \param  field    the field name
         /// \param  op       the operator
         /// \param  field    the value
         /// \return          A reference to itself to allow method chaining
         //     
         template <class T1, class T2>
         inline CQuery& Where(const T1& field, const std::string& op, const T2& value);

         //
         /// \brief           Append the where in clause
         ///                  Where field in("value1", "value2", "value3")
         /// \param  field    the field name
         /// \param  values   the list of values
         /// \return          A reference to itself to allow method chaining
         //
         template <class T1, class T2>
         inline CQuery& WhereIn(const T1& field, const std::vector<T2>& values);


         //
         /// \brief           Append the where clause with a start parenthesis
         ///                  Where("id", "=", "12")
         /// \param  field    the field name
         /// \param  op       the operator
         /// \param  value    the value
         /// \return          A reference to itself to allow method chaining
         //     
         template <class T1, class T2>
         inline CQuery& WhereParenthesis(const T1& field, const std::string& op, const T2& value);


         //
         /// \brief           Append the AND clause
         /// \param  condition the condition
         /// \return          A reference to itself to allow method chaining
         //        
         CQuery& And(const std::string& condition);

         //
         /// \brief           Append the And clause
         ///                  And("id", "=", "12")
         /// \param  field    the field name
         /// \param  op       the operator
         /// \param  value    the value
         /// \return          A reference to itself to allow method chaining
         //     
         template <class T1, class T2>
         inline CQuery& And(const T1& field, const std::string& op, const T2& value);


         //
         /// \brief           Append the And clause with a start parenthesis
         ///                  And("id", "=", "12")
         /// \param  field    the field name
         /// \param  op       the operator
         /// \param  value    the value
         /// \return          A reference to itself to allow method chaining
         //     
         template <class T1, class T2>
         inline CQuery& AndParenthesis(const T1& field, const std::string& op, const T2& value);

         //
         /// \brief           Append the OR clause
         /// \param  condition the condition
         /// \return          A reference to itself to allow method chaining
         //          
         CQuery& Or(const std::string& condition);

         //
         /// \brief           Append the Or clause
         ///                  Or("id", "=", "12")
         /// \param  field    the field name
         /// \param  op       the operator
         /// \param  value    the value
         /// \return          A reference to itself to allow method chaining
         //        
         template <class T1, class T2>
         inline CQuery& Or(const T1& field, const std::string& op, const T2& value);

         //
         /// \brief           Append the Or clause  with a start parenthesis
         ///                  Or("id", "=", "12")
         /// \param  field    the field name
         /// \param  op       the operator
         /// \param  value    the value
         /// \return          A reference to itself to allow method chaining
         //        
         template <class T1, class T2>
         inline CQuery& OrParenthesis(const T1& field, const std::string& op, const T2& value);

         //
         /// \brief           Append  a closing parenthesis
         /// \return          A reference to itself to allow method chaining
         //       
         CQuery& EndParenthesis();

         //
         /// \brief           Append 'ORDER BY field1 [way1][,field2 [way2]]'
         /// \param  field1   a field to order by
         /// \param  way1     the way of field1  (values can be ASC or DESC, empty do not append the way)
         /// \param  field2   a field to order by
         /// \param  way2     the way of field2  (values can be ASC or DESC, empty do not append the way)
         /// \param  field3   a field to order by
         /// \param  way3     the way of field3  (values can be ASC or DESC, empty do not append the way)
         /// \param  field4   a field to order by
         /// \param  way4     the way of field4  (values can be ASC or DESC, empty do not append the way)
         /// \param  field5   a field to order by
         /// \param  way5     the way of field5  (values can be ASC or DESC, empty do not append the way)
         /// \param  field6   a field to order by
         /// \param  way6     the way of field6  (values can be ASC or DESC, empty do not append the way)
         /// \param  field7   a field to order by
         /// \param  way7     the way of field7  (values can be ASC or DESC, empty do not append the way)
         /// \param  field8   a field to order by
         /// \param  way8     the way of field8  (values can be ASC or DESC, empty do not append the way)
         /// \param  field9   a field to order by
         /// \param  way9     the way of field9  (values can be ASC or DESC, empty do not append the way)
         /// \param  field10  a field to order by
         /// \param  way10    the way of field10 (values can be ASC or DESC, empty do not append the way)   
         /// \return          A reference to itself to allow method chaining
         //     
         template <class T1, class T2 = CNotUsedTemplateField, class T3 = CNotUsedTemplateField, class T4 = CNotUsedTemplateField, class T5 = CNotUsedTemplateField, class T6 = CNotUsedTemplateField, class T7 = CNotUsedTemplateField, class T8 = CNotUsedTemplateField, class T9 = CNotUsedTemplateField, class T10 = CNotUsedTemplateField>
         inline CQuery& OrderBy(const T1& value1, const E_OrderWay way1 = kAsc,
                                const T2& value2 = T2(), const E_OrderWay way2 = kAsc,
                                const T3& value3 = T3(), const E_OrderWay way3 = kAsc,
                                const T4& value4 = T4(), const E_OrderWay way4 = kAsc,
                                const T5& value5 = T5(), const E_OrderWay way5 = kAsc,
                                const T6& value6 = T6(), const E_OrderWay way6 = kAsc,
                                const T7& value7 = T7(), const E_OrderWay way7 = kAsc,
                                const T8& value8 = T8(), const E_OrderWay way8 = kAsc,
                                const T9& value9 = T9(), const E_OrderWay way9 = kAsc,
                                const T10& value10 = T10(), const E_OrderWay way10 = kAsc);


         //
         /// \brief           Append 'GROUP BY field1 [,field2 [,field3...]]'
         /// \param  field1   a field name to append to the group by clause
         /// \param  field2   a field name to append to the group by clause
         /// \param  field3   a field name to append to the group by clause
         /// \param  field4   a field name to append to the group by clause
         /// \param  field5   a field name to append to the group by clause
         /// \param  field6   a field name to append to the group by clause
         /// \param  field7   a field name to append to the group by clause
         /// \param  field8   a field name to append to the group by clause
         /// \param  field9   a field name to append to the group by clause
         /// \param  field10  a field name to append to the group by clause
         /// \return          A reference to itself to allow method chaining
         //      
         template <class T1, class T2 = CNotUsedTemplateField, class T3 = CNotUsedTemplateField, class T4 = CNotUsedTemplateField, class T5 = CNotUsedTemplateField, class T6 = CNotUsedTemplateField, class T7 = CNotUsedTemplateField, class T8 = CNotUsedTemplateField, class T9 = CNotUsedTemplateField, class T10 = CNotUsedTemplateField>
         inline CQuery& GroupBy(const T1& field1,
                                const T2& field2 = T2(),
                                const T3& field3 = T3(),
                                const T4& field4 = T4(),
                                const T5& field5 = T5(),
                                const T6& field6 = T6(),
                                const T7& field7 = T7(),
                                const T8& field8 = T8(),
                                const T9& field9 = T9(),
                                const T10& field10 = T10());

         //
         /// \brief           Append 'INSERT INTO table (field1 [,field2 [,field3...]])'
         /// \param  table    the target table 
         /// \param  field1   a field name to append to the insert into clause
         /// \param  field2   a field name to append to the insert into clause
         /// \param  field3   a field name to append to the insert into clause
         /// \param  field4   a field name to append to the insert into clause
         /// \param  field5   a field name to append to the insert into clause
         /// \param  field6   a field name to append to the insert into clause
         /// \param  field7   a field name to append to the insert into clause
         /// \param  field8   a field name to append to the insert into clause
         /// \param  field9   a field name to append to the insert into clause
         /// \param  field10  a field name to append to the insert into clause
         /// \return          A reference to itself to allow method chaining
         //         
         template <class T1, class T2 = CNotUsedTemplateField, class T3 = CNotUsedTemplateField, class T4 = CNotUsedTemplateField, class T5 = CNotUsedTemplateField, class T6 = CNotUsedTemplateField, class T7 = CNotUsedTemplateField, class T8 = CNotUsedTemplateField, class T9 = CNotUsedTemplateField, class T10 = CNotUsedTemplateField>
         inline CQuery& InsertInto(const database::common::CDatabaseTable& table,
                                   const T1& field1,
                                   const T2& field2 = T2(),
                                   const T3& field3 = T3(),
                                   const T4& field4 = T4(),
                                   const T5& field5 = T5(),
                                   const T6& field6 = T6(),
                                   const T7& field7 = T7(),
                                   const T8& field8 = T8(),
                                   const T9& field9 = T9(),
                                   const T10& field10 = T10());

         //
         /// \brief           Append 'INSERT OR REPLACE INTO table (field1 [,field2 [,field3...]])'
         /// \param  table    the target table 
         /// \param  field1   a field name to append to the insert into clause
         /// \param  field2   a field name to append to the insert into clause
         /// \param  field3   a field name to append to the insert into clause
         /// \param  field4   a field name to append to the insert into clause
         /// \param  field5   a field name to append to the insert into clause
         /// \param  field6   a field name to append to the insert into clause
         /// \param  field7   a field name to append to the insert into clause
         /// \param  field8   a field name to append to the insert into clause
         /// \param  field9   a field name to append to the insert into clause
         /// \param  field10  a field name to append to the insert into clause
         /// \return          A reference to itself to allow method chaining
         //         
         template <class T1, class T2 = CNotUsedTemplateField, class T3 = CNotUsedTemplateField, class T4 = CNotUsedTemplateField, class T5 = CNotUsedTemplateField, class T6 = CNotUsedTemplateField, class T7 = CNotUsedTemplateField, class T8 = CNotUsedTemplateField, class T9 = CNotUsedTemplateField, class T10 = CNotUsedTemplateField>
         inline CQuery& InsertOrReplaceInto(const database::common::CDatabaseTable& table,
                                            const T1& field1,
                                            const T2& field2 = T2(),
                                            const T3& field3 = T3(),
                                            const T4& field4 = T4(),
                                            const T5& field5 = T5(),
                                            const T6& field6 = T6(),
                                            const T7& field7 = T7(),
                                            const T8& field8 = T8(),
                                            const T9& field9 = T9(),
                                            const T10& field10 = T10());

         //
         /// \brief           Append 'VALUES (field1 [,field2 [,field3...]])'
         /// \param  value1   a value of the field
         /// \param  value2   a value of the field
         /// \param  value3   a value of the field
         /// \param  value4   a value of the field
         /// \param  value5   a value of the field
         /// \param  value6   a value of the field
         /// \param  value7   a value of the field
         /// \param  value8   a value of the field
         /// \param  value9   a value of the field
         /// \param  value10  a value of the field
         /// \return          A reference to itself to allow method chaining
         //      
         template <class T1, class T2 = CNotUsedTemplateField, class T3 = CNotUsedTemplateField, class T4 = CNotUsedTemplateField, class T5 = CNotUsedTemplateField, class T6 = CNotUsedTemplateField, class T7 = CNotUsedTemplateField, class T8 = CNotUsedTemplateField, class T9 = CNotUsedTemplateField, class T10 = CNotUsedTemplateField>
         inline CQuery& Values(const T1& value1,
                               const T2& value2 = T2(),
                               const T3& value3 = T3(),
                               const T4& value4 = T4(),
                               const T5& value5 = T5(),
                               const T6& value6 = T6(),
                               const T7& value7 = T7(),
                               const T8& value8 = T8(),
                               const T9& value9 = T9(),
                               const T10& value10 = T10());

         //
         /// \brief           Append the UPDATE clause
         /// \param  table    the table name
         /// \return          A reference to itself to allow method chaining
         //        
         CQuery& Update(const database::common::CDatabaseTable& table);

         //
         /// \brief           Append 'SET field1=value1 [,field2=value2...]]'
         /// \param  field1   a field to set
         /// \param  value1   the value of field1 
         /// \param  field2   a field to set
         /// \param  value2   the value of field2 
         /// \param  field3   a field to set
         /// \param  value3   the value of field3 
         /// \param  field4   a field to set
         /// \param  value4   the value of field4 
         /// \param  field5   a field to set
         /// \param  value5   the value of field5 
         /// \param  field6   a field to set
         /// \param  value6   the value of field6 
         /// \param  field7   a field to set
         /// \param  value7   the value of field7 
         /// \param  field8   a field to set
         /// \param  value8   the value of field8 
         /// \param  field9   a field to set
         /// \param  value9   the value of field9 
         /// \param  field10  a field to set
         /// \param  value10  the value of field10   
         /// \return          A reference to itself to allow method chaining
         //     
         template <class T01, class T1,
                   class T02 = CNotUsedTemplateField, class T2 = CNotUsedTemplateField,
                   class T03 = CNotUsedTemplateField, class T3 = CNotUsedTemplateField,
                   class T04 = CNotUsedTemplateField, class T4 = CNotUsedTemplateField,
                   class T05 = CNotUsedTemplateField, class T5 = CNotUsedTemplateField,
                   class T06 = CNotUsedTemplateField, class T6 = CNotUsedTemplateField,
                   class T07 = CNotUsedTemplateField, class T7 = CNotUsedTemplateField,
                   class T08 = CNotUsedTemplateField, class T8 = CNotUsedTemplateField,
                   class T09 = CNotUsedTemplateField, class T9 = CNotUsedTemplateField,
                   class T11 = CNotUsedTemplateField, class T10 = CNotUsedTemplateField>
         inline CQuery& Set(const T01& field1, const T1& value1,
                            const T02& field2 = T02(), const T2& value2 = T2(),
                            const T03& field3 = T03(), const T3& value3 = T3(),
                            const T04& field4 = T04(), const T4& value4 = T4(),
                            const T05& field5 = T05(), const T5& value5 = T5(),
                            const T06& field6 = T06(), const T6& value6 = T6(),
                            const T07& field7 = T07(), const T7& value7 = T7(),
                            const T08& field8 = T08(), const T8& value8 = T8(),
                            const T09& field9 = T09(), const T9& value9 = T9(),
                            const T11& field10 = T11(), const T10& value10 = T10());


         //
         /// \brief           Append the WITH clause with a subquery 
         ///                  //WITH tableName AS (subQuery)
         /// \param  tableName   The table name
         /// \param  subQuery    the subquery
         /// \return          A reference to itself to allow method chaining
         //    
         CQuery& With(const std::string& tableName1, const CQuery& subQuery1,
                      const std::string& tableName2 = EMPTY_STR, const CQuery& subQuery2 = CQuery::EmptyQuery,
                      const std::string& tableName3 = EMPTY_STR, const CQuery& subQuery3 = CQuery::EmptyQuery,
                      const std::string& tableName4 = EMPTY_STR, const CQuery& subQuery4 = CQuery::EmptyQuery,
                      const std::string& tableName5 = EMPTY_STR, const CQuery& subQuery5 = CQuery::EmptyQuery,
                      const std::string& tableName6 = EMPTY_STR, const CQuery& subQuery6 = CQuery::EmptyQuery,
                      const std::string& tableName7 = EMPTY_STR, const CQuery& subQuery7 = CQuery::EmptyQuery,
                      const std::string& tableName8 = EMPTY_STR, const CQuery& subQuery8 = CQuery::EmptyQuery,
                      const std::string& tableName9 = EMPTY_STR, const CQuery& subQuery9 = CQuery::EmptyQuery,
                      const std::string& tableName10 = EMPTY_STR, const CQuery& subQuery10 = CQuery::EmptyQuery);


         //
         /// \brief           create the LIMIT clause
         /// \param  numberOfRowsToReturn    the maximum number of rows to return
         /// \return          A reference to itself to allow method chaining
         //     
         CQuery& Limit(const int numberOfRowsToReturn);

         //
         /// \brief           create the LIMIT clause with OFFSET
         /// \param  numberOfRowsToReturn    the maximum number of rows to return
         /// \param  offset    the offset (number of rows to skip)
         /// \return          A reference to itself to allow method chaining
         //     
         CQuery& Limit(const int numberOfRowsToReturn, const int offset);


         //
         /// \brief           create the DELETE clause
         /// \param  table    the table name
         /// \return          A reference to itself to allow method chaining
         //     
         CQuery& DeleteFrom(const database::common::CDatabaseTable& table);

         //
         /// \brief              append text to query
         /// \param  queryPart   the query part to append
         /// \return             A reference to itself to allow method chaining
         //   
         CQuery& Append(const std::ostringstream& queryPart);

         //
         /// \brief              append text to query
         /// \param  queryPart   the query part to append
         /// \return             A reference to itself to allow method chaining
         //   
         CQuery& Append(const std::string& queryPart);

         //
         /// \brief              append text to query
         /// \param  queryPart   another query
         /// \return             A reference to itself to allow method chaining
         //   
         CQuery& Append(CQuery& queryPart);

         //
         /// \brief              append union
         /// \return             A reference to itself to allow method chaining
         //   
         CQuery& Union();

         //
         /// \brief              append union all
         /// \return             A reference to itself to allow method chaining
         //   
         CQuery& UnionAll();

         //
         /// \brief                    append "LEFT JOIN"
         /// \param  tableName         the table
         /// \param  tableIdentifier   the table identifier (ex: "A" -> LEFT JOIN myTable A )
         /// \return             A reference to itself to allow method chaining
         //   
         CQuery& JoinLeft(const database::common::CDatabaseTable& tableName, const std::string& tableIdentifier = EMPTY_STR);

         //
         /// \brief              append "ON tableIdentifier.columnName = table2Identifier.column2Name"
         /// \param              tableIdentifier    the table identifier
         /// \param              columnName         the column name
         /// \param              table2Identifier   the table identifier
         /// \param              column2Name        the column name
         /// \return             A reference to itself to allow method chaining
         //   
         CQuery& On(const std::string& tableIdentifier, const std::string& columnName, const std::string& table2Identifier, const std::string& column2Name);

         //
         /// \brief              append "ON columnName = column2Name"
         /// \param              columnName         the column name
         /// \param              column2Name        the column name
         /// \return             A reference to itself to allow method chaining
         //   
         CQuery& On(const std::string& columnName, const std::string& column2Name);


         //
         /// \brief              Vacuum the dataabse
         /// \return             A reference to itself to allow method chaining
         //  
         CQuery& Vacuum();

         //
         /// \brief    get the full query
         /// \return   the full query
         //     
         const std::string& str() const;

         //
         /// \brief    get the full query (C string)
         /// \return   the full query (C string)
         //     
         const char* c_str() const;


         //
         /// \brief    create a full custom query
         /// \param [in] customQuery the custom query
         /// \param [in] typeOfQuery the type of the query
         /// \return   the query object
         //     
         static CQuery CustomQuery(const std::string& customQuery, const EQueryType& typeOfQuery);


         //
         /// \brief              create a drop table query
         /// \param  tableName   the table name to delete
         /// \return             A reference to itself to allow method chaining
         //   
         CQuery& DropTable(const database::common::CDatabaseTable& tableName);

         //
         /// \brief              add a column to a table
         /// \param  tableName   the table name to alter
         /// \param  columnDefinition   the column definition
         /// \return             A reference to itself to allow method chaining
         //   
         CQuery& AddTableColumn(const database::common::CDatabaseTable& tableName, const std::string& columnDefinition);

         //
         /// \brief              get the query type
         /// \return             the type if this query
         //  
         const EQueryType& GetQueryType() const
         {
            return m_queryType;
         }

         //
         /// \brief  The type of query
         //
         static CQuery EmptyQuery;

      public:

         //================================================================================================
         //====== Formatters
         //================================================================================================
         virtual std::string formatStringToSql(const std::string& anyStringValue);
         virtual std::string formatInt8ToSql(const Poco::Int8& anyStringValue);
         virtual std::string formatUInt8ToSql(const Poco::UInt8& anyStringValue);
         virtual std::string formatInt16ToSql(const Poco::Int16& anyStringValue);
         virtual std::string formatUInt16ToSql(const Poco::UInt16& anyStringValue);
         virtual std::string formatInt32ToSql(const Poco::Int32& anyStringValue);
         virtual std::string formatUInt32ToSql(const Poco::UInt32& anyStringValue);
         virtual std::string formatInt64ToSql(const Poco::Int64& anyStringValue);
         virtual std::string formatUInt64ToSql(const Poco::UInt64& anyStringValue);
         virtual std::string formatFloatToSql(const float& anyStringValue);
         virtual std::string formatDoubleToSql(const double& anyStringValue);
         virtual std::string formatBooleanToSql(const bool& anyStringValue);
         virtual std::string formatDateToSql(const boost::posix_time::ptime& time);
         virtual std::string formatDateToSql(const Poco::DateTime& time);
         virtual std::string formatDateToSql(const Poco::Timestamp& time);
         virtual std::string formatEnumToSql(const shared::enumeration::IExtendedEnum& enumValue);
         virtual std::string formatSubQueryToSql(const CQuery& subQuery);
         virtual std::string functionMin(const std::string& sqlPart);
         virtual std::string functionMax(const std::string& sqlPart);
         virtual std::string functionAvg(const std::string& sqlPart);
         virtual std::string functionCoalesce(const std::string& sqlPart, const std::string& defaultValue);
         virtual std::string functionCast(const std::string& sqlPart, const std::string& destinationType);
         virtual std::string functionCastNumeric(const std::string& sqlPart);
         virtual std::string functionDateToIsoString(const std::string& sqlPart);
         virtual std::string functionConcatenate(const std::string& sqlPart1, const std::string& sqlPart2);
         virtual std::string functionAs(const std::string& sqlPart, const std::string& name);
         virtual std::string functionFromSubquery(const std::string& subQueryName, const std::string& subQueryFieldName);
         virtual std::string functionDistinct(const std::string& field);

         class CFunction
         {
         public:
            CFunction()
            {
            }

            explicit CFunction(const std::string& sql)
               : m_sql(sql)
            {
            }

            CFunction(const CFunction& cpy)
               : m_sql(cpy.m_sql)
            {
            }

            CFunction& operator=(const CFunction&) = delete;

            virtual ~CFunction()
            {
            }

            std::string toSql() const
            {
               return m_sql;
            }

         private:
            std::string m_sql;
         };

         //--------------------------------------------------------------
         ///\brief	generate min function ( ie: min(field0) )
         ///\param [in]	field    The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T1, class T2>
         inline const CFunction math(const T1& value, const std::string& op, const T2& value2);

         //--------------------------------------------------------------
         ///\brief	generate min function ( ie: min(field0) )
         ///\param [in]	field    The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T>
         inline const CFunction min(const T& value);

         //--------------------------------------------------------------
         ///\brief	generate min function ( ie: min(field0) ) with numeric cast
         ///\param [in]	field    The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T>
         inline const CFunction minWithCast(const T& fieldOrQuery);

         //--------------------------------------------------------------
         ///\brief	generate max function ( ie: max(field0) )
         ///\param [in]	field    The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T>
         inline const CFunction max(const T& fieldOrQuery);

         //--------------------------------------------------------------
         ///\brief	generate max function ( ie: max(field0) ) with numeric cast
         ///\param [in]	field    The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T>
         inline const CFunction maxWithCast(const T& fieldOrQuery);

         //--------------------------------------------------------------
         ///\brief	generate average function ( ie: average(field0) )
         ///\param [in]	field    The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T>
         inline const CFunction average(const T& fieldOrQuery);

         //--------------------------------------------------------------
         ///\brief	generate average function ( ie: average(field0) ) with numeric cast
         ///\param [in]	field    The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T>
         inline const CFunction averageWithCast(const T& fieldOrQuery);

         //--------------------------------------------------------------
         ///\brief	generate coalesce function ( ie: coalesce(field0, default) )
         ///\param [in]	field       The field or query
         ///\param [in]	valueIfNull The fallback value
         ///\return The query function
         //--------------------------------------------------------------
         template <class T, class T2>
         inline const CFunction coalesce(const T& field, const T2& valueIfNull);

         //--------------------------------------------------------------
         ///\brief	generate cast function ( ie: CAST (field0 AS numeric) )
         ///\param [in]	field       The field or query
         ///\param [in]	type        The typing cast
         ///\return The query function
         //--------------------------------------------------------------
         template <class T>
         inline const CFunction cast(const T& fieldOrQuery, const std::string& type);

         //--------------------------------------------------------------
         ///\brief	generate cast function ( ie: CAST (field0 AS numeric) )
         ///\param [in]	field       The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T>
         inline const CFunction castNumeric(const T& fieldOrQuery);

         //--------------------------------------------------------------
         ///\brief	generate date to iso string function 
         ///\param [in]	field       The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T>
         inline const CFunction dateToIsoString(const T& fieldOrQuery);


         //--------------------------------------------------------------
         ///\brief	generate column naming sql
         ///\param [in]	field       The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T>
         inline const CFunction as(const T& fieldOrQuery, const std::string& columnName);

         //--------------------------------------------------------------
         ///\brief	generate distinct sql
         ///\param [in]	fieldOrQuery       The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T>
         inline const CFunction distinct(const T& fieldOrQuery);


         //--------------------------------------------------------------
         ///\brief	generate column naming sql
         ///\param [in]	subQueryName       The subquery name
         ///\param [in]	subQueryFieldName  The subquery field name
         ///\return The query function
         //--------------------------------------------------------------
         CFunction fromSubquery(const std::string& subQueryName, const std::string& subQueryFieldName);

         //--------------------------------------------------------------
         ///\brief	generate column naming sql
         ///\param [in]	subQueryName     The subquery name
         ///\param [in]	column           The subquery column
         ///\return The query function
         //--------------------------------------------------------------
         CFunction fromSubquery(const std::string& subQueryName, const CDatabaseColumn& column);

         //--------------------------------------------------------------
         ///\brief	generate concatenation function 
         ///\param [in]	field       The field or query
         ///\return The query function
         //--------------------------------------------------------------
         template <class T1, class T2, class T3 = CNotUsedTemplateField, class T4 = CNotUsedTemplateField, class T5 = CNotUsedTemplateField, class T6 = CNotUsedTemplateField, class T7 = CNotUsedTemplateField, class T8 = CNotUsedTemplateField, class T9 = CNotUsedTemplateField, class T10 = CNotUsedTemplateField>
         const CFunction concatenate(const T1& field1, const T2& field2, const T3& field3 = T3(), const T4& field4 = T4(), const T5& field5 = T5(), const T6& field6 = T6(), const T7& field7 = T7(), const T8& field8 = T8(), const T9& field9 = T9(), const T10& field10 = T10());

      private:
         //
         /// \brief              Append a predicate clause
         /// \param  predicate   the predicate to use (can be WHERE, AND or OR)
         /// \param  condition   the  condition
         /// \return             A reference to itself to allow method chaining
         //    
         CQuery& PredicateInternal(const std::string& predicate, const std::string& condition);

         //
         /// \brief              Append a predicate clause
         /// \param  predicate   the predicate to use (can be WHERE, AND or OR)
         /// \param  field       the field name
         /// \param  op          the operator
         /// \param  value       the value
         /// \return             A reference to itself to allow method chaining
         //   
         CQuery& PredicateInternal(const std::string& predicate, const std::string& field, const std::string& op, const std::string& value);


         //
         /// \brief              Append a field to the current query stream
         /// \param  ss          the stream containing current query
         /// \param  field       the field name
         //
         static void AppendField(std::ostringstream& ss, const std::string& field);

         //
         /// \brief              Append a order field to the current query stream
         /// \param  ss          the stream containing current query
         /// \param  field       the field name
         /// \param  value       the way for order by (kAsc or kDesc)
         //
         static void AppendOrderField(std::ostringstream& ss, const std::string& field, const E_OrderWay way);

         //
         /// \brief              Append a set(field=value) to the current query stream
         /// \param  ss          the stream containing current query
         /// \param  field       the field name
         /// \param  value       the value
         //
         static void AppendSet(std::ostringstream& ss, const std::string& field, const std::string& value);

         //
         /// \brief              Append a value to the current query stream
         /// \param  ss          the stream containing current query
         /// \param  value       the value
         //
         static void AppendValue(std::ostringstream& ss, const std::string& value);


      protected:
         //
         /// \brief              Change the type of the query
         /// \param [in]         newType : the new type
         /// \param [in]         changeOnlyIfNeverSet : update the value only if it as never been set
         /// \return             A reference to itself to allow method chaining
         //
         CQuery& ChangeQueryType(const EQueryType newType, bool changeOnlyIfNeverSet = true);


         //
         /// \brief  The insert or update statement name (can be 'insert or replace', 'upsert',...)
         //
         const std::string m_insertOrUpdateName;

      private:
         //
         /// \brief  The query
         //
         std::string m_currentQuery;


         //
         /// \brief  The type of query
         //
         EQueryType m_queryType;

         //
         /// \brief  Tells if adding from clause is needed to use WITH clause result (pgsql:true, sqlite:false)
         //
         bool m_fromWithClauseNeeded;
      };

      //include template specializations
#include "QuerySpecializations.h"
   } //namespace common
} //namespace database 


