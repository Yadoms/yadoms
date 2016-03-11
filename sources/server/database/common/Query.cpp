#include "stdafx.h"
#include "Query.h"
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>


namespace database { namespace common { 

      CQuery CQuery::EmptyQuery;

      CQuery::CQuery()
         :m_queryType(kNotYetDefined)
      {
      }

      CQuery::~CQuery()
      {
      }

      CQuery::CQuery(const CQuery &toCopy)
         :m_currentQuery(toCopy.m_currentQuery), m_queryType(toCopy.m_queryType)
      {

      }

      CQuery &  CQuery::Clear()
      {
         m_queryType = kNotYetDefined;
         m_currentQuery = "";
         return *this;
      }

      CQuery & CQuery::Select()
      { 
         ChangeQueryType(kSelect);
         return Append("SELECT * "); 
      }


      //
      /// \brief           Start a query with 'SELECT COUNT(*)'
      /// \return          A reference to itself to allow method chaining
      //
      CQuery & CQuery::SelectCount()
      {
         ChangeQueryType(kSelect);
         return Append("SELECT COUNT(*) "); 
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

		//
		/// \brief           Append 'From subquery'
		/// \param  subquery   the subquery
		/// \return          A reference to itself to allow method chaining
		//   
		CQuery & CQuery::From(const CQuery & subquery)
		{
			std::ostringstream ss;
			ss << " FROM " << subquery.str();
			ss << " ";
			return Append(ss);
		}

		//
		/// \brief           Append 'From (subquery)'
		/// \param  subquery   the subquery
		/// \return          A reference to itself to allow method chaining
		//   
		CQuery & CQuery::FromParenthesis(const CQuery & subquery)
		{
			std::ostringstream ss;
			ss << " FROM (" << subquery.str() << ")";
			ss << " ";
			return Append(ss);
		}

      CQuery & CQuery::Where(const std::string & condition) 
      {
         return PredicateInternal("WHERE ", condition);
      } 

      CQuery & CQuery::And(const std::string & condition) 
      {
         return PredicateInternal("AND ", condition);
      } 

      CQuery & CQuery::Or(const std::string & condition) 
      {
         return PredicateInternal("OR ", condition);
      } 

		CQuery & CQuery::EndParenthesis()
		{
			std::ostringstream ss;
			ss << " ) ";
			return Append(ss);
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
         ss << " ORDER BY " << field1;
         if(way1.size()>0) 
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
         ss << " GROUP BY " << field1;
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
         ChangeQueryType(kInsert);
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

      CQuery & CQuery::Update(const std::string & table) 
      {
         ChangeQueryType(kUpdate);
         std::ostringstream ss;
         ss << " UPDATE  " << table << " ";
         return Append(ss); 
      }

      CQuery & CQuery::With(const std::string & tableName1, const CQuery & subQuery1,
         const std::string & tableName2, const CQuery & subQuery2,
         const std::string & tableName3, const CQuery & subQuery3,
         const std::string & tableName4, const CQuery & subQuery4,
         const std::string & tableName5, const CQuery & subQuery5,
         const std::string & tableName6, const CQuery & subQuery6,
         const std::string & tableName7, const CQuery & subQuery7,
         const std::string & tableName8, const CQuery & subQuery8,
         const std::string & tableName9, const CQuery & subQuery9,
         const std::string & tableName10, const CQuery & subQuery10)
      {
         std::ostringstream ss;
         ss << " WITH " << tableName1 << " AS ( " << subQuery1.str() << " ) ";
         if(!tableName2.empty())
            ss << ", " << tableName2 << " AS ( " << subQuery2.str() << " ) ";
         if(!tableName3.empty())
            ss << ", " << tableName3 << " AS ( " << subQuery3.str() << " ) ";
         if(!tableName4.empty())
            ss << ", " << tableName4 << " AS ( " << subQuery4.str() << " ) ";
         if(!tableName5.empty())
            ss << ", " << tableName5 << " AS ( " << subQuery5.str() << " ) ";
         if(!tableName6.empty())
            ss << ", " << tableName6 << " AS ( " << subQuery6.str() << " ) ";
         if(!tableName7.empty())
            ss << ", " << tableName7 << " AS ( " << subQuery7.str() << " ) ";
         if(!tableName8.empty())
            ss << ", " << tableName8 << " AS ( " << subQuery8.str() << " ) ";
         if(!tableName9.empty())
            ss << ", " << tableName9 << " AS ( " << subQuery9.str() << " ) ";
         if(!tableName10.empty())
            ss << ", " << tableName10 << " AS ( " << subQuery10.str() << " ) ";
         return Append(ss);
      }


      CQuery & CQuery::Limit(const int numberOfRowsToReturn)
      {
         std::ostringstream ss;
         ss << " LIMIT " << numberOfRowsToReturn;
         ss << " ";
         return Append(ss); 
      }

      CQuery & CQuery::Limit(const int numberOfRowsToReturn, const int offset)
      {
         std::ostringstream ss;
         ss << " LIMIT " << numberOfRowsToReturn << " OFFSET " << offset;
         ss << " ";
         return Append(ss); 
      }

      CQuery & CQuery::DeleteFrom(const std::string & table) 
      {
         ChangeQueryType(kDelete);
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


		CQuery & CQuery::Union()
		{
			std::ostringstream ss;
			ss << " UNION " ;
			return Append(ss);
		}

  
		CQuery & CQuery::UnionAll()
		{
			std::ostringstream ss;
			ss << " UNION ALL ";
			return Append(ss);

		}


      CQuery & CQuery::JoinLeft(const std::string & tableName, const std::string & tableIdentifier)
      {
         std::ostringstream ss;
         ss << " LEFT JOIN " << tableName << " " << tableIdentifier << " ";
         return Append(ss);
      }

      CQuery & CQuery::On(const std::string & tableIdentifier, const std::string & columnName, const std::string & table2Identifier, const std::string & column2Name)
      {
         std::ostringstream ss;
         ss << " ON " << tableIdentifier << "." << columnName << " = " << table2Identifier << "." << column2Name << " ";
         return Append(ss);
      }


      CQuery & CQuery::On(const std::string & columnName, const std::string & column2Name)
      {
         std::ostringstream ss;
         ss << " ON " << columnName << " = " << column2Name << " ";
         return Append(ss);
      }


      CQuery & CQuery::Vacuum()
      {
         ChangeQueryType(kVacuum);
         m_currentQuery = "VACUUM;";
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

      CQuery CQuery::CustomQuery(const std::string & customQuery, const EQueryType & typeOfQuery)
      {
         BOOST_ASSERT(typeOfQuery != kNotYetDefined);
         return CQuery().Append(customQuery).ChangeQueryType(typeOfQuery);
      }

      CQuery & CQuery::DropTable(const std::string & tableName)
      {
         ChangeQueryType(kDrop);
         std::ostringstream ss;
         ss << "DROP TABLE " << tableName;
         return Append(ss);
      }


      CQuery & CQuery::PredicateInternal(const std::string & predicate, const std::string & condition) 
      {
         std::ostringstream ss;
         ss << predicate << condition << " ";
         return Append(ss); 
      } 

      CQuery & CQuery::PredicateInternal(const std::string & predicate, const std::string & field, const std::string & op, const std::string & value)
      {
         std::ostringstream ss;
         ss << predicate << field << " " << op << " " << value << " ";
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
            if(way.size()>0)
               ss << " " << way; 
         }
      }

      void CQuery::AppendSet(std::ostringstream & ss, const std::string & field, const std::string & value)
      {
         if(field.size()>0) 
            ss << "," << field << "=" << value;
      }

      void CQuery::AppendValue(std::ostringstream & ss, const std::string & value)
      {
         if(!value.empty()) 
            ss << "," << value;
      }


      CQuery & CQuery::ChangeQueryType(const EQueryType newType, bool changeOnlyIfNeverSet /*= true*/)
      {
         if(newType != kNotYetDefined)
         {
            if( (changeOnlyIfNeverSet && m_queryType == kNotYetDefined) || !changeOnlyIfNeverSet)
               m_queryType = newType;
         }
         return *this;
      }


      const std::string CQuery::formatStringToSql(const std::string & anystringValue)
      {
         return (boost::format("'%1%'") % boost::replace_all_copy(anystringValue, "'", "\\'")).str();
      }

      const std::string CQuery::formatDateToSql(const boost::posix_time::ptime & ptime)
      {
         return boost::posix_time::to_iso_string(ptime);
      }

      const std::string CQuery::formatDateToSql(const Poco::DateTime & datetime)
      {
         return Poco::DateTimeFormatter::format(datetime, "%Y%m%dT%H%M%S");
      }

      const std::string CQuery::formatDateToSql(const Poco::Timestamp & timestamp)
      {
         return Poco::DateTimeFormatter::format(timestamp, "%Y%m%dT%H%M%S");
      }

      const std::string CQuery::formatEnumToSql(const shared::enumeration::IExtendedEnum & enumValue)
      {
         return enumValue.toString();
      }

      const std::string CQuery::formatSubQueryToSql(const CQuery & subQuery)
      {
         return (boost::format("(%1%)") % subQuery.str()).str();
      }


      const std::string CQuery::formatInt8ToSql(const Poco::Int8 & anyStringValue)
      {
         return boost::lexical_cast<std::string>(anyStringValue);
      }

      const std::string CQuery::formatUInt8ToSql(const Poco::UInt8 & anyStringValue)
      {
         return boost::lexical_cast<std::string>(anyStringValue);
      }

      const std::string CQuery::formatInt16ToSql(const Poco::Int16 & anyStringValue)
      {
         return boost::lexical_cast<std::string>(anyStringValue);
      }

      const std::string CQuery::formatUInt16ToSql(const Poco::UInt16 & anyStringValue)
      {
         return boost::lexical_cast<std::string>(anyStringValue);
      }

      const std::string CQuery::formatInt32ToSql(const Poco::Int32 & anyStringValue)
      {
         return boost::lexical_cast<std::string>(anyStringValue);
      }

      const std::string CQuery::formatUInt32ToSql(const Poco::UInt32 & anyStringValue)
      {
         return boost::lexical_cast<std::string>(anyStringValue);
      }

      const std::string CQuery::formatInt64ToSql(const Poco::Int64 & anyStringValue)
      {
         return boost::lexical_cast<std::string>(anyStringValue);
      }

      const std::string CQuery::formatUInt64ToSql(const Poco::UInt64 & anyStringValue)
      {
         return boost::lexical_cast<std::string>(anyStringValue);
      }



      const std::string CQuery::formatFloatToSql(const float & anyValue)
      {
         return boost::lexical_cast<std::string>(anyValue);
      }

      const std::string CQuery::formatDoubleToSql(const double & anyValue)
      {
         return boost::lexical_cast<std::string>(anyValue);
      }

      const std::string CQuery::formatBooleanToSql(const bool & anyValue)
      {
         return boost::lexical_cast<std::string>(anyValue);
      }

      const std::string CQuery::functionMin(const std::string & sqlPart)
      {
         return (boost::format("min(%1%)") % sqlPart).str();
      }

      const std::string CQuery::functionMax(const std::string & sqlPart)
      {
         return (boost::format("max(%1%)") % sqlPart).str();
      }

      const std::string CQuery::functionAvg(const std::string & sqlPart)
      {
         return (boost::format("avg(%1%)") % sqlPart).str();
      }

      const std::string CQuery::functionCoalesce(const std::string & sqlPart, const std::string & defaultValue)
      {
         return (boost::format("coalesce(%1%, %2%)") % sqlPart % defaultValue).str();
      }

      const std::string CQuery::functionCast(const std::string & sqlPart, const std::string & destinationType)
      {
         return (boost::format("cast(%1% AS %2%)") % sqlPart % destinationType).str();
      }
      const std::string CQuery::functionCastNumeric(const std::string & sqlPart)
      {
         return functionCast(sqlPart, "numeric");
      }

      const std::string CQuery::functionDateToIsoString(const std::string &sqlPart)
      {
         return sqlPart;
      }

      const std::string CQuery::functionConcatenate(const std::string &sqlPart1, const std::string &sqlPart2)
      {
         return (boost::format("%1% || %2%") % sqlPart1 % sqlPart2).str();
      }      

      const std::string CQuery::functionAs(const std::string & sqlPart, const std::string &name)
      {
         return (boost::format("%1% AS %2%") % sqlPart % name).str();
      }


      const std::string CQuery::functionFromSubquery(const std::string & subQueryName, const std::string &subQueryFieldName)
      {
         return (boost::format("%1%.%2%") % subQueryName % subQueryFieldName).str();
      }
      
      const CQuery::CFunction CQuery::fromSubquery(const std::string& subQueryName, const std::string & subQueryFieldName)
      {
         return CFunction(functionFromSubquery(subQueryName, subQueryFieldName));
      }

} //namespace common
} //namespace database 

