#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CAcquisition : public IRestService
         {
         public:
            explicit CAcquisition(boost::shared_ptr<database::IDataProvider> dataProvider);
            virtual ~CAcquisition();

         public:
            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();

         private:
            //-----------------------------------------
            ///\brief   Get the last data of a keyword
            //-----------------------------------------
            shared::CDataContainer getKeywordLastData(const std::vector<std::string>& parameters,
                                                      const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the last data of a list of keywords
            //-----------------------------------------
            shared::CDataContainer getKeywordListLastData(const std::vector<std::string>& parameters,
                                                          const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter)
            //-----------------------------------------
            shared::CDataContainer getKeywordData(const std::vector<std::string>& parameters,
                                                  const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter) by hour (avg, min and max)
            //-----------------------------------------
            shared::CDataContainer getKeywordDataByHour(const std::vector<std::string>& parameters,
                                                        const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter) by day (avg, min and max)
            //-----------------------------------------
            shared::CDataContainer getKeywordDataByDay(const std::vector<std::string>& parameters,
                                                       const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter) by month (avg, min and max)
            //-----------------------------------------
            shared::CDataContainer getKeywordDataByMonth(const std::vector<std::string>& parameters,
                                                         const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter) by year (avg, min and max)
            //-----------------------------------------
            shared::CDataContainer getKeywordDataByYear(const std::vector<std::string>& parameters,
                                                        const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter) for highchart treatment (fast queries, optimized for highcharts js)
            //-----------------------------------------
            shared::CDataContainer getHighchartKeywordData(const std::vector<std::string>& parameters,
                                                           const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter)  by hour (avg, min and max) for highchart treatment (fast queries, optimized for highcharts js)
            //-----------------------------------------
            shared::CDataContainer getHighchartKeywordDataByHour(const std::vector<std::string>& parameters,
                                                                 const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter)  by day (avg, min and max)for highchart treatment (fast queries, optimized for highcharts js)
            //-----------------------------------------
            shared::CDataContainer getHighchartKeywordDataByDay(const std::vector<std::string>& parameters,
                                                                const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter)  by month (avg, min and max)for highchart treatment (fast queries, optimized for highcharts js)
            //-----------------------------------------
            shared::CDataContainer getHighchartKeywordDataByMonth(const std::vector<std::string>& parameters,
                                                                  const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter)  by year (avg, min and max)for highchart treatment (fast queries, optimized for highcharts js)
            //-----------------------------------------
            shared::CDataContainer getHighchartKeywordDataByYear(const std::vector<std::string>& parameters,
                                                                 const std::string& requestContent) const;

            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            static std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


