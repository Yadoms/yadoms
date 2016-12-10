#pragma once

#include "communication/callback/ISynchronousCallback.h"
#include "communication/callback/ICallbackRequest.h"
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief Class which handle ManuallyDeviceCreationRequest
   ///       It provides data; and answers method 
   //-----------------------------------------------------
   class CManuallyDeviceCreationRequest : public shared::plugin::yPluginApi::IManuallyDeviceCreationRequest
   {
   public:
      //-----------------------------------------------------
      ///\brief Constructor
      ///\param [in]  data       The data container
      ///\param [in]  callback   The callback to call when request returns
      //-----------------------------------------------------
      CManuallyDeviceCreationRequest(const shared::plugin::yPluginApi::IManuallyDeviceCreationData& data,
                                     communication::callback::ISynchronousCallback<std::string>& callback);

      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CManuallyDeviceCreationRequest();

      // IManuallyDeviceCreationRequest Implementation
      const shared::plugin::yPluginApi::IManuallyDeviceCreationData& getData() override;
      void sendSuccess(const std::string& newDeviceName) override;
      void sendError(const std::string& errorMessage) override;
      // [END] IManuallyDeviceCreationRequest Implementation

   private:
      //-----------------------------------------------------
      ///\brief Internal data
      //-----------------------------------------------------
      boost::shared_ptr<communication::callback::ICallbackRequest<shared::plugin::yPluginApi::IManuallyDeviceCreationData, std::string> > m_requestPtr;
   };
} // namespace pluginSystem


