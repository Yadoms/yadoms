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
   explicit CDecoder();

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CDecoder();

   // IDecoder implementation
   std::map<std::string, boost::shared_ptr<equipments::IEquipment>> decodeDevicesMessage(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer& message) override;
   bool isFrameComplete(shared::CDataContainer& message) override;
   shared::CDataContainer getLastData(shared::CDataContainer& message) override;
   // [END] IDecoder implementation

private:

};
