#pragma once

#include <shared/communication/callback/ISynchronousCallback.h>
#include <shared/communication/callback/ICallbackRequest.h>
#include "IManuallyDeviceCreationData.h"

namespace shared { namespace plugin { namespace yadomsApi
{
   typedef  boost::shared_ptr< shared::communication::callback::ICallbackRequest<shared::plugin::yadomsApi::IManuallyDeviceCreationData, std::string> > CManuallyDeviceCreationRequest;
   
   
} } } // namespace shared::plugin::yadomsApi	
	
	