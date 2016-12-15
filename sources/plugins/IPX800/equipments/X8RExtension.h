#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IEquipment.h"
#include "../specificHistorizers/inputOutput.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

#define X8R_RELAY_QTY 8

namespace equipments
{
   //-----------------------------------------------------
   ///\brief X8-R Extension
   //-----------------------------------------------------
   class CX8RExtension : public IEquipment
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in]   api           Yadoms API
      ///\param[in] device          The device name
      ///\param[in] position        The position of the extension into the IPX800
      //-----------------------------------------------------
      CX8RExtension(boost::shared_ptr<yApi::IYPluginApi> api,
                    const std::string& device,
                    const int position);

      // IExtension implementation
      std::string getDeviceName() const override;
      std::string getDeviceType() const override;
      int getSlot() const override;
      void updateFromDevice(const std::string& type, boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer& value, bool forceHistorization = false) override;
      void historizePendingCommand(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<const yApi::IDeviceCommand> command) override;
      void resetPendingCommand() override;
      shared::CDataContainer buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer& parameters, boost::shared_ptr<const yApi::IDeviceCommand> command) override;
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
      ///\brief                     The device type
      //-----------------------------------------------------
      std::string m_deviceType;

      //-----------------------------------------------------
      ///\brief                     The slot position
      //-----------------------------------------------------
      int m_position;

      //-----------------------------------------------------
      ///\brief                     The list of keyword
      //-----------------------------------------------------
      std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> > m_keywordList;

      //-----------------------------------------------------
      ///\brief                     Waiting command
      //-----------------------------------------------------
      boost::shared_ptr<specificHistorizers::CInputOuput> m_pendingHistorizer;
   };
} // namespace equipments