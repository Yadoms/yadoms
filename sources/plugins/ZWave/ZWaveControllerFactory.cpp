#include "stdafx.h"
#include "ZWaveControllerFactory.h"
#include "OpenZWaveController.h"

boost::shared_ptr<IZWaveController> CZWaveControllerFactory::Create()
{
   return boost::make_shared<COpenZWaveController>();
}
