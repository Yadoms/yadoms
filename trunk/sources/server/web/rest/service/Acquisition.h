#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CAcquisition : public IRestService
   {
   public:
      CAcquisition(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CAcquisition();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation


      static const std::string & getRestKeyword();

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      static std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 
