#pragma once
//
// This file provide macros used to declare header and implementation for JSON entity behavior
//  -> it generates the class that defines the id attached to an entity propertt (the id the the text used in json data to identify a property)
//  -> it generates a serializer class which enable to serialize and deserialize JSON data
//
// ==========================================================================
//        HEADER PART
// ==========================================================================
//
// Example : Hardware entity => class CHardwareEntityJsonDefinition and CHardwareEntitySerializer
//
//   Input :
//             DECLARE_JSON_ENTITY_HEADER(Hardware, 
//                (Id)
//                (Name)
//                (PluginName)
//                (Configuration)
//                (Enabled)
//                (Deleted)
//             )
//
//    Ouptut :
//             class CHardwareEntityJsonDefinition
//             {
//             private:
//                CHardwareEntityJsonDefinition() {}
//                virtual ~CHardwareEntityJsonDefinition() {}
//             
//             public:
//                static const std::string & getIdIdentifier() { return m_hardwareIdIdentifier; }
//                static const std::string & getNameIdentifier() { return m_hardwareNameIdentifier; }
//                static const std::string & getPluginNameIdentifier() { return m_hardwarePluginNameIdentifier; }
//                static const std::string & getConfigurationIdentifier() { return m_hardwareConfigurationIdentifier; }
//                static const std::string & getEnabledIdentifier() { return m_hardwareEnabledIdentifier; }
//                static const std::string & getDeletedIdentifier() { return m_hardwareDeletedIdentifier; }
//             
//             private:
//                static std::string m_hardwareIdIdentifier;
//                static std::string m_hardwareNameIdentifier;
//                static std::string m_hardwarePluginNameIdentifier;
//                static std::string m_hardwareConfigurationIdentifier;
//                static std::string m_hardwareEnabledIdentifier;
//                static std::string m_hardwareDeletedIdentifier;
//             };
//             
//             class CHardwareEntitySerializer : public IEntitySerializer<CHardware>, public IEntityDeserializer<CHardware>
//             {
//             public:
//                CHardwareEntitySerializer();
//                virtual ~CHardwareEntitySerializer();
//                
//                // IEntitySerializer implementation
//                CJson serialize(const CHardware & object);
//                // [END] IEntitySerializer implementation
//             
//                // IEntityDeserializer implementation
//                CHardware deserialize(const CJson & object);
//                // [END] IEntityDeserializer implementation
//             }
//         
//
// ==========================================================================
//        SOURCE PART
// ==========================================================================
//
//
// Example : Hardware entity => implementation of class CHardwareEntityJsonDefinition and CHardwareEntitySerializer
//
//   Input :
//          DECLARE_JSON_ENTITY_IMPLEMENTATION(Hardware, "Hardware",
//             ((Id)("id")JSON_INT)
//             ((Name)("name")JSON_STRING)
//             ((PluginName)("pluginName")JSON_STRING)
//             ((Configuration)("configuration")JSON_STRING)
//             ((Enabled)("enabled")JSON_BOOL)
//             ((Deleted)("deleted")JSON_BOOL)
//          )
//
//    Ouptut :
//             //implementation of class CHardwareEntityJsonDefinition
//             std::string CHardwareEntityJsonDefinition::m_hardwareIdIdentifier = "Id";
//             std::string CHardwareEntityJsonDefinition::m_hardwareNameIdentifier = "name";
//             std::string CHardwareEntityJsonDefinition::m_hardwarePluginNameIdentifier = "pluginName";
//             std::string CHardwareEntityJsonDefinition::m_hardwareConfigurationIdentifier = "configuration";
//             std::string CHardwareEntityJsonDefinition::m_hardwareEnabledIdentifier = "enabled";
//             std::string CHardwareEntityJsonDefinition::m_hardwareDeletedIdentifier = "deleted";
//
//             //implementation of class CHardwareEntitySerializer
//             CHardwareEntitySerializer::CHardwareEntitySerializer()
//             {
//             }
//             
//             CHardwareEntitySerializer::~CHardwareEntitySerializer()
//             {
//             }
//             
//             // IEntitySerializer implementation
//             CJson CHardwareEntitySerializer::serialize(const CHardware & object)
//             {
//                CJson result;
//                result.put(CHardwareEntityJsonDefinition::getIdIdentifier(), object.getId());
//                result.put(CHardwareEntityJsonDefinition::getEnabledIdentifier(), object.getEnabled());
//                result.put(CHardwareEntityJsonDefinition::getDeletedIdentifier(), object.getDeleted());
//                result.put(CHardwareEntityJsonDefinition::getConfigurationIdentifier(), object.getConfiguration());
//                result.put(CHardwareEntityJsonDefinition::getNameIdentifier(), object.getName());
//                result.put(CHardwareEntityJsonDefinition::getPluginNameIdentifier(), object.getPluginName());
//                return result;
//             }
//             // [END] IEntitySerializer implementation
//             
//             
//             // IEntityDeserializer implementation
//             CHardware CHardwareEntitySerializer::deserialize(const CJson & object)
//             {
//                CHardware entity;
//             
//                if(object.find(CHardwareEntityJsonDefinition::getIdIdentifier()) != object.not_found())
//                   entity.setId(object.get<int>(CHardwareEntityJsonDefinition::getIdIdentifier()));
//             
//                if(object.find(CHardwareEntityJsonDefinition::getEnabledIdentifier()) != object.not_found())
//                   entity.setEnabled(object.get<bool>(CHardwareEntityJsonDefinition::getEnabledIdentifier()));
//             
//                if(object.find(CHardwareEntityJsonDefinition::getDeletedIdentifier()) != object.not_found())
//                   entity.setDeleted(object.get<bool>(CHardwareEntityJsonDefinition::getDeletedIdentifier()));
//             
//                if(object.find(CHardwareEntityJsonDefinition::getConfigurationIdentifier()) != object.not_found())
//                   entity.setConfiguration(object.get<std::string>(CHardwareEntityJsonDefinition::getConfigurationIdentifier()));
//             
//                if(object.find(CHardwareEntityJsonDefinition::getNameIdentifier()) != object.not_found())
//                   entity.setName(object.get<std::string>(CHardwareEntityJsonDefinition::getNameIdentifier()));
//             
//                if(object.find(CHardwareEntityJsonDefinition::getPluginNameIdentifier()) != object.not_found())
//                   entity.setPluginName(object.get<std::string>(CHardwareEntityJsonDefinition::getPluginNameIdentifier()));
//             
//                return entity;
//             }



