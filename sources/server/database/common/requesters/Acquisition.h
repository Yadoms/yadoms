#pragma once

#include "database/IAcquisitionRequester.h"
#include "database/IDatabaseRequester.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         class CKeyword;

         //--------------------------------------------------------------
         /// \Brief		   Acquisition requester for SQLite database
         //--------------------------------------------------------------
         class CAcquisition : public IAcquisitionRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	databaseRequester the database handler
            /// \param [in]   keywordRequester  The keyword requester
            //--------------------------------------------------------------
            CAcquisition(boost::shared_ptr<IDatabaseRequester> databaseRequester,
                         boost::shared_ptr<CKeyword> keywordRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CAcquisition() = default;

            // IAcquisitionRequester implementation
            boost::shared_ptr<entities::CAcquisition> saveData(int keywordId,
                                                               const std::string& data,
                                                               boost::posix_time::ptime& dataTime) override;
            boost::shared_ptr<entities::CAcquisition> incrementData(int keywordId,
                                                                    const std::string& increment,
                                                                    boost::posix_time::ptime& dataTime) override;
            boost::shared_ptr<entities::CAcquisitionSummary> saveSummaryData(int keywordId,
                                                                             database::entities::EAcquisitionSummaryType curType,
                                                                             boost::posix_time::ptime& dataTime) override;
            void getKeywordsHavingDate(const boost::posix_time::ptime& timeFrom,
                                       const boost::posix_time::ptime& timeTo,
                                       std::vector<int>& results) override;
            bool summaryDataExists(int keywordId,
                                   entities::EAcquisitionSummaryType curType,
                                   boost::posix_time::ptime& date) override;
            void moveAllData(int fromKw, int toKw) override;
            void removeKeywordData(int keywordId) override;
            boost::shared_ptr<entities::CAcquisition> getAcquisitionByKeywordAndDate(int keywordId,
                                                                                     boost::posix_time::ptime time) override;
            void exportAcquisitions(const int keywordId,
                                    std::function<void(const boost::posix_time::ptime& date,
                                                       const std::string& value,
                                                       int nbTotalLines)> exportOneLineFunctionn) override;
            std::vector<boost::tuple<boost::posix_time::ptime, std::string>> getKeywordData(int keywordId,
                                                                                            boost::posix_time::ptime timeFrom = boost::posix_time::
                                                                                               not_a_date_time,
                                                                                            boost::posix_time::ptime timeTo = boost::posix_time::
                                                                                               not_a_date_time,
                                                                                            int limit = -1) override;

            std::vector<boost::shared_ptr<entities::CAcquisitionSummary>> getKeywordDataByDay(int keywordId,
                                                                                              boost::posix_time::ptime timeFrom,
                                                                                              boost::posix_time::ptime timeTo) override;
            std::vector<boost::shared_ptr<entities::CAcquisitionSummary>> getKeywordDataByHour(int keywordId,
                                                                                               boost::posix_time::ptime timeFrom,
                                                                                               boost::posix_time::ptime timeTo) override;
            std::vector<boost::shared_ptr<entities::CAcquisitionSummary>> getKeywordDataByMonth(int keywordId,
                                                                                                boost::posix_time::ptime timeFrom,
                                                                                                boost::posix_time::ptime timeTo) override;
            std::vector<boost::shared_ptr<entities::CAcquisitionSummary>> getKeywordDataByYear(int keywordId,
                                                                                               boost::posix_time::ptime timeFrom,
                                                                                               boost::posix_time::ptime timeTo) override;
            std::string getHugeVectorKeywordDataByDay(int keywordId,
                                                      boost::posix_time::ptime timeFrom,
                                                      boost::posix_time::ptime timeTo) override;
            std::string getHugeVectorKeywordDataByHour(int keywordId,
                                                       boost::posix_time::ptime timeFrom,
                                                       boost::posix_time::ptime timeTo) override;
            std::string getHugeVectorKeywordDataByMonth(int keywordId,
                                                        boost::posix_time::ptime timeFrom,
                                                        boost::posix_time::ptime timeTo) override;
            std::string getHugeVectorKeywordDataByYear(int keywordId,
                                                       boost::posix_time::ptime timeFrom,
                                                       boost::posix_time::ptime timeTo) override;

            int purgeAcquisitions(boost::posix_time::ptime purgeDate,
                                  int limit = -1) override;
            // [END] IAcquisitionRequester implementation

         private:

            //--------------------------------------------------------------
            /// \brief                    Get the data  by type (avg, min, max)
            /// \param [in] type          Summary type
            /// \param [in] keywordId     keywordId Id
            /// \param [in] timeFrom      The start date (optional)
            /// \param [in] timeTo        The end date (optional)
            /// \return                   CAcquisitionSummary data
            /// \throw                    CInvalidParameter if deviceId is unknown
            //--------------------------------------------------------------
            std::vector<boost::shared_ptr<database::entities::CAcquisitionSummary>> getKeywordSummaryDataByType(
               const entities::EAcquisitionSummaryType& type,
               int keywordId,
               boost::posix_time::ptime timeFrom,
               boost::posix_time::ptime timeTo) const;

            //--------------------------------------------------------------
            /// \brief                    Get the data  by type (avg, min, max)
            /// \param [in] type          Summary type
            /// \param [in] keywordId     keywordId Id
            /// \param [in] timeFrom      The start date (optional)
            /// \param [in] timeTo        The end date (optional)
            /// \return                   CAcquisitionSummary data
            /// \throw                    CInvalidParameter if deviceId is unknown
            //--------------------------------------------------------------
            std::string getHugeVectorKeywordSummaryDataByType(const entities::EAcquisitionSummaryType& type,
                                                              int keywordId,
                                                              boost::posix_time::ptime timeFrom,
                                                              boost::posix_time::ptime timeTo) const;

            //--------------------------------------------------------------
            /// \Brief		   Pointer to keyword requester
            //--------------------------------------------------------------
            boost::shared_ptr<CKeyword> m_keywordRequester;

            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
         };
      }
   }
} //namespace database::common::requesters
