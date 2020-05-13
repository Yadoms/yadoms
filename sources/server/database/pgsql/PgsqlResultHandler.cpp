#include "stdafx.h"

#include "PgsqlResultHandler.h"
#include <Poco/DateTimeParser.h>

#include <Poco/ByteOrder.h>


namespace database
{
   namespace pgsql
   {
      CPgsqlResultHandler::CPgsqlResultHandler(boost::shared_ptr<CPgsqlLibrary> pgsqlLibrary, PGresult* res)
         : m_pgsqlLibrary(pgsqlLibrary),
           m_res(res)
      {
         m_currentResultIndex = -1;
         m_currentResultRowCount = m_pgsqlLibrary->PQntuples(m_res);
      }

      CPgsqlResultHandler::~CPgsqlResultHandler()
      {
      }

      // database::common::IResultHandler implementation
      int CPgsqlResultHandler::getColumnCount()
      {
         return m_pgsqlLibrary->PQnfields(m_res);
      }

      std::string CPgsqlResultHandler::getColumnName(const int columnIndex)
      {
         return std::string(m_pgsqlLibrary->PQfname(m_res, columnIndex));
      }

      bool CPgsqlResultHandler::next_step()
      {
         m_currentResultIndex++;
         return m_currentResultIndex < m_currentResultRowCount;
      }

      std::string CPgsqlResultHandler::extractValueAsString(const int columnIndex)
      {
         return std::string(m_pgsqlLibrary->PQgetvalue(m_res, m_currentResultIndex, columnIndex));
      }

      int CPgsqlResultHandler::extractValueAsInt(const int columnIndex)
      {
         char* valuePoint = m_pgsqlLibrary->PQgetvalue(m_res, m_currentResultIndex, columnIndex);
         if (m_pgsqlLibrary->PQbinaryTuples(m_res) == 1)
         {
            int nbBytes = m_pgsqlLibrary->PQfsize(m_res, columnIndex);

            switch (nbBytes)
            {
            case 8:
               return static_cast<int>(Poco::ByteOrder::fromNetwork(*reinterpret_cast<Poco::Int64 *>(valuePoint)));
            default:
            case 4:
               return Poco::ByteOrder::fromNetwork(*reinterpret_cast<Poco::Int32 *>(valuePoint));
            case 2:
               return Poco::ByteOrder::fromNetwork(*reinterpret_cast<Poco::Int16 *>(valuePoint));
            }
         }
         return atoi(valuePoint);
      }

      float CPgsqlResultHandler::extractValueAsFloat(const int columnIndex)
      {
         auto valuePointer = m_pgsqlLibrary->PQgetvalue(m_res, m_currentResultIndex, columnIndex);
         union
         {
            float result;
            Poco::UInt32 raw;
         } conv;
         conv.raw = Poco::ByteOrder::fromNetwork(*reinterpret_cast<Poco::UInt32 *>(valuePointer));
         return conv.result;
      }

      double CPgsqlResultHandler::extractValueAsDouble(const int columnIndex)
      {
         auto valuePointer = m_pgsqlLibrary->PQgetvalue(m_res, m_currentResultIndex, columnIndex);
         union
         {
            double result;
            Poco::UInt64 raw;
         } conv;
         conv.raw = Poco::ByteOrder::fromNetwork(*reinterpret_cast<Poco::UInt64 *>(valuePointer));
         return conv.result;
      }

      unsigned char* CPgsqlResultHandler::extractValueAsBlob(const int columnIndex)
      {
         return reinterpret_cast<unsigned char*>(m_pgsqlLibrary->PQgetvalue(m_res, m_currentResultIndex, columnIndex));
      }

      bool CPgsqlResultHandler::extractValueAsBool(const int columnIndex)
      {
         auto valuePoint = m_pgsqlLibrary->PQgetvalue(m_res, m_currentResultIndex, columnIndex);
         return valuePoint[0] == 't' || valuePoint[0] == '1';
      }

      bool CPgsqlResultHandler::isValueNull(const int columnIndex)
      {
         return m_pgsqlLibrary->PQgetisnull(m_res, m_currentResultIndex, columnIndex) == 1;
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

      boost::shared_ptr<shared::CDataContainer> CPgsqlResultHandler::extractValueAsDataContainer(const int columnIndex)
      {
         return shared::CDataContainer::make(extractValueAsString(columnIndex));
      }
   } //namespace sqlite
} //namespace database 


