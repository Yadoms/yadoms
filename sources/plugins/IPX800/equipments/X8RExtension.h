#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IExtension.h"
#include "../specificHistorizers/inputOutput.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

#define X8R_RELAY_QTY 8

namespace extensions
{
   //-----------------------------------------------------
   ///\brief X8-R Extension
   //-----------------------------------------------------
   class CX8RExtension : public IExtension
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CX8RExtension(boost::shared_ptr<yApi::IYPluginApi> api,
                    const std::string& device,
                    const int& position);

      // IExtension implementation
      std::string getDeviceName() const override;
      int getSlot() const override;
      void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer& values) const override;
      // [END] IExtension implementation

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CX8RExtension();

   private:

      //-----------------------------------------------------
      ///\brief                     The device name
      //-----------------------------------------------------
      std::string m_deviceName;

      //-----------------------------------------------------
      ///\brief                     The slot number
      //-----------------------------------------------------
      int m_slotNumber;

      //-----------------------------------------------------
      ///\brief                     The list of keyword
      //-----------------------------------------------------
      std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> > m_keywordList;
   };
} // namespace extensions