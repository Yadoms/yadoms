#include "stdafx.h"
#include "Query.h"


CQuery::CQuery()
{
}

CQuery::~CQuery()
{
}

CQuery & CQuery::Select()
{ 
   return Append("SELECT * "); 
}

CQuery & CQuery::Select(const std::string & field1, const std::string & field2/* = EMPTY_STR*/, const std::string & field3/* = EMPTY_STR*/, const std::string & field4/* = EMPTY_STR*/, const std::string & field5/* = EMPTY_STR*/, const std::string & field6/* = EMPTY_STR*/, const std::string & field7/* = EMPTY_STR*/, const std::string & field8/* = EMPTY_STR*/, const std::string & field9/* = EMPTY_STR*/, const std::string & field10/* = EMPTY_STR*/) 
{ 
   std::ostringstream ss;
   ss << "SELECT " << field1;
   AppendField(ss, field2);
   AppendField(ss, field3);
   AppendField(ss, field4);
   AppendField(ss, field5);
   AppendField(ss, field6);
   AppendField(ss, field7);
   AppendField(ss, field8);
   AppendField(ss, field9);
   AppendField(ss, field10);     
   ss << " ";      
   return Append(ss); 
}

//
/// \brief           Start a query with 'SELECT COUNT(*)'
/// \return          A reference to itself to allow method chaining
//
CQuery & CQuery::SelectCount()
{
   return Append("SELECT COUNT(*) "); 
}

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
CQuery & CQuery::SelectCount(const std::string & field1, const std::string & field2 /*= EMPTY_STR*/, const std::string & field3 /*= EMPTY_STR*/, const std::string & field4 /*= EMPTY_STR*/, const std::string & field5 /*= EMPTY_STR*/, const std::string & field6 /*= EMPTY_STR*/, const std::string & field7 /*= EMPTY_STR*/, const std::string & field8 /*= EMPTY_STR*/, const std::string & field9 /*= EMPTY_STR*/, const std::string & field10 /*= EMPTY_STR*/)
{
   std::ostringstream ss;
   ss << "SELECT COUNT(" << field1;
   AppendField(ss, field2);
   AppendField(ss, field3);
   AppendField(ss, field4);
   AppendField(ss, field5);
   AppendField(ss, field6);
   AppendField(ss, field7);
   AppendField(ss, field8);
   AppendField(ss, field9);
   AppendField(ss, field10);
   ss << ") ";      
   return Append(ss); 
}


CQuery & CQuery::From(const std::string & table1, const std::string & table2/* = EMPTY_STR*/, const std::string & table3/* = EMPTY_STR*/, const std::string & table4/* = EMPTY_STR*/, const std::string & table5/* = EMPTY_STR*/, const std::string & table6/* = EMPTY_STR*/, const std::string & table7/* = EMPTY_STR*/, const std::string & table8/* = EMPTY_STR*/, const std::string & table9/* = EMPTY_STR*/, const std::string & table10/* = EMPTY_STR*/)  
{
   std::ostringstream ss;
   ss << " FROM " << table1;
   AppendField(ss, table2);
   AppendField(ss, table3);
   AppendField(ss, table4);
   AppendField(ss, table5);
   AppendField(ss, table6);
   AppendField(ss, table7);
   AppendField(ss, table8);
   AppendField(ss, table9);
   AppendField(ss, table10);   
   ss << " ";      
   return Append(ss); 
}

CQuery & CQuery::Where(const std::string & condition) 
{
   return WhereInternal("WHERE ", condition);
} 

CQuery & CQuery::Where(const std::string & field, const std::string & op, const CQueryValue & value) 
{
   return WhereInternal("WHERE ", field, op, value);
}   

CQuery & CQuery::Where(const std::string & field, const std::string & op, CQuery & subQuery) 
{
   return WhereInternal("WHERE ", field, op, subQuery);
}   

CQuery & CQuery::And(const std::string & condition) 
{
   return WhereInternal("AND ", condition);
} 

CQuery & CQuery::And(const std::string & field, const std::string & op, const CQueryValue & value) 
{
   return WhereInternal("AND ", field, op, value);
} 

CQuery & CQuery::And(const std::string & field, const std::string & op, CQuery & subQuery) 
{
   return WhereInternal("AND ", field, op, subQuery);
}     

CQuery & CQuery::Or(const std::string & condition) 
{
   return WhereInternal("OR ", condition);
} 

CQuery & CQuery::Or(const std::string & field, const std::string & op, const CQueryValue & value) 
{
   return WhereInternal("OR ", field, op, value);
} 

