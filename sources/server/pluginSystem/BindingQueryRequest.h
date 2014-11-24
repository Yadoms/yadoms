#pragma once

#include "communication/callback/ISynchronousCallback.h"
#include "communication/callback/ICallbackRequest.h"
#include <shared/plugin/yadomsApi/IBindingQueryRequest.h>

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
      CBindingQueryRequest(const shared::plugin::yadomsApi::IBindingQueryData & data, communication::callback::ISynchronousCallback< std::vector<std::string> > & callback);

      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CBindingQueryRequest();
   
      // IBindingQueryRequest implementation
      const shared::plugin::yadomsApi::IBindingQueryData& getData();
      void sendSuccess(const std::vector<std::string>& newDeviceName);
      void sendError(const std::string& errorMessage);
      // [END] - IBindingQueryRequest implementation

   private:
      //-----------------------------------------------------
      ///\brief Internal data
      //-----------------------------------------------------
      boost::shared_ptr< communication::callback::ICallbackRequest<shared::plugin::yadomsApi::IBindingQueryData, std::vector<std::string> > > m_requestPtr;
   };   
   
} // namespace pluginSystem
	
	