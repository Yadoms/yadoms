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

   
   static shared::CDataContainerSharedPtr generateForHistorizer(boost::shared_ptr<IOpenZWaveNodeKeyword> historizer);
   static std::string generateValidKeyName(const std::string & keyname);
private:
   static shared::CDataContainerSharedPtr generateForDouble(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass);
   static shared::CDataContainerSharedPtr generateForInteger(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass);
   static shared::CDataContainerSharedPtr generateForBool(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass);
   static shared::CDataContainerSharedPtr generateForString(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass);
   static shared::CDataContainerSharedPtr generateForEnum(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass);
   static std::string generateDescription(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass);
};
