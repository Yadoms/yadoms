#pragma once

//
// This file provide macros used to declare header and implementation for SQLite entity adapter
//
// Macros :
//          DECLARE_ADAPTER_HEADER(entityName)
//
//             generate the adapter class header
//             i.e. : DECLARE_ADAPTER_HEADER(Configuration) generates CConfigurationAdapter class
//
//
//          DECLARE_ADAPTER_IMPLEMENTATION(entityName,
//             ((entityField1)(entityField1Type)(entityField1DefaultValue)(optional_internal_data_type)),
//             ((entityField2)(entityField1Type)(entityField1DefaultValue)(optional_internal_data_type)),
//             ...
//             )
//
//             generate the adapter class implementation
//
//
// ==========================================================================
//        HEADER PART
// ==========================================================================
//
// Example :
//
//   Input :
//             DECLARE_ADAPTER_HEADER(Configuration)
//
//    Ouptut :
//             class CConfigurationAdapter : public ISQLiteResultAdapter<boost::shared_ptr<CConfiguration> >
//             {
//             public:
//             	CConfigurationAdapter ();
//             	virtual~ CConfigurationAdapter();
//             	bool adapt(sqlite3_stmt * pStatement);
//             	std::vector<boost::shared_ptr<CConfiguration> > getResults()
//             	{
//             		return m_results;
//             	}
//             private:
//             	std::vector<boost::shared_ptr<CConfiguration> > m_results;
//             };
//
// ==========================================================================
//        SOURCE PART
// ==========================================================================
//
//
// Example : CConfiguration entity => implementation of class
//
//   Input :
//             DECLARE_ADAPTER_IMPLEMENTATION(Configuration,
//                ((Section)(std::string)(""))
//                ((Name)(std::string)(""))
//                ((Value)(std::string)(""))
//                ((DefaultValue)(std::string)(""))
//                ((Description)(std::string)(""))
//                ((SecurityAccess)(CConfiguration::ESecurityAccess)(CConfiguration::kNone)(int))
//                ((LastModificationDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
//                )
//
//    Ouptut :
//                CConfigurationAdapter::CConfigurationAdapter()
//                {
//                }
//
//                CConfigurationAdapter::~CConfigurationAdapter()
//                {
//                }
//
//                bool CConfigurationAdapter::adapt(sqlite3_stmt * pStatement)
//                {
//                	int nCols = sqlite3_column_count(pStatement);
//                	if (nCols)
//                	{
//                		std::vector<std::string> cols;
//                		for (int nCol = 0; nCol < nCols; nCol++)
//                			cols.push_back(std::string(sqlite3_column_name(pStatement, nCol)));
//
//                		int rc;
//                		while ((rc = sqlite3_step(pStatement)) == SQLITE_ROW)
//                		{
//                			boost::shared_ptr<CConfiguration> newEntity(new CConfiguration());
//                			for (int nCol = 0; nCol < nCols; nCol++)
//                			{
//                				if(boost::iequals(CConfigurationTable::getSectionColumnName(), cols[nCol]))
//                				{
//                					if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL)
//                						newEntity->setSection (""  );
//                					else
//                						newEntity->setSection ( CSQLite3Extension::extractData< std::string  >(pStatement, nCol) );
//                				}
//                				else if(boost::iequals(CConfigurationTable::getNameColumnName(), cols[nCol]))
//                				{
//                					if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL)
//                						newEntity->setName (""  );
//                					else
//                						newEntity->setName ( CSQLite3Extension::extractData< std::string  >(pStatement, nCol) );
//                				}
//                				else if(boost::iequals(CConfigurationTable::getValueColumnName(), cols[nCol]))
//                				{
//                					if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL)
//                						newEntity->setValue (""  );
//                					else
//                						newEntity->setValue ( CSQLite3Extension::extractData< std::string  >(pStatement, nCol) );
//                				}
//                				else if(boost::iequals(CConfigurationTable::getDefaultValueColumnName(), cols[nCol]))
//                				{
//                					if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL)
//                						newEntity->setDefaultValue (""  );
//                					else
//                						newEntity->setDefaultValue ( CSQLite3Extension::extractData< std::string  >(pStatement, nCol) );
//                				}
//                				else if(boost::iequals(CConfigurationTable::getDescriptionColumnName(), cols[nCol]))
//                				{
//                					if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL)
//                						newEntity->setDescription (""  );
//                					else
//                						newEntity->setDescription ( CSQLite3Extension::extractData< std::string  >(pStatement, nCol) );
//                				}
//                				else if(boost::iequals(CConfigurationTable::getSecurityAccessColumnName(), cols[nCol]))
//                				{
//                					if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL)
//                						newEntity->setSecurityAccess (CConfiguration::kNone );
//                					else
//                						newEntity->setSecurityAccess ( (CConfiguration::ESecurityAccess)CSQLite3Extension::extractData< int  >(pStatement, nCol) );
//                				}
//                				else if(boost::iequals(CConfigurationTable::getLastModificationDateColumnName(), cols[nCol]))
//                				{
//                					if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL)
//                						newEntity->setLastModificationDate (boost::posix_time::second_clock::universal_time()  );
//                					else
//                						newEntity->setLastModificationDate ( CSQLite3Extension::extractData< boost::posix_time::ptime  >(pStatement, nCol) );
//                					}
//                				else
//                				{
//                					 YADOMS_LOG(warning) << "Unknown column Name= " << cols[nCol] << " Value=" << CSQLite3Extension::sqlite3_column_text_utf8(pStatement, nCol);
//                				}
//                			}
//                			m_results.push_back(newEntity);
//                		}
//                		return true;
//                	}
//                	return false;
//                }




