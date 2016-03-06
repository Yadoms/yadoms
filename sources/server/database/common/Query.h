#pragma once

#include "QueryValue.h"


namespace database { namespace common {

#define EMPTY_STR ""

#define CQUERY_OP_EQUAL "="
#define CQUERY_OP_LIKE " LIKE "
#define CQUERY_DISTINCT(_x) "DISTINCT(" +  _x  + ")"
#define CQUERY_OP_IN " IN "
#define CQUERY_OP_SUP ">"
#define CQUERY_OP_INF "<"
#define CQUERY_OP_NOT_EQUAL "<>"
#define CQUERY_OP_SUP_EQUAL ">="
#define CQUERY_OP_INF_EQUAL "<="

#define CQUERY_ORDER_ASC ""
#define CQUERY_ORDER_DESC "DESC"


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
         kVacuum
      };
   public:
      //
      /// \brief           Constructor
      //
      CQuery();

      //
      /// \brief           Destructor
      //
      virtual ~CQuery();

      //
      /// \brief              Clear current query
      /// \return             A reference to itself to allow method chaining
      //  
      CQuery & Clear();

      //
      /// \brief           Start a query with 'SELECT *'
      /// \return          A reference to itself to allow method chaining
      //
      CQuery & Select();

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
      CQuery & Select(const std::string & field1, const std::string & field2 = EMPTY_STR, const std::string & field3 = EMPTY_STR, const std::string & field4 = EMPTY_STR, const std::string & field5 = EMPTY_STR, const std::string & field6 = EMPTY_STR, const std::string & field7 = EMPTY_STR, const std::string & field8 = EMPTY_STR, const std::string & field9 = EMPTY_STR, const std::string & field10 = EMPTY_STR);


      //
      /// \brief           Start a query with 'SELECT COUNT(*)'
      /// \return          A reference to itself to allow method chaining
      //
      CQuery & SelectCount();

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
      CQuery & SelectCount(const std::string & field1, const std::string & field2 = EMPTY_STR, const std::string & field3 = EMPTY_STR, const std::string & field4 = EMPTY_STR, const std::string & field5 = EMPTY_STR, const std::string & field6 = EMPTY_STR, const std::string & field7 = EMPTY_STR, const std::string & field8 = EMPTY_STR, const std::string & field9 = EMPTY_STR, const std::string & field10 = EMPTY_STR);

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
      CQuery & From(const std::string & table1, const std::string & table2 = EMPTY_STR, const std::string & table3 = EMPTY_STR, const std::string & table4 = EMPTY_STR, const std::string & table5 = EMPTY_STR, const std::string & table6 = EMPTY_STR, const std::string & table7 = EMPTY_STR, const std::string & table8 = EMPTY_STR, const std::string & table9 = EMPTY_STR, const std::string & table10 = EMPTY_STR) ;

		//
      /// \brief           Append 'From subquery'
      /// \param  subquery   the subquery
      /// \return          A reference to itself to allow method chaining
      //   
		CQuery & From(const CQuery & subquery);

		//
      /// \brief           Append 'From (subquery)'
      /// \param  subquery   the subquery
      /// \return          A reference to itself to allow method chaining
      //   
		CQuery & FromParenthesis(const CQuery & subquery);

      //
      /// \brief           Append the where clause
      /// \param  condition the where condition
      /// \return          A reference to itself to allow method chaining
      //     
      CQuery & Where(const std::string & condition);

      //
      /// \brief           Append the where clause
      ///                  Where("id", "=", "12")
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the value
      /// \return          A reference to itself to allow method chaining
      //     
      CQuery & Where(const std::string & field, const std::string & op, const CQueryValue & value);

      //
      /// \brief           Append the where clause with a subquery
      ///                  CQuery q2 = ... ; //SELECT id FROM table WHERE...
      ///                  Where("id", "in", q2)
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the subquery
      /// \return          A reference to itself to allow method chaining
      //    
      CQuery & Where(const std::string & field, const std::string & op, CQuery & subQuery);     
		
		//
      /// \brief           Append the where clause with a start parenthesis
      ///                  Where("id", "=", "12")
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the value
      /// \return          A reference to itself to allow method chaining
      //     
		CQuery & WhereParenthesis(const std::string & field, const std::string & op, const CQueryValue & value);

      //
      /// \brief           Append the where clause with a subquery  with a start parenthesis
      ///                  CQuery q2 = ... ; //SELECT id FROM table WHERE...
      ///                  Where("id", "in", q2)
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the subquery
      /// \return          A reference to itself to allow method chaining
      //    
		CQuery & WhereParenthesis(const std::string & field, const std::string & op, CQuery & subQuery);

      //
      /// \brief           Append the AND clause
      /// \param  condition the condition
      /// \return          A reference to itself to allow method chaining
      //        
      CQuery & And(const std::string & condition) ;

