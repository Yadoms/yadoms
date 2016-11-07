#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IExtension.h"
#include "../specificHistorizers/inputOutput.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

#define X8D_DI_QTY 8

namespace extensions
{
   //-----------------------------------------------------
   ///\brief X8-D Extension
   //-----------------------------------------------------
   class CX8DExtension : public IExtension
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CX8DExtension(boost::shared_ptr<yApi::IYPluginApi> api,
                    const std::string& device,
                    const int& position,
                    std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >& DIList);

      // IExtension implementation
      std::string getDeviceName() const override;
      int getSlot() const override;
      // [END] IExtension implementation

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CX8DExtension();

   private:

      //-----------------------------------------------------
      ///\brief                     The device name
      //-----------------------------------------------------
      std::string m_deviceName;

      //-----------------------------------------------------
      ///\brief                     The device name
      //-----------------------------------------------------
      int m_slotNumber;
   };
} // namespace extensions