//
/// \brief The column identifier in input sequence
//
#define JSON_COLUMN_ID 0
//
/// \brief The column name in input sequence
//
#define JSON_COLUMN_NAME 1
//
/// \brief The column name in input sequence
//
#define JSON_COLUMN_TYPE 2
//
/// \brief The column converter FROM in input sequence
//
#define JSON_COLUMN_CONVERT_FROM 3
//
/// \brief The column converter TO in input sequence
//
#define JSON_COLUMN_CONVERT_TO 4


#define JSON_NO_CONV(x) x
#define JSON_FROM_DATE(x) boost::posix_time::time_from_string(x)
#define JSON_TO_DATE(x) boost::posix_time::to_simple_string(x)

#define JSON_INT (int)(JSON_NO_CONV)(JSON_NO_CONV)
#define JSON_FLOAT (float)(JSON_NO_CONV)(JSON_NO_CONV)
#define JSON_DOUBLE (double)(JSON_NO_CONV)(JSON_NO_CONV)
#define JSON_BOOL (bool)(JSON_NO_CONV)(JSON_NO_CONV)
#define JSON_DATE (std::string)(JSON_FROM_DATE)(JSON_TO_DATE)
#define JSON_STRING (std::string)(JSON_NO_CONV)(JSON_NO_CONV)
#define JSON_ENUM(_e) (int)((_e))((int))

//-------------------------------------------------------
///\brief Macro which provide the entity class:
///       Example for _classname= Hardware -> CHardware
//-------------------------------------------------------
#define DECLARE_JSON_ENTITY_CLASS(_classname) BOOST_PP_CAT(C,_classname)

//-------------------------------------------------------
///\brief Macro which provide the entity definition class:
///       Example for _classname= Hardware -> CHardwareEntityJsonDefinition
//-------------------------------------------------------
#define DECLARE_JSON_ENTITY_DEFINITION_CLASS(_classname) BOOST_PP_CAT(C, BOOST_PP_CAT(_classname, EntityJsonDefinition))

//-------------------------------------------------------
///\brief Macro which provide the entity serializer class:
///       Example for _classname= Hardware -> CHardwareEntitySerializer
//-------------------------------------------------------
#define DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname) BOOST_PP_CAT(C, BOOST_PP_CAT(_classname, EntitySerializer))

