#pragma once

#include "entities/Entities.h"

namespace database
{
   class IAcquisitionRequester
   {
   public:
      virtual ~IAcquisitionRequester() = default;

      //--------------------------------------------------------------
      /// \brief           Save a new data into base
      /// \param [in]      keywordId   The keyword id
      /// \param [in]      data        The data
      /// \param [in]      dataTime    The datetime of the data
      /// \return          The inserted acquisition
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CAcquisition> saveData(int keywordId,
                                                                 const std::string& data,
                                                                 boost::posix_time::ptime& dataTime) = 0;

      //--------------------------------------------------------------
      /// \brief           Increment a data into base
      /// \param [in]      keywordId   The keyword id
      /// \param [in]      increment   The increment value
      /// \param [in]      dataTime    The datetime of the data
      /// \return          The inserted acquisition
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CAcquisition> incrementData(int keywordId,
                                                                      const std::string& increment,
                                                                      boost::posix_time::ptime& dataTime) = 0;

      //--------------------------------------------------------------
      /// \brief     Container for summary data
      ///            first is summary on Day
      ///            second is summary on Hour
      //--------------------------------------------------------------
      typedef boost::tuple<boost::shared_ptr<entities::CAcquisitionSummary>,
                           boost::shared_ptr<entities::CAcquisitionSummary>> LastSummaryData;

      //--------------------------------------------------------------
      /// \brief           Save a new summary data into base
      /// \param [in]      keywordId   The keyword id
      /// \param [in]      curType     The type of summary data to save
      /// \param [in]      dataTime    The datetime of the data
      /// \return          The inserted acquisition summary
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CAcquisitionSummary> saveSummaryData(int keywordId,
                                                                               entities::EAcquisitionSummaryType
                                                                               curType,
                                                                               boost::posix_time::ptime& dataTime) = 0;

      //--------------------------------------------------------------
      /// \brief           Get the keywords id which have at least one acquisition between dates
      /// \param [in]      timeFrom    The time from (inclusive)
      /// \param [in]      timeTo      The time to (exclusive)
      /// \param [out]     results     The vector which will contains the keywords id
      //--------------------------------------------------------------
      virtual void getKeywordsHavingDate(const boost::posix_time::ptime& timeFrom,
                                         const boost::posix_time::ptime& timeTo,
                                         std::vector<int>& results) = 0;

      //--------------------------------------------------------------
      /// \brief           Check if a summary entry already exists
      /// \param [in]      keywordId   The keyword id
      /// \param [in]      curType     The type of summary data to check
      /// \param [in]      date        The datetime of the summary data
      //--------------------------------------------------------------
      virtual bool summaryDataExists(int keywordId,
                                     entities::EAcquisitionSummaryType curType,
                                     boost::posix_time::ptime& date) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove all data associated to a keyword
      /// \param [in]      keywordId   The keyword id
      //--------------------------------------------------------------
      virtual void removeKeywordData(int keywordId) = 0;

      //--------------------------------------------------------------
      /// \brief           Move all acquisitions and summary from one keyword to another
      /// \param [in]      fromKw      The source keyword id
      /// \param [in]      toKw        The target keyword id
      //--------------------------------------------------------------
      virtual void moveAllData(int fromKw,
                               int toKw) = 0;

      //-----------------------------------------
      ///\brief      Get an acquisition by keywordId and date
      ///\param [in] keywordId  The keyword id
      ///\param [in] time  The acquisition date/time
      ///\return     the acquisition
      //-----------------------------------------
      virtual boost::shared_ptr<entities::CAcquisition> getAcquisitionByKeywordAndDate(int keywordId,
                                                                                       boost::posix_time::ptime time) =
      0;

