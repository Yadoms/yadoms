#pragma once

#include "IIO.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/event/EventHandler.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	the structure sent with the event
//--------------------------------------------------------------
typedef struct CIOState
{
   int pin;
   boost::shared_ptr<yApi::historization::IHistorizable> keyword;
}IOState;

//--------------------------------------------------------------
/// \brief	All IOs operations
//--------------------------------------------------------------
class CIO : public IIO
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] keywordName   The keyword name
   /// \param[in] pin           The pin number for the initialization
   /// \param[in] configuration The configuration of the pin
   //--------------------------------------------------------------
   explicit CIO(const std::string& keywordName, 
                const int pin, 
                const yApi::EKeywordAccessMode& accessMode);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CIO();

   // ILoad Implementation
   boost::shared_ptr<const yApi::historization::IHistorizable> historizable() const override;
   // [END] ILoad Implementation

   // TODO : A mettre éventuellement dans l'IIO
   //--------------------------------------------------------------
   /// \brief	                     Subscribe to the connection/disconnection events
   /// \param [in] forEventHandler  The event handler to notify for these events
   /// \param [in] forId            The event id to send for these events (set kNoEvent to unsubscribe)
   /// \throw exception::CInvalidParameter if try to subscribe on event for which a subscription already exists (user must unsubscribe first)
   /// \note The raised event contains a bool data : true if port was connected, false if port was disconnected
   /// \note Must be called before start
   //--------------------------------------------------------------
   void subscribeForConnectionEvents(shared::event::CEventHandler& forEventHandler, int forId);

   void write(bool state);

private:

   //--------------------------------------------------------------
   /// \brief	    function used to post a message
   //--------------------------------------------------------------
   void notifyEventHandler(void);

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_value;

   //--------------------------------------------------------------
   /// \brief	The event handler to notify for events ( interrupt due to change value )
   //--------------------------------------------------------------
   shared::event::CEventHandler* m_InterruptEventHandler;

   //--------------------------------------------------------------
   /// \brief	The event id to notify for events ( interrupt due to change value )
   //--------------------------------------------------------------
   int m_InterruptEventId;

   //--------------------------------------------------------------
   /// \brief	The port used in the Pi2
   //--------------------------------------------------------------
   int m_portUsed;

   bool m_Writable;
};