#pragma once

#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

class CTemperatureSensor
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] keywordName The keyword name
   //--------------------------------------------------------------
   explicit CTemperatureSensor(const std::string& keywordName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CTemperatureSensor();

   // ILoad Implementation
   void read() override;

   boost::shared_ptr<const yApi::historization::IHistorizable> historizable() const override
   {
      return m_keyword;
   }

   // [END] ILoad Implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CTemperature> m_keyword;
};

