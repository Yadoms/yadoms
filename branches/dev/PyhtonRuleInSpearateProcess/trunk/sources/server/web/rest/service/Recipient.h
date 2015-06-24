#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"


namespace web { namespace rest { namespace service {

   class CRecipient : public IRestService
   {
   public:
      CRecipient(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CRecipient();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      static const std::string & getRestKeyword();
      shared::CDataContainer getOneRecipient(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   
      shared::CDataContainer getAllRecipients(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer addRecipient(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer updateRecipient(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer removeOneRecipient(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer removeAllRecipients(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

      shared::CDataContainer getAllRecipientFields(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer getAllRecipientsByField(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

   private:
      static std::string m_restKeyword;
      static std::string m_restFieldKeyword;
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      shared::CDataContainer transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const std::string & requestContent);
   };


} //namespace service
} //namespace rest
} //namespace web 
