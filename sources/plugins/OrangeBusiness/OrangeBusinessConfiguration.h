#pragma once

#include "IOrangeBusinessConfiguration.h"


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class COrangeBusinessConfiguration : public IOrangeBusinessConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COrangeBusinessConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COrangeBusinessConfiguration();

   // ISigfoxConfiguration implementation
   void initializeWith(const shared::CDataContainerSharedPtr& data) override;
   const std::string getAPIKey() const override;
   // [END] ISigfoxConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};