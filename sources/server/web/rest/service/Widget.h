#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web::rest::service
{
   class CWidget final : public IRestService
   {
   public:
      explicit CWidget(const boost::shared_ptr<database::IDataProvider>& dataProvider,
                       std::string webServerPath);

      CWidget(const CWidget&) = delete;
      CWidget(CWidget&&) = delete;
      CWidget& operator=(const CWidget&) = delete;
      CWidget& operator=(CWidget&&) = delete;
      ~CWidget() override = default;

      // IRestService implementation
      void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
      boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
      // [END] IRestService implementation

   private:
      static std::string m_restKeyword;
      static const std::string& getRestKeyword();

      // V1
      boost::shared_ptr<shared::serialization::IDataSerializable> getOneWidgetV1(const std::vector<std::string>& parameters,
                                                                               const std::string& requestContent) const;
      boost::shared_ptr<shared::serialization::IDataSerializable> getAllWidgetsV1(const std::vector<std::string>& parameters,
                                                                                const std::string& requestContent) const;
      boost::shared_ptr<shared::serialization::IDataSerializable> addWidgetV1(const std::vector<std::string>& parameters,
                                                                            const std::string& requestContent) const;
      boost::shared_ptr<shared::serialization::IDataSerializable> deleteOneWidgetV1(const std::vector<std::string>& parameters,
                                                                                  const std::string& requestContent) const;
      boost::shared_ptr<shared::serialization::IDataSerializable> deleteAllWidgetsV1(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
      boost::shared_ptr<shared::serialization::IDataSerializable> replaceAllWidgetsV1(const std::vector<std::string>& parameters,
                                                                                    const std::string& requestContent) const;
      boost::shared_ptr<shared::serialization::IDataSerializable> updateOneWidgetV1(const std::vector<std::string>& parameters,
                                                                                  const std::string& requestContent) const;
      boost::shared_ptr<shared::serialization::IDataSerializable> findWidgetPackagesV1(
         const std::vector<std::string>& parameters,
         const std::string& requestContent) const;

      boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(
         const poco::CRestDispatcher::CRestMethodHandler& realMethod,
         const std::vector<std::string>& parameters,
         const std::string& requestContent) const;

      std::string m_webServerPathV1;


      // V2
      boost::shared_ptr<IAnswer> getWidgetsV2(const boost::shared_ptr<IRequest>& request) const;

      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;

   };
}
