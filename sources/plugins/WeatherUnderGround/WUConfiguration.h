#pragma once

#include "IWUConfiguration.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CWUConfiguration : public IWUConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CWUConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWUConfiguration();

   // IWUConfiguration implementation
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual std::string getAPIKey() const;
   virtual std::string getLocalisation() const;
   bool IsWindEnabled (void) const;
   bool IsStandardInformationEnabled (void ) const;
   // [END] IWUConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;

   //--------------------------------------------------------------
   /// \brief	    State of each option
   //--------------------------------------------------------------

   bool m_isWindEnabled;
   bool m_isStandardInformationEnabled;
};