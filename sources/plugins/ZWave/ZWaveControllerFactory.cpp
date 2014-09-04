#include "stdafx.h"
#include "ZWaveControllerFactory.h"
#include "OpenZWaveController.h"

boost::shared_ptr<IZWaveController> CZWaveControllerFactory::Create()
{
   return boost::shared_ptr<IZWaveController>(dynamic_cast<IZWaveController*>(new COpenZWaveController()));
}
