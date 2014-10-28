#pragma once

#include "entities/Entities.h"


namespace database { 

   class IAcquisitionRequester
   {
   public:

      //--------------------------------------------------------------
      /// \brief           Save a new data into base
      /// \param [in]      keywordId   The keyword id
      /// \param [in]      data        The data
      /// \param [in]      dataTime    The datetime of the data
      /// \return          The inserted acquisition
      //--------------------------------------------------------------
      virtual boost::shared_ptr< database::entities::CAcquisition > saveData(const int keywordId, const std::string & data, boost::posix_time::ptime & dataTime) = 0;

      //--------------------------------------------------------------
      /// \brief           Increment a data into base
      /// \param [in]      keywordId   The keyword id
      /// \param [in]      increment   The increment value
      /// \param [in]      dataTime    The datetime of the data
      /// \return          The inserted acquisition
      //--------------------------------------------------------------
      virtual boost::shared_ptr< database::entities::CAcquisition > incrementData(const int keywordId, const std::string & increment, boost::posix_time::ptime & dataTime) = 0;

      //--------------------------------------------------------------
      /// \brief     Container for summary data
      ///            first is summary on Day
      ///            second is summary on Hour
      //--------------------------------------------------------------
      typedef boost::tuple< boost::shared_ptr< database::entities::CAcquisitionSummary >, boost::shared_ptr< database::entities::CAcquisitionSummary > > LastSummaryData;

      //--------------------------------------------------------------
      /// \brief           Save a new summary data into base
      /// \param [in]      keywordId   The keyword id
      /// \param [in]      dataTime    The datetime of the data
      /// \return          The inserted acquisition summary (day, hour)
      //--------------------------------------------------------------
      virtual LastSummaryData saveSummaryData(const int keywordId, boost::posix_time::ptime & dataTime) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove all data associated to a keyword
      /// \param [in]      keywordId   The keyword id
      //--------------------------------------------------------------
      virtual void removeKeywordData(const int keywordId) = 0;

      

      //-----------------------------------------
      ///\brief      Get an acquisition by keywordid and date
      ///\param [in] keywordId  The keyword id
      ///\param [in] acqId  The acquisition date/time
      ///\return     the acquisition
      //-----------------------------------------
      virtual boost::shared_ptr< database::entities::CAcquisition > getAcquisitionByKeywordAndDate(const int keywordId, boost::posix_time::ptime time) = 0;
      
      //-----------------------------------------
      ///\brief      Get the last data of a keyword
      ///\param [in] keywordId  The keyword id
      ///\return     the last acquisition for the keyword
      ///\throw      CEmptyResult if no data is available
      //-----------------------------------------
      virtual boost::shared_ptr< database::entities::CAcquisition > getKeywordLastData(const int keywordId) = 0;   

      //--------------------------------------------------------------
      /// \brief                 Get the data
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optionnal)
      /// \param [in] timeTo     The end date (optionnal)
      /// \return                Map of data : (date, value)
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::vector< boost::tuple<boost::posix_time::ptime, std::string>  > getKeywordData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) = 0;
      
      //--------------------------------------------------------------
      /// \brief                 Get the data  by day (avg, min, max per day)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optionnal)
      /// \param [in] timeTo     The end date (optionnal)
      /// \return                Map of data : (date, value)
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > getKeywordDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) = 0;
      
      //--------------------------------------------------------------
      /// \brief                 Get the data  by hour (avg, min, max per hour)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optionnal)
      /// \param [in] timeTo     The end date (optionnal)
      /// \return                Map of data : (date, value)
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > getKeywordDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) = 0;
      
      //--------------------------------------------------------------
      /// \brief                 Get the data (highchart js format) : [[date,value],[date,value],...]
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optionnal)
      /// \param [in] timeTo     The end date (optionnal)
      /// \return                Map of data : (date, value)
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::string getKeywordHighchartData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) = 0;

      
      //--------------------------------------------------------------
      /// \brief                 Get the summary data (highchart js format) : [[date,value],[date,value],...] by day (one value per day)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optionnal)
      /// \param [in] timeTo     The end date (optionnal)
      /// \return                Map of data : (date, value). One value per day
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::string getKeywordHighchartDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the summary data (highchart js format) : [[date,value],[date,value],...] by hour (one value per hour)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optionnal)
      /// \param [in] timeTo     The end date (optionnal)
      /// \return                Map of data : (date, value). One value per hour
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::string getKeywordHighchartDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) = 0;


      
      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IAcquisitionRequester()
      {
      }
   };
 
} //namespace database 
