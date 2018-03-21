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
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordLastData(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the last data of a list of keywords
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordListLastData(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter)
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordData(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter) by hour (avg, min and max)
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordDataByHour(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter) by day (avg, min and max)
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordDataByDay(const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter) by month (avg, min and max)
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordDataByMonth(const std::vector<std::string>& parameters,
                                                                                              const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Get the data of a keyword (with optional date filter) by year (avg, min and max)
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordDataByYear(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const;

            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            static std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


