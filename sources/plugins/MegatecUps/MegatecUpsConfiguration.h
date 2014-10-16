#pragma once

#include "IMegatecUpsConfiguration.h"


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CMegatecUpsConfiguration : public IMegatecUpsConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CMegatecUpsConfiguration();

   // IMegatecUpsConfiguration implementation
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual std::string getSerialPort() const;
   virtual bool upsBeepEnable() const;
   virtual EPowerFailureManagement powerFailureManagement() const;
   virtual unsigned int powerFailureFilterDelay() const;
   virtual unsigned int powerFailureRemainingBatteryThreshold() const;
   virtual double outuputShutdownDelay() const;
   virtual double outuputRestoreDelay() const;
   // [END] IMegatecUpsConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};

