#include "stdafx.h"
#include "SQLiteDeviceRequester.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include <shared/Exceptions/EmptyResultException.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"

namespace server { 
   namespace database { 
      namespace sqlite { 
         namespace requesters { 

            CSQLiteDeviceRequester::CSQLiteDeviceRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
               :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
            {
            }

            CSQLiteDeviceRequester::~CSQLiteDeviceRequester()
            {
            }


            // IDeviceRequester implementation
            int CSQLiteDeviceRequester::addDevice(boost::shared_ptr<server::database::entities::CDevice> newDevice)
            {
               CQuery qInsert;
               qInsert. InsertInto(CDeviceTable::getTableName(), CDeviceTable::getDataSourceColumnName(), CDeviceTable::getNameColumnName(), CDeviceTable::getConfigurationColumnName()).
                  Values(newDevice->getDataSource(), newDevice->getName(), newDevice->getConfiguration());
               if(m_databaseRequester->queryStatement(qInsert) <= 0)
                  throw CEmptyResultException("No lines affected");

               CQuery qSelect;
               qSelect. Select(CDeviceTable::getIdColumnName()).
                  From(CDeviceTable::getTableName()).
                  Where(CDeviceTable::getDataSourceColumnName(), CQUERY_OP_EQUAL, newDevice->getDataSource()).
                  And(CDeviceTable::getNameColumnName(), CQUERY_OP_EQUAL, newDevice->getName()).
                  And(CDeviceTable::getConfigurationColumnName(), CQUERY_OP_EQUAL, newDevice->getConfiguration()).
                  OrderBy(CDeviceTable::getIdColumnName(), CQUERY_ORDER_DESC);

               server::database::sqlite::adapters::CSingleValueAdapter<int> adapter;
               m_databaseRequester->queryEntities<int>(&adapter, qSelect);
               if(adapter.getResults().size() >= 1)
                  return adapter.getResults()[0];
               else
                  throw CEmptyResultException("Cannot retrieve inserted Hardware");      
            }

            boost::shared_ptr<server::database::entities::CDevice> CSQLiteDeviceRequester::getDevice(int deviceId)
            {
               CQuery qSelect;
               qSelect. Select().
                  From(CDeviceTable::getTableName()).
                  Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

               server::database::sqlite::adapters::CDeviceAdapter adapter;
               m_databaseRequester->queryEntities<boost::shared_ptr<server::database::entities::CDevice> >(&adapter, qSelect);
               if(adapter.getResults().size() >= 1)
                  return adapter.getResults()[0];
               else
               {
                  std::string sEx = (boost::format("Cannot retrieve Device Id=%1% in database") % deviceId).str(); 
                  throw CEmptyResultException(sEx);
               }
            }

            std::vector<boost::shared_ptr<server::database::entities::CDevice> > CSQLiteDeviceRequester::getDevices()
            {
               CQuery qSelect;
               qSelect. Select().
                  From(CDeviceTable::getTableName());

               server::database::sqlite::adapters::CDeviceAdapter adapter;
               m_databaseRequester->queryEntities<boost::shared_ptr<server::database::entities::CDevice> >(&adapter, qSelect);
               return adapter.getResults();
            }

            void CSQLiteDeviceRequester::updateDeviceConfiguration(int deviceId, const std::string& newConfiguration)
            {
               CQuery qUpdate;
               qUpdate. Update(CHardwareTable::getTableName()).
                  Set(CDeviceTable::getConfigurationColumnName(), newConfiguration).
                  Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

               if(m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw CEmptyResultException("No lines affected");
            }

            void CSQLiteDeviceRequester::removeDevice(int deviceId)
            {
               CQuery qDelete;
               qDelete. DeleteFrom(CDeviceTable::getTableName()).
                  Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);
               if(m_databaseRequester->queryStatement(qDelete) <= 0)
                  throw  CEmptyResultException("No lines affected");
            }
            // [END] IDeviceRequester implementation

         } //namespace requesters
      } //namespace sqlite
   } //namespace database 
} //namespace server

