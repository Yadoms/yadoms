#pragma once


//--------------------------------------------------------------
/// \class Hardware plugin configuration serializer interface
//--------------------------------------------------------------
class IConfigurationSerializer
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IConfigurationSerializer() {}

   //--------------------------------------------------------------
   /// \brief              Serialize the provided property tree
   /// \par pt             Property tree to serialize
   /// \return             Result of serialization, as string
   //--------------------------------------------------------------
   virtual const std::string serialize(const boost::property_tree::ptree& pt) const = 0;

   //--------------------------------------------------------------
   /// \brief              Unserialize string to property tree
   /// \par str            Input string
   /// \par [out] pt       Property tree populated
   /// \throw              CInvalidParameterException if str is not in the valid format
   //--------------------------------------------------------------
   virtual void unserialize(const std::string& str, boost::property_tree::ptree& pt) = 0;
};

