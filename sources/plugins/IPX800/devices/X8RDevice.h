#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IDevice.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

#define X8R_RELAY_QTY 8

namespace devices
{
   //-----------------------------------------------------
   ///\brief X8-R Extension
   //-----------------------------------------------------
   class CX8RDevice : public IDevice
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CX8RDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& device);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CX8RDevice();

   private:

   };
} // namespace devices