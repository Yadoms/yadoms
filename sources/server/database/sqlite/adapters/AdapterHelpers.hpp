#pragma once



//
/// \brief  Empty adapter (used when no need to convert from string)
//
#define STRING_ADAPTER(x) x
#define INT_ADAPTER(x) boost::lexical_cast<int>(x)
#define BOOL_ADAPTER(x) boost::lexical_cast<bool>(x)
#define DOUBLE_ADAPTER(x) boost::lexical_cast<double>(x)
#define FLOAT_ADAPTER(x) boost::lexical_cast<float>(x)
#define DATE_ADAPTER(x) boost::gregorian::from_string(x)


#define ADAPTER_COLUMN_ID 0
#define ADAPTER_COLUMN_ADAPTER 1
#define ADAPTER_COLUMN_DEFAULT 2



#define ADAPT_COLUMN(r, data, idx, elem) \
   BOOST_PP_IF(BOOST_PP_EQUAL(idx,0), BOOST_PP_EMPTY(), else) \
   if(boost::iequals(BOOST_PP_CAT(C##data##Table::get,BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(ADAPTER_COLUMN_ID, elem), ColumnName())), columnNames[i])) \
	{ \
      if(columValues[i] == NULL) \
   		BOOST_PP_CAT(newEntity->set, BOOST_PP_SEQ_ELEM(ADAPTER_COLUMN_ID, elem)) (BOOST_PP_SEQ_ELEM(ADAPTER_COLUMN_DEFAULT, elem) ); \
      else \
         BOOST_PP_CAT(newEntity->set, BOOST_PP_SEQ_ELEM(ADAPTER_COLUMN_ID, elem)) (BOOST_PP_SEQ_ELEM(ADAPTER_COLUMN_ADAPTER, elem)(columValues[i]) ); \
   }


//
/// \brief  Macro which calls ADAPT_COLUMN for each value in input set
//
#define ADAPT_COLUMNS(_tablename, _seq) \
   BOOST_PP_SEQ_FOR_EACH_I(ADAPT_COLUMN, _tablename, _seq)


//
/// \brief  Retreive the entity class name
//
#define ENTITY_CLASS(_tablename) BOOST_PP_CAT(C,_tablename)


//
/// \brief  Retreive the entity class name
//
#define ADAPTER_CLASS(_tablename) C##_tablename##Adapter


//
/// \brief  Provide code for method ISQLiteResultAdapter::adapt(int column, char** columValues, char** columnNames)
///         The aim is to automatically map  SQLite result into entity class instance
//
#define ADAPT(_tablename, _seq) \
   bool returnValue = false; \
   if(column == 0 || columValues == NULL || columnNames == NULL) \
   { \
      YADOMS_LOG(warning) << "Invalid arguments"; \
      returnValue = false; \
   } \
   else \
   { \
      boost::shared_ptr<ENTITY_CLASS(_tablename)> newEntity(new ENTITY_CLASS(_tablename)()); \
      for(int i=0; i<column ; i++) \
      { \
         ADAPT_COLUMNS(_tablename, _seq) \
         else \
         { \
            YADOMS_LOG(warning) << "Unknown column Name= " << columnNames[i] << " Value=" << columValues[i]; \
         } \
      } \
      m_results.push_back(newEntity); \
      returnValue = true; \
   } \
   return returnValue;


//
/// \brief  Declare the full adapter class
//
#define DECLARE_ADAPTER(_tablename, _seq) \
   class ADAPTER_CLASS(_tablename) : public ISQLiteResultAdapter<boost::shared_ptr<ENTITY_CLASS(_tablename)> > \
   { \
   public: \
      ADAPTER_CLASS(_tablename) () { } \
      virtual~ ADAPTER_CLASS(_tablename)() { } \
      bool adapt(int column, char** columValues, char** columnNames) { ADAPT(_tablename, _seq) } \
      std::vector<boost::shared_ptr<ENTITY_CLASS(_tablename)> > getResults() { return m_results; } \
   private: \
      std::vector<boost::shared_ptr<ENTITY_CLASS(_tablename)> > m_results;\
   };