CQuery & CQuery::Or(const std::string & field, const std::string & op, CQuery & subQuery) 
{
   return WhereInternal("OR ", field, op, subQuery);
}   


CQuery & CQuery::OrderBy( const std::string & field1, const std::string & way1, 
                         const std::string & field2 /* = EMPTY_STR*/, const std::string &  way2 /* = EMPTY_STR*/,
                         const std::string & field3 /* = EMPTY_STR*/, const std::string &  way3 /* = EMPTY_STR*/,
                         const std::string & field4 /* = EMPTY_STR*/, const std::string &  way4 /* = EMPTY_STR*/,
                         const std::string & field5 /* = EMPTY_STR*/, const std::string &  way5 /* = EMPTY_STR*/,
                         const std::string & field6 /* = EMPTY_STR*/, const std::string &  way6 /* = EMPTY_STR*/,
                         const std::string & field7 /* = EMPTY_STR*/, const std::string &  way7 /* = EMPTY_STR*/,
                         const std::string & field8 /* = EMPTY_STR*/, const std::string &  way8 /* = EMPTY_STR*/,
                         const std::string & field9 /* = EMPTY_STR*/, const std::string &  way9 /* = EMPTY_STR*/,
                         const std::string & field10/* = EMPTY_STR*/, const std::string & way10/* = EMPTY_STR*/)
{
   std::ostringstream ss;
   ss << "ORDER BY " << field1;
   if(way1.size()>=0) 
      ss << " " << way1;
   AppendOrderField(ss, field2 , way2 );
   AppendOrderField(ss, field3 , way3 );
   AppendOrderField(ss, field4 , way4 );
   AppendOrderField(ss, field5 , way5 );
   AppendOrderField(ss, field6 , way6 );
   AppendOrderField(ss, field7 , way7 );
   AppendOrderField(ss, field8 , way8 );
   AppendOrderField(ss, field9 , way9 );
   AppendOrderField(ss, field10, way10);
   return Append(ss); 
} 

CQuery & CQuery::GroupBy(const std::string & field1, const std::string & field2/* = EMPTY_STR*/, const std::string & field3/* = EMPTY_STR*/, const std::string & field4/* = EMPTY_STR*/, const std::string & field5/* = EMPTY_STR*/, const std::string & field6/* = EMPTY_STR*/, const std::string & field7/* = EMPTY_STR*/, const std::string & field8/* = EMPTY_STR*/, const std::string & field9/* = EMPTY_STR*/, const std::string & field10/* = EMPTY_STR*/) 
{
   std::ostringstream ss;
   ss << "GROUP BY " << field1;
   AppendField(ss, field2);
   AppendField(ss, field3);
   AppendField(ss, field4);
   AppendField(ss, field5);
   AppendField(ss, field6);
   AppendField(ss, field7);
   AppendField(ss, field8);
   AppendField(ss, field9);
   AppendField(ss, field10);      
   return Append(ss); 
} 

CQuery & CQuery::InsertInto(const std::string & table, const std::string & field1, const std::string & field2/* = EMPTY_STR*/, const std::string & field3/* = EMPTY_STR*/, const std::string & field4/* = EMPTY_STR*/, const std::string & field5/* = EMPTY_STR*/, const std::string & field6/* = EMPTY_STR*/, const std::string & field7/* = EMPTY_STR*/, const std::string & field8/* = EMPTY_STR*/, const std::string & field9/* = EMPTY_STR*/, const std::string & field10/* = EMPTY_STR*/) 
{
   std::ostringstream ss;
   ss << " INSERT INTO " << table << " (" << field1;
   AppendField(ss, field2);
   AppendField(ss, field3);
   AppendField(ss, field4);
   AppendField(ss, field5);
   AppendField(ss, field6);
   AppendField(ss, field7);
   AppendField(ss, field8);
   AppendField(ss, field9);
   AppendField(ss, field10);
   ss << ") ";
   return Append(ss); 
}


CQuery & CQuery::Values(const CQueryValue & value1, const CQueryValue & value2, const CQueryValue & value3 , const CQueryValue & value4 , const CQueryValue & value5 , const CQueryValue & value6 , const CQueryValue & value7 , const CQueryValue & value8 , const CQueryValue & value9 , const CQueryValue & value10 ) 
{
   std::ostringstream ss;
   ss << " VALUES  (" << value1.str();
   AppendValue(ss, value2);
   AppendValue(ss, value3);
   AppendValue(ss, value4);
   AppendValue(ss, value5);
   AppendValue(ss, value6);
   AppendValue(ss, value7);
   AppendValue(ss, value8);
   AppendValue(ss, value9);
   AppendValue(ss, value10);
   ss << ") ";
   return Append(ss); 
}