      //
      /// \brief           Append the And clause
      ///                  And("id", "=", "12")
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the value
      /// \return          A reference to itself to allow method chaining
      //     
      CQuery & And(const std::string & field, const std::string & op, const CQueryValue & value);

      //
      /// \brief           Append the AND clause with a subquery
      ///                  CQuery q2 = ... ; //SELECT id FROM table WHERE...
      ///                  AND("id", "in", q2)
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the subquery
      /// \return          A reference to itself to allow method chaining
      //    
      CQuery & And(const std::string & field, const std::string & op, CQuery & subQuery); 
		
		//
      /// \brief           Append the And clause with a start parenthesis
      ///                  And("id", "=", "12")
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the value
      /// \return          A reference to itself to allow method chaining
      //     
		CQuery & AndParenthesis(const std::string & field, const std::string & op, const CQueryValue & value);

      //
      /// \brief           Append the AND clause with a subquery with a start parenthesis
      ///                  CQuery q2 = ... ; //SELECT id FROM table WHERE...
      ///                  AND("id", "in", q2)
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the subquery
      /// \return          A reference to itself to allow method chaining
      //    
		CQuery & AndParenthesis(const std::string & field, const std::string & op, CQuery & subQuery);

      //
      /// \brief           Append the OR clause
      /// \param  condition the condition
      /// \return          A reference to itself to allow method chaining
      //          
      CQuery & Or(const std::string & condition) ;

      //
      /// \brief           Append the Or clause
      ///                  Or("id", "=", "12")
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the value
      /// \return          A reference to itself to allow method chaining
      //        
      CQuery & Or(const std::string & field, const std::string & op, const CQueryValue & value);

      //
      /// \brief           Append the OR clause with a subquery
      ///                  CQuery q2 = ... ; //SELECT id FROM table WHERE...
      ///                  OR("id", "in", q2)
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the subquery
      /// \return          A reference to itself to allow method chaining
      //       
      CQuery & Or(const std::string & field, const std::string & op, CQuery & subQuery);  
		
		//
      /// \brief           Append the Or clause  with a start parenthesis
      ///                  Or("id", "=", "12")
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the value
      /// \return          A reference to itself to allow method chaining
      //        
		CQuery & OrParenthesis(const std::string & field, const std::string & op, const CQueryValue & value);

      //
      /// \brief           Append the OR clause with a subquery  with a start parenthesis
      ///                  CQuery q2 = ... ; //SELECT id FROM table WHERE...
      ///                  OR("id", "in", q2)
      /// \param  field    the field name
      /// \param  op       the operator
      /// \param  value    the subquery
      /// \return          A reference to itself to allow method chaining
      //       
		CQuery & OrParenthesis(const std::string & field, const std::string & op, CQuery & subQuery);

		//
		/// \brief           Append  a closing parenthesis
		/// \return          A reference to itself to allow method chaining
		//       
		CQuery & EndParenthesis();

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
      CQuery & OrderBy( const std::string & field1, const std::string & way1 = EMPTY_STR, 
         const std::string & field2  = EMPTY_STR, const std::string &  way2  =EMPTY_STR,
         const std::string & field3  = EMPTY_STR, const std::string &  way3  =EMPTY_STR,
         const std::string & field4  = EMPTY_STR, const std::string &  way4  =EMPTY_STR,
         const std::string & field5  = EMPTY_STR, const std::string &  way5  =EMPTY_STR,
         const std::string & field6  = EMPTY_STR, const std::string &  way6  =EMPTY_STR,
         const std::string & field7  = EMPTY_STR, const std::string &  way7  =EMPTY_STR,
         const std::string & field8  = EMPTY_STR, const std::string &  way8  =EMPTY_STR,
         const std::string & field9  = EMPTY_STR, const std::string &  way9  =EMPTY_STR,
         const std::string & field10 = EMPTY_STR, const std::string & way10 =EMPTY_STR);

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
      CQuery & GroupBy(const std::string & field1, const std::string & field2 = EMPTY_STR, const std::string & field3 = EMPTY_STR, const std::string & field4 = EMPTY_STR, const std::string & field5 = EMPTY_STR, const std::string & field6 = EMPTY_STR, const std::string & field7 = EMPTY_STR, const std::string & field8 = EMPTY_STR, const std::string & field9 = EMPTY_STR, const std::string & field10 = EMPTY_STR);

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
      CQuery & InsertInto(const std::string & table, const std::string & field1, const std::string & field2 = EMPTY_STR, const std::string & field3 = EMPTY_STR, const std::string & field4 = EMPTY_STR, const std::string & field5 = EMPTY_STR, const std::string & field6 = EMPTY_STR, const std::string & field7 = EMPTY_STR, const std::string & field8 = EMPTY_STR, const std::string & field9 = EMPTY_STR, const std::string & field10 = EMPTY_STR);


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
      CQuery & Values(const CQueryValue & value1, const CQueryValue & value2 = CQueryValue(), const CQueryValue & value3 = CQueryValue(), const CQueryValue & value4 = CQueryValue(), const CQueryValue & value5 = CQueryValue(), const CQueryValue & value6 = CQueryValue(), const CQueryValue & value7 = CQueryValue(), const CQueryValue & value8 = CQueryValue(), const CQueryValue & value9 = CQueryValue(), const CQueryValue & value10 = CQueryValue()) ;




