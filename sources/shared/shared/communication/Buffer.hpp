#pragma once

#include <Poco/Buffer.h>

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This class manage a raw data buffer
      // It's the copyable version of Poco::Buffer
      // template T is the type of data contained in the buffer
      /// \note Can not inherite from Poco::Buffer because Poco::Buffer dtor is not virtual
      //--------------------------------------------------------------
      template <class T>
      class CBuffer
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                     Empty buffer constructor
         /// \param[in] size              Buffer size
         //--------------------------------------------------------------
         explicit CBuffer(std::size_t size) :
            m_buffer(size)
         {
         }

         //--------------------------------------------------------------
         /// \brief	                     Contructor from array (with data copy)
         /// \param[in] buffer            A raw buffer 
         /// \param[in] size              Buffer size
         //--------------------------------------------------------------
         CBuffer(const T* buffer, std::size_t size) :
            m_buffer(size)
         {
            memcpy(begin(), buffer, size);
         }

         //--------------------------------------------------------------
         /// \brief	                     Contructor from vector (with data copy)
         /// \param[in] src               Source vector
         //--------------------------------------------------------------
         explicit CBuffer(const std::vector<T>& src) :
            m_buffer(src.size())
         {
            if (!src.empty())
               memcpy(m_buffer.begin(), &src[0], m_buffer.size() * sizeof(T));
         }

         //--------------------------------------------------------------
         /// \brief	                     Copy constructor (copy all the buffer, with its content)
         /// \param[in] src               Source buffer
         //--------------------------------------------------------------
         CBuffer(const CBuffer& src)
            : m_buffer(src.size())
         {
            memcpy(m_buffer.begin(), src.begin(), m_buffer.size() * sizeof(T));
         }

         //--------------------------------------------------------------
         /// \brief	                     Copy constructor (copy all the buffer, with its content)
         /// \param[in] src               Source buffer
         //--------------------------------------------------------------
         CBuffer& operator =(const CBuffer& src)
         {
            m_buffer.resize(src.size(), false);
            memcpy(m_buffer.begin(), src.begin(), m_buffer.size() * sizeof(T));
            return *this;
         }

         //--------------------------------------------------------------
         /// \brief	                     Copy from vector
         /// \param[in] src               Source vector
         //--------------------------------------------------------------
         CBuffer& operator =(const std::vector<T>& src)
         {
            m_buffer.resize(src.size(), false);
            if (!src.empty())
               memcpy(m_buffer.begin(), &src[0], m_buffer.size() * sizeof(T));
            return *this;
         }

         //--------------------------------------------------------------
         /// \brief	                     Destructor
         //--------------------------------------------------------------
         virtual ~CBuffer()
         {
         }

         //--------------------------------------------------------------
         /// \brief	                     Resize the buffer
         /// \param[in] newSize           The new buffer size
         /// \param[in] preserveContent   Copy the content to the new buffer
         //--------------------------------------------------------------
         void resize(std::size_t newSize, bool preserveContent = true)
         {
            m_buffer.resize(newSize, preserveContent);
         }

         //--------------------------------------------------------------
         /// \brief	                     Get the buffer size
         /// \return                      The buffer size (in nb of elements)
         //--------------------------------------------------------------
         std::size_t size() const
         {
            return m_buffer.size();
         }

         //--------------------------------------------------------------
         /// \brief	                     Get the first buffer element
         /// \return                      Returns a pointer to the beginning of the buffer
         //--------------------------------------------------------------
         T* begin()
         {
            return m_buffer.begin();
         }

         //--------------------------------------------------------------
         /// \brief	                     Get the first buffer element
         /// \return                      Returns a pointer to the beginning of the buffer
         //--------------------------------------------------------------
         const T* begin() const
         {
            return m_buffer.begin();
         }

         //--------------------------------------------------------------
         /// \brief	                     Get the end of the buffer (after last element)
         /// \return                      Returns a pointer to the end of the buffer
         //--------------------------------------------------------------
         T* end()
         {
            return m_buffer.end();
         }

         //--------------------------------------------------------------
         /// \brief	                     Get the end of the buffer (after last element)
         /// \return                      Returns a pointer to the end of the buffer
         //--------------------------------------------------------------
         const T* end() const
         {
            return m_buffer.end();
         }

         //--------------------------------------------------------------
         /// \brief	                     Get an element
         /// \return                      The element at specified index
         //--------------------------------------------------------------
         T& operator [](std::size_t index)
         {
            return m_buffer[index];
         }

         //--------------------------------------------------------------
         /// \brief	                     Get an element
         /// \return                      The element at specified index
         //--------------------------------------------------------------
         const T& operator [](std::size_t index) const
         {
            return m_buffer[index];
         }

      private:
         //--------------------------------------------------------------
         /// \brief	                     The original Poco buffer
         //--------------------------------------------------------------
         Poco::Buffer<T> m_buffer;
      };


      //--------------------------------------------------------------
      /// \brief	A buffer of bytes
      //--------------------------------------------------------------
      typedef CBuffer<unsigned char> CByteBuffer;
   }
} // namespace shared::communication