#define ADAPTER_COLUMN_ID 0
#define ADAPTER_COLUMN_TYPE 1
#define ADAPTER_COLUMN_DEFAULT 2
#define ADAPTER_COLUMN_INTERNAL_TYPE 3


//-------------------------------------------------------------------
/// \brief  Macro which extract the internal data type
///     -> in common case it returns the type of the field (int, double,...)
///     -> in case of enum, it returns the internal type (int)
//-------------------------------------------------------------------
#define ADAPT_COLUMN_GET_INTERNAL_TYPE(_elem) \
   BOOST_PP_SEQ_ELEM (BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(_elem), 4 ), ADAPTER_COLUMN_INTERNAL_TYPE, ADAPTER_COLUMN_TYPE), _elem)

//-------------------------------------------------------------------
/// \brief  Macro which extract the real data type
///     -> in common case it returns nothing (empty)
///     -> in case of enum, it returns the enum type
//-------------------------------------------------------------------
#define ADAPT_COLUMN_GET_REAL_TYPE(_elem) \
   BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(_elem), 4 ), (BOOST_PP_SEQ_ELEM(ADAPTER_COLUMN_TYPE, _elem)), BOOST_PP_EMPTY())

//-------------------------------------------------------------------
/// \brief  Macro which is called for each entity member and provide setter implementation
//-------------------------------------------------------------------
#define ADAPT_COLUMN(r, data, idx, elem) \
   BOOST_PP_IF(BOOST_PP_EQUAL(idx,0), BOOST_PP_EMPTY(), else) \
   if(boost::iequals(BOOST_PP_CAT(C##data##Table::get,BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(ADAPTER_COLUMN_ID, elem), ColumnName())), cols[nCol])) \
	{ \
      if(sqlite3_column_type(pStatement, nCol) == SQLITE_NULL) \
   		BOOST_PP_CAT(newEntity->set, BOOST_PP_SEQ_ELEM(ADAPTER_COLUMN_ID, elem)) (BOOST_PP_SEQ_ELEM(ADAPTER_COLUMN_DEFAULT, elem) ); \
      else \
         BOOST_PP_CAT(newEntity->set, BOOST_PP_SEQ_ELEM(ADAPTER_COLUMN_ID, elem)) ( ADAPT_COLUMN_GET_REAL_TYPE(elem) CSQLite3Extension::extractData< ADAPT_COLUMN_GET_INTERNAL_TYPE(elem) >(pStatement, nCol) ); \
   }

//-------------------------------------------------------------------
/// \brief  Macro which calls ADAPT_COLUMN for each value in input set
//-------------------------------------------------------------------
#define ADAPT_COLUMNS(_tablename, _seq) \
   BOOST_PP_SEQ_FOR_EACH_I(ADAPT_COLUMN, _tablename, _seq)


//-------------------------------------------------------------------
/// \brief  Retreive the entity class name
//-------------------------------------------------------------------
#define ENTITY_CLASS(_tablename) BOOST_PP_CAT(C,_tablename)


//-------------------------------------------------------------------
/// \brief  Retreive the entity class name
//-------------------------------------------------------------------
#define ADAPTER_CLASS(_tablename) C##_tablename##Adapter


//-------------------------------------------------------------------
/// \brief  Declare the entity adapter class header
//-------------------------------------------------------------------
#define DECLARE_ADAPTER_HEADER(_tablename)\
   class ADAPTER_CLASS(_tablename) : public ISQLiteResultAdapter<boost::shared_ptr<ENTITY_CLASS(_tablename)> > \
   { \
   public: \
      ADAPTER_CLASS(_tablename) (); \
      virtual~ ADAPTER_CLASS(_tablename)(); \
      bool adapt(sqlite3_stmt * pStatement); \
      std::vector<boost::shared_ptr<ENTITY_CLASS(_tablename)> > getResults() { return m_results; } \
   private: \
      std::vector<boost::shared_ptr<ENTITY_CLASS(_tablename)> > m_results;\
   };

//-------------------------------------------------------------------
/// \brief  Declare the entity adapter class constructor implementation
//-------------------------------------------------------------------
#define DECLARE_ADAPTER_IMPLEMENTATION_CTOR(_tablename) ADAPTER_CLASS(_tablename)::ADAPTER_CLASS(_tablename)() {}

//-------------------------------------------------------------------
/// \brief  Declare the entity adapter class destructor implementation
//-------------------------------------------------------------------
#define DECLARE_ADAPTER_IMPLEMENTATION_DTOR(_tablename) ADAPTER_CLASS(_tablename)::~ADAPTER_CLASS(_tablename)() {}


//-------------------------------------------------------------------
/// \brief  Declare the entity adapter class adapt method implementation
//-------------------------------------------------------------------
#define DECLARE_ADAPTER_IMPLEMENTATION_ADAPT(_tablename, _seq)\
   bool ADAPTER_CLASS(_tablename)::adapt(sqlite3_stmt * pStatement) \
   { \
      int nCols = sqlite3_column_count(pStatement); \
      if (nCols) \
      { \
         std::vector<std::string> cols; \
         for (int nCol = 0; nCol < nCols; nCol++) \
            cols.push_back(std::string(sqlite3_column_name(pStatement, nCol))); \
         int rc; \
         while ((rc = sqlite3_step(pStatement)) == SQLITE_ROW) \
         { \
            boost::shared_ptr<ENTITY_CLASS(_tablename)> newEntity(new ENTITY_CLASS(_tablename)()); \
            for (int nCol = 0; nCol < nCols; nCol++) \
            { \
               ADAPT_COLUMNS(_tablename, _seq) \
               else \
               { \
                  YADOMS_LOG(warning) << "Unknown column Name= " << cols[nCol] << " Value=" << CSQLite3Extension::sqlite3_column_text_utf8(pStatement, nCol); \
               } \
            } \
            m_results.push_back(newEntity); \
         } \
         return true; \
      } \
      return false; \
   }

//-------------------------------------------------------------------
/// \brief  Declare the entity adapter class full implementation
//-------------------------------------------------------------------
#define DECLARE_ADAPTER_IMPLEMENTATION(_tablename, _seq)\
   DECLARE_ADAPTER_IMPLEMENTATION_CTOR(_tablename)\
   DECLARE_ADAPTER_IMPLEMENTATION_DTOR(_tablename)\
   DECLARE_ADAPTER_IMPLEMENTATION_ADAPT(_tablename, _seq)




