class IDatabaseProvider
{
public:
  virtual ~IDatabaseProvider()
  {
  }

  //--------------------------------------------------------------
  /// \Brief		Load the database system
  /// \param [in]	pConfig: startup configuration
  /// \return 		true if database is loaded with success
  /// \throws 		
  //--------------------------------------------------------------
  bool load(IStartupConfiguration & pConfig);
  
  //--------------------------------------------------------------
  /// \Brief	Unoad the database system
  //--------------------------------------------------------------
  void unload();

  IHardwareRequester * getHardwareRequester();
  
protected:
  IDatabaseProvider()
  {
  }
};

class IDatabaseQueryController
{
public:
  virtual ~IDatabaseHandler()
  {
  }
  
  //--------------------------------------------------------------
  /// \Brief		execute a single statement
  /// \param [in]	query: the sql query
  /// \return 		number of rows affected
  /// \throws 		
  //--------------------------------------------------------------
  int executeStatement(const std::string & query) = 0;

  //--------------------------------------------------------------
  /// \Brief		execute a query
  /// \param [in]	query: the sql query
  /// \return 		the results
  /// \throws 		
  //--------------------------------------------------------------
  std::list<IResultHandler> executeQuery(const std::string & query) = 0;
  
protected:
  IDatabaseHandler()
  {
  }
  
};

class CSQliteQueryController : public IDatabaseQueryController
{
public:
  CSQliteQueryController(sqlite * pDatabaseHandler)
	:m_pDatabaseHandler(pDatabaseHandler)
  {
  }
  
  virtual ~CSQliteQueryController()
  {
  }
  
  int executeStatement(const std::string & query)
  {
	return 0;
  }

  template<class T>
  std::list<T> executeQuery(const std::string & queryFormat, ...)
  {
	//ASSERT(T is IRowHandler);
	
	CResultEntityController<T> results;
	
	//create query
	char *zSql;
	va_list ap;
	va_start(ap, queryFormat.c_str());
	zSql = sqlite3_vmprintf(queryFormat.c_str(), ap);
	va_end(ap);
  
	//exec a first time
	char *zErrMsg = 0;
	int rc = sqlite3_exec(m_pDatabaseHandler, zSql, handleOneRow, &results, zErrMsg);
  
	//if SQLITE_SCHEMA error, retry one time
	if( rc==SQLITE_SCHEMA )
	{
		if( zErrMsg ) 
			free(zErrMsg);
		rc = sqlite3_exec(m_pDatabaseHandler, zSql, handleOneRow, &results, zErrMsg);
	}

	if( zErrMsg )
	{
		fprintf(stdout,"%s: query failed: %s - %s\n", zFile, zSql, zErrMsg);
	}
	
	//free
	sqlite3_free(zErrMsg);
	sqlite3_free(zSql);
	
	return results.getResults();
  }
  
  
  
  int handleOneRow(void* params ,int columnCount,char** columnValues,char** columnNames)
  {
	CResultEntityController<IRowHandler>  * pResults = (CResultEntityController<IRowHandler>  *)params;
	
	//create map of <columnName, columnValue>
	std::map<std::string, std::string> columnsWithValues;
	for(int i=0; i<columnCount; i++)
	{
		columnsWithValues.insert(std::make_pair(columnNames[i], sqlite3_mprintf("%s",columnValues[i] ? columnValues[i] : "")));
	}
	
	//make entity parse result
	pResults->createNewEntity()->HandleRow(columnsWithValues);
  }
  
private:
	sqlite * m_pDatabaseHandler;
};


template<class T>
class CResultEntityController
{
public:
	T * createNewEntity() 
	{ 
		T * pNewEntity  = new T();
		m_results.push_back(T);
		return pNewEntity; 
	}
	
	std::list<T> getResults() { return m_results; }
private:
	std::list<T> m_results;
};

class IRowHandler
{
public:
	virtual ~IRowHandler()
	{
	}
	
	virtual void HandleRow(int columnCount,char** columnValues,char** columnNames) =0;

protected:
	IRowHandler()
	{
	}
};

/*
	Exemple Table:
		Hardware
			id (int)
			name (text)
			pluginName (text)
*/
	class CHardware : public CEntityBase, public IRowHandler
	{
	public:
		CHardware()
		{
		}
		
		virtual CHardware()
		{
		}
		
	public:
		void HandleRow(std::map<std::string, std::string> dataFromQuery)
		{
			std::map<std::string, std::string>::iterator i;
			for(i=dataFromQuery.begin(); i!=dataFromQuery.end(); i++)
			{
				if(i->first == ColmunNameId)
					setId(boost::lexical_cast<int>(i->second));
				else if(i->first == ColmunName)
					setName(i->second);
				else if(i->first == ColmunPluginName)
					setPluginName(i->second);
			}
			Reset();
		}
		
		
		
	public:
		const int getId() {return m_id; }
		void setId(const int newId) { m_id = newId; RaisePropertyChanged(ColmunNameId); }

		const std::string getName() {return m_name; }
		void setName(const int newName) { m_name = newName; RaisePropertyChanged(ColmunName); }

		const std::string getPluginName() {return m_pluginName; }
		void setPluginName(const int newPluginName) { m_pluginName = newPluginName; RaisePropertyChanged(ColmunPluginName); }
	
	private:
		static std::string ColmunId = "id";
		static std::string ColmunName = "name";
		static std::string ColmunPluginName = "pluginName";
	
	private:
		int m_id;
		std::string m_name;
		std::stirng m_pluginName;
	};
	
	class CEntityBase
	{
	protected:
		CEntityBase() 
		{
		}
		
		virtual CEntityBase()
		{
		}
		
		void RaisePropertyChanged(const std::string & fieldName)
		{
			if(m_modifiedFieldsList.find(fieldName) == m_modifiedFieldsList.end())
			{
				m_modifiedFieldsList.push_back(fieldName);
			}
		}
		
		void Reset()
		{
			m_modifiedFieldsList.clear();
		}
		
		
	private:
		std::list<std::string>	m_modifiedFieldsList;
	};
	
	

	/*
	boost::scoped_ptr<IDatabaseHandler> pDatabaseHandler = new ...();
	
	boost::scoped_ptr<IDatabaseQueryController> queryController = pDatabaseHandler->getQueryController();
	
	std::list<CHardware> allHardware = queryController->executeQuery<CHardware>("SELECT * FROM Hardware");
	*/
	
	
	


