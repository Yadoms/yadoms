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

   
   static boost::shared_ptr<shared::CDataContainer> generateForHistorizer(boost::shared_ptr<IOpenZWaveNodeKeyword> historizer);
   static std::string generateValidKeyName(const std::string & keyname);
private:
   static boost::shared_ptr<shared::CDataContainer> generateForDouble(boost::shared_ptr<shared::CDataContainer> zwaveTypeInfo, ECommandClass commandclass);
   static boost::shared_ptr<shared::CDataContainer> generateForInteger(boost::shared_ptr<shared::CDataContainer> zwaveTypeInfo, ECommandClass commandclass);
   static boost::shared_ptr<shared::CDataContainer> generateForBool(boost::shared_ptr<shared::CDataContainer> zwaveTypeInfo, ECommandClass commandclass);
   static boost::shared_ptr<shared::CDataContainer> generateForString(boost::shared_ptr<shared::CDataContainer> zwaveTypeInfo, ECommandClass commandclass);
   static boost::shared_ptr<shared::CDataContainer> generateForEnum(boost::shared_ptr<shared::CDataContainer> zwaveTypeInfo, ECommandClass commandclass);
   static std::string generateDescription(boost::shared_ptr<shared::CDataContainer> zwaveTypeInfo, ECommandClass commandclass);
};
