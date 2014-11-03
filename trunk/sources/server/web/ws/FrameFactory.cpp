#include "stdafx.h"
#include "FrameFactory.h"
#include "AcquisitionFilterFrame.h"
#include "NewDeviceFrame.h"

namespace web { namespace ws {

   boost::shared_ptr<CFrameBase> CFrameFactory::tryParse(const std::string & frameAsString)
   {
      shared::CDataContainer obj(frameAsString);
      switch (obj.get<CFrameBase::EFrameType>(CFrameBase::getTypeFieldName()))
      {
         case CFrameBase::EFrameType::kAcquisitionFilterValue:
            return boost::shared_ptr<CFrameBase>(new CAcquisitionFilterFrame(frameAsString));

         default:
            throw shared::exception::COutOfRange("Unknown type");
      }
   }




} //namespace ws
} //namespace web