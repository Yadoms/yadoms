#pragma once

#include "server/database/IAcquisitionRequester.h"


namespace database { namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   XplMessage requester for SQLite database
      //--------------------------------------------------------------
      class CAcquisition: public IAcquisitionRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         CAcquisition(CSQLiteDataProvider * databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CAcquisition();

         // IAcquisitionRequester implementation
         virtual void saveData(const int keywordId, const std::string & data);
         virtual void saveData(const int keywordId, const std::string & data, boost::posix_time::ptime & dataTime);
         virtual void removeKeywordData(const int keywordId);
         virtual boost::shared_ptr< database::entities::CAcquisition > getAcquisitionById(const int acqId);   
         virtual boost::shared_ptr< database::entities::CAcquisition > getKeywordLastData(const int keywordId);   
         virtual std::vector< boost::tuple<boost::posix_time::ptime, std::string>  > getKeywordData(int keywordId,  boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo);   
         // [END] IAcquisitionRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Pointer to SQLiteDatabseHandler
         //--------------------------------------------------------------
         CSQLiteDataProvider * m_databaseHandler;

         //--------------------------------------------------------------
         /// \Brief		   Reference to SQLiteRequester
         //--------------------------------------------------------------
         boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
      };

   } //namespace requesters
} //namespace sqlite
} //namespace database 