      //
      /// \brief           Append the UPDATE clause
      /// \param  table    the table name
      /// \return          A reference to itself to allow method chaining
      //        
      CQuery & Update(const std::string & table);

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
      CQuery & Set(const std::string & field1, const CQueryValue & value1, 
         const std::string & field2  = EMPTY_STR, const CQueryValue &  value2  =CQueryValue(),
         const std::string & field3  = EMPTY_STR, const CQueryValue &  value3  =CQueryValue(),
         const std::string & field4  = EMPTY_STR, const CQueryValue &  value4  =CQueryValue(),
         const std::string & field5  = EMPTY_STR, const CQueryValue &  value5  =CQueryValue(),
         const std::string & field6  = EMPTY_STR, const CQueryValue &  value6  =CQueryValue(),
         const std::string & field7  = EMPTY_STR, const CQueryValue &  value7  =CQueryValue(),
         const std::string & field8  = EMPTY_STR, const CQueryValue &  value8  =CQueryValue(),
         const std::string & field9  = EMPTY_STR, const CQueryValue &  value9  =CQueryValue(),
         const std::string & field10 = EMPTY_STR, const CQueryValue &  value10 =CQueryValue());


      //
      /// \brief           Append the WITH clause with a subquery 
      ///                  //WITH tableName AS (subQuery)
      /// \param  tableName   The table name
      /// \param  subQuery    the subquery
      /// \return          A reference to itself to allow method chaining
      //    
      CQuery & With( const std::string & tableName1, const CQuery & subQuery1,
                     const std::string & tableName2  = EMPTY_STR, const CQuery & subQuery2 = CQuery::EmptyQuery,
                     const std::string & tableName3 = EMPTY_STR, const CQuery & subQuery3 = CQuery::EmptyQuery,
                     const std::string & tableName4 = EMPTY_STR, const CQuery & subQuery4 = CQuery::EmptyQuery,
                     const std::string & tableName5 = EMPTY_STR, const CQuery & subQuery5 = CQuery::EmptyQuery,
                     const std::string & tableName6 = EMPTY_STR, const CQuery & subQuery6 = CQuery::EmptyQuery,
                     const std::string & tableName7 = EMPTY_STR, const CQuery & subQuery7 = CQuery::EmptyQuery,
                     const std::string & tableName8 = EMPTY_STR, const CQuery & subQuery8 = CQuery::EmptyQuery,
                     const std::string & tableName9 = EMPTY_STR, const CQuery & subQuery9 = CQuery::EmptyQuery,
                     const std::string & tableName10 = EMPTY_STR, const CQuery & subQuery10 = CQuery::EmptyQuery);


      //
      /// \brief           create the LIMIT clause
      /// \param  numberOfRowsToReturn    the maximum number of rows to return
      /// \return          A reference to itself to allow method chaining
      //     
      CQuery & Limit(const int numberOfRowsToReturn);

      //
      /// \brief           create the LIMIT clause with OFFSET
      /// \param  numberOfRowsToReturn    the maximum number of rows to return
      /// \param  offset    the offset (number of rows to skip)
      /// \return          A reference to itself to allow method chaining
      //     
      CQuery & Limit(const int numberOfRowsToReturn, const int offset);


      //
      /// \brief           create the DELETE clause
      /// \param  table    the table name
      /// \return          A reference to itself to allow method chaining
      //     
      CQuery & DeleteFrom(const std::string & table);

      //
      /// \brief              append text to query
      /// \param  queryPart   the query part to append
      /// \return             A reference to itself to allow method chaining
      //   
      CQuery & Append(const std::ostringstream & queryPart);

      //
      /// \brief              append text to query
      /// \param  queryPart   the query part to append
      /// \return             A reference to itself to allow method chaining
      //   
      CQuery & Append(const std::string & queryPart);

      //
      /// \brief              append text to query
      /// \param  queryPart   another query
      /// \return             A reference to itself to allow method chaining
      //   
      CQuery & Append(CQuery & queryPart);

      //
      /// \brief              append union
      /// \return             A reference to itself to allow method chaining
      //   
      CQuery & Union();

      //
      /// \brief              append union all
      /// \return             A reference to itself to allow method chaining
      //   
      CQuery & UnionAll();

