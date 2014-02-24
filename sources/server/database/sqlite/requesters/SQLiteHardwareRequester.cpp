#include "stdafx.h"
#include "SQLiteHardwareRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


   namespace database { 
      namespace sqlite { 
         namespace requesters { 

            CSQLiteHardwareRequester::CSQLiteHardwareRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
               :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
            {
            }

            CSQLiteHardwareRequester::~CSQLiteHardwareRequester()
            {
            }

            // IHardwareRequester implementation
            int CSQLiteHardwareRequester::addHardware(boost::shared_ptr<database::entities::CHardware> newHardware)
            {
               CQuery qInsert;

               qInsert.InsertInto(CHardwareTable::getTableName(), CHardwareTable::getNameColumnName(), CHardwareTable::getPluginNameColumnName(), CHardwareTable::getConfigurationColumnName(), CHardwareTable::getEnabledColumnName() ).
                  Values(newHardware->getName(), 
                  newHardware->getPluginName(),
                  newHardware->getConfiguration(), 
                  newHardware->getEnabled());

               if(m_databaseRequester->queryStatement(qInsert) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");


               CQuery qSelect;
               qSelect. Select(CHardwareTable::getIdColumnName()).
                  From(CHardwareTable::getTableName()).
                  Where(CHardwareTable::getNameColumnName(), CQUERY_OP_EQUAL, newHardware->getName()).
                  And(CHardwareTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, newHardware->getPluginName()).
                  OrderBy(CHardwareTable::getIdColumnName(), CQUERY_ORDER_DESC);

               database::sqlite::adapters::CSingleValueAdapter<int> adapter;
               m_databaseRequester->queryEntities<int>(&adapter, qSelect);
               if(adapter.getResults().size() >= 1)
                  return adapter.getResults()[0];
               else
                  throw shared::exception::CEmptyResult("Cannot retrieve inserted Hardware");
            }

            boost::shared_ptr<database::entities::CHardware> CSQLiteHardwareRequester::getHardware(int hardwareId)
            {
               database::sqlite::adapters::CHardwareAdapter adapter;

               CQuery qSelect;

               qSelect. Select().
                  From(CHardwareTable::getTableName()).
                  Where(CHardwareTable::getIdColumnName(), CQUERY_OP_EQUAL, hardwareId).
                  And(CHardwareTable::getDeletedColumnName(), CQUERY_OP_EQUAL, false);

               m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CHardware> >(&adapter, qSelect);
               if (adapter.getResults().empty())
               {
                  // Hardware not found
                  std::string sEx = (boost::format("Hardware Id %1% not found in database") % hardwareId).str(); 
                  throw shared::exception::CException(sEx);
               }
               return adapter.getResults().at(0);
            }

            std::vector<boost::shared_ptr<database::entities::CHardware> > CSQLiteHardwareRequester::getHardwares(bool evenDeleted)
            {
               database::sqlite::adapters::CHardwareAdapter adapter;

               CQuery qSelect;
               qSelect.Select().From(CHardwareTable::getTableName());

               if (!evenDeleted)
                  qSelect.Where(CHardwareTable::getDeletedColumnName(), CQUERY_OP_EQUAL, false);

               m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CHardware> >(&adapter, qSelect);
               return adapter.getResults();
            }

            //test
            std::vector<std::string> CSQLiteHardwareRequester::getHardwareNameList()
            {
               database::sqlite::adapters::CSingleValueAdapter<std::string> adapter;

               CQuery qSelect;
               qSelect. Select(CHardwareTable::getNameColumnName()).
                  From(CHardwareTable::getTableName()).
                  Where(CHardwareTable::getDeletedColumnName(), CQUERY_OP_EQUAL, false).
                  OrderBy(CHardwareTable::getNameColumnName());

               m_databaseRequester->queryEntities<std::string>(&adapter, qSelect);
               return adapter.getResults();
            }

            void CSQLiteHardwareRequester::updateHardwareConfiguration(int hardwareId, const std::string& newConfiguration)
            {
               CQuery qUpdate;
               qUpdate. Update(CHardwareTable::getTableName()).
                  Set(CHardwareTable::getConfigurationColumnName(), newConfiguration).
                  Where(CHardwareTable::getIdColumnName(), CQUERY_OP_EQUAL, hardwareId);

               if(m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");
            }

            void CSQLiteHardwareRequester::removeHardware(int hardwareId)
            {
               CQuery qUpdate;
               qUpdate. Update(CHardwareTable::getTableName()).
                  Set(CHardwareTable::getDeletedColumnName(), true).
                  Where(CHardwareTable::getIdColumnName(), CQUERY_OP_EQUAL, hardwareId);

               if(m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");
            }

            void CSQLiteHardwareRequester::enableInstance(int hardwareId, bool enable)
            {
               CQuery qUpdate;
               qUpdate. Update(CHardwareTable::getTableName()).
                  Set(CHardwareTable::getEnabledColumnName(), enable).
                  Where(CHardwareTable::getIdColumnName(), CQUERY_OP_EQUAL, hardwareId);

               if(m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");
            }

            void CSQLiteHardwareRequester::disableAllPluginInstance(const std::string& pluginName)
            {
               CQuery qUpdate;
               qUpdate. Update(CHardwareTable::getTableName()).
                  Set(CHardwareTable::getEnabledColumnName(), false).
                  Where(CHardwareTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName);

               m_databaseRequester->queryStatement(qUpdate);
            }

            // [END] IHardwareRequester implementation

         } //namespace requesters
      } //namespace sqlite
   } //namespace database 

