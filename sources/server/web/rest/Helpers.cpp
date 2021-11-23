#include "stdafx.h"
#include "Helpers.h"

#include "ErrorAnswer.h"

namespace web
{
   namespace rest
   {
      boost::shared_ptr<IAnswer> CHelpers::transactionalMethodV2(
         const boost::shared_ptr<IRequest>& request,
         const boost::shared_ptr<database::IDataProvider>& dataProvider,
         const std::function<boost::shared_ptr<IAnswer>(const boost::shared_ptr<IRequest>&)>& realMethod)
      {
         const auto pTransactionalEngine = dataProvider->getTransactionalEngine();
         boost::shared_ptr<IAnswer> answer;
         try
         {
            if (pTransactionalEngine)
               pTransactionalEngine->transactionBegin();
            answer = realMethod(request);
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

      std::unique_ptr<std::set<int>> CHelpers::convertToIntSet(const std::unique_ptr<std::set<std::string>> in)
      {
         auto out = std::make_unique<std::set<int>>();
         for (const auto& inItem : *in)
            out->insert(std::stol(inItem));
         return out;
      }
   } //namespace rest
} //namespace web 
