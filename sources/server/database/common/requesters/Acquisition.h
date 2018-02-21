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
         class CAcquisition: public IAcquisitionRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	databaseRequester the database handler
            //--------------------------------------------------------------
            CAcquisition(boost::shared_ptr<IDatabaseRequester> databaseRequester,
                         boost::shared_ptr<CKeyword> keywordRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CAcquisition();

            // IAcquisitionRequester implementation
            boost::shared_ptr<entities::CAcquisition> saveData(const int keywordId,
                                                               const std::string& data,
                                                               boost::posix_time::ptime& dataTime) override;
            boost::shared_ptr<entities::CAcquisition> incrementData(const int keywordId,
                                                                    const std::string& increment,
                                                                    boost::posix_time::ptime& dataTime) override;
            boost::shared_ptr<entities::CAcquisitionSummary> saveSummaryData(const int keywordId,
                                                                             database::entities::EAcquisitionSummaryType curType,
                                                                             boost::posix_time::ptime& dataTime) override;
            void getKeywordsHavingDate(const boost::posix_time::ptime& timeFrom,
                                       const boost::posix_time::ptime& timeTo,
                                       std::vector<int>& results) override;
            bool summaryDataExists(const int keywordId,
                                   database::entities::EAcquisitionSummaryType curType,
                                   boost::posix_time::ptime& date) override;
            void removeKeywordData(const int keywordId) override;
            boost::shared_ptr<entities::CAcquisition> getAcquisitionByKeywordAndDate(const int keywordId,
                                                                                     boost::posix_time::ptime time) override;
            std::vector<boost::tuple<boost::posix_time::ptime, std::string> > getKeywordData(int keywordId,
                                                                                             boost::posix_time::ptime timeFrom,
                                                                                             boost::posix_time::ptime timeTo) override;

            virtual std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > getKeywordDataByDay(int keywordId,
                                                                                                                  boost::posix_time::ptime timeFrom,
                                                                                                                  boost::posix_time::ptime timeTo) override;
            virtual std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > getKeywordDataByHour(int keywordId,
                                                                                                                   boost::posix_time::ptime timeFrom,
                                                                                                                   boost::posix_time::ptime timeTo) override;
            virtual std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > getKeywordDataByMonth(int keywordId,
                                                                                                                    boost::posix_time::ptime timeFrom,
                                                                                                                    boost::posix_time::ptime timeTo) override;
            virtual std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > getKeywordDataByYear(int keywordId,
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

            int purgeAcquisitions(boost::posix_time::ptime purgeDate) override;
            // [END] IAcquisitionRequester implementation

         private:

            //--------------------------------------------------------------
            /// \brief                    Get the data  by type (avg, min, max)
            /// \param [in] keywordId     keywordId Id
            /// \param [in] timeFrom      The start date (optionnal)
            /// \param [in] timeTo        The end date (optionnal)
            /// \return                   CAcquisitionSummary data
            /// \throw                    CInvalidParameter if deviceId is unknown
            //--------------------------------------------------------------
            std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > getKeywordSummaryDataByType(const entities::EAcquisitionSummaryType& type, int keywordId,
                                                                                                                  boost::posix_time::ptime timeFrom,
                                                                                                                  boost::posix_time::ptime timeTo) const;

            //--------------------------------------------------------------
            /// \brief                    Get the data  by type (avg, min, max)
            /// \param [in] keywordId     keywordId Id
            /// \param [in] timeFrom      The start date (optionnal)
            /// \param [in] timeTo        The end date (optionnal)
            /// \return                   CAcquisitionSummary data
            /// \throw                    CInvalidParameter if deviceId is unknown
            //--------------------------------------------------------------
            std::string getHugeVectorKeywordSummaryDataByType(const entities::EAcquisitionSummaryType& type, int keywordId,
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