//-------------------------------------------------------
/// \brief Macro which declares the static member
//-------------------------------------------------------
#define DECLARE_JSON_FIELD_NAME(r, data, elem) static std::string BOOST_PP_CAT(BOOST_PP_CAT(m_, elem), Identifier);

//-------------------------------------------------------
/// \brief Macro which declares a getter (used by foreach)
//-------------------------------------------------------
#define DECLARE_JSON_FIELD_NAME_GETTER(r, data, elem) \
   static const std::string & BOOST_PP_CAT(BOOST_PP_CAT(get, elem), Identifier()) { return BOOST_PP_CAT(BOOST_PP_CAT(m_, elem), Identifier); }

//-------------------------------------------------------
/// \brief  Macro which declares all the getters
///         For each ID in input sequence, call DECLARE_JSON_FIELD_NAME_GETTER
//-------------------------------------------------------
#define DECLARE_JSON_GETTERS(_seq) \
   BOOST_PP_SEQ_FOR_EACH(DECLARE_JSON_FIELD_NAME_GETTER, _, _seq)

//-------------------------------------------------------
/// \brief  Macro which declares all the getters
///         For each ID in input sequence, call DECLARE_JSON_COLUMN_NAME
//-------------------------------------------------------
#define DECLARE_JSON_MEMBERS(_seq) \
   BOOST_PP_SEQ_FOR_EACH(DECLARE_JSON_FIELD_NAME, _, _seq)


//-------------------------------------------------------
/// \brief  Declare one static value (used by BOOST_PP_SEQ_FOR_EACH_I)
//-------------------------------------------------------
#define DELCARE_STATIC_JSON_IDENTIFIER(r, classname, elem) \
   std::string BOOST_PP_CAT(BOOST_PP_CAT(C##classname##EntityJsonDefinition::m_ , BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem)), Identifier) = BOOST_PP_SEQ_ELEM(JSON_COLUMN_NAME, elem);


//-------------------------------------------------------
/// \brief  Declare a EntityJsonDefinition
/// \param  name  the EntityJsonDefinition name
/// \param  _seq  the sequence of columns
//-------------------------------------------------------
#define DECLARE_JSON_DEFINITION(name, _seq) \
class BOOST_PP_CAT(C, BOOST_PP_CAT(name, EntityJsonDefinition)) \
{ \
public:\
   DECLARE_JSON_GETTERS(_seq) \
   \
private:\
   DECLARE_JSON_MEMBERS(_seq) \
};

//-------------------------------------------------------
/// \brief  Initialize all static members of the table
/// \param  name  the table name
/// \param  _seq  the sequence of columns
//-------------------------------------------------------
#define DECLARE_JSON_IDENTIFIER(_classname, _tablename, _seq) \
   BOOST_PP_SEQ_FOR_EACH(DELCARE_STATIC_JSON_IDENTIFIER, _classname, _seq)


//-------------------------------------------------------
///\brief   Declare the JSON serializer DEFINITION
///       write the class header 
//-------------------------------------------------------   
#define DECLARE_JSON_SERIALIZER(_classname) \
class BOOST_PP_CAT(C, BOOST_PP_CAT(_classname, EntitySerializer)) : public IEntitySerializer<BOOST_PP_CAT(C,_classname)>, public IEntityDeserializer<BOOST_PP_CAT(C,_classname)> \
{ \
public:\
   BOOST_PP_CAT(C, BOOST_PP_CAT(_classname, EntitySerializer))(); \
   virtual BOOST_PP_CAT(~C, BOOST_PP_CAT(_classname, EntitySerializer))(); \
   \
   CJson serialize(const BOOST_PP_CAT(C,_classname) & object); \
   boost::shared_ptr< BOOST_PP_CAT(C,_classname) > deserialize(const CJson & object); \
 };   

 
//-------------------------------------------------------
///\brief   Declare the JSON serializer for one field IMPLEMENTATION
///       write the serialize() part for one field
//-------------------------------------------------------
#define DECLARE_JSON_SERIALIZER_FIELD_CONTENT(r, _classname, elem) \
   result.put(DECLARE_JSON_ENTITY_DEFINITION_CLASS(_classname)::BOOST_PP_CAT(BOOST_PP_CAT(get, BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem)), Identifier()), BOOST_PP_SEQ_ELEM(JSON_COLUMN_CONVERT_TO, elem)(object.BOOST_PP_CAT(get, BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem))()));

