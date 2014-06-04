#include "stdafx.h"
#include "Keyword.h"
#include <shared/exception/NotImplemented.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


namespace database { namespace sqlite { namespace requesters { 

   CKeyword::CKeyword(CSQLiteDataProvider * databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
      BOOST_ASSERT(m_databaseHandler != NULL);
   }

   CKeyword::~CKeyword()
   {
   }

   // IKeywordRequester implementation
   void CKeyword::addKeyword(boost::shared_ptr<database::entities::CKeyword> newKeyword)
   {
      CQuery qSelect;

      qSelect. SelectCount().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword->DeviceId()).
         And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword->Name());

      int count = m_databaseRequester->queryCount(qSelect);
      if(count == 0)
      {
         //create the database entry with needed fields
         CQuery qInsert;
         qInsert.InsertInto(CKeywordTable::getTableName(), CKeywordTable::getDeviceIdColumnName(), CKeywordTable::getCapacityNameColumnName(), CKeywordTable::getCapacityAccessModeColumnName(), CKeywordTable::getNameColumnName()).
            Values(newKeyword->DeviceId(), newKeyword->CapacityName(), newKeyword->CapacityAccessMode(), newKeyword->Name());

         if(m_databaseRequester->queryStatement(qInsert) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert keyword into table");

         //update fields
         if(newKeyword->FriendlyName.isDefined())
         {
            CQuery update;
            update.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getFriendlyNameColumnName(), newKeyword->FriendlyName()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword->DeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword->Name());

            if(m_databaseRequester->queryStatement(update) <= 0)
               throw shared::exception::CEmptyResult("Fail to update Type field");
         }

         if(newKeyword->Units.isDefined())
         {
            CQuery update;
            update.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getUnitsColumnName(), newKeyword->Units()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword->DeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword->Name());

            if(m_databaseRequester->queryStatement(update) <= 0)
               throw shared::exception::CEmptyResult("Fail to update Unit field");
         }

         if(newKeyword->Minimum.isDefined())
         {
            CQuery update;
            update.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getMinimumColumnName(), newKeyword->Minimum()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword->DeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword->Name());

            if(m_databaseRequester->queryStatement(update) <= 0)
               throw shared::exception::CEmptyResult("Fail to update Minimum field");
         }

         if(newKeyword->Maximum.isDefined())
         {
            CQuery update;
            update.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getMaximumColumnName(), newKeyword->Maximum()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword->DeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword->Name());

            if(m_databaseRequester->queryStatement(update) <= 0)
               throw shared::exception::CEmptyResult("Fail to update Maximum field");
         }    
      }
      else
      {
         //already exist, do nothing
      }
   }

   boost::shared_ptr<database::entities::CKeyword> CKeyword::getKeyword(const int deviceId, const std::string & keyword)
   {
      database::sqlite::adapters::CKeywordAdapter adapter;

      CQuery qSelect;

      qSelect. Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
         And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, keyword);

      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CKeyword> >(&adapter, qSelect);
      if (adapter.getResults().empty())
         throw shared::exception::CException((boost::format("Keyword name %1% for device %2% not found in database") % keyword % deviceId).str());

      return adapter.getResults().at(0);
   }

   boost::shared_ptr<entities::CKeyword> CKeyword::getKeyword(const int keywordId)
   {
      database::sqlite::adapters::CKeywordAdapter adapter;

      CQuery qSelect;
      qSelect. Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CKeyword> >(&adapter, qSelect);
      if (adapter.getResults().empty())
         throw shared::exception::CException((boost::format("Keyword id %1% not found in database") % keywordId).str());

      return adapter.getResults().at(0);
   }

   std::vector<boost::shared_ptr<database::entities::CKeyword> > CKeyword::getKeywords(const int deviceId)
   {
      database::sqlite::adapters::CKeywordAdapter adapter;
      CQuery qSelect;
      qSelect. Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId);
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CKeyword> >(&adapter, qSelect);
      return adapter.getResults();
   }


   std::vector<boost::shared_ptr<database::entities::CKeyword> > CKeyword::getDeviceKeywordsWithCapacity(const int deviceId, const std::string & capacityName, const database::entities::ECapacityAccessMode capacityAccessMode)
   {
      database::sqlite::adapters::CKeywordAdapter adapter;
      CQuery qSelect;
      qSelect. Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
         And(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, capacityName).
         And(CKeywordTable::getCapacityAccessModeColumnName(), CQUERY_OP_EQUAL, (int)capacityAccessMode);

      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CKeyword> >(&adapter, qSelect);
      return adapter.getResults();
   }

   void CKeyword::removeKeyword(const int deviceId, const std::string & keyword)
   {
      boost::shared_ptr<database::entities::CKeyword> keywordToDelete = getKeyword(deviceId, keyword);
      if(keywordToDelete)
         removeKeyword(keywordToDelete->Id());
      else
         throw shared::exception::CEmptyResult("Can not find keyword");
   }

   void CKeyword::removeKeyword(const int keywordId)
   {
      //delete acquisition data
      m_databaseHandler->getAcquisitionRequester()->removeKeywordData(keywordId);

      //delete keyword
      CQuery qDelete;
      qDelete. DeleteFrom(CKeywordTable::getTableName()).
         Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CKeyword::updateKeywordFriendlyName(const int keywordId, const std::string & newFriendlyName)
   {
      //get a good name
      if(newFriendlyName != shared::CStringExtension::EmptyString)
      {
         boost::shared_ptr<database::entities::CKeyword> keywordToUpdate = getKeyword(keywordId);
         if(keywordToUpdate)
         {
            //insert in db
            CQuery qUpdate;
            qUpdate. Update(CKeywordTable::getTableName()).
               Set(CKeywordTable::getFriendlyNameColumnName(), newFriendlyName).
               Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if(m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update keyword friendlyName");
         }
         else
         {
            throw shared::exception::CEmptyResult("Can not find keyword"); 
         }
      }
   }

   void CKeyword::updateKeywordFriendlyName(const int deviceId, const std::string & keyword, const std::string & newFriendlyName)
   {
      boost::shared_ptr<database::entities::CKeyword> keywordToUpdate = getKeyword(deviceId, keyword);
      if(keywordToUpdate)
         updateKeywordFriendlyName(keywordToUpdate->Id(), keyword, newFriendlyName);
      else
         throw shared::exception::CEmptyResult("Can not find keyword");
   }
   // [END] IKeywordRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 

