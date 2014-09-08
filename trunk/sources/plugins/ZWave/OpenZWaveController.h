#pragma once

#include "IZWaveController.h"

#include "Options.h"
#include "Manager.h"
#include "Driver.h"
#include "Node.h"
#include "Group.h"
#include "Notification.h"
#include "ValueStore.h"
#include "Value.h"
#include "ValueBool.h"
#include "Log.h"



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
   virtual bool start(CZWaveConfiguration & configuration, shared::event::CEventHandler & handler);
   virtual void stop();
   // [END] IZWaveController implementation


private:
   typedef struct
   {
      uint32			m_homeId;
      uint8			m_nodeId;
      bool			m_polled;
      std::vector<OpenZWave::ValueID>	m_values;
   }NodeInfo;

   //-----------------------------------------------------------------------------
   /// \brief	Return the NodeInfo object associated with this notification
   //-----------------------------------------------------------------------------   
   NodeInfo* GetNodeInfo(OpenZWave::Notification const* _notification);

public:
   //-----------------------------------------------------------------------------
   /// \brief	Callback that is triggered when a value, group or node changes
   //-----------------------------------------------------------------------------
   void OnNotification(OpenZWave::Notification const* _notification, void* _context);


private:
   //--------------------------------------------------------------
   /// \brief	   Mutex protecting the configuration content
   //--------------------------------------------------------------
   mutable boost::mutex m_treeMutex;


   int    m_homeId;
   bool   m_initFailed;
   bool   m_nodesQueried;
   std::vector<COpenZWaveController::NodeInfo*> m_nodes;
};