//-------------------------------------------------------
///\brief   Declare the JSON serializer IMPLEMENTATION
///       write the ctor and dtor  method
///       write the serialize() method
//-------------------------------------------------------
#define DECLARE_JSON_SERIALIZER_CONTENT(_classname, _seq) \
 DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)::DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)() { } \
 DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)::~DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)() { } \
CJson DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)::serialize(const DECLARE_JSON_ENTITY_CLASS(_classname) & object) \
{ \
   CJson result; \
   BOOST_PP_SEQ_FOR_EACH(DECLARE_JSON_SERIALIZER_FIELD_CONTENT, _classname, _seq) \
   return result; \
}
 
 
//-------------------------------------------------------
///\brief   Declare the JSON deserializer ofr one field IMPLEMENTATION
///       write the deserialize() part for one field
//-------------------------------------------------------
#define DECLARE_JSON_DESERIALIZER_FIELD_CONTENT(r, _classname, elem) \
    if(object.find(DECLARE_JSON_ENTITY_DEFINITION_CLASS(_classname)::BOOST_PP_CAT(BOOST_PP_CAT(get, BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem)), Identifier())) != object.not_found())\
      entity->BOOST_PP_CAT(set, BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem))(BOOST_PP_SEQ_ELEM(JSON_COLUMN_CONVERT_FROM, elem)(object.get<BOOST_PP_SEQ_ELEM(JSON_COLUMN_TYPE, elem)>(DECLARE_JSON_ENTITY_DEFINITION_CLASS(_classname)::BOOST_PP_CAT(BOOST_PP_CAT(get, BOOST_PP_SEQ_ELEM(JSON_COLUMN_ID, elem)), Identifier()))));\
  
//-------------------------------------------------------
///\brief   Declare the JSON deserializer IMPLEMENTATION
///       write the deserialize() method
//-------------------------------------------------------
#define DECLARE_JSON_DESERIALIZER_CONTENT(_classname, _seq) \
boost::shared_ptr< DECLARE_JSON_ENTITY_CLASS(_classname) > DECLARE_JSON_ENTITY_SERIALIZER_CLASS(_classname)::deserialize(const CJson & object)\
{\
   boost::shared_ptr< DECLARE_JSON_ENTITY_CLASS(_classname) > entity(new DECLARE_JSON_ENTITY_CLASS(_classname)() );\
   BOOST_PP_SEQ_FOR_EACH(DECLARE_JSON_DESERIALIZER_FIELD_CONTENT, _classname, _seq) \
   return entity;\
}

//-------------------------------------------------------
///\brief   Declare the JSON serializer IMPLEMENTATION
///            -> declare serializer implementation
///            -> declare deserializer implementation
//-------------------------------------------------------
#define DECLARE_JSON_SERIALIZER_IMPLEMENTATION(_classname, _seq)\
   DECLARE_JSON_SERIALIZER_CONTENT(_classname, _seq)\
   DECLARE_JSON_DESERIALIZER_CONTENT(_classname, _seq)
 

//-------------------------------------------------------
///\brief   Declare the JSON entity classes header
///         -> a class CxxxxEntityJsonDefinition which contains the name of each field (static declaration)
///         Example : for entity class CHardware :
///               class CHardwareEntityJsonDefinition 
///               { 
///               public: 
///               	static const std::string & getIdIdentifier() { return m_IdIdentifier; } 
///               	static const std::string & getNameIdentifier() { return m_NameIdentifier; } 
///               	static const std::string & getPluginNameIdentifier() { return m_PluginNameIdentifier; } 
///               	static const std::string & getConfigurationIdentifier() { return m_ConfigurationIdentifier; } 
///               	static const std::string & getEnabledIdentifier() { return m_EnabledIdentifier; } 
///               	static const std::string & getDeletedIdentifier() { return m_DeletedIdentifier; }   
///               private: 
///               	static std::string m_IdIdentifier; 
///               	static std::string m_NameIdentifier; 
///               	static std::string m_PluginNameIdentifier; 
///               	static std::string m_ConfigurationIdentifier; 
///               	static std::string m_EnabledIdentifier; 
///               	static std::string m_DeletedIdentifier;   
///               };
///
///
///
///         -> a class CxxxxEntitySerializer which contains the entity serializer
///               class CHardwareEntitySerializer : public IEntitySerializer<CHardware>, public IEntityDeserializer<CHardware> 
///               { 
///               public: 
///               	CHardwareEntitySerializer(); 
///               	virtual ~CHardwareEntitySerializer(); 
///               	
///               	CJson serialize(const CHardware & object); 
///               	boost::shared_ptr<CHardware> deserialize(const CJson & object); 
///               };
//-------------------------------------------------------
#define DECLARE_JSON_ENTITY_HEADER(_classname, _seq)\
   DECLARE_JSON_DEFINITION(_classname, _seq)\
   DECLARE_JSON_SERIALIZER(_classname)

