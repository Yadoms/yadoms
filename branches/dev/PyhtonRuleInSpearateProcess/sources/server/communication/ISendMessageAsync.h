#pragma once

#include "callback/ISynchronousCallback.h"
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationData.h>
#include <shared/plugin/yPluginApi/IBindingQueryData.h>
#include <shared/DataContainer.h>

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
      ///\param [in] keywordId      Target keyword Id
      ///\param [in] body           The command body to send
      ///\throw shared::exception::CEmptyResult if keywordId not found
      //----------------------------------------------
      virtual void sendCommandAsync(int keywordId, const std::string & body) = 0;

      //----------------------------------------------
      ///\brief                     Send a manually device creation request to a plugin with a mandatory callback
      ///\param [in] pluginId       Plugin Id on which to create device
      ///\param [in] data           Reference to manually device creation data
      ///\param [in] callback       The callback
      //----------------------------------------------
      virtual void sendManuallyDeviceCreationRequest(int pluginId, const shared::plugin::yPluginApi::IManuallyDeviceCreationData & data, communication::callback::ISynchronousCallback<std::string> & callback) = 0;

		//----------------------------------------------
      ///\brief                     Send a binding query request to a plugin with a mandatory callback
      ///\param [in] pluginId       Plugin Id on which to create device
      ///\param [in] data           Reference to the binding query data
      ///\param [in] callback       The callback
      //----------------------------------------------
      virtual void sendBindingQueryRequest(int pluginId, const shared::plugin::yPluginApi::IBindingQueryData & data, communication::callback::ISynchronousCallback< shared::CDataContainer > & callback) = 0;
   };

} //namespace communication