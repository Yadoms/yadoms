#pragma once

#include <plugin_IPC/plugin_IPC.h>
#include <shared/plugin/information/IInformation.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>


namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	yScriptApi context accessor, used by script to interact with Yadoms
   //--------------------------------------------------------------
   class IContextAccessor//TODO renommer (et son implémentation) en IPluginDialer ?
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IContextAccessor()
      {
      }

      //--------------------------------------------------------------
      /// \brief	Get the context accessor ID (unique on full system)
      /// \return The context accessor ID
      //--------------------------------------------------------------
      virtual std::string id() const = 0;

      //--------------------------------------------------------------
      /// \brief	Post the plugin stop request
      //--------------------------------------------------------------
      virtual void postStopRequest() = 0;

      //--------------------------------------------------------------
      /// \brief	Post the plugin information
      /// \param[in] information The plugin information
      //--------------------------------------------------------------
      virtual void postPluginInformation(boost::shared_ptr<const shared::plugin::information::IInformation> information) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post a custom query request to a plugin
      /// \param [in] request    Request data
      //--------------------------------------------------------------
      virtual void postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request) = 0;

   };
} // namespace pluginSystem

