#pragma once

#include "ISigfoxConfiguration.h"


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CSigfoxConfiguration : public ISigfoxConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CSigfoxConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSigfoxConfiguration();

   // ISigfoxConfiguration implementation
   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data) override;
   double getRssiMin() const override;
   double getRssiMax() const override;
   double getTensionMin() const override;
   double getTensionMax() const override;
   unsigned short getSocketPort() const override;
   // [END] ISigfoxConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   boost::shared_ptr<shared::CDataContainer> m_data;
};