#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CPage : public IRestService
         {
         public:
            explicit CPage(boost::shared_ptr<database::IDataProvider> dataProvider);
            virtual ~CPage() = default;

         public:
            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();
            boost::shared_ptr<shared::serialization::IDataSerializable> getOnePage(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllPages(const std::vector<std::string>& parameters,
                                                                                    const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getPageWidget(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> addPage(const std::vector<std::string>& parameters,
                                                                                const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updatePage(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updateAllPages(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deletePage(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteAllPages(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;


            boost::shared_ptr<shared::serialization::IDataSerializable> addWidgetForPage(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> replaceAllWidgetsForPage(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteAllWidgetsForPage(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(
               CRestDispatcher::CRestMethodHandler realMethod,
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

         private:
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            static std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
