#include "stdafx.h"
#include "FrameFactory.h"
#include "AcquisitionFilterFrame.h"
#include <shared/Log.h>

#include "IsAliveFrame.h"

namespace web
{
   namespace poco
   {
      namespace ws
      {
         boost::shared_ptr<CFrameBase> CFrameFactory::tryParse(const std::string& frameAsString)
         {
            try
            {
               const shared::CDataContainer obj(frameAsString);
               if (obj.containsValue(CFrameBase::getTypeFieldName()))
               {
                  switch (obj.get<CFrameBase::EFrameType>(CFrameBase::getTypeFieldName()))
                  {
                  case CFrameBase::EFrameType::kAcquisitionFilterValue:
                     return boost::make_shared<CAcquisitionFilterFrame>(frameAsString);
                  case CFrameBase::EFrameType::kIsAliveValue:
                     return boost::make_shared<CIsAliveFrame>();

                  default:
                     throw shared::exception::COutOfRange("Unknown type");
                  }
               }

               YADOMS_LOG(debug) << "Cannot find frame type : " << frameAsString;
               return boost::shared_ptr<CFrameBase>();
            }
            catch (shared::exception::CInvalidParameter&)
            {
               YADOMS_LOG(debug) << "Fail to parse JSON : " << frameAsString;
               throw;
            }
            catch (shared::exception::COutOfRange&)
            {
               YADOMS_LOG(debug) << "Fail to parse frame. Unknown type : " << frameAsString;
               throw;
            }
            catch (std::exception&)
            {
               YADOMS_LOG(debug) << "Fail to parse frame. Unknown error : " << frameAsString;
               throw;
            }
         }
      } //namespace ws
   } //namespace poco
} //namespace web
