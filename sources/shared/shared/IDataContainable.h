#pragma once


namespace shared
{
   class CDataContainer;

   //--------------------------------------------------------------
   /// \brief		Interface defining a containable class
   //--------------------------------------------------------------
   class IDataContainable
   {
   public:
      virtual ~IDataContainable() { }

      //--------------------------------------------------------------
      /// \brief		Extract the data into a data container 
      /// \return    The data container ( a copy )
      //--------------------------------------------------------------
      virtual void extractContent(CDataContainer & containerToFill) const = 0;

      //--------------------------------------------------------------
      /// \brief		Fill this object instance with a container
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      virtual void fillFromContent(const CDataContainer & initialData) = 0;

      //--------------------------------------------------------------
      /// \brief		Fill this object instance with its serialized form
      /// \param [in] serializedData    Initial serialized data for this container
      //--------------------------------------------------------------
      virtual void fillFromSerializedString(const std::string & serializedData) = 0;
   };

} // namespace shared
