#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Fake counter
/// \note   Use to simulate a counter, with 2 variables values :
///         - incrementCount : return from 0 to 15 at each read
///         - totalCount : increment from 0 to 15 at each read
//--------------------------------------------------------------
class CFakeCounter
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceName    The device name
   //--------------------------------------------------------------
   explicit CFakeCounter(const std::string& deviceName, const shared::CDataContainer & deviceConfiguration = shared::CDataContainer::EmptyContainer);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakeCounter();

   //--------------------------------------------------------------
   /// \brief	            Declare device and associated keywords if necessary
   /// \param[in] api      yPluginApi context
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	    Make a sensor read (compute new values)
   //--------------------------------------------------------------
   void read();

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \param[in] api      yPluginApi context to which historize data
   //--------------------------------------------------------------
   void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	    Returns the fake sensor device name
   /// \return     The device name
   //--------------------------------------------------------------
   const std::string& getDeviceName() const;

   //--------------------------------------------------------------
   /// \brief	    Returns the fake sensor model
   /// \return     The list of device capacities
   //--------------------------------------------------------------
   static const std::string& getModel();

private:
   //--------------------------------------------------------------
   /// \brief	    Device name
   //--------------------------------------------------------------
   const std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	The device configuration
   //--------------------------------------------------------------
   shared::CDataContainer m_deviceConfiguration;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with incremental counter
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CCounter> m_incrementCount;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with total counter
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CCounter> m_totalCount;

   //--------------------------------------------------------------
   /// \brief	Historizers vector
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> m_historizers;

   //--------------------------------------------------------------
   /// \brief	    Random number generator, used to simulate increments
   //--------------------------------------------------------------
   boost::random::mt19937 m_gen;
   boost::random::uniform_int_distribution<> m_dist;
};

