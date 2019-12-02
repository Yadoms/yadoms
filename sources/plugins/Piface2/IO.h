#pragma once

#include "IIO.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include "IPf2Configuration.h"
#include "eventDefinitions.h"
#include <boost/thread.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

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
   //--------------------------------------------------------------
   explicit CIO(const std::string& keywordName,
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
   //--------------------------------------------------------------
   void set(bool state);

   //--------------------------------------------------------------
   /// \brief	    get
   /// \note       read the state of the IO
   /// \return     state of the IO
   //--------------------------------------------------------------
   bool get(void);

protected:

private:

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_value;
   
   //--------------------------------------------------------------
   /// \brief	The port used in the Pi2/Pi3
   //--------------------------------------------------------------
   int m_portUsed;
};