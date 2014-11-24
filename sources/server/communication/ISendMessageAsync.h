#pragma once

#include "callback/ISynchronousCallback.h"
#include <shared/plugin/yadomsApi/IManuallyDeviceCreationData.h>
#include <shared/plugin/yadomsApi/IBindingQueryData.h>

namespace communication {

   //----------------------------------------------
   ///\brief Interface used to send a command
   //----------------------------------------------
   class ISendMessageAsync
   {
   public:
      //----------------------------------------------
      ///\brief Destructor
      //----------------------------------------------
      virtual ~ISendMessageAsync()
      {
      }

      //----------------------------------------------
      ///\brief                     Send a command
      ///\param [in] deviceId       Target device Id
      ///\param [in] keywordId      Target keyword Id
      ///\param [in] body           The command body to send
      //----------------------------------------------
      virtual void sendCommandAsync(int deviceId, int keywordId, const shared::CDataContainer & body) = 0;

      //----------------------------------------------
      ///\brief                     Send a manually device creation request to a plugin with a mandatory callback
      ///\param [in] pluginId       Plugin Id on which to create device
      ///\param [in] data           Reference to manually device creation data
      ///\param [in] callback       The callback
      //----------------------------------------------
      virtual void sendManuallyDeviceCreationRequest(int pluginId, const shared::plugin::yadomsApi::IManuallyDeviceCreationData & data, communication::callback::ISynchronousCallback<std::string> & callback) = 0;

		//----------------------------------------------
      ///\brief                     Send a binding query request to a plugin with a mandatory callback
      ///\param [in] pluginId       Plugin Id on which to create device
      ///\param [in] data           Reference to the binding query data
      ///\param [in] callback       The callback
      //----------------------------------------------
      virtual void sendBindingQueryRequest(int pluginId, const shared::plugin::yadomsApi::IBindingQueryData & data, communication::callback::ISynchronousCallback< std::vector<std::string> > & callback) = 0;
   };

} //namespace communication