//-------------------------------------------------------
///\brief   Declare the JSON entity classes implementation
///         -> the class CxxxxEntityJsonDefinition static fields definition 
///             Example : for entity class CHardware :
///                   std::string CHardwareEntityJsonDefinition::m_IdIdentifier = "id" ;
///                   std::string CHardwareEntityJsonDefinition::m_NameIdentifier = "name" ;
///                   std::string CHardwareEntityJsonDefinition::m_PluginNameIdentifier = "pluginName" ;
///                   std::string CHardwareEntityJsonDefinition::m_ConfigurationIdentifier = "configuration" ;
///                   std::string CHardwareEntityJsonDefinition::m_EnabledIdentifier = "enabled" ;
///                   std::string CHardwareEntityJsonDefinition::m_DeletedIdentifier = "deleted" ;
///                   
///               
///         -> the class CxxxxxEntitySerializer implementation
///             Example : for entity class CHardware : 
///                  CHardwareEntitySerializer::CHardwareEntitySerializer()
///                  {
///                  }
///                  
///                  CHardwareEntitySerializer::~CHardwareEntitySerializer()
///                  {
///                  }
///                  
///                  // IEntitySerializer implementation
///                  CJson CHardwareEntitySerializer::serialize(const CHardware & object)
///                  {
///                     CJson result;
///                     result.put(CHardwareEntityJsonDefinition::getIdIdentifier(), object.getId());
///                     result.put(CHardwareEntityJsonDefinition::getEnabledIdentifier(), object.getEnabled());
///                     result.put(CHardwareEntityJsonDefinition::getDeletedIdentifier(), object.getDeleted());
///                     result.put(CHardwareEntityJsonDefinition::getConfigurationIdentifier(), object.getConfiguration());
///                     result.put(CHardwareEntityJsonDefinition::getNameIdentifier(), object.getName());
///                     result.put(CHardwareEntityJsonDefinition::getPluginNameIdentifier(), object.getPluginName());
///                     return result;
///                  }
///                  
///                  boost::shared_ptr<CHardware> CHardwareEntitySerializer::deserialize(const CJson & object)
///                  {
///                     CHardware entity;
///                  
///                     if(object.find(CHardwareEntityJsonDefinition::getIdIdentifier()) != object.not_found())
///                        entity.setId(object.get<int>(CHardwareEntityJsonDefinition::getIdIdentifier()));
///                  
///                     if(object.find(CHardwareEntityJsonDefinition::getEnabledIdentifier()) != object.not_found())
///                        entity.setEnabled(object.get<bool>(CHardwareEntityJsonDefinition::getEnabledIdentifier()));
///                  
///                     if(object.find(CHardwareEntityJsonDefinition::getDeletedIdentifier()) != object.not_found())
///                        entity.setDeleted(object.get<bool>(CHardwareEntityJsonDefinition::getDeletedIdentifier()));
///                  
///                     if(object.find(CHardwareEntityJsonDefinition::getConfigurationIdentifier()) != object.not_found())
///                        entity.setConfiguration(object.get<std::string>(CHardwareEntityJsonDefinition::getConfigurationIdentifier()));
///                  
///                     if(object.find(CHardwareEntityJsonDefinition::getNameIdentifier()) != object.not_found())
///                        entity.setName(object.get<std::string>(CHardwareEntityJsonDefinition::getNameIdentifier()));
///                  
///                     if(object.find(CHardwareEntityJsonDefinition::getPluginNameIdentifier()) != object.not_found())
///                        entity.setPluginName(object.get<std::string>(CHardwareEntityJsonDefinition::getPluginNameIdentifier()));
///                  
///                     return entity;
///                  }
#define DECLARE_JSON_ENTITY_IMPLEMENTATION(_classname, _usualname, _seq)\
   DECLARE_JSON_IDENTIFIER(_classname, _usualname, _seq)\
   DECLARE_JSON_SERIALIZER_IMPLEMENTATION(_classname, _seq)