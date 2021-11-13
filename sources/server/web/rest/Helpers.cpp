#include "stdafx.h"
#include "Helpers.h"

#include "ErrorAnswer.h"

namespace web
{
   namespace rest
   {
      boost::shared_ptr<IAnswer> CHelpers::transactionalMethodV2(
         boost::shared_ptr<IRequest> request,
         boost::shared_ptr<database::IDataProvider> dataProvider,
         const std::function<boost::shared_ptr<IAnswer>(boost::shared_ptr<IRequest>)>& realMethod)
      {
         const auto pTransactionalEngine = dataProvider->getTransactionalEngine();
         boost::shared_ptr<IAnswer> answer;
         try
         {
            if (pTransactionalEngine)
               pTransactionalEngine->transactionBegin();
            answer = realMethod(std::move(request));
         }
         catch (std::exception&)
         {
            answer = boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                      "Fail to process transactional method");
         }

         if (pTransactionalEngine)
         {
            if (answer->code() / 100 == shared::http::ECodes::kOKValue / 100)
               pTransactionalEngine->transactionCommit();
            else
               pTransactionalEngine->transactionRollback();
         }

         return answer;
      }
   } //namespace rest
} //namespace web 
