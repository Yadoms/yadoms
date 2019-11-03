#pragma once

#include <shared/DataContainer.h>
#include "IOpenZWaveNodeKeyword.h"

//--------------------------------------------------------------
/// \brief	Interface for ZWave controller
//--------------------------------------------------------------
class CConfigurationSchemaFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	Virtual destructor
   //--------------------------------------------------------------
   virtual ~CConfigurationSchemaFactory() {}

   
   static shared::CDataContainer generateForHistorizer(boost::shared_ptr<IOpenZWaveNodeKeyword> historizer);
   static std::string generateValidKeyName(const std::string & keyname);
private:
   static shared::CDataContainer generateForDouble(shared::CDataContainer zwaveTypeInfo, ECommandClass commandclass);
   static shared::CDataContainer generateForInteger(shared::CDataContainer zwaveTypeInfo, ECommandClass commandclass);
   static shared::CDataContainer generateForBool(shared::CDataContainer zwaveTypeInfo, ECommandClass commandclass);
   static shared::CDataContainer generateForString(shared::CDataContainer zwaveTypeInfo, ECommandClass commandclass);
   static shared::CDataContainer generateForEnum(shared::CDataContainer zwaveTypeInfo, ECommandClass commandclass);
   static std::string generateDescription(shared::CDataContainer zwaveTypeInfo, ECommandClass commandclass);
};