CQuery & CQuery::Update(const std::string & table) 
{
   std::ostringstream ss;
   ss << " UPDATE  " << table << " ";
   return Append(ss); 
}

CQuery & CQuery::Set(const std::string & field1, const CQueryValue & value1, 
                     const std::string & field2 /* = EMPTY_STR*/, const CQueryValue &  value2 /* = CQueryValue()*/,
                     const std::string & field3 /* = EMPTY_STR*/, const CQueryValue &  value3 /* = CQueryValue()*/,
                     const std::string & field4 /* = EMPTY_STR*/, const CQueryValue &  value4 /* = CQueryValue()*/,
                     const std::string & field5 /* = EMPTY_STR*/, const CQueryValue &  value5 /* = CQueryValue()*/,
                     const std::string & field6 /* = EMPTY_STR*/, const CQueryValue &  value6 /* = CQueryValue()*/,
                     const std::string & field7 /* = EMPTY_STR*/, const CQueryValue &  value7 /* = CQueryValue()*/,
                     const std::string & field8 /* = EMPTY_STR*/, const CQueryValue &  value8 /* = CQueryValue()*/,
                     const std::string & field9 /* = EMPTY_STR*/, const CQueryValue &  value9 /* = CQueryValue()*/,
                     const std::string & field10/* = EMPTY_STR*/, const CQueryValue &  value10/* = CQueryValue()*/)
{
   std::ostringstream ss;
   ss << " SET  " << field1 << " = " << value1.str();
   AppendSet(ss, field2 , value2 );
   AppendSet(ss, field3 , value3 );
   AppendSet(ss, field4 , value4 );
   AppendSet(ss, field5 , value5 );
   AppendSet(ss, field6 , value6 );
   AppendSet(ss, field7 , value7 );
   AppendSet(ss, field8 , value8 );
   AppendSet(ss, field9 , value9 );
   AppendSet(ss, field10, value10);      
   ss << " ";
   return Append(ss); 
}

CQuery & CQuery::DeleteFrom(const std::string & table) 
{
   std::ostringstream ss;
   ss << " DELETE FROM  " << table << " ";
   return Append(ss); 
}


CQuery & CQuery::Append(const std::ostringstream & queryPart)
{
   m_currentQuery += queryPart.str();
   return *this;
}

CQuery & CQuery::Append(const std::string & queryPart)
{
   m_currentQuery += queryPart;
   return *this;
}

CQuery & CQuery::Append(CQuery & queryPart)
{
   m_currentQuery += queryPart.str();
   return *this;
}




const std::string & CQuery::str() const
{
   return m_currentQuery;
}

const char * CQuery::c_str() const
{
   return m_currentQuery.c_str();
}

CQuery & CQuery::WhereInternal(const std::string & predicate, const std::string & condition) 
{
   std::ostringstream ss;
   ss << predicate << condition << " ";
   return Append(ss); 
} 

CQuery & CQuery::WhereInternal(const std::string & predicate, const std::string & field, const std::string & op, const CQueryValue & value) 
{
   std::ostringstream ss;
   ss << predicate << field << " " << op << " " << value.str() << " ";
   return Append(ss); 
} 

CQuery & CQuery::WhereInternal(const std::string & predicate, const std::string & field, const std::string & op, CQuery & subQuery) 
{
   std::ostringstream ss;
   ss << predicate << field << " " << op << " (" << subQuery.str() << ") ";
   return Append(ss); 
}   

void CQuery::AppendField(std::ostringstream & ss, const std::string & field)
{
   if(field.size()>0) 
      ss << "," << field;
}

void CQuery::AppendOrderField(std::ostringstream & ss, const std::string & field, const std::string & way)
{
   if(field.size()>0) 
   { 
      ss << "," << field; 
      if(way.size()>=0) 
         ss << " " << way; 
   }
}

void CQuery::AppendSet(std::ostringstream & ss, const std::string & field, const CQueryValue & value)
{
   if(field.size()>0 && value.IsDefined()) 
      ss << "," << field << "=" << value.str();
}

void CQuery::AppendValue(std::ostringstream & ss, const CQueryValue & value)
{
   if(value.IsDefined()) 
      ss << "," << value.str();
}