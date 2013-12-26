#include "stdafx.h"
#include "Query.h"


CQuery::CQuery()
{
}

CQuery::~CQuery()
{
}

CQuery & CQuery::select()
{ 
   return append("SELECT * "); 
}

CQuery & CQuery::select(const std::string & field1, const std::string & field2/* = EMPTY_STR*/, const std::string & field3/* = EMPTY_STR*/, const std::string & field4/* = EMPTY_STR*/, const std::string & field5/* = EMPTY_STR*/, const std::string & field6/* = EMPTY_STR*/, const std::string & field7/* = EMPTY_STR*/, const std::string & field8/* = EMPTY_STR*/, const std::string & field9/* = EMPTY_STR*/, const std::string & field10/* = EMPTY_STR*/) 
{ 
   std::ostringstream ss;
   ss << "SELECT " << field1;
   appendField(ss, field2);
   appendField(ss, field3);
   appendField(ss, field4);
   appendField(ss, field5);
   appendField(ss, field6);
   appendField(ss, field7);
   appendField(ss, field8);
   appendField(ss, field9);
   appendField(ss, field10);     
   ss << " ";      
   return append(ss); 
}

//
/// \brief           Start a query with 'SELECT COUNT(*)'
/// \return          A reference to itself to allow method chaining
//
CQuery & CQuery::selectCount()
{
   return append("SELECT COUNT(*) "); 
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
CQuery & CQuery::selectCount(const std::string & field1, const std::string & field2 /*= EMPTY_STR*/, const std::string & field3 /*= EMPTY_STR*/, const std::string & field4 /*= EMPTY_STR*/, const std::string & field5 /*= EMPTY_STR*/, const std::string & field6 /*= EMPTY_STR*/, const std::string & field7 /*= EMPTY_STR*/, const std::string & field8 /*= EMPTY_STR*/, const std::string & field9 /*= EMPTY_STR*/, const std::string & field10 /*= EMPTY_STR*/)
{
   std::ostringstream ss;
   ss << "SELECT COUNT(" << field1;
   appendField(ss, field2);
   appendField(ss, field3);
   appendField(ss, field4);
   appendField(ss, field5);
   appendField(ss, field6);
   appendField(ss, field7);
   appendField(ss, field8);
   appendField(ss, field9);
   appendField(ss, field10);
   ss << ") ";      
   return append(ss); 
}


CQuery & CQuery::from(const std::string & table1, const std::string & table2/* = EMPTY_STR*/, const std::string & table3/* = EMPTY_STR*/, const std::string & table4/* = EMPTY_STR*/, const std::string & table5/* = EMPTY_STR*/, const std::string & table6/* = EMPTY_STR*/, const std::string & table7/* = EMPTY_STR*/, const std::string & table8/* = EMPTY_STR*/, const std::string & table9/* = EMPTY_STR*/, const std::string & table10/* = EMPTY_STR*/)  
{
   std::ostringstream ss;
   ss << " FROM " << table1;
   appendField(ss, table2);
   appendField(ss, table3);
   appendField(ss, table4);
   appendField(ss, table5);
   appendField(ss, table6);
   appendField(ss, table7);
   appendField(ss, table8);
   appendField(ss, table9);
   appendField(ss, table10);   
   ss << " ";      
   return append(ss); 
}

CQuery & CQuery::where(const std::string & condition) 
{
   return whereInternal("WHERE ", condition);
} 

CQuery & CQuery::where(const std::string & field, const std::string & op, const CQueryValue & value) 
{
   return whereInternal("WHERE ", field, op, value);
}   

CQuery & CQuery::where(const std::string & field, const std::string & op, CQuery & subQuery) 
{
   return whereInternal("WHERE ", field, op, subQuery);
}   

CQuery & CQuery::and(const std::string & condition) 
{
   return whereInternal("AND ", condition);
} 

CQuery & CQuery::and(const std::string & field, const std::string & op, const CQueryValue & value) 
{
   return whereInternal("AND ", field, op, value);
} 

CQuery & CQuery::and(const std::string & field, const std::string & op, CQuery & subQuery) 
{
   return whereInternal("AND ", field, op, subQuery);
}     

CQuery & CQuery::or(const std::string & condition) 
{
   return whereInternal("OR ", condition);
} 

CQuery & CQuery::or(const std::string & field, const std::string & op, const CQueryValue & value) 
{
   return whereInternal("OR ", field, op, value);
} 

CQuery & CQuery::or(const std::string & field, const std::string & op, CQuery & subQuery) 
{
   return whereInternal("OR ", field, op, subQuery);
}   


CQuery & CQuery::orderBy( const std::string & field1, const std::string & way1, 
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
   appendOrderField(ss, field2 , way2 );
   appendOrderField(ss, field3 , way3 );
   appendOrderField(ss, field4 , way4 );
   appendOrderField(ss, field5 , way5 );
   appendOrderField(ss, field6 , way6 );
   appendOrderField(ss, field7 , way7 );
   appendOrderField(ss, field8 , way8 );
   appendOrderField(ss, field9 , way9 );
   appendOrderField(ss, field10, way10);
   return append(ss); 
} 

CQuery & CQuery::groupBy(const std::string & field1, const std::string & field2/* = EMPTY_STR*/, const std::string & field3/* = EMPTY_STR*/, const std::string & field4/* = EMPTY_STR*/, const std::string & field5/* = EMPTY_STR*/, const std::string & field6/* = EMPTY_STR*/, const std::string & field7/* = EMPTY_STR*/, const std::string & field8/* = EMPTY_STR*/, const std::string & field9/* = EMPTY_STR*/, const std::string & field10/* = EMPTY_STR*/) 
{
   std::ostringstream ss;
   ss << "GROUP BY " << field1;
   appendField(ss, field2);
   appendField(ss, field3);
   appendField(ss, field4);
   appendField(ss, field5);
   appendField(ss, field6);
   appendField(ss, field7);
   appendField(ss, field8);
   appendField(ss, field9);
   appendField(ss, field10);      
   return append(ss); 
} 

CQuery & CQuery::insertInto(const std::string & table, const std::string & field1, const std::string & field2/* = EMPTY_STR*/, const std::string & field3/* = EMPTY_STR*/, const std::string & field4/* = EMPTY_STR*/, const std::string & field5/* = EMPTY_STR*/, const std::string & field6/* = EMPTY_STR*/, const std::string & field7/* = EMPTY_STR*/, const std::string & field8/* = EMPTY_STR*/, const std::string & field9/* = EMPTY_STR*/, const std::string & field10/* = EMPTY_STR*/) 
{
   std::ostringstream ss;
   ss << " INSERT INTO " << table << " (" << field1;
   appendField(ss, field2);
   appendField(ss, field3);
   appendField(ss, field4);
   appendField(ss, field5);
   appendField(ss, field6);
   appendField(ss, field7);
   appendField(ss, field8);
   appendField(ss, field9);
   appendField(ss, field10);
   ss << ") ";
   return append(ss); 
}


CQuery & CQuery::values(const CQueryValue & value1, const CQueryValue & value2, const CQueryValue & value3 , const CQueryValue & value4 , const CQueryValue & value5 , const CQueryValue & value6 , const CQueryValue & value7 , const CQueryValue & value8 , const CQueryValue & value9 , const CQueryValue & value10 ) 
{
   std::ostringstream ss;
   ss << " VALUES  (" << value1.str();
   appendValue(ss, value2);
   appendValue(ss, value3);
   appendValue(ss, value4);
   appendValue(ss, value5);
   appendValue(ss, value6);
   appendValue(ss, value7);
   appendValue(ss, value8);
   appendValue(ss, value9);
   appendValue(ss, value10);
   ss << ") ";
   return append(ss); 
}

CQuery & CQuery::update(const std::string & table) 
{
   std::ostringstream ss;
   ss << " UPDATE  " << table << " ";
   return append(ss); 
}

CQuery & CQuery::set(const std::string & field1, const CQueryValue & value1, 
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
   appendSet(ss, field2 , value2 );
   appendSet(ss, field3 , value3 );
   appendSet(ss, field4 , value4 );
   appendSet(ss, field5 , value5 );
   appendSet(ss, field6 , value6 );
   appendSet(ss, field7 , value7 );
   appendSet(ss, field8 , value8 );
   appendSet(ss, field9 , value9 );
   appendSet(ss, field10, value10);      
   ss << " ";
   return append(ss); 
}

CQuery & CQuery::deleteFrom(const std::string & table) 
{
   std::ostringstream ss;
   ss << " DELETE FROM  " << table << " ";
   return append(ss); 
}


CQuery & CQuery::append(const std::ostringstream & queryPart)
{
   m_currentQuery += queryPart.str();
   return *this;
}

CQuery & CQuery::append(const std::string & queryPart)
{
   m_currentQuery += queryPart;
   return *this;
}

CQuery & CQuery::append(CQuery & queryPart)
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

CQuery & CQuery::whereInternal(const std::string & predicate, const std::string & condition) 
{
   std::ostringstream ss;
   ss << predicate << condition << " ";
   return append(ss); 
} 

CQuery & CQuery::whereInternal(const std::string & predicate, const std::string & field, const std::string & op, const CQueryValue & value) 
{
   std::ostringstream ss;
   ss << predicate << field << " " << op << " " << value.str() << " ";
   return append(ss); 
} 

CQuery & CQuery::whereInternal(const std::string & predicate, const std::string & field, const std::string & op, CQuery & subQuery) 
{
   std::ostringstream ss;
   ss << predicate << field << " " << op << " (" << subQuery.str() << ") ";
   return append(ss); 
}   

void CQuery::appendField(std::ostringstream & ss, const std::string & field)
{
   if(field.size()>0) 
      ss << "," << field;
}

void CQuery::appendOrderField(std::ostringstream & ss, const std::string & field, const std::string & way)
{
   if(field.size()>0) 
   { 
      ss << "," << field; 
      if(way.size()>=0) 
         ss << " " << way; 
   }
}

void CQuery::appendSet(std::ostringstream & ss, const std::string & field, const CQueryValue & value)
{
   if(field.size()>0 && value.isDefined()) 
      ss << "," << field << "=" << value.str();
}

void CQuery::appendValue(std::ostringstream & ss, const CQueryValue & value)
{
   if(value.isDefined()) 
      ss << "," << value.str();
}