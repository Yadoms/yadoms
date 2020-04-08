#pragma once

#include "ISomfySituoConfiguration.h"

//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class CSomfySituoConfiguration : public ISomfySituoConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CSomfySituoConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSomfySituoConfiguration();

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   void initializeWith(const shared::CDataContainerSharedPtr& data) override;
   std::string getSerialPort() const override;


private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   shared::CDataContainerSharedPtr m_data;
};
