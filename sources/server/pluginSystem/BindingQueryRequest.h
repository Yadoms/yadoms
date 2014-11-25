#pragma once

#include "communication/callback/ISynchronousCallback.h"
#include "communication/callback/ICallbackRequest.h"
#include <shared/plugin/yadomsApi/IBindingQueryRequest.h>
#include <shared/DataContainer.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief Class which handle BindingQueryRequest
   ///       It provides data; and answers method 
   //-----------------------------------------------------
   class CBindingQueryRequest  : public shared::plugin::yadomsApi::IBindingQueryRequest
   {
   public:
      //-----------------------------------------------------
      ///\brief Constructor
      ///\param [in]  data       The data container
      //-----------------------------------------------------
      CBindingQueryRequest(const shared::plugin::yadomsApi::IBindingQueryData & data, communication::callback::ISynchronousCallback< shared::CDataContainer > & callback);

      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CBindingQueryRequest();
   
      // IBindingQueryRequest implementation
      const shared::plugin::yadomsApi::IBindingQueryData& getData();
      void sendSuccess(const shared::CDataContainer& newDeviceName);
      void sendError(const std::string& errorMessage);
      // [END] - IBindingQueryRequest implementation

   private:
      //-----------------------------------------------------
      ///\brief Internal data
      //-----------------------------------------------------
      boost::shared_ptr< communication::callback::ICallbackRequest<shared::plugin::yadomsApi::IBindingQueryData, shared::CDataContainer > > m_requestPtr;
   };   
   
} // namespace pluginSystem
	
	