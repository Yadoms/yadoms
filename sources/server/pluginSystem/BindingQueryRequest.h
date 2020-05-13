#pragma once
#include "communication/callback/ISynchronousCallback.h"
#include "communication/callback/ICallbackRequest.h"
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/DataContainer.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief Class which handle BindingQueryRequest
   ///       It provides data; and answers method 
   //-----------------------------------------------------
   class CBindingQueryRequest : public shared::plugin::yPluginApi::IBindingQueryRequest
   {
   public:
      //-----------------------------------------------------
      ///\brief Constructor
      ///\param [in]  data       The data container
      //-----------------------------------------------------
      CBindingQueryRequest(const shared::plugin::yPluginApi::IBindingQueryData& data,
                           communication::callback::ISynchronousCallback<boost::shared_ptr<shared::CDataContainer>>& callback);

      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CBindingQueryRequest();

      // IBindingQueryRequest implementation
      const shared::plugin::yPluginApi::IBindingQueryData& getData() override;
      void sendSuccess(const boost::shared_ptr<shared::CDataContainer>& newDeviceName) override;
      void sendError(const std::string& errorMessage) override;
      // [END] - IBindingQueryRequest implementation

   private:
      //-----------------------------------------------------
      ///\brief Internal data
      //-----------------------------------------------------
      boost::shared_ptr<communication::callback::ICallbackRequest<shared::plugin::yPluginApi::IBindingQueryData, boost::shared_ptr<shared::CDataContainer>> > m_requestPtr;
   };
} // namespace pluginSystem


