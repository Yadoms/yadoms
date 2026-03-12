#pragma once

#include "Exception.hpp"

namespace shared::exception
{
   //--------------------------------------------------------------
   /// \class CEmptyResult Exception for non implemented part of code
   //--------------------------------------------------------------
   class CEmptyResult final : public CException
   {
   public:
      explicit CEmptyResult(const std::string& message)
         : CException(message)
      {
      }

      CEmptyResult() = delete;
      CEmptyResult(const CEmptyResult&) = default;
      CEmptyResult(CEmptyResult&&) = default;
      CEmptyResult& operator=(const CEmptyResult&) = default;
      CEmptyResult& operator=(CEmptyResult&&) = default;

      ~CEmptyResult() noexcept override = default;
   };
}
