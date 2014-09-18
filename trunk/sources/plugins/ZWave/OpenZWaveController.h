#pragma once

#include "IZWaveController.h"
#include "OpenZWaveCommandClass.h"
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
#include "CommandClasses.h"
#include "CommandClass.h"

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
   typedef struct
   {
      std::list<OpenZWave::ValueID>	Values;
      time_t							m_LastSeen;
   }NodeCommandClass;


   typedef struct
   {
      uint32			m_homeId;
      uint8			m_nodeId;
      bool			m_polled;
      time_t							m_LastSeen;
      
      std::map<int, std::map<int, NodeCommandClass> >	Instances;
   }NodeInfo;



   class CZWNodeCapacity
   {
   public:
      CZWNodeCapacity(const int homeId, const int nodeId, const ECommandClass classIdentifier)
         :m_classIdentifier(classIdentifier), m_class(OpenZWave::CommandClasses::CreateCommandClass(classIdentifier, homeId, nodeId))
      {
         
      }

      boost::shared_ptr<OpenZWave::CommandClass> get() 
      {
         return m_class;
      }

      ECommandClass getClassIdentifier()
      {
         return m_classIdentifier;
      }


      OpenZWave::ValueID& getValueId(const std::string & keyword)
      {
         KeywordsContainer::iterator i;
         for (i = m_keywords.begin(); i != m_keywords.end(); ++i)
         {
            if (i->first == keyword)
               return i->second;
         }
         throw shared::exception::CException("The keyword don't exists");
      }

      void registerKeyword(const std::string & keyword, OpenZWave::ValueID & value)
      {
         KeywordsContainer::iterator i;
         bool alreadyExists = false;
         for (i = m_keywords.begin(); i!= m_keywords.end(); ++i)
         {
            if (i->first == keyword)
            {
               i->second = value;
               alreadyExists = true;
            }
         }
         if (!alreadyExists)
            m_keywords.push_back(std::make_pair(keyword, value));
      }

   private:
      ECommandClass m_classIdentifier;
      boost::shared_ptr<OpenZWave::CommandClass> m_class;
      typedef std::vector< std::pair<std::string, OpenZWave::ValueID> > KeywordsContainer;
      KeywordsContainer m_keywords;
   };



   class CZWNode
   {
   public:
      CZWNode(const int homeId, const int nodeId)
         :m_homeId(homeId), m_nodeId(nodeId)
      {

      }

      boost::shared_ptr<CZWNodeCapacity> getCapacity(ECommandClass classIdentifier) 
      {
         if (m_nodeCapacities.find(classIdentifier) == m_nodeCapacities.end())
            registerCapacity(classIdentifier);
         return m_nodeCapacities[classIdentifier];
      }

      void registerCapacity(ECommandClass classIdentifier)
      {
         if (m_nodeCapacities.find(classIdentifier) == m_nodeCapacities.end())
            m_nodeCapacities[classIdentifier] = boost::shared_ptr<CZWNodeCapacity>(new CZWNodeCapacity(m_homeId, m_nodeId, classIdentifier));
         else
         {
            //already exists !!!
            //throw shared::exception::CException("The capacity already exists");
         }
      }

      void registerKeyword(ECommandClass classIdentifier, const std::string & keyword, OpenZWave::ValueID & value)
      {
         getCapacity(classIdentifier)->registerKeyword(keyword, value);
      }

      OpenZWave::ValueID& getValueId(ECommandClass classIdentifier, const std::string & keyword)
      {
         return getCapacity(classIdentifier)->getValueId(keyword);
      }

      const bool match(const int homeId, const int nodeId)
      {
         return m_homeId == homeId && m_nodeId == nodeId;
      }

      const uint32	getHomeId() { return m_homeId; }
      const uint8		getNodeId() { return m_nodeId; }

   private:
      uint32		m_homeId;
      uint8			m_nodeId;

      std::map<ECommandClass, boost::shared_ptr<CZWNodeCapacity> > m_nodeCapacities;
   };



   //-----------------------------------------------------------------------------
   /// \brief	Return the NodeInfo object associated with this notification
   //-----------------------------------------------------------------------------   
   NodeInfo* GetNodeInfo(OpenZWave::Notification const* _notification);
   boost::shared_ptr<CZWNode> GetNode(OpenZWave::Notification const* _notification);

   //-----------------------------------------------------------------------------
   /// \brief	Return the NodeInfo object matching homeId and nodeId
   //-----------------------------------------------------------------------------   
   NodeInfo* GetNodeInfo(const int homeId, const int nodeId);
   boost::shared_ptr<CZWNode> GetNode(const int homeId, const int nodeId);



   //--------------------------------------------------------------
   /// \brief	   Generate the deviceId from homeId&nodeId
   /// \param     homeId	   the home id
   /// \param     nodeId	   the node id
   /// \return	   the device id
   //--------------------------------------------------------------
   std::string GenerateDeviceStringID(uint32 homeId, uint8 nodeId);

   //--------------------------------------------------------------
   /// \brief	   Generate the keywordId from label&instance
   /// \param     label	      the keyword label
   /// \param     instance	   the keyword instance
   /// \return	   the keyword id
   //--------------------------------------------------------------
   std::string GenerateKeywordStringID(std::string label, uint8 instance);

   bool GetValueByCommandClass(const int homeId, const int nodeID, const int instanceID, const int commandClass, OpenZWave::ValueID &nValue);


   //--------------------------------------------------------------
   /// \brief	   Mutex protecting the configuration content
   //--------------------------------------------------------------
   mutable boost::mutex m_treeMutex;


   int    m_homeId;
   bool   m_initFailed;
   bool   m_nodesQueried;
   std::vector<COpenZWaveController::NodeInfo*> m_nodes;

   typedef std::vector< boost::shared_ptr<CZWNode> > NodeListType;
   NodeListType   m_allNodes;

   shared::event::CEventHandler * m_handler;
   CZWaveConfiguration * m_configuration;
};

