#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IEquipment.h"
#include "../specificHistorizers/inputOutput.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

#define X8D_DI_QTY 8

namespace equipments
{
   //-----------------------------------------------------
   ///\brief X8-D Extension
   //-----------------------------------------------------
   class CX8DExtension : public IEquipment
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in]   api           Yadoms API
      ///\param[in] device          The device name
      ///\param[in] position        The position of the extension into the IPX800
      //-----------------------------------------------------
      CX8DExtension(boost::shared_ptr<yApi::IYPluginApi> api,
                    const std::string& device,
                    const int position);

      // IExtension implementation
      std::string getDeviceName() const override;
      std::string getDeviceType() const override;
      int getSlot() const override;
      void historizePendingCommand(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<const yApi::IDeviceCommand> command) override;
      void resetPendingCommand() override;
      void updateFromDevice(const std::string& type, boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainerSharedPtr& values, bool forceHistorization = false) override;
      shared::CDataContainerSharedPtr buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainerSharedPtr& parameters, boost::shared_ptr<const yApi::IDeviceCommand> command) override;
      void setNewConfiguration(const shared::CDataContainerSharedPtr& newConfiguration) override;
      // [END] IExtension implementation

      static const std::string& deviceType();

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
      ///\brief                     The slot position
      //-----------------------------------------------------
      int m_position;

      //-----------------------------------------------------
      ///\brief                     The list of keyword
      //-----------------------------------------------------
      std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> > m_keywordList;
   };
} // namespace equipments