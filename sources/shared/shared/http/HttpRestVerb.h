#pragma once
#include <stdexcept>
#include <string>

namespace shared
{
   namespace http
   {
      //--------------------------------------------------------------
      /// \brief	REST Verb type
      //--------------------------------------------------------------
      enum class ERestVerb
      {
         kGet = 0,
         kPut,
         kPost,
         kHead,
         kDelete,
         kPatch,
      };

      static const std::string RestVerbGetString("GET");
      static const std::string RestVerbPutString("PUT");
      static const std::string RestVerbPostString("POST");
      static const std::string RestVerbHeadString("HEAD");
      static const std::string RestVerbDeleteString("DELETE");
      static const std::string RestVerbPatchString("PATCH");

      static const std::string& ToString(const ERestVerb& verb)
      {
         switch (verb)
         {
         case ERestVerb::kGet: return RestVerbGetString;
         case ERestVerb::kPut: return RestVerbPutString;
         case ERestVerb::kPost: return RestVerbPostString;
         case ERestVerb::kHead: return RestVerbHeadString;
         case ERestVerb::kDelete: return RestVerbDeleteString;
         case ERestVerb::kPatch: return RestVerbPatchString;
         default:  // NOLINT(clang-diagnostic-covered-switch-default)
            throw std::out_of_range(
               "Can not convert HTTP REST verb into string, invalid verb : " + std::to_string(static_cast<unsigned int>(verb)));
         }
      }

      static ERestVerb ToRestVerb(const std::string& verb)
      {
         if (verb == RestVerbGetString)
            return ERestVerb::kGet;
         if (verb == RestVerbPutString)
            return ERestVerb::kPut;
         if (verb == RestVerbPostString)
            return ERestVerb::kPost;
         if (verb == RestVerbHeadString)
            return ERestVerb::kHead;
         if (verb == RestVerbDeleteString)
            return ERestVerb::kDelete;
         if (verb == RestVerbPatchString)
            return ERestVerb::kPatch;

         throw std::out_of_range("Can not convert HTTP REST verb from string, invalid verb : " + verb);
      }
   }
} // namespace shared::http
