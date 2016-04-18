//
// Instance.h
//
// yadoms-plugin instance handler
//
#pragma once

#include <shared/process/IProcess.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/information/IInformation.h>
#include <shared/plugin/yPluginApi/IExtraCommand.h>
#include "IInstance.h"
#include "yPluginApiImplementation.h"

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	this class is used to manage a plugin instance. 
   //--------------------------------------------------------------
   class CInstance : public IInstance
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param [in]	pluginInformation          Information on the plugin
      /// \param [in]   process                    The instance process
      /// \param [in]   yPluginApi                 The api context accessor
      //--------------------------------------------------------------
      CInstance(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                boost::shared_ptr<shared::process::IProcess> process,
                boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> yPluginApi);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CInstance();


        //--------------------------------------------------------------
      /// \brief			            Post an extra command to the plugin
      /// \param  command           Extra command to post
      //--------------------------------------------------------------
      virtual void postExtraCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IExtraCommand> extraCommand) const;

      // IInstance Implementation
      void updateConfiguration(const shared::CDataContainer& newConfiguration) const override;
      void requestStop() override;
      void kill() override;
      std::string getPluginName() const override;
      // [END] IInstance Implementation


   protected:
      void postStopRequest() const;

      //TODO
      //    //--------------------------------------------------------------
      //    /// \brief			            Post a command to the plugin
      //    /// \param  command           Command to post
      //    //--------------------------------------------------------------
      //    virtual void postCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command) const;

      //    //--------------------------------------------------------------
      //    /// \brief			            Post a manually device creation request to the plugin
      //    /// \param  request           Request data
      //    //--------------------------------------------------------------
      //    virtual void postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> & request) const;

      ////--------------------------------------------------------------
      ///// \brief                 Post a custom query request to a plugin
      ///// \param [in] request    Request data
      ////--------------------------------------------------------------
      //    virtual void postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> & request) const;

   private:
      //--------------------------------------------------------------
      /// \brief			      The plugin information
      //--------------------------------------------------------------
      const boost::shared_ptr<const shared::plugin::information::IInformation> m_pluginInformation;

      //-----------------------------------------------------
      ///\brief               The script process
      //-----------------------------------------------------
      boost::shared_ptr<shared::process::IProcess> m_process;

      //-----------------------------------------------------
      ///\brief               The api context accessor
      //-----------------------------------------------------
      boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> m_yPluginApi;
   };
} // namespace pluginSystem
