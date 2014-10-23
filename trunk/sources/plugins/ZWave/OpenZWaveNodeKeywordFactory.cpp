#include "stdafx.h"
#include "OpenZWaveNodeKeywordFactory.h"
#include <shared/exception/NotSupported.hpp>
#include "OpenZWaveNodeKeywordSwitch.h"
#include "OpenZWaveCommandClass.h"

boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::createKeyword(ECommandClass commandClass, const std::string & keyword, OpenZWave::ValueID & value)
{
   switch(commandClass())
   {
   case ECommandClass::kSwitchBinaryValue:
      return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordSwitch(keyword, value));
      break;
    default:
      throw shared::exception::CNotSupported(commandClass.toString());
      break;
   }
}