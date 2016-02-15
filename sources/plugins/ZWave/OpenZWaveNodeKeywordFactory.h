#pragma once

#include "OpenZWaveCommandClass.h"
#include <value_classes/Value.h>
#include "IOpenZWaveNodeKeyword.h"

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

};





