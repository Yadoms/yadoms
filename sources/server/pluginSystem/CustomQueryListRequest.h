#pragma once

#include "communication/callback/ISynchronousCallback.h"
#include "communication/callback/ICallbackRequest.h"
#include <shared/plugin/yadomsApi/ICustomQueryListRequest.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief Class which handle CustomQueryListRequest
   ///       It provides data; and answers method 
   //-----------------------------------------------------
   class CCustomQueryListRequest  : public shared::plugin::yadomsApi::ICustomQueryListRequest
   {
   public:
      //-----------------------------------------------------
      ///\brief Constructor
      ///\param [in]  data       The data container
      //-----------------------------------------------------
      CCustomQueryListRequest(const shared::plugin::yadomsApi::ICustomQueryData & data, communication::callback::ISynchronousCallback< std::vector<std::string> > & callback);

      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CCustomQueryListRequest();
   
      // ICustomQueryListRequest implementation
      const shared::plugin::yadomsApi::ICustomQueryData& getData();
      void sendSuccess(const std::vector<std::string>& newDeviceName);
      void sendError(const std::string& errorMessage);
      // [END] - ICustomQueryListRequest implementation

   private:
      //-----------------------------------------------------
      ///\brief Internal data
      //-----------------------------------------------------
      boost::shared_ptr< communication::callback::ICallbackRequest<shared::plugin::yadomsApi::ICustomQueryData, std::vector<std::string> > > m_requestPtr;
   };   
   
} // namespace pluginSystem
	
	