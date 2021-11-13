#pragma once
#include "IAnswer.h"
#include "IRequest.h"
#include "database/IDataProvider.h"

namespace web
{
   namespace rest
   {
      class CHelpers final
      {
      public:
         CHelpers() = delete;
         ~CHelpers() = delete;

         static boost::shared_ptr<IAnswer> transactionalMethodV2(boost::shared_ptr<IRequest> request,
                                                                 boost::shared_ptr<database::IDataProvider> dataProvider,
                                                                 const std::function<boost::shared_ptr<IAnswer>(boost::shared_ptr<IRequest>)>&
                                                                 realMethod);
      };
   } //namespace rest
} //namespace web 
