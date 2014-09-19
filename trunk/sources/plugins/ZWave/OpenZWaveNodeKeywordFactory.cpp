#include "stdafx.h"
#include "OpenZWaveNodeKeywordFactory.h"
#include <shared/exception/NotSupported.hpp>
#include "OpenZWaveNodeKeywordSwitch.h"
#include "OpenZWaveCommandClass.h"

boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::createKeyword(ECommandClass commandClass, OpenZWave::ValueID & value)
{
   switch(commandClass())
   {
   case ECommandClass::kSwitchBinary:
      return boost::shared_ptr<IOpenZWaveNodeKeyword> (new COpenZWaveNodeKeywordSwitch(value));
      break;
    default:
      throw shared::exception::CNotSupported(commandClass.getAsString());
      break;
   }
}