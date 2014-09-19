#pragma once

#include "IZWaveController.h"
#include "OpenZWaveCommandClass.h"
#include "CommandClasses.h"
#include "CommandClass.h"
#include "OpenZWaveNode.h"

class COpenZWaveController : public IZWaveController
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveController();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveController();

   // IZWaveController implementation
   virtual void configure(CZWaveConfiguration * configuration, shared::event::CEventHandler * handler);
   virtual bool start();
   virtual void stop();
   virtual void SendCommand(const std::string & device, const std::string & keyword, const std::string & value);
   // [END] IZWaveController implementation


   //-----------------------------------------------------------------------------
   /// \brief	Callback that is triggered when a value, group or node changes
   //-----------------------------------------------------------------------------
   void OnNotification(OpenZWave::Notification const* _notification, void* _context);

private:
   //-----------------------------------------------------------------------------
   /// \brief	Return the NodeInfo object associated with this notification
   //-----------------------------------------------------------------------------   
   boost::shared_ptr<COpenZWaveNode> GetNode(OpenZWave::Notification const* _notification);

   //-----------------------------------------------------------------------------
   /// \brief	Return the NodeInfo object matching homeId and nodeId
   //-----------------------------------------------------------------------------   
   boost::shared_ptr<COpenZWaveNode> GetNode(const int homeId, const int nodeId);

   //--------------------------------------------------------------
   /// \brief	   Generate the deviceId from homeId&nodeId
   /// \param     homeId	   the home id
   /// \param     nodeId	   the node id
   /// \return	   the device id
   //--------------------------------------------------------------
   std::string GenerateDeviceStringID(uint32 homeId, uint8 nodeId);

   //--------------------------------------------------------------
   /// \brief	   Generate the keywordId from label&instance
   /// \param     label	            the keyword label
   /// \param     commandClass	   the command class
   /// \return	   the keyword id
   //--------------------------------------------------------------
   std::string GenerateKeywordStringID(const std::string & label, const ECommandClass commandClass);

   
   //--------------------------------------------------------------
   /// \brief	   Retreive openzwave ids from yadoms data
   /// \param [in]    device	            the yadoms device
   /// \param [in]    keyword	            the yadoms keyword
   /// \param [out]   homeId	            the openzwave homeid
   /// \param [out]   nodeId	            the openzwave nodeId
   /// \param [out]   keywordName	      the openzwave keywordName
   /// \param [out]   keywordClass	      the openzwave keyword class
   /// \throw shared::exception::CException if any yadoms data do not match pattern
   /// \note:  patterns:   device = homeId.nodeId
   /// \note:  patterns:   keyword = keywordName.keywordClass
   //--------------------------------------------------------------
   void RetreiveOpenZWaveIds(const std::string & device, const std::string & keyword,
      int & homeId, uint8 & nodeId, std::string & keywordName, ECommandClass & keywordClass);

   //--------------------------------------------------------------
   /// \brief	   Mutex protecting the configuration content
   //--------------------------------------------------------------
   mutable boost::mutex m_treeMutex;


   int    m_homeId;
   bool   m_initFailed;
   bool   m_nodesQueried;

   typedef std::vector< boost::shared_ptr<COpenZWaveNode> > NodeListType;
   NodeListType   m_nodes;

   shared::event::CEventHandler * m_handler;
   CZWaveConfiguration * m_configuration;
};

