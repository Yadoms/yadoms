//
// Instance.h
//
// yadoms-plugin instance handler
//
#pragma once

#include <shared/process/IRunner.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include "IInstance.h"
#include "ILibrary.h"

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
      /// \param [in]   runner                     The instance runner
      //--------------------------------------------------------------
      CInstance(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
         boost::shared_ptr<shared::process::IRunner> runner);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CInstance();


      // IInstance Implementation
      void updateConfiguration(const shared::CDataContainer & newConfiguration) const override;
      void requestStop() override;
      std::string getPluginName() const override;
      // [END] IInstance Implementation



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
      ///\brief               The script runner
      //-----------------------------------------------------
      boost::shared_ptr<shared::process::IRunner> m_runner;
   };

} // namespace pluginSystem
