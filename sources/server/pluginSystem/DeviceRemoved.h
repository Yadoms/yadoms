#pragma once
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IDeviceRemoved implementation
   //-----------------------------------------------------
   class CDeviceRemoved : public shared::plugin::yPluginApi::IDeviceRemoved
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] device       The removed device
      ///\param[in] details      The removed device details
      //-----------------------------------------------------
      CDeviceRemoved(const std::string& device,
                     const shared::CDataContainer& details);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CDeviceRemoved();

      // IDeviceRemoved implementation
      const std::string& device() const override;
      const shared::CDataContainer& details() const override;
      // [END] IDeviceRemoved implementation

   private:
      //-----------------------------------------------------
      ///\brief               Device
      //-----------------------------------------------------
      std::string m_device;

      //-----------------------------------------------------
      ///\brief               Device details
      //-----------------------------------------------------
      shared::CDataContainer m_details;
   };
} // namespace pluginSystem	


