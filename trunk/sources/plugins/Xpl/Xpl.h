#pragma once
#include <shared/plugin/IPlugin.h>
#include "XplConfiguration.h"
#include "xplcore/XplMessage.h"
#include "xplrules/RulerFactory.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	This class is a simple plugin example
/// \note   This plugin do nothing useful, just demonstrate how to :
///         - use a configuration
///         - process a command from Yadoms
///         - send sensor state to Yadoms
//--------------------------------------------------------------
class CXpl : public shared::plugin::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CXpl();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CXpl();

   // IPlugin implementation
   virtual void doWork(boost::shared_ptr<yApi::IYadomsApi> context);
   // [END] IPlugin implementation

private:
   //----------------------------------------------
   ///\brief Function handler when receiving XplMessage
   ///\param [in] The xpl message received
   //----------------------------------------------
   void OnXplMessageReceived(xplcore::CXplMessage & message, boost::shared_ptr<yApi::IYadomsApi> context);

   //----------------------------------------------
   ///\brief  The plugin xpl device id
   //----------------------------------------------
   static std::string m_xpl_gateway_id;

   //----------------------------------------------
   ///\brief  The ruler factory
   //----------------------------------------------
   xplrules::CRulerFactory m_rulerFactory;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CXplConfiguration m_configuration;
};



