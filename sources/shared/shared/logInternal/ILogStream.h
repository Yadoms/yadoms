#pragma once

namespace shared {
   namespace logInternal {
      class ILogStream
      {
      public:
         virtual ~ILogStream()
         {
         }

         virtual std::ostream& fatal() = 0;
         virtual std::ostream& critical() = 0;
         virtual std::ostream& error() = 0;
         virtual std::ostream& warning() = 0;
         virtual std::ostream& notice() = 0;
         virtual std::ostream& information() = 0;
         virtual std::ostream& debug() = 0;
         virtual std::ostream& trace() = 0;
      };
   } // namespace logInternal
} // namespace shared
