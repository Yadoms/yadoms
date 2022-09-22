#pragma once
#include "FrameBase.h"

namespace web
{
   namespace poco
   {
      namespace ws
      {
         class CFrameFactory
         {
         public:
            static boost::shared_ptr<CFrameBase> tryParse(const std::string& frameAsString);
         };
      } //namespace ws
   } //namespace poco
} //namespace web
