#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IDevice.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

#define X8D_DI_QTY 8

namespace devices
{
   //-----------------------------------------------------
   ///\brief X8-D Extension
   //-----------------------------------------------------
   class CX8DDevice : public IDevice
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CX8DDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& device);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CX8DDevice();

   private:

   };
} // namespace devices