#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "web/poco/RestDispatcher.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CAcquisition final : public IRestService
         {
         public:
            explicit CAcquisition(boost::shared_ptr<database::IDataProvider> dataProvider);
            ~CAcquisition() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();

         private:
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordLastData(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const;

            void readKeywordInfo(int keywordId,
                                 const std::string& info,
                                 shared::CDataContainer& keywordResult) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordListLastData(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordListInfo(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordData(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordInfo(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordDataByHour(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordDataByDay(const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordDataByMonth(const std::vector<std::string>& parameters,
                                                                                              const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordDataByYear(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const;

            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
            static std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
