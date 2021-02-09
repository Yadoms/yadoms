#include "stdafx.h"
#include "SerialPort.h"
#include "database/common/DatabaseTables.h"
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/Query.h"
#include "shared/exception/EmptyResult.hpp"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         CSerialPort::CSerialPort(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         std::vector<boost::shared_ptr<entities::CSerialPort>> CSerialPort::getSerialPorts()
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CSerialPortTable::getTableName());

            adapters::CSerialPortAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CSerialPort>> CSerialPort::getSerialPortsFromLastKnownSerialPortPath(
            const std::string& lastKnownSerialPortPath)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CSerialPortTable::getTableName()).
                     Where(CSerialPortTable::getLastKnownSerialPortPathColumnName(), CQUERY_OP_EQUAL, lastKnownSerialPortPath);

            adapters::CSerialPortAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            return adapter.getResults();
         }

         void CSerialPort::addSerialPort(const entities::CSerialPort& serialPort)
         {
            if (serialPort.LastKnownSerialPortPath().empty() || serialPort.AdapterKind() == entities::ESerialPortAdapterKind::kUnknown)
               throw std::invalid_argument("Tried to add invalid serial port");

            auto qInsert = m_databaseRequester->newQuery();
            qInsert->InsertInto(CSerialPortTable::getTableName(),
                                CSerialPortTable::getLastKnownSerialPortPathColumnName(),
                                CSerialPortTable::getAdapterKindColumnName(),
                                CSerialPortTable::getAdapterParametersColumnName()).
                     Values(serialPort.LastKnownSerialPortPath(),
                            serialPort.AdapterKind(),
                            serialPort.AdapterParameters());
            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("Fail to insert serial port");
         }

         void CSerialPort::removeSerialPort(int id)
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CSerialPortTable::getTableName()).
                     Where(CSerialPortTable::getIdColumnName(), CQUERY_OP_EQUAL, id);

            if (m_databaseRequester->queryStatement(*qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CSerialPort::updateSerialPort(const std::string& lastKnownSerialPortPath,
                                            const entities::CSerialPort& serialPort)
         {
            if (serialPort.LastKnownSerialPortPath().empty() || serialPort.AdapterKind() == entities::ESerialPortAdapterKind::kUnknown)
               throw std::invalid_argument("Tried to update invalid serial port");

            auto query = m_databaseRequester->newQuery();
            query->InsertOrReplaceInto(CSerialPortTable::getTableName(),
                                       CSerialPortTable::getAdapterKindColumnName(),
                                       CSerialPortTable::getAdapterParametersColumnName()).
                   Select().
                   From(CSerialPortTable::getTableName()).
                   Where(CSerialPortTable::getLastKnownSerialPortPathColumnName(), CQUERY_OP_EQUAL, lastKnownSerialPortPath).
                   Values(serialPort.AdapterKind(),
                          serialPort.AdapterParameters());

            if (m_databaseRequester->queryStatement(*query) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }
      } //namespace requesters
   } //namespace common
} //namespace database 
