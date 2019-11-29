#pragma once


namespace shared { namespace serialization {

   //--------------------------------------------------------------
   /// \brief	    Interface for a serializable object
   //--------------------------------------------------------------
   class IDataSerializable
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IDataSerializable() {}

      //--------------------------------------------------------------
      /// \brief	    Serialize this CDataContainer instance
      /// \return     the data serialized
      //--------------------------------------------------------------
      virtual std::string serialize() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Deserialize a string and set data into this CDataContainer instance
      /// \param [in] data    data to deserialize
      //--------------------------------------------------------------
      virtual void deserialize(const std::string & data) = 0;
   };

} // namespace serialization   
} // namespace shared   