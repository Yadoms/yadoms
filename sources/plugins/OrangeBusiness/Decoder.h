#pragma once

#include "IDecoder.h"
#include "IEquipment.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

enum commandState { //TODO : to be completed with other possible status ERROR => But this one is already defined
   SENT = 0
};

//--------------------------------------------------------------
/// \brief	This class implement the json message
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
   std::map<std::string, boost::shared_ptr<equipments::IEquipment>> decodeDevicesMessage(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<shared::CDataContainer>& message) override;
   bool isFrameComplete(boost::shared_ptr<shared::CDataContainer>& message) override;
   boost::shared_ptr<shared::CDataContainer> getLastData(boost::shared_ptr<shared::CDataContainer> & message) override;
   // [END] IDecoder implementation

private:

};
