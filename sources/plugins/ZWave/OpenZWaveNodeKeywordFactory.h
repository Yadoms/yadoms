#pragma once

#include "OpenZWaveCommandClass.h"
#include <value_classes/Value.h>
#include "IOpenZWaveNodeKeyword.h"
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <Notification.h>

class COpenZWaveNodeKeywordFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	      Create a keyword form its commandClass
   /// \param [in]   commandClass      the command class
   /// \param [in]   keyword           The keyword name
   /// \param [in]   value             the value id
   /// \return       the keyword
   //--------------------------------------------------------------
   static boost::shared_ptr<IOpenZWaveNodeKeyword> createKeyword(ECommandClass commandClass, const std::string & keyword, OpenZWave::ValueID & value);

   static boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> generateHistoriser(OpenZWave::Notification const* _notification);
};





