#pragma once

#include "FrameBase.h"
#include "database/entities/Entities.h"
#include "../../server/task/IInstance.h"


namespace web { namespace ws {

   //-----------------------------
   ///\brief Frame which can be send to GUI to notify for log event
   //-----------------------------
   class CLogEventFrame : public CFrameBase
   {
   public:
      //-----------------------------
      ///\brief Constructor
      ///\param [in]    logEvent     The log event data
      //-----------------------------
      explicit CLogEventFrame(boost::shared_ptr<const database::entities::CEventLogger> logEvent);

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CLogEventFrame();
   };

} //namespace ws
} //namespace web