#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IDevice.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

#define X24D_DI_QTY 24

namespace devices
{
   //-----------------------------------------------------
   ///\brief X24-D Extension
   //-----------------------------------------------------
   class CX24DDevice : public IDevice
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CX24DDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& device);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CX24DDevice();

   private:

   };
} // namespace devices