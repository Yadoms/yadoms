#pragma once

#include "Exception.hpp"

namespace shared::exception
{
   //--------------------------------------------------------------
   /// \brief Exception for accessing NULL objects
   //--------------------------------------------------------------
   class CParse final : public CException
   {
   public:
      explicit CParse(const std::string& message)
         : CException(message)
      {
      }

      CParse(const CParse&) = delete;
      CParse(CParse&&) = delete;
      CParse& operator=(const CParse&) = delete;
      CParse& operator=(CParse&&) = delete;


      ~CParse() noexcept override = default;
   };
}
