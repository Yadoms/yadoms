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
   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data) override;
   std::string getSerialPort() const override;


private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   boost::shared_ptr<shared::CDataContainer> m_data;
};
