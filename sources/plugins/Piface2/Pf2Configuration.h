#pragma once
#include "IPf2Configuration.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CPf2Configuration : public IPf2Configuration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CPf2Configuration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CPf2Configuration();

   // IPf2Configuration implementation
   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data) override;
   EPullResistance PullResistanceState(int pin) const override;
   // [END] IPf2Configuration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   boost::shared_ptr<shared::CDataContainer> m_data;
};

