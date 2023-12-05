#include "stdafx.h"
#include "Helpers.h"

#include <Poco/Net/NetworkInterface.h>

#include "AcceptedAnswer.h"
#include "ErrorAnswer.h"
#include "NoContentAnswer.h"
#include "SeeOtherLocationAnswer.h"
#include "SerialPortsLister.h"
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

      boost::shared_ptr<shared::CDataContainer> CHelpers::getSerialPortsV2()
      {
         const auto serialPorts = hardware::serial::CSerialPortsLister::listSerialPorts();

         auto result = shared::CDataContainer::make();
         for (const auto& serialPort : *serialPorts)
            //in case of key contains a dot, just ensure the full key is taken into account
            result->set(serialPort.first, serialPort.second, 0x00);

         return result;
      }

      boost::shared_ptr<shared::CDataContainer> CHelpers::getUsbDevicesV2(const std::vector<std::pair<int, int>>& requestedUsbDevices,
                                                                          boost::shared_ptr<hardware::usb::IDevicesLister> usbDevicesLister)
      {
         const auto existingDevices = usbDevicesLister->listUsbDevices();
         YADOMS_LOG(debug) << "USB existing devices :";
         for (const auto& device : existingDevices)
         {
            YADOMS_LOG(debug) << "  - "
               << "vid=" << device->vendorId()
               << ", pid=" << device->productId()
               << ", name=" << device->yadomsFriendlyName()
               << ", connectionId=" << device->nativeConnectionString()
               << ", serial=" << device->serialNumber();
         }

         // If request content is empty, return all existing USB devices
         if (requestedUsbDevices.empty())
         {
            auto result = boost::make_shared<shared::CDataContainer>();
            for (const auto& device : existingDevices)
               //in case of key contains a dot, just ensure the full key is taken into account
               result->set(device->nativeConnectionString(), device->yadomsFriendlyName(), 0x00);
            return result;
         }

         // Filter USB devices by request content
         auto result = boost::make_shared<shared::CDataContainer>();
         YADOMS_LOG(debug) << "USB requested devices :";
         for (const auto& usbDevice : requestedUsbDevices)
         {
            YADOMS_LOG(debug) << "  - "
               << "vid=" << usbDevice.first
               << ", pid=" << usbDevice.second;

            for (const auto& existingDevice : existingDevices)
            {
               if (existingDevice->vendorId() == usbDevice.first
                  && existingDevice->productId() == usbDevice.second)
               {
                  //in case of key contains a dot, just ensure the full key is taken into account
                  result->set(existingDevice->nativeConnectionString(), existingDevice->yadomsFriendlyName(),
                              0x00);
               }
            }
         }

         return result;
      }

      boost::shared_ptr<shared::CDataContainer> CHelpers::getNetworkInterfacesV2(bool includeLoopback)
      {
         const auto networkInterfaces = Poco::Net::NetworkInterface::list();

         auto result = shared::CDataContainer::make();
         for (const auto& nit : networkInterfaces)
         {
            if (nit.address().isLoopback() && !includeLoopback)
               continue;

            //in case of key contains a dot, just ensure the full key is taken into account
            result->set(nit.name(),
                        (boost::format("%1% (%2%)") % nit.displayName() % nit.address().toString()).str(),
                        0x00);
         }

         return result;
      }

      boost::shared_ptr<shared::CDataContainer> CHelpers::getSupportedTimezonesV2(const std::set<std::string>& filters,
                                                                                  boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase)
      {
         const auto& supportedTimezones = timezoneDatabase->allIds();

         if (supportedTimezones.empty())
            return {};

         auto result = shared::CDataContainer::make();
         if (filters.empty())
         {
            for (const auto& timezone : supportedTimezones)
               result->set(timezone, timezone);
            return result;
         }

         for (const auto& timezone : supportedTimezones)
         {
            for (const auto& filterValue : filters)
            {
               if (timezone.find(filterValue) == std::string::npos)
                  continue;

               result->set(timezone, timezone);
               break; // Don't add twice
            }
         }

         return result;
      }
   } //namespace rest
} //namespace web 
