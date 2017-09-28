#pragma once

#include "IDecoder.h"
#include "IEquipment.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class implement the TeleInfo protocol
//--------------------------------------------------------------
class CDecoder : public IDecoder
{
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   //--------------------------------------------------------------
   explicit CDecoder(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CDecoder();

   // IDecoder implementation
   void decodeDevicesMessage(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer message) override;
   bool isFrameComplete(shared::CDataContainer message) override;
   std::vector<boost::shared_ptr<equipments::IEquipment>> getDevices() override;
   std::string getLastData(shared::CDataContainer message) override;
   std::string getLastDataReceivedDate(shared::CDataContainer message) override;
   // [END] IDecoder implementation

private:

   std::vector<boost::shared_ptr<equipments::IEquipment>> m_equipments; // TODO : to create with a map
};
