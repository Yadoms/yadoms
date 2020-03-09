#pragma once

#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	An example of an enum type of parameter
//--------------------------------------------------------------
enum EEnumType
{
   kEnumValue1 = 7,
   kEnumValue2 = 12,
   kEnumValue3
};

//--------------------------------------------------------------
/// \brief	Configuration of the fake plugin
//--------------------------------------------------------------
class CFakePluginConfiguration
{
public:
   virtual ~CFakePluginConfiguration() = default;

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   void initializeWith(const shared::CDataContainerSharedPtr & data);

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   /// \param [in] packageJsonPath The package.json path
   //--------------------------------------------------------------
   void initializeWith(shared::CDataContainerSharedPtr data,
                       const boost::filesystem::path& packageJsonPath);

   //--------------------------------------------------------------
   /// \brief	    Just for test, not needed for real plugin
   //--------------------------------------------------------------
   void trace() const;

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // You can add your own accessors for better code readability
   //--------------------------------------------------------------
   //--------------------------------------------------------------

   //--------------------------------------------------------------
   /// \brief	    Enum parameter
   //--------------------------------------------------------------
   EEnumType getEnumParameter() const;

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_configuration;
};
