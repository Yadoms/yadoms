#pragma once

//
// This file provide macros used to declare database entities
//
// An entity is a container class to encapsulate of database row (if using database)
//
// Macros :
//          DECLARE_ENTITY_CLASS(entityName,
//             ((entityField1)(entityField1Type)(entityField1DefaultValue)),
//             ((entityField2)(entityField1Type)(entityField1DefaultValue)),
//             ...
//             )
//
//             generate the entoty class 
//
//
// ==========================================================================
//        HEADER PART
// ==========================================================================
//
// Example :
//
//   Input :
//                DECLARE_ENTITY_CLASS(Acquisition,
//                   ((Id)(int)(0))
//                   ((Source)(std::string)(""))
//                   ((Keyword)(std::string)(""))
//                   ((Value)(std::string)(""))
//                   ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
//                   )
//
//    Ouptut :
//                class CAcquisition
//                {
//                public: 
//                    CAcquisition()
//                    {
//                        m_Id_filled = false;
//                        m_Id = 0 ;
//                        m_Source_filled = false;
//                        m_Source = "" ;
//                        m_Keyword_filled = false;
//                        m_Keyword = "" ;
//                        m_Value_filled = false;
//                        m_Value = "" ;
//                        m_Date_filled = false;
//                        m_Date = boost::posix_time::second_clock::universal_time() ;
//                    }
//                    virtual ~CAcquisition()
//                    {
//                    }
//             
//                public: 
//                   const int  getId() const { return m_Id; }
//                   const bool isIdFilled() const { return m_Id_filled; }
//                   CAcquisition& setId(const int  newValue)
//                   {
//                      m_Id = newValue;
//                      m_Id_filled = true;
//                      return *this;
//                   }
//             
//                private: 
//                   int  m_Id;
//                   bool m_Id_filled;
//             
//                public: 
//                   const std::string  getSource() const { return m_Source; }
//                   const bool isSourceFilled() const { return m_Source_filled; }
//                   CAcquisition& setSource(const std::string  newValue)
//                   {
//                      m_Source = newValue;
//                      m_Source_filled = true;
//                      return *this;
//                   }
//                private: 
//                   std::string  m_Source;
//                   bool m_Source_filled;
//                public: 
//                   const std::string  getKeyword() const { return m_Keyword; }
//                   const bool isKeywordFilled() const { return m_Keyword_filled; }
//                   CAcquisition& setKeyword(const std::string  newValue)
//                   {
//                      m_Keyword = newValue;
//                      m_Keyword_filled = true;
//                      return *this;
//                   }
//                private: 
//                   std::string  m_Keyword;
//                   bool m_Keyword_filled;
//                public: 
//                   const std::string  getValue() const { return m_Value; }
//                   const bool isValueFilled() const { return m_Value_filled; }
//                   CAcquisition& setValue(const std::string  newValue)
//                   {
//                      m_Value = newValue;
//                      m_Value_filled = true;
//                      return *this;
//                   }
//                private: 
//                   std::string  m_Value;
//                   bool m_Value_filled;
//                public: 
//                   const boost::posix_time::ptime  getDate() const { return m_Date; }
//                   const bool isDateFilled() const { return m_Date_filled; }
//                   CAcquisition& setDate(const boost::posix_time::ptime  newValue)
//                   {
//                      m_Date = newValue;
//                      m_Date_filled = true;
//                      return *this;
//                   }
//                private: 
//                   boost::posix_time::ptime  m_Date;
//                   bool m_Date_filled;
//                };



#define ENTITY_COLUMN_NAME   0
#define ENTITY_COLUMN_TYPE   1
#define ENTITY_COLUMN_DEFAULT   2

//
/// \brief Macro used to declare the entity class name
//
#define ENTITY_CLASSNAME(_classname) C##_classname


//
/// \brief Macro used to declare the entity field filled status
//
#define ENTITY_FIELD(_fieldName) _fieldName
//#define ENTITY_FIELD(_fieldName) BOOST_PP_CAT(m_, _fieldName)


//
/// \brief Macro used to declare the entity field filled status
//
#define ENTITY_FIELD_FILLED(_fieldName) BOOST_PP_CAT(BOOST_PP_CAT(m_, _fieldName), _filled)

//
/// \brief Macro used to declare a private field with public getter and setter
//
#define DELCARE_FIELD(className, fieldType, fieldName) \
   CField< fieldType > ENTITY_FIELD(fieldName);
   /*
   public:\
      const CField< fieldType > fieldName() const {return ENTITY_FIELD(fieldName); } \
   private:\
      CField< fieldType > ENTITY_FIELD(fieldName);\*/
      
//
/// \brief Macro which initialize a field filled status to false (the field is not yet filled)
//
#define DECLARE_ENTITY_FILLED_INITIALISER(r, data, idx, elem) \
   BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(idx,0)) \
   ENTITY_FIELD(BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_NAME, elem))(BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_DEFAULT, elem))
      
//
/// \brief  Macro which initialize all fields state to false
///         For each ID in input sequence, call DECLARE_ENTITY_FILLED_INITIALISER
//
#define DECLARE_ENTITY_FILLED_INITIALISERS(_seq) \
   BOOST_PP_SEQ_FOR_EACH_I(DECLARE_ENTITY_FILLED_INITIALISER, _, _seq)      


//
/// \brief  Macro which declare one field and its filled status
//
#define DECLARE_ENTITY_FIELD(r, _classname, elem) \
   DELCARE_FIELD(_classname, BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_TYPE, elem), BOOST_PP_SEQ_ELEM(ENTITY_COLUMN_NAME, elem))
 
//
/// \brief  Macro which declare all fields and its filled status
///         For each ID in input sequence, call DECLARE_ENTITY_FILLED_INITIALISER
//
#define DECLARE_ENTITY_FIELDS(_classname, _seq) \
   BOOST_PP_SEQ_FOR_EACH(DECLARE_ENTITY_FIELD, _classname, _seq)      



   
//
/// \brief  Declare a table
/// \param  name  the table name
/// \param  _seq  the sequence of columns
//
#define DECLARE_ENTITY_CLASS(_classname, _seq)                               \
class ENTITY_CLASSNAME(_classname)                                           \
{                                                                            \
   public:                                                                   \
      ENTITY_CLASSNAME(_classname)()                                         \
        : DECLARE_ENTITY_FILLED_INITIALISERS(_seq)                           \
      {                                                                      \
      }                                                                      \
      virtual ~ENTITY_CLASSNAME(_classname)()                                \
      {                                                                      \
      }                                                                      \
public:                                                                      \
   DECLARE_ENTITY_FIELDS(_classname, _seq)                                   \
};
