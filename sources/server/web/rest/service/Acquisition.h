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
            virtual ~CAcquisition() = default;

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
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
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordDataInternal(const std::vector<std::string>& parameters,
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
            static std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
