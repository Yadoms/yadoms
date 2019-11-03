#include "stdafx.h"
#include "TypeInfoFactory.h"
#include "../OpenZWaveHelpers.h"

#include "BoolTypeInfo.h"
#include "DecimalTypeInfo.h"
#include "EnumTypeInfo.h"
#include "IntegerTypeInfo.h"
#include "StringTypeInfo.h"

boost::shared_ptr<shared::plugin::yPluginApi::typeInfo::ITypeInfo> CTypeInfoFactory::create(OpenZWave::ValueID& valueId)
{
   switch (valueId.GetType())
   {
   case OpenZWave::ValueID::ValueType_Bool:
      return boost::make_shared<CBoolTypeInfo>(valueId);

   case OpenZWave::ValueID::ValueType_Byte:
   case OpenZWave::ValueID::ValueType_Int:
   case OpenZWave::ValueID::ValueType_Short:
      return boost::make_shared<CIntegerTypeInfo>(valueId);

   case OpenZWave::ValueID::ValueType_Decimal:
      return boost::make_shared<CDecimalTypeInfo>(valueId);

   case OpenZWave::ValueID::ValueType_List:
      return boost::make_shared<CEnumTypeInfo>(valueId);

   case OpenZWave::ValueID::ValueType_String:
      return boost::make_shared<CStringTypeInfo>(valueId);

   default:
      return boost::shared_ptr<shared::plugin::yPluginApi::typeInfo::ITypeInfo>();
   }
}
