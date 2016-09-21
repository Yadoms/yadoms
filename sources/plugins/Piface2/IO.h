#pragma once

#include "IIO.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/event/EventHandler.hpp>
#include "IPf2Configuration.h"
#include "eventDefinitions.h"
#include <boost/thread.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	the structure sent with the event
//--------------------------------------------------------------
typedef struct CIOState
{
   int pin;
   std::string keywordName;
   bool value;
}IOState;

//--------------------------------------------------------------
/// \brief	All IOs operations
//--------------------------------------------------------------
class CIO : public IIO
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] keywordName           The keyword name
   /// \param[in] pin                   The pin number for the initialization
   /// \param[in] pullResistanceState   Pullup resistance is enabled
   /// \param[in] accessMode            access R/W of the IO
   /// \param[in] interruptEventHandler interrupt Event handler to read values.
   //--------------------------------------------------------------
   explicit CIO(const std::string& keywordName,
                const int pin,
                const EPullResistance pullResistanceState, 
                const yApi::EKeywordAccessMode& accessMode,
                shared::event::CEventHandler& interruptEventHandler);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CIO();

   // ILoad Implementation
   boost::shared_ptr<const yApi::historization::IHistorizable> historizable() const override;
   // [END] ILoad Implementation

   //--------------------------------------------------------------
   /// \brief	    ConfigurePullResistance
   /// \param[in] pullResistanceState state of the pull resistanceord
   //--------------------------------------------------------------
   void ConfigurePullResistance(const EPullResistance pullResistanceState);

   //--------------------------------------------------------------
   /// \brief	    Set
   /// \param[in] state          the new state of the IO
   /// \param[in] boardAccess    writing the value to the board is necessary
   //--------------------------------------------------------------
   void set(bool state, bool boardAccess);

   //--------------------------------------------------------------
   /// \brief	    get
   /// \note       read the state of the IO
   /// \return     state of the IO
   //--------------------------------------------------------------
   bool readHardware(void);

protected:

   //--------------------------------------------------------------
   /// \brief	    writeHardware
   /// \param[in] state          the new state of the IO when writing to the physical board
   //--------------------------------------------------------------
   void writeHardware(bool state);

private:

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_value;

   //--------------------------------------------------------------
   /// \brief	The event handler to notify for events ( interrupt due to change value )
   //--------------------------------------------------------------
   shared::event::CEventHandler& m_InterruptEventHandler;

   //--------------------------------------------------------------
   /// \brief	The port used in the Pi2/Pi3
   //--------------------------------------------------------------
   int m_portUsed;

   //--------------------------------------------------------------
   ///\brief               The thread used to receive messages from Yadoms
   //--------------------------------------------------------------
   boost::thread m_interruptReceiverThread;

   //--------------------------------------------------------------
   ///\brief               The thread function to receive interrupts from the component
   //--------------------------------------------------------------
   void interruptReceiverThreaded(const int portUsed, const std::string& keywordName) const;
};
