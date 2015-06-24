#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CConfiguration : public IRestService
   {
   public:
      CConfiguration(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CConfiguration();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation


      static const std::string & getRestKeyword();

   public:
      shared::CDataContainer getConfiguration(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer getSectionConfigurations(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer getAllConfigurations(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

      shared::CDataContainer createOneConfiguration(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer updateOneConfiguration(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer updateAllConfigurations(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer deleteOneConfiguration(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      static std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 
