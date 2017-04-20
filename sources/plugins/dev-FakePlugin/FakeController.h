#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

DECLARE_ENUM_HEADER(EFakeControllerValues,
   ((Stop))
   ((Run))
   ((Back))
   ((Left))
   ((Right))
)

class CControllerValue : public yApi::historization::CSingleHistorizableData<EFakeControllerValues>
{
public:
   //-----------------------------------------------------
   ///\brief                     Constructor
   ///\param[in] keywordName     Yadoms keyword name
   //-----------------------------------------------------
   explicit CControllerValue(const std::string& keywordName);

   //-----------------------------------------------------
   ///\brief                     Destructor
   //-----------------------------------------------------
   virtual ~CControllerValue();
};

//--------------------------------------------------------------
/// \brief	Fake controller
/// \note   Use to simulate a controller based on an enuemration value
//--------------------------------------------------------------
class CFakeController
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceName    The device name
   //--------------------------------------------------------------
   explicit CFakeController(const std::string& deviceName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakeController();

   //--------------------------------------------------------------
   /// \brief	            Declare device and associated keywords if necessary
   /// \param[in] api      yPluginApi context
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	    Make a sensor read (compute new values)
   //--------------------------------------------------------------
   void read() const;

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
   /// \brief	    Returns the fake sensor type
   /// \return     The fake sensor type
   //--------------------------------------------------------------
   static const std::string& getType();

   //--------------------------------------------------------------
   /// \brief	    Returns the fake sensor model
   /// \return     The fake sensor model
   //--------------------------------------------------------------
   static const std::string& getModel();

private:
   //--------------------------------------------------------------
   /// \brief	    Device name
   //--------------------------------------------------------------
   const std::string m_deviceName;


   //--------------------------------------------------------------
   /// \brief	    Current values
   //--------------------------------------------------------------
   boost::shared_ptr<CControllerValue> m_currentValues;
};

