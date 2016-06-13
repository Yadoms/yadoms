#include "stdafx.h"

#include "PgsqlResultHandler.h"
#include <Poco/DateTimeParser.h>

#include <Poco/ByteOrder.h>


namespace database { namespace pgsql {

   CPgsqlResultHandler::CPgsqlResultHandler(PGresult *res)
      :m_res(res)
   {
      m_currentResultIndex = -1;
      m_currentResultRowCount = PQntuples(m_res);
   }

   CPgsqlResultHandler::~CPgsqlResultHandler()
   {

   }

   // database::common::IResultHandler implementation
   int CPgsqlResultHandler::getColumnCount()
   {
      return PQnfields(m_res);
   }

   std::string CPgsqlResultHandler::getColumnName(const int columnIndex)
   {
      return std::string(PQfname(m_res, columnIndex));
   }

   bool CPgsqlResultHandler::next_step()
   {
      m_currentResultIndex++;
      return m_currentResultIndex < m_currentResultRowCount;
   }

   std::string CPgsqlResultHandler::extractValueAsString(const int columnIndex)
   {
      return std::string(PQgetvalue(m_res, m_currentResultIndex, columnIndex));
   }

   int CPgsqlResultHandler::extractValueAsInt(const int columnIndex)
   {
      char * valuePoint = PQgetvalue(m_res, m_currentResultIndex, columnIndex);
      int result = 0;
      if (PQbinaryTuples(m_res) == 1)
      {
         int nbBytes = PQfsize(m_res, columnIndex);

         switch (nbBytes)
         {
         case 8:
            result = Poco::ByteOrder::fromNetwork(*((Poco::Int64 *)valuePoint));
            break;
         default:
         case 4:
            result = Poco::ByteOrder::fromNetwork(*((Poco::Int32 *)valuePoint));
            break;
         case 2:
            result = Poco::ByteOrder::fromNetwork(*((Poco::Int16 *)valuePoint));
            break;
         }
      }
      else
      {
         result = atoi(valuePoint);
      }
      return result;
   }

   float CPgsqlResultHandler::extractValueAsFloat(const int columnIndex)
   {
      char * valuePointer = PQgetvalue(m_res, m_currentResultIndex, columnIndex);
      union
      {
         float result;
         Poco::UInt32 raw;
      } conv;
      conv.raw = Poco::ByteOrder::fromNetwork(*((Poco::UInt32 *)valuePointer));
      return conv.result;
   }

   double CPgsqlResultHandler::extractValueAsDouble(const int columnIndex)
   {
      char * valuePointer = PQgetvalue(m_res, m_currentResultIndex, columnIndex);
      union
      {
         double result;
         Poco::UInt64 raw;
      } conv;
      conv.raw = Poco::ByteOrder::fromNetwork(*((Poco::UInt64 *)valuePointer));
      return conv.result;
   }

   unsigned char* CPgsqlResultHandler::extractValueAsBlob(const int columnIndex)
   {
      return (unsigned char*)PQgetvalue(m_res, m_currentResultIndex, columnIndex);
   }

   bool CPgsqlResultHandler::extractValueAsBool(const int columnIndex)
   {
      char * valuePoint = PQgetvalue(m_res, m_currentResultIndex, columnIndex);
      return valuePoint[0] == 't' || valuePoint[0] == '1';
   }

   bool CPgsqlResultHandler::isValueNull(const int columnIndex)
   {
      return PQgetisnull(m_res, m_currentResultIndex, columnIndex) == 1;
   }

   boost::posix_time::ptime CPgsqlResultHandler::extractValueAsBoostTime(const int columnIndex)
   {
      return boost::posix_time::from_iso_string(extractValueAsString(columnIndex));
   }

   Poco::DateTime CPgsqlResultHandler::extractValueAsPocoTime(const int columnIndex)
   {
      int timeZoneDifferential;
      return Poco::DateTimeParser::parse("%Y%m%dT%H%M%S", extractValueAsString(columnIndex), timeZoneDifferential);
   }

   shared::CDataContainer CPgsqlResultHandler::extractValueAsDataContainer(const int columnIndex)
   {
      return shared::CDataContainer(extractValueAsString(columnIndex));
   }


} //namespace sqlite
} //namespace database 

