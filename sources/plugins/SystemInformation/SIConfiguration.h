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
   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data) override;
   // [END] ISIConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   boost::shared_ptr<shared::CDataContainer> m_data;
};

