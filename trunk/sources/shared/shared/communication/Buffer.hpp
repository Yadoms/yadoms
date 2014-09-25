#pragma once

namespace shared { namespace communication {

   //--------------------------------------------------------------
   /// \brief	This class manage a raw data buffer
   // The class owns the buffer content
   // template DataType is the type of data content in the buffer
   //--------------------------------------------------------------
   template <typename DataType>
   class CBuffer
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                     The data type stored in the buffer
      //--------------------------------------------------------------
      typedef DataType value_type;

   public:
      //--------------------------------------------------------------
      /// \brief	                     Constructor
      //--------------------------------------------------------------
      CBuffer()
      {
      }

      //--------------------------------------------------------------
      /// \brief	                     Constructor
      //--------------------------------------------------------------
      CBuffer(const DataType* content, std::size_t size)
         :m_size(size)
      {
         m_content.reset(new DataType[m_size]);
         memcpy(m_content.get(), content, sizeof(DataType) * m_size);
      }

      //--------------------------------------------------------------
      /// \brief	                     Destructor
      //--------------------------------------------------------------
      virtual ~CBuffer()
      {
      }

      //--------------------------------------------------------------
      /// \brief	                     Get the buffer size
      /// \return                      The buffer size (in nb of elements)
      //--------------------------------------------------------------
      std::size_t size() const
      {
         return m_size;
      }

      //--------------------------------------------------------------
      /// \brief	                     Get the buffer content
      /// \return                      The buffer content
      //--------------------------------------------------------------
      const DataType* content() const
      {
         return m_content.get();
      }

   private:
      //--------------------------------------------------------------
      /// \brief	Buffer size
      //--------------------------------------------------------------
      std::size_t m_size;

      //--------------------------------------------------------------
      /// \brief	Buffer content
      //--------------------------------------------------------------
      boost::shared_ptr<DataType[]> m_content;

   };

   typedef CBuffer<unsigned char> CByteBuffer;

} } // namespace shared::communication
