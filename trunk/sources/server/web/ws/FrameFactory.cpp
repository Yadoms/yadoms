#include "stdafx.h"
#include "FrameFactory.h"
#include "AcquisitionFilterFrame.h"

namespace web { namespace ws {

   boost::shared_ptr<CFrameBase> CFrameFactory::tryParse(const std::string & frameAsString)
   {
      shared::CDataContainer obj(frameAsString);
      if (obj.get<CFrameBase::EFrameType>(CFrameBase::getTypeFieldName()) == CFrameBase::EFrameType::kAcquisitionFilter)
         return boost::shared_ptr<CFrameBase>(new CAcquisitionFilterFrame(frameAsString));
      throw shared::exception::COutOfRange("Unknown type");
   }




} //namespace ws
} //namespace web