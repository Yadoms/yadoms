#include "stdafx.h"
#include "ZWaveControllerFactory.h"
#include "OpenZWaveController.h"

boost::shared_ptr<IZWaveController> CZWaveControllerFactory::Create()
{
   boost::shared_ptr<IZWaveController> controller(new COpenZWaveController());
   return controller;
}
