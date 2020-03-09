#pragma once
#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IEquipment.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The Decoder protocol interface
//--------------------------------------------------------------
class IDecoder
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IDecoder()
   {
   }

   //--------------------------------------------------------------
   /// \brief	                    Decode Devices message
   /// \param [in] api             Plugin execution context (Yadoms API)
   /// \param [in] message         the received message
   //--------------------------------------------------------------
   virtual std::map<std::string, boost::shared_ptr<equipments::IEquipment>> decodeDevicesMessage(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainerSharedPtr& message) = 0;

   //--------------------------------------------------------------
   /// \brief	                    return if we need to ask the server for new frames
   /// \param [in] message         the received message
   //--------------------------------------------------------------
   virtual bool isFrameComplete(shared::CDataContainerSharedPtr& message) = 0;

   //--------------------------------------------------------------
   /// \brief	                    return if we need to ask the server for new frames
   /// \param [in] message         the received message
   //--------------------------------------------------------------
   virtual shared::CDataContainerSharedPtr getLastData(shared::CDataContainerSharedPtr& message) = 0;
};