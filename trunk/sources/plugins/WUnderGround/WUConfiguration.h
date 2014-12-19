#pragma once

#include "IWUConfiguration.h"


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CWUConfiguration : public IWUConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWUConfiguration();

   // IWUConfiguration implementation
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual std::string getAPIKey() const;
   virtual std::string getLocalisation() const;
   // [END] IWUConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};