#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	The enum state disable/pullup/pulldown
//--------------------------------------------------------------
enum EPullResistance
{
   kDisable,
   kPullUp,
   kPullDown
};

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IPf2Configuration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IPf2Configuration()
   {
   }

   //--------------------------------------------------------------
   /// \brief      Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data) = 0;

   //--------------------------------------------------------------
   /// \brief      return if the pullupof a pin is enable
   /// \return     the state of the pin
   //--------------------------------------------------------------
   virtual EPullResistance PullResistanceState(int pin) const = 0;
};
