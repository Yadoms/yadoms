#pragma once

#include "ISIConfiguration.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CSIConfiguration : public ISIConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CSIConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSIConfiguration();

   // ISIConfiguration implementation
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual bool IsAdvancedEnabled (void) const;
   // [END] ISIConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;

   //--------------------------------------------------------------
   /// \brief	    State of each option
   //--------------------------------------------------------------
   bool m_isAdvancedEnabled;
};