#pragma once

#include "IIO.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/event/EventHandler.hpp>
#include "IPf2Configuration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//TODO : Ajouter les pull-up, ainsi que dans la configuration du plugin, pour les 8 IOs, indépendamment.

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
   /// \param[in] keywordName   The keyword name
   /// \param[in] pin           The pin number for the initialization
   /// \param[in] pullupEnabled Pullup resistance is enabled
   /// \param[in] configuration The configuration of the pin
   //--------------------------------------------------------------
   explicit CIO(const std::string& keywordName,
                const int baseAddress,
                const int pin,
                const EPullResistance pullResistanceState, 
                const yApi::EKeywordAccessMode& accessMode);

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

protected:
   void writeHardware(bool state);

private:

   //std::string m_keywordName;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_value;

   //--------------------------------------------------------------
   /// \brief	The event handler to notify for events ( interrupt due to change value )
   //--------------------------------------------------------------
   //shared::event::CEventHandler* m_InterruptEventHandler;

   //--------------------------------------------------------------
   /// \brief	The event id to notify for events ( interrupt due to change value )
   //--------------------------------------------------------------
   //int m_InterruptEventId;

   //--------------------------------------------------------------
   /// \brief	The port used in the Pi2
   //--------------------------------------------------------------
   int m_portUsed;
};
