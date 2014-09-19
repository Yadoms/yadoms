#pragma once

#include "OpenZWaveCommandClass.h"
#include "Value.h"
#include "IOpenZWaveNodeKeyword.h"

class COpenZWaveNodeKeywordFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	      Create a keyword form its commandClass
   /// \param [in]   commandClass      the command class
   /// \param [in]   value             the value id
   /// \return       the keyword
   //--------------------------------------------------------------
   static boost::shared_ptr<IOpenZWaveNodeKeyword> createKeyword(ECommandClass commandClass, OpenZWave::ValueID & value);

};





