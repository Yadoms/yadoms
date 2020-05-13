#pragma once
#include <shared/DataContainer.h>
#include "PairingHelper.h"

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IConfiguration()
   {
   }

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const boost::shared_ptr<shared::CDataContainer> & data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the dongle
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual std::string getSerialPort() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Pairing mode
   /// \return     The pairing mode
   //--------------------------------------------------------------
   virtual CPairingHelper::EPairingMode getPairingMode() const = 0;
};

