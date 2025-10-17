#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CWidget final : public IRestService
         {
         public:
            explicit CWidget(boost::shared_ptr<database::IDataProvider> dataProvider, const std::string& webServerPath);
            ~CWidget() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation


            static const std::string& getRestKeyword();

            boost::shared_ptr<shared::serialization::IDataSerializable> getOneWidget(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllWidgets(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> addWidget(const std::vector<std::string>& parameters,
                                                                                  const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteOneWidget(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteAllWidgets(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> replaceAllWidgets(const std::vector<std::string>& parameters,
                                                                                          const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updateOneWidget(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> findWidgetPackages(
               const std::vector<std::string>& parameters, const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(
               poco::CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
         private:
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
            static std::string m_restKeyword;
            std::string m_webServerPath;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
