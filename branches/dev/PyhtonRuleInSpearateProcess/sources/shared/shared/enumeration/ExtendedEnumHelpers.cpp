#include "stdafx.h"
#include "IExtendedEnum.h"

namespace shared {	namespace enumeration {

   std::ostream& operator <<(std::ostream& stream, const shared::enumeration::IExtendedEnum& enumValue)
   {
      stream << enumValue.toString();
      return stream;
   }

} //namespace enumeration
} //namespace shared