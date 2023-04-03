#pragma once
#include "IAnswer.h"
#include "IRequest.h"
#include "Paging.h"
#include "database/IDataProvider.h"
#include "dateTime/TimeZoneDatabase.h"
#include "hardware/usb/IDevicesLister.h"
#include "shared/http/ssdp/Client.h"
#include "task/IInstance.h"

namespace web
{
   namespace rest
   {
      class CHelpers final
      {
      public:
         CHelpers() = delete;
         ~CHelpers() = delete;

         //-----------------------------------------
         ///\brief   Transactional method
         //-----------------------------------------
         static boost::shared_ptr<IAnswer> transactionalMethodV2(
            const boost::shared_ptr<IRequest>& request,
            const boost::shared_ptr<database::IDataProvider>& dataProvider,
            const std::function<boost::shared_ptr<IAnswer>(const boost::shared_ptr<IRequest>&)>& realMethod);

         //-----------------------------------------
         ///\brief   Convert set of strings into set of int
         //-----------------------------------------
         static std::unique_ptr<std::set<int>> convertToIntSet(const std::unique_ptr<std::set<std::string>>& in);

         //-----------------------------------------
         ///\brief   Convert set of strings into set of ExtendedEnum
         //-----------------------------------------
         template <typename T>
         static std::unique_ptr<std::set<T>> convertToEnumSet(const std::unique_ptr<std::set<std::string>>& in)
         {
            // ReSharper disable once CppUseTypeTraitAlias
            static_assert(std::is_base_of<shared::enumeration::IExtendedEnum, T>::value, "T must be derived from shared::enumeration::IExtendedEnum");
            auto out = std::make_unique<std::set<T>>();
            for (const auto& inItem : *in)
               out->insert(T(inItem));
            return out;
         }

         //-----------------------------------------
         ///\brief         Format answer of a GET request with one or more items asked
         ///\description   A GET request can be called for one or several items.
         ///               When several items are requested, result will be expressed as an array. Paging data can be added (optional).
         ///               If just one item was asked, result should be expressed as item values (no more "decoration")
         ///               Example :
         ///               - Result for several items :
         ///               { "items" : [
         ///                     {
         ///                        "key1" : "value1",
         ///                        "key2" : "value2"
         ///                     },
         ///                     {
         ///                        "key1" : "value3",
         ///                        "key2" : "value4"
         ///                     }
         ///                  ],
         ///                  "paging" : {
         ///                     "currentPage" : 3,
         ///                     "totalPage" : 8,
         ///                     "pageSize" : 10
         ///                  }
         ///               }
         ///
         ///               - Result for one item :
         ///               {
         ///                  "key1" : "value1",
         ///                  "key2" : "value2"
         ///               }
         ///
         /// \param[in] hasOnlyOneItem     Flag indicating that only one item was asked
         /// \param[in] outputDataEntries  List of data to out in answer (only first data will be used if hasOnlyOneItem is true)
         /// \param[in] rootTag            The root tag to write if several data
         /// \param[in] paging             The paging data (optional)
         /// \return The formatted answer
         //-----------------------------------------
         static boost::shared_ptr<IAnswer> formatGetMultiItemsAnswer(bool hasOnlyOneItem,
                                                                     const std::vector<boost::shared_ptr<shared::CDataContainer>>& outputDataEntries,
                                                                     const std::string& rootTag,
                                                                     boost::optional<CPaging> paging = boost::none);
         //-----------------------------------------
         ///\brief         Build long running operation creation answer for the "Long running Operation" pattern (see http://restalk-patterns.org/long-running-operation-polling.html)
         /// \param[in] taskUid            The created task Uid
         /// \return The formatted answer
         //-----------------------------------------
         static boost::shared_ptr<IAnswer> createLongRunningOperationAnswer(const std::string& taskUid);

         //-----------------------------------------
         ///\brief         Build long running operation getting answer for the "Long running Operation" pattern (see http://restalk-patterns.org/long-running-operation-polling.html)
         /// \param[in] task               The operation task
         /// \param[in] taskEntry          The task entry (send only UUID if not provided)
         /// \return The formatted answer
         //-----------------------------------------
         static boost::shared_ptr<IAnswer> getLongRunningOperationAnswer(boost::shared_ptr<task::IInstance> task,
                                                                         boost::shared_ptr<shared::CDataContainer> taskEntry = nullptr);

         static boost::shared_ptr<shared::CDataContainer> getSerialPortsV2();
         static boost::shared_ptr<shared::CDataContainer> getUsbDevicesV2(const std::vector<std::pair<int, int>>& requestedUsbDevices,
                                                                          boost::shared_ptr<hardware::usb::IDevicesLister> usbDevicesLister);
         static boost::shared_ptr<shared::CDataContainer> getNetworkInterfacesV2(bool includeLoopback);
         static std::vector<std::string> getSupportedTimezonesV2(std::unique_ptr<std::set<std::string>> filters,
                                                                 boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase);
      };
   } //namespace rest
} //namespace web 