      //
      /// \brief                    append "LEFT JOIN"
      /// \param  tableName         the table
      /// \param  tableIdentifier   the table identifier (ex: "A" -> LEFT JOIN myTable A )
      /// \return             A reference to itself to allow method chaining
      //   
      CQuery & JoinLeft(const std::string & tableName, const std::string & tableIdentifier = EMPTY_STR);

      //
      /// \brief              append "ON tableIdentifier.columnName = table2Identifier.column2Name"
      /// \param              tableIdentifier    the table identifier
      /// \param              columnName         the column name
      /// \param              table2Identifier   the table identifier
      /// \param              column2Name        the column name
      /// \return             A reference to itself to allow method chaining
      //   
      CQuery & On(const std::string & tableIdentifier, const std::string & columnName, const std::string & table2Identifier, const std::string & column2Name);

      //
      /// \brief              append "ON columnName = column2Name"
      /// \param              columnName         the column name
      /// \param              column2Name        the column name
      /// \return             A reference to itself to allow method chaining
      //   
      CQuery & On(const std::string & columnName, const std::string & column2Name);


      //
      /// \brief              Vacuum the dataabse
      /// \return             A reference to itself to allow method chaining
      //  
      CQuery & Vacuum();

      //
      /// \brief    get the full query
      /// \return   the full query
      //     
      const std::string & str() const;

      //
      /// \brief    get the full query (C string)
      /// \return   the full query (C string)
      //     
      const char * c_str() const;


      //
      /// \brief    create a full custom query
      /// \param [in] customQuery the custom query
      /// \param [in] typeOfQuery the type of the query
      /// \return   the query object
      //     
      static CQuery CustomQuery(const std::string & customQuery, const EQueryType & typeOfQuery);


      //
      /// \brief              create a drop table query
      /// \param  tableName   the table name to delete
      /// \return             A reference to itself to allow method chaining
      //   
      CQuery & DropTable(const std::string & tableName);

      //
      /// \brief              get the query type
      /// \return             the type if this query
      //  
      const EQueryType & GetQueryType() const { return m_queryType; }

      //
      /// \brief  The type of query
      //
      static CQuery EmptyQuery;

   private:
      //
      /// \brief              Append the where clause
      /// \param  predicate   the predicate to use (can be WHERE, AND or OR)
      /// \param  condition   the  condition
      /// \return             A reference to itself to allow method chaining
      //    
      CQuery & WhereInternal(const std::string & predicate, const std::string & condition);

      //
      /// \brief              Append the where clause
      /// \param  predicate   the predicate to use (can be WHERE, AND or OR)
      /// \param  field       the field name
      /// \param  op          the operator
      /// \param  value       the value
      /// \return             A reference to itself to allow method chaining
      //   
      CQuery & WhereInternal(const std::string & predicate, const std::string & field, const std::string & op, const CQueryValue & value);

      //
      /// \brief              Append the where clause with a subquery
      /// \param  predicate   the predicate to use (can be WHERE, AND or OR)
      /// \param  field       the field name
      /// \param  op          the operator
      /// \param  value       the subquery
      /// \return             A reference to itself to allow method chaining
      //     
      CQuery & WhereInternal(const std::string & predicate, const std::string & field, const std::string & op, CQuery & subQuery);


      //
      /// \brief              Append a field to the current query stream
      /// \param  ss          the stream containing current query
      /// \param  field       the field name
      //
      void AppendField(std::ostringstream & ss, const std::string & field);

      //
      /// \brief              Append a order field to the current query stream
      /// \param  ss          the stream containing current query
      /// \param  field       the field name
      /// \param  value       the way for order by (ASC or DESC)
      //
      void AppendOrderField(std::ostringstream & ss, const std::string & field, const std::string & way);

      //
      /// \brief              Append a set(field=value) to the current query stream
      /// \param  ss          the stream containing current query
      /// \param  field       the field name
      /// \param  value       the value
      //
      void AppendSet(std::ostringstream & ss, const std::string & field, const CQueryValue & value);

      //
      /// \brief              Append a value to the current query stream
      /// \param  ss          the stream containing current query
      /// \param  value       the value
      //
      void AppendValue(std::ostringstream & ss, const CQueryValue & value);

   protected:
      //
      /// \brief              Change the type of the query
      /// \param [in]         newType : the new type
      /// \param [in]         changeOnlyIfNeverSet : update the value only if it as never been set
      /// \return             A reference to itself to allow method chaining
      //
      CQuery & ChangeQueryType(const EQueryType newType, bool changeOnlyIfNeverSet = true);


   private:
      //
      /// \brief  The query
      //
      std::string m_currentQuery;


      //
      /// \brief  The type of query
      //
      EQueryType m_queryType;

   };


} //namespace common
} //namespace database 

