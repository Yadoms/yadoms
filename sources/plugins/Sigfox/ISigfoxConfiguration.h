#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class ISigfoxConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~ISigfoxConfiguration()
   {
   }

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer& data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the TeleInfo reader module
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual double getRssiMin() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the TeleInfo reader module
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual double getRssiMax() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the TeleInfo reader module
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual double getTensionMin() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the TeleInfo reader module
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual double getTensionMax() const = 0;
};

