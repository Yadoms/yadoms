#include "stdafx.h"
#include "Helpers.h"

#include "AcceptedAnswer.h"
#include "ErrorAnswer.h"
#include "NoContentAnswer.h"
#include "SeeOtherLocationAnswer.h"
#include "SuccessAnswer.h"
#include "task/IInstance.h"

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

      std::unique_ptr<std::set<int>> CHelpers::convertToIntSet(const std::unique_ptr<std::set<std::string>>& in)
      {
         auto out = std::make_unique<std::set<int>>();
         for (const auto& inItem : *in)
            out->insert(std::stol(inItem));
         return out;
      }

      boost::shared_ptr<IAnswer> CHelpers::formatGetMultiItemsAnswer(bool hasOnlyOneItem,
                                                                     const std::vector<boost::shared_ptr<shared::CDataContainer>>& outputDataEntries,
                                                                     const std::string& rootTag,
                                                                     boost::optional<CPaging> paging)
      {
         if (outputDataEntries.empty())
            return boost::make_shared<CNoContentAnswer>();

         if (hasOnlyOneItem)
            return boost::make_shared<CSuccessAnswer>(*outputDataEntries.at(0));

         shared::CDataContainer container;
         container.set(rootTag, outputDataEntries);

         if (paging)
         {
            shared::CDataContainer pagingData;
            pagingData.set("currentPage", paging->page());
            pagingData.set("totalPage", paging->pagesCount());
            pagingData.set("pageSize", paging->pageSize());
            container.set("paging", pagingData);
         }

         return boost::make_shared<CSuccessAnswer>(container);
      }

      boost::shared_ptr<IAnswer> CHelpers::createLongRunningOperationAnswer(const std::string& taskUid)
      {
         return boost::make_shared<CAcceptedAnswer>("tasks/" + taskUid);
      }

      boost::shared_ptr<IAnswer> CHelpers::getLongRunningOperationAnswer(boost::shared_ptr<task::IInstance> task,
                                                                         boost::shared_ptr<shared::CDataContainer> taskEntry)
      {
         if (task->getStatus() != task::ETaskStatus::kStarted)
            return boost::make_shared<CSeeOtherLocationAnswer>(task->getGuid() + "/result");

         if (taskEntry)
            return boost::make_shared<CSuccessAnswer>(*taskEntry);

         shared::CDataContainer result;
         result.set("taskId", task->getGuid());
         return boost::make_shared<CSuccessAnswer>(result);
      }
   } //namespace rest
} //namespace web 
