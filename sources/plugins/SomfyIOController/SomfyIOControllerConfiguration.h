#pragma once

#include "ISomfyIOControllerConfiguration.h"

//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class CSomfyIOControllerConfiguration : public ISomfyIOControllerConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSomfyIOControllerConfiguration();

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   void initializeWith(const shared::CDataContainer& data) override;
   std::string getSerialPort() const override;



private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};