      //-----------------------------------------
      ///\brief      Export acquisitions for a keyword
      ///\param [in] keywordId  The keyword id
      ///\param [in] exportOneLineFunction : callback called for each acquisitions, used to write the exported file
      //-----------------------------------------
      virtual void exportAcquisitions(int keywordId,
                                      std::function<void(const boost::posix_time::ptime& date,
                                                         const std::string& value,
                                                         int nbTotalLines)> exportOneLineFunction) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the data
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optional)
      /// \param [in] timeTo     The end date (optional)
      /// \param [in] limit      Max count of records to return (optional, -1 if no limit)
      /// \return                Map of data : (date, value)
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::vector<boost::tuple<boost::posix_time::ptime, std::string>> getKeywordData(
         int keywordId,
         boost::posix_time::ptime timeFrom = boost::posix_time::not_a_date_time,
         boost::posix_time::ptime timeTo = boost::posix_time::not_a_date_time,
         int limit = -1) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the data  by hour (avg, min, max per hour)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optional)
      /// \param [in] timeTo     The end date (optional)
      /// \return                Map of data : (date, value)
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CAcquisitionSummary>> getKeywordDataByHour(
         int keywordId,
         boost::posix_time::ptime timeFrom,
         boost::posix_time::ptime timeTo) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the data  by day (avg, min, max per day)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optional)
      /// \param [in] timeTo     The end date (optional)
      /// \return                Map of data : (date, value)
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CAcquisitionSummary>> getKeywordDataByDay(
         int keywordId,
         boost::posix_time::ptime timeFrom,
         boost::posix_time::ptime timeTo) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the data by month (avg, min, max per day)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optional)
      /// \param [in] timeTo     The end date (optional)
      /// \return                Map of data : (date, value)
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CAcquisitionSummary>> getKeywordDataByMonth(
         int keywordId,
         boost::posix_time::ptime timeFrom,
         boost::posix_time::ptime timeTo) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the data by year (avg, min, max per day)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optional)
      /// \param [in] timeTo     The end date (optional)
      /// \return                Map of data : (date, value)
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CAcquisitionSummary>> getKeywordDataByYear(
         int keywordId,
         boost::posix_time::ptime timeFrom,
         boost::posix_time::ptime timeTo) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the data
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optional)
      /// \param [in] timeTo     The end date (optional)
      /// \param [in] limit      Max count of records to return (optional, -1 if no limit)
      /// \return                Json string as a map of {date, value} tuples
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::string getHugeVectorKeywordData(
         int keywordId,
         boost::posix_time::ptime timeFrom = boost::posix_time::not_a_date_time,
         boost::posix_time::ptime timeTo = boost::posix_time::not_a_date_time,
         int limit = -1) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the data  by hour (avg, min, max per hour)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optional)
      /// \param [in] timeTo     The end date (optional)
      /// \return                Json string as a map of {date, value} tuples
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::string getHugeVectorKeywordDataByHour(int keywordId,
                                                         boost::posix_time::ptime timeFrom,
                                                         boost::posix_time::ptime timeTo) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the data  by day (avg, min, max per day)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optional)
      /// \param [in] timeTo     The end date (optional)
      /// \return                Json string as a map of {date, value} tuples
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::string getHugeVectorKeywordDataByDay(int keywordId,
                                                        boost::posix_time::ptime timeFrom,
                                                        boost::posix_time::ptime timeTo) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the data by month (avg, min, max per day)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optional)
      /// \param [in] timeTo     The end date (optional)
      /// \return                Json string as a map of {date, value} tuples
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::string getHugeVectorKeywordDataByMonth(int keywordId,
                                                          boost::posix_time::ptime timeFrom,
                                                          boost::posix_time::ptime timeTo) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the data by year (avg, min, max per day)
      /// \param [in] keywordId  keywordId Id
      /// \param [in] timeFrom   The start date (optional)
      /// \param [in] timeTo     The end date (optional)
      /// \return                Json string as a map of {date, value} tuples
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::string getHugeVectorKeywordDataByYear(int keywordId,
                                                         boost::posix_time::ptime timeFrom,
                                                         boost::posix_time::ptime timeTo) = 0;

      //--------------------------------------------------------------
      /// \brief                 Delete old acquisition
      /// \param [in] purgeDate  The date of purge (any data prior ro his date will be deleted)
      /// \param [in] limit      Purge limitation (no limit by default)
      /// \return                Number of deleted rows
      //--------------------------------------------------------------
      virtual int purgeAcquisitions(boost::posix_time::ptime purgeDate,
                                    int limit = -1) = 0;
   };
} //namespace database 
