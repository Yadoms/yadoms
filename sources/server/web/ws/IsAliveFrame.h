#pragma once
#include "FrameBase.h"

namespace web
{
   namespace ws
   {
      //-----------------------------
      ///\brief Frame which can be send to GUI to notify that server is alive
      //-----------------------------
      class CIsAliveFrame : public CFrameBase
      {
      public:
         explicit CIsAliveFrame();
         virtual ~CIsAliveFrame() = default;
      };
   } //namespace ws
} //namespace web
