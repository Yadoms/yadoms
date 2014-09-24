#pragma once

#include "server/database/IAcquisitionRequester.h"


namespace database { namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   Acquisition requester for SQLite database
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
         virtual void saveSummaryData(const database::entities::EAcquisitionSummaryType type, const int keywordId, boost::posix_time::ptime & dataTime, double mean, double min, double max);
         virtual void removeKeywordData(const int keywordId);
         virtual boost::shared_ptr< database::entities::CAcquisition > getAcquisitionById(const int acqId);   
         virtual boost::shared_ptr< database::entities::CAcquisition > getAcquisitionByKeywordAndDate(const int keywordId, boost::posix_time::ptime time);
         virtual boost::shared_ptr< database::entities::CAcquisition > getKeywordLastData(const int keywordId);
         virtual std::vector< boost::tuple<boost::posix_time::ptime, std::string>  > getKeywordData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo);
         virtual std::string getKeywordHighchartData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo);
         virtual std::string getKeywordHighchartDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo);
         virtual std::string getKeywordHighchartDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo);
         // [END] IAcquisitionRequester implementation

      private:
         //--------------------------------------------------------------
         /// \brief                 Get the summary data (highchart js format) : [[date,value],[date,value],...] by hour (one value per hour)
         /// \param [in] type       Type of data to select
         /// \param [in] keywordId  keywordId Id
         /// \param [in] timeFrom   The start date (optionnal)
         /// \param [in] timeTo     The end date (optionnal)
         /// \return                Map of data : (date, value). One value per hour
         /// \throw                 CInvalidParameter if deviceId is unknown
         //--------------------------------------------------------------
         std::string getKeywordHighchartDataByDay(const database::entities::EAcquisitionSummaryType & type, int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo);


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

