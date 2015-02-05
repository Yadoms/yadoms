#pragma once

#include "IZWaveController.h"


//--------------------------------------------------------------
/// \brief	Interface for ZWave controller
//--------------------------------------------------------------
class CZWaveControllerFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	Virtual destructor
   //--------------------------------------------------------------
   virtual ~CZWaveControllerFactory() {}

   //--------------------------------------------------------------
   /// \brief	Create the appropriate ZWave controller
   //--------------------------------------------------------------
   static boost::shared_ptr<IZWaveController> Create();
};

