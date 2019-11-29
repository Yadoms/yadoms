#pragma once

#include "Buffer.hpp"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This class manage a raw char buffer
      //--------------------------------------------------------------
      class CStringBuffer : public CBuffer<char>
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                     Empty buffer constructor
         /// \param[in] size              Buffer size
         //--------------------------------------------------------------
         explicit CStringBuffer(std::size_t size);

         //--------------------------------------------------------------
         /// \brief	                     Contructor from array (with data copy)
         /// \param[in] buffer            A raw buffer 
         /// \param[in] size              Buffer size
         //--------------------------------------------------------------
         CStringBuffer(const char* buffer, std::size_t size);

         //--------------------------------------------------------------
         /// \brief	                     Contructor from array (with data copy)
         /// \param[in] buffer            A string
         //--------------------------------------------------------------
         explicit CStringBuffer(const std::string & buffer);

         //--------------------------------------------------------------
         /// \brief	                     Contructor from vector (with data copy)
         /// \param[in] buffer            A raw buffer 
         /// \param[in] size              Buffer size
         //--------------------------------------------------------------
         explicit CStringBuffer(const std::vector<char>& src);

         //--------------------------------------------------------------
         /// \brief	                     Constructor from CByteBuffer
         /// \param[in] src               Source byte buffer
         //--------------------------------------------------------------
         explicit CStringBuffer(const CByteBuffer& src);

         //--------------------------------------------------------------
         /// \brief	                     Copy constructor (copy all the buffer, with its content)
         /// \param[in] src               Source buffer
         //--------------------------------------------------------------
         CStringBuffer(const CStringBuffer& src);

         //--------------------------------------------------------------
         /// \brief	                     Copy constructor (copy all the buffer, with its content)
         /// \param[in] src               Source buffer
         //--------------------------------------------------------------
         CStringBuffer& operator =(const CStringBuffer& src);
         
         //--------------------------------------------------------------
         /// \brief	                     Copy constructor (copy all the buffer, with its content)
         /// \param[in] src               Source buffer
         //--------------------------------------------------------------
         CStringBuffer& operator =(const std::string& src);         
         
         
         //--------------------------------------------------------------
         /// \brief	                     Destructor
         //--------------------------------------------------------------
         virtual ~CStringBuffer();

         //--------------------------------------------------------------
         /// \brief	                     Get the buffer content as string
         /// \return                      The buffer content as a string
         //--------------------------------------------------------------
         const std::string toString() const;
      };


   }
} // namespace shared::communication


