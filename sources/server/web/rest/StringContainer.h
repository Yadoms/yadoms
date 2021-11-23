#pragma once

#include <shared/serialization/IDataSerializable.h>
#include <string>

namespace web
{
   namespace rest
   {
      class CStringContainer final : public shared::serialization::IDataSerializable
      {
      public:
         explicit CStringContainer(std::string content);
         ~CStringContainer() override = default;

         // IDataSerializable implementation
         std::string serialize() const override;
         void deserialize(const std::string& data) override;
         // [END] IDataSerializable implementation 

      private:
         std::string m_content;
      };
   } //namespace rest
} //namespace web 
