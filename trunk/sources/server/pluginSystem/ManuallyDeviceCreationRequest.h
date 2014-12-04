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
   class CManuallyDeviceCreationRequest  : public shared::plugin::yPluginApi::IManuallyDeviceCreationRequest
   {
   public:
      //-----------------------------------------------------
      ///\brief Constructor
      ///\param [in]  data       The data container
      //-----------------------------------------------------
      CManuallyDeviceCreationRequest(const shared::plugin::yPluginApi::IManuallyDeviceCreationData & data, communication::callback::ISynchronousCallback<std::string> & callback);

      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CManuallyDeviceCreationRequest();
   
      //-----------------------------------------------------
      ///\brief Get the request data
      //-----------------------------------------------------
      const shared::plugin::yPluginApi::IManuallyDeviceCreationData& getData();

      //-----------------------------------------------------
      ///\brief Answer the request with success, providing device name
      ///\param [in] newDeviceName  The created device name
      //-----------------------------------------------------
      void sendSuccess(const std::string& newDeviceName);

      //-----------------------------------------------------
      ///\brief Answer the request with error
      ///\param [in] errorMessage  The error message
      //-----------------------------------------------------
      void sendError(const std::string& errorMessage);

   private:
      //-----------------------------------------------------
      ///\brief Internal data
      //-----------------------------------------------------
      boost::shared_ptr< communication::callback::ICallbackRequest<shared::plugin::yPluginApi::IManuallyDeviceCreationData, std::string> > m_requestPtr;
   };   
   
} // namespace pluginSystem
	
	