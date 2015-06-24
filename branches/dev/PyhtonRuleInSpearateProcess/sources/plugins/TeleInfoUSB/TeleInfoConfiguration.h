#pragma once

#include "ITeleInfoConfiguration.h"


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CTeleInfoConfiguration : public ITeleInfoConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------

	CTeleInfoConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CTeleInfoConfiguration();

   // ITeleInfoConfiguration implementation
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual std::string getSerialPort() const;
   // [END] ITeleInfoConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};