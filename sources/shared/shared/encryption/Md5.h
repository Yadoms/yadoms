#pragma once

namespace shared
{
   namespace encryption
   {
      //--------------------------------------------------------------
      /// \brief	Md5 hash generator
      //--------------------------------------------------------------
      class CMd5
      {
      public:
         //--------------------------------------------------------------
         /// \brief	compute a md5 hash on a file
         /// \param [in] file  The file to compute hash
         /// \return	The md5 hash
         //--------------------------------------------------------------
         static std::string digestFile(const std::string& file);
      };
   } // namespace encryption
} // namespace shared
