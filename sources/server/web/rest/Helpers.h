#pragma once
#include "IAnswer.h"
#include "IRequest.h"
#include "database/IDataProvider.h"

namespace web
{
   namespace rest
   {
      class CHelpers final
      {
      public:
         CHelpers() = delete;
         ~CHelpers() = delete;

         //-----------------------------------------
         ///\brief   Transactional method
         //-----------------------------------------
         static boost::shared_ptr<IAnswer> transactionalMethodV2(
            const boost::shared_ptr<IRequest>& request,
            const boost::shared_ptr<database::IDataProvider>& dataProvider,
            const std::function<boost::shared_ptr<IAnswer>(const boost::shared_ptr<IRequest>&)>& realMethod);

         //-----------------------------------------
         ///\brief   Convert set of strings into set of int
         //-----------------------------------------
         static std::unique_ptr<std::set<int>> convertToIntSet(const std::unique_ptr<std::set<std::string>> in);

         //-----------------------------------------
         ///\brief   Convert set of strings into set of ExtendedEnum
         //-----------------------------------------
         template <typename T>
         static std::unique_ptr<std::set<T>> convertToEnumSet(const std::unique_ptr<std::set<std::string>> in)
         {
            // ReSharper disable once CppUseTypeTraitAlias
            static_assert(std::is_base_of<shared::enumeration::IExtendedEnum, T>::value, "T must be derived from shared::enumeration::IExtendedEnum");
            auto out = std::make_unique<std::set<T>>();
            for (const auto& inItem : *in)
               out->insert(T(inItem));
            return out;
         }
      };
   } //namespace rest
} //namespace web 
