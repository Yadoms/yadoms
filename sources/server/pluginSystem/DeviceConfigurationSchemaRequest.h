#pragma once
#include "communication/callback/ISynchronousCallback.h"
#include "communication/callback/ICallbackRequest.h"
#include <shared/plugin/yPluginApi/IDeviceConfigurationSchemaRequest.h>
#include <shared/DataContainer.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief Class which handle BindingQueryRequest
   ///       It provides data; and answers method 
   //-----------------------------------------------------
   class CDeviceConfigurationSchemaRequest : public shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest
   {
   public:
      //-----------------------------------------------------
      ///\brief Constructor
      ///\param [in]  data       The data container
      //-----------------------------------------------------
      CDeviceConfigurationSchemaRequest(const std::string& device,
                                        communication::callback::ISynchronousCallback<shared::CDataContainer>& callback);

      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CDeviceConfigurationSchemaRequest();

      // IDeviceConfigurationSchemaRequest implementation
      std::string device() const override;
      void sendSuccess(const shared::CDataContainer& deviceConfigurationSchema) override;
      void sendError(const std::string& errorMessage) override;
      // [END] - IDeviceConfigurationSchemaRequest implementation

   private:
      //-----------------------------------------------------
      ///\brief Internal data
      //-----------------------------------------------------
      boost::shared_ptr<communication::callback::ICallbackRequest<const std::string&, shared::CDataContainer> > m_requestPtr;
   };
} // namespace pluginSystem


