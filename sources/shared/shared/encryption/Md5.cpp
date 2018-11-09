#include "stdafx.h"
#include "Md5.h"
#include <Poco/DigestStream.h>
#include <Poco/MD5Engine.h>
#include <Poco/Exception.h>

namespace shared
{
   namespace encryption
   {
      std::string CMd5::digestFile(const std::string& file)
      {
         Poco::MD5Engine md5;
         std::string digest;

         std::ifstream ifHash(file.c_str(), std::ios::binary);
         if (ifHash.is_open())
         {
            Poco::DigestInputStream isMd5(md5, ifHash);
            const size_t bufSize = 0x100000;
            char* buf = new char[bufSize];

            isMd5.read(buf, bufSize);
            while ((isMd5.rdstate() && std::ifstream::failbit) == 0)
            {
               isMd5.read(buf, bufSize);
            }

            if ((isMd5.rdstate() && std::ifstream::eofbit) != 0)
            {
               digest = Poco::DigestEngine::digestToHex(md5.digest());
            }
            else
            {
               delete [] buf;
               throw Poco::FileException(file.c_str());
            }

            delete [] buf;
            return digest;
         }

         throw Poco::FileNotFoundException(file.c_str());
      }
   } // namespace encryption
} // namespace shared
