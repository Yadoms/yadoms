#pragma once


namespace shared { namespace serialization {

   //--------------------------------------------------------------
   /// \brief	    Interface for a file serializable object
   //--------------------------------------------------------------
   class IDataFileSerializable
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IDataFileSerializable() {}

      //--------------------------------------------------------------
      /// \brief	    Serialize this CDataContainer instance
      /// \return     the data serialized
      //--------------------------------------------------------------
      virtual void serializeToFile(const std::string & filename) const = 0;

      //--------------------------------------------------------------
      /// \brief	    Deserialize a string and set data into this CDataContainer instance
      /// \param [in] data    data to deserialize
      /// \throw      exception::CException on any error
      //--------------------------------------------------------------
      virtual void deserializeFromFile(const std::string & filename) = 0;
   };


} // namespace serialization   
} // namespace shared   