#pragma once

#include <shared/DataContainer.h>
#include <Poco/Net/NetworkInterface.h>


//--------------------------------------------------------------
/// \brief	Configuration of the xpl plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CRfxLanXplConfiguration
{
public:
   ~CRfxLanXplConfiguration() = default;

   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& configuration);

   //--------------------------------------------------------------
   /// \brief	    Determine if an xplhub should be ran at start
   //--------------------------------------------------------------
   bool getStartXplhub() const;

   //--------------------------------------------------------------
   /// \brief	    Get the xpl network interface to use
   //--------------------------------------------------------------
   Poco::Net::NetworkInterface getXplNetworkInterface() const;

private:
   shared::CDataContainer m_contener;
};
