#pragma once
#include <shared/exception/OutOfRange.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include "serialization/IDataSerializable.h"
#include "serialization/IDataFileSerializable.h"
#include "IDataContainable.h"
#include "enumeration/IExtendedEnum.h"
#include "Field.hpp"
 


namespace shared
{
   //--------------------------------------------------------------
   // ReSharper disable CommentTypo
   ///			***********************
   ///			** USAGE
   ///			***********************
   ///			A/ Simple type USAGE
   ///			
   ///			   1. simple data 
   ///			   	shared::CDataContainer test;
   ///			   	test.set("intValue", 5);
   ///			   	test.set("doubleValue", 4.0);
   ///			   	test.set("stringValue", "plouf");
   ///			   
   ///			   	int i = test.get<int>("intValue");
   ///			   	double d = test.get<double>("doubleValue");
   ///			   	std::string s = test.get<std::string>("stringValue");
   ///			   
   ///			   
   ///			   2. vector of data
   ///			   	std::vector<int> vi;
   ///			   	...
   ///			   	test.set("vectorint", vi);
   ///			   
   ///			   	std::vector<double> vd;
   ///			   	...
   ///			   	test.set("vectordouble", vd);
   ///			   
   ///			   
   ///			   	std::vector<int> vi2 = test.get< std::vector<int> >("vectorint");
   ///			   	std::vector<double> vd2 = test.get< std::vector<double> >("vectordouble");
   ///			   
   ///			   
   ///			   
   ///			   3. sub container (act as a tree)
   ///               
   ///               //create another container
   ///               shared::CDataContainer subContainer;
   ///               subContainer.set("int4", 6);
   ///               subContainer.set("double1", 8.0);
   ///               subContainer.set("string2", "plouf2");
   ///               
   ///               //add it to main container
   ///               test.set("config1", subContainer);
   ///               //retrieve container
   ///               shared::CDataContainer subt = test.get<shared::CDataContainer>("config1");
   ///               //direct access to a sub container data
   ///               double d = test.get<double>("config1.double1");
   ///
   ///            4. boost::shared_ptr<>
   ///               The use of shared_ptr is completly transparent. It works like if you get/set directly the encapsulated type
   ///               
   ///               //works with boost::shared_ptr of any simple type
   ///               //you can put a shared_ptr and get the data direclty without using shared_ptr for reading
   ///
   ///               boost::shared_ptr<std::string> shs(new std::string("un shared ptr"));
   ///               test.set("StringSharedPtr", shs);
   ///               
   ///               boost::shared_ptr<std::string> shs2 = test.get< boost::shared_ptr<std::string> >("StringSharedPtr");
   ///               std::string direct = test.get< std::string >("StringSharedPtr");
   ///               
   ///             5. std::vector< boost::shared_ptr<> >
   ///               
   ///               //check vector of shared_ptr<int>
   ///               std::vector< boost::shared_ptr<int> > vish;
   ///               ....
   ///               test.set("vectorintsh", vish);
   ///               std::vector< boost::shared_ptr<int> > vish2 = test.get< std::vector< boost::shared_ptr<int> > >("vectorintsh");
   ///               std::vector< int > vi2 = test.get< std::vector< int > >("vectorintsh");
   ///            
   ///             6. std::vector< shared::CDataContainer >
   ///               
   ///               std::vector< shared::CDataContainer > vcd;
   ///               ....
   ///               test.set("containers", vcd);
   ///               std::vector< shared::CDataContainer > vcd2 = test.get< std::vector< shared::CDataContainer > >("containers");
   ///            
   ///
   ///         B. Enhanced usage (IDataContainable)
   ///            This is an interface which allow any class to be get/set in this container.
   ///            This interface have only two methods to override, one for filling the class instance from a container, 
   ///            and another to fill the container from the class instance
   ///
   ///            Example:
   ///               
   ///               class CTestClass : public shared::IDataContainable
   ///               {
   ///               public:
   ///                  CTestClass(int i, double d, std::string s)
   ///                     :m_aIntValue(i), m_dValue(d), m_sValue(s)
   ///                  {
   ///               
   ///                  }
   ///               
   ///                  virtual void extractContent(shared::CDataContainer & cont) const
   ///                  {
   ///                     cont.set("Value1", m_aIntValue);
   ///                     cont.set("Value2", m_dValue);
   ///                     cont.set("Value3", m_sValue);
   ///                  }
   ///               
   ///                  virtual void fillFromContent(const shared::CDataContainer & initialData)
   ///                  {
   ///                     m_aIntValue = initialData.get<int>("Value1");
   ///                     m_dValue = initialData.get<double>("Value2");
   ///                     m_sValue = initialData.get<std::string>("Value3");
   ///                  }
   ///               
   ///               private:
   ///                  int m_aIntValue;
   ///                  double m_dValue;
   ///                  std::string m_sValue;
   ///               };
   ///
   ///            1. Simple use
   ///               shared::CDataContainer cont;
   ///
   ///               CTestClass obj(1, 42.0, "test de datacontainble");
   ///               cont.set("myobject", obj);
   ///               CTestClass result = cont.get<CTestClass>("myobject");
   ///               
   ///            2. boost::shared_ptr<IDataContainable>
   ///
   ///               boost::shared_ptr<CTestClass> sp(new CTestClass(2, 43.0, "chaine1"));
   ///               cont.set("myobjectsh", sp);
   ///               boost::shared_ptr<CTestClass> result2 = cont.get< boost::shared_ptr<CTestClass> >("myobjectsh");
   ///               CTestClass result3 = cont.get< CTestClass >("myobjectsh");
   ///
   ///            3. std::vector<shared::IDataContainable>
   ///               
   ///               std::vector<CTestClass> vc;
   ///               for (int i = 0; i < 10; ++i)
   ///                  vc.push_back(CTestClass(i, 42.0 * i, "test de std::vector<IDataContainable>"));
   ///               shared::CDataContainer contvec;
   ///               contvec.set("mycollection", vc);
   ///               std::vector<CTestClass> vc2 = contvec.get< std::vector<CTestClass> >("mycollection");
   ///               
   ///             4. std::vector< boost::shared_ptr<IDataContainable> >
   ///
   ///               std::vector< boost::shared_ptr<CTestClass> > vcsh;
   ///               for (int i = 0; i < 10; ++i)
   ///                  vcsh.push_back(boost::shared_ptr<CTestClass>(new CTestClass(i, 42.0 * i, "test de std::vector<IDataContainable>")));
   ///               shared::CDataContainer contvecsh;
   ///               contvecsh.set("mycollectionofshared", vcsh);
   ///               std::vector< boost::shared_ptr<CTestClass> >  vcsh2 = contvecsh.get< std::vector< boost::shared_ptr<CTestClass> > >("mycollectionofshared");
   ///               std::vector< CTestClass >  vc2 = contvecsh.get< std::vector< CTestClass > >("mycollectionofshared");
   ///
   ///
   // ReSharper restore CommentTypo
   //--------------------------------------------------------------

   //--------------------------------------------------------------
   /// \brief	This class should be used as a data container
   ///			The container should contains data by a get/set method
   ///			Each data is associated to a key name
   ///			The data could be a simple type, a list of data, or a DataContainer child
   ///
   ///			The container has the serialize feature
   ///			This is done be serializing/deserializing a string,
   ///			or a file
   //--------------------------------------------------------------
   class CDataContainer : public serialization::IDataSerializable, public serialization::IDataFileSerializable /*, public IDataContainable*/
   {
   public:
      //--------------------------------------------------------------
      /// \brief		Constructor
      //--------------------------------------------------------------
      static boost::shared_ptr<CDataContainer> make();

      //--------------------------------------------------------------
      /// \brief		Constructor
      /// \param [in] initialData    Initial data for this container (json data)
      //--------------------------------------------------------------
      static boost::shared_ptr<CDataContainer> make(const std::string& initialData);

      //--------------------------------------------------------------
      /// \brief		Constructor
      /// \param [in] initialData    Initial data for this container (key,value)
      //--------------------------------------------------------------
      static boost::shared_ptr<CDataContainer> make(const std::map<std::string, std::string>& initialData);

      //--------------------------------------------------------------
      /// \brief		Constructor
      /// \param [in] d    Initial data for this container (boost::json::value&)
      //--------------------------------------------------------------
      static boost::shared_ptr<CDataContainer> make(const boost::json::object & d);
      
      //--------------------------------------------------------------
      /// \brief		Constructor
      /// \param [in] d    Initial data for this container (boost::json::value*)
      //--------------------------------------------------------------
      static boost::shared_ptr<CDataContainer> make(const boost::json::object* d);

      
      //--------------------------------------------------------------
      //
      //
      //
      // Constructors & destructor (internal use ONLY prefer use shared::CDataContainer::make(...) )
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief		Constructor. Empty data
      // ************************************************************************************
      // *********** internal use ONLY prefer use shared::CDataContainer::make(...) *********
      // ************************************************************************************
      //--------------------------------------------------------------
      CDataContainer();

      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      // ************************************************************************************
      // *********** internal use ONLY prefer use shared::CDataContainer::make(...) *********
      // ************************************************************************************
      //--------------------------------------------------------------
      explicit CDataContainer(const std::string & initialData);

      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial map data
      // ************************************************************************************
      // *********** internal use ONLY prefer use shared::CDataContainer::make(...) *********
      // ************************************************************************************
      //--------------------------------------------------------------
      explicit CDataContainer(const std::map<std::string, std::string> & initialData);


   private:
      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      CDataContainer(const CDataContainer & initialData);

   public:
	  //--------------------------------------------------------------
	  /// \brief		Constructor
	  /// \param [in] d    Initial data for this container (boost::json::value&)
     // ************************************************************************************
     // *********** internal use ONLY prefer use shared::CDataContainer::make(...) *********
     // ************************************************************************************
	  //--------------------------------------------------------------
      explicit CDataContainer(const boost::json::object& d);

	  //--------------------------------------------------------------
	  /// \brief		Constructor
	  /// \param [in] d    Initial data for this container (boost::json::value*)
     // ************************************************************************************
     // *********** internal use ONLY prefer use shared::CDataContainer::make(...) *********
     // ************************************************************************************
     //--------------------------------------------------------------
	  explicit CDataContainer(const boost::json::object * d);
      
      //--------------------------------------------------------------
      /// \brief			Destructor
      /// \return    	void
      //--------------------------------------------------------------
      virtual ~CDataContainer();

      //--------------------------------------------------------------
      //
      //
      //
      // Public get/set methods
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief	    Get parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      T get(const std::string& parameterName = std::string(), char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] defaultValue     The default value in case the parameter do not exists
      /// \param [in] pathChar         The path separator to use (default is '.')
      /// \return     The parameter value, or the default value if parameter not found
      //--------------------------------------------------------------
      template<class T>
      T getWithDefault(const std::string& parameterName, const T & defaultValue, char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Get child value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      /// \return     The child as boost::shared_ptr<CDataContainer>
      //--------------------------------------------------------------
      boost::shared_ptr<CDataContainer> getChild(const std::string& parameterName, char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      //--------------------------------------------------------------
      template<class T>
      void set(const std::string& parameterName, const T & value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      //--------------------------------------------------------------
      template<class T>
      void set(const char* parameterName, const T & value, char pathChar = '.');


      //--------------------------------------------------------------
      /// \brief	    Append value to an already existing array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      //--------------------------------------------------------------
      template<class T>
      void appendArray(const std::string& parameterName, const T& value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Append value to an already existing array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      //--------------------------------------------------------------
      template<class T>
      void appendArray(const char* parameterName, const T& value, char pathChar = '.');

      //--------------------------------------------------------------
      //
      //
      //
      // "Specializations" of get/set//appendArray method 
      //  for char* (cannot be managed as template specialization easily)
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      //--------------------------------------------------------------
      void set(const std::string & parameterName, const char* value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      //--------------------------------------------------------------
      void set(const char* parameterName, const char* value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Append a value to an array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      //--------------------------------------------------------------
      void appendArray(const char* parameterName, const char* value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Append a value to an array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      //--------------------------------------------------------------
      void appendArray(const std::string& parameterName, const char* value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Get parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      std::string get(const std::string & parameterName = std::string(), char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter value as map<string, string>
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<typename Type>
      std::map<std::string, Type> getAsMap(const std::string& parameterName = std::string(), char pathChar = '.') const;


      //--------------------------------------------------------------
      /// \brief	    Get parameter sub-keys as vector<string>
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      std::vector<std::string> getKeys(const std::string& parameterName = std::string(), char pathChar = '.') const;


      //--------------------------------------------------------------
      /// \brief	    Find a sub-parameter with criteria
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] where_fct         Criteria : lambda must returns true if item is found
      /// \param [in] pathChar         The path separator to use (default is '.')
      /// \return     The found parameter
      /// \throw      shared::exception::CEmptyResutl if no parameter matching criteria was found
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      CDataContainer find(const std::string& parameterName, boost::function<bool(const CDataContainer&)> where_fct, char pathChar = '.') const;


      //--------------------------------------------------------------
      /// \brief	    Make a full copy of current container
      /// \return     The newly created container
      //--------------------------------------------------------------
      boost::shared_ptr<CDataContainer> copy() const;

      //--------------------------------------------------------------
      //
      //
      //
      // Public get/set methods for ENUMS treated as string
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief	    Type representing authorized enum values list
      //--------------------------------------------------------------
      typedef std::map<std::string, unsigned int> EnumValuesNames;

      //--------------------------------------------------------------
      /// \brief	    Get enum parameter value
      /// \param [in] parameterName    Name of the enum parameter
      /// \param [in] valuesNames      List of authorized values for the enum
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter value is not in the provided list
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<typename EnumType>
      EnumType getEnumValue(const std::string& parameterName, const EnumValuesNames& valuesNames, char pathChar = '.') const;


      //--------------------------------------------------------------
      //
      //
      //
      // Public initialization methods
      //
      //
      //
      //--------------------------------------------------------------


      //--------------------------------------------------------------
      /// \brief		Copy another container to this instance
      /// \param [in] rhs  The container to copy
      //--------------------------------------------------------------
      void initializeWith(const CDataContainer &rhs);

      //--------------------------------------------------------------
      /// \brief		Copy another container to this instance
      /// \param [in] rhs  The container to copy
      //--------------------------------------------------------------
      void initializeWith(const boost::shared_ptr<CDataContainer>& rhs);

      //--------------------------------------------------------------
      //
      //
      //
      // Public utility methods
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief	    Check if parameter is present
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     true if parameter found
      //--------------------------------------------------------------
      bool exists(const std::string& parameterName, char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Check if a node contains at least a child which is an array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      /// \return     true if the parameter contains child (CDataContainer), false if it is a termination node with data
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      bool containsChildArray(const std::string& parameterName = std::string(), char pathChar = '.') const;
      
	  //--------------------------------------------------------------
      /// \brief	    Check if a node contains at least a child object (with a value : {...})
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      /// \return     true if the parameter contains child (CDataContainer), false if it is a termination node with data
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      bool containsChild(const std::string& parameterName = std::string(), char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Check if a node is terminal value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      /// \return     true if the path contains only single value, false if it is contains a subtree
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      bool containsValue(const std::string& parameterName = std::string(), char pathChar = '.') const;

	   //--------------------------------------------------------------
	   /// \brief	  Set a value as null
	   /// \param [in] parameterName    Name of the parameter
	   /// \param [in] pathChar         The path separator to use (default is '.')
	   //--------------------------------------------------------------
      void setNull(const std::string& parameterName = std::string(), char pathChar = '.');

	   //--------------------------------------------------------------
	   /// \brief	  Check if a value is null
	   /// \param [in] parameterName    Name of the parameter
	   /// \param [in] pathChar         The path separator to use (default is '.')
	   /// \return  true is the value is null
	   //--------------------------------------------------------------
	   bool isNull(const std::string& parameterName = std::string(), char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	  Create an empty array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      //--------------------------------------------------------------
      bool createArray(const std::string& parameterName, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	  Check if targeted value is an array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path separator to use (default is '.')
      //--------------------------------------------------------------
      bool isArray(const std::string& parameterName, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief		Equality operator
      /// \param [in] rhs  The container to compare with
      /// \return   true if the container equals to this instance
      //--------------------------------------------------------------
      bool operator ==(const CDataContainer &rhs) const;

      //--------------------------------------------------------------
      /// \brief		Difference operator
      /// \param [in] rhs  The container to compare with
      /// \return   true if the container is different to this instance
      //--------------------------------------------------------------
      bool operator !=(const CDataContainer &rhs) const;

      //--------------------------------------------------------------
      /// \brief		Affectation operator
      /// \param [in] rhs  The container to copy
      /// \return   Reference to this instance
      //--------------------------------------------------------------
      CDataContainer & operator=(const CDataContainer &rhs);

      //--------------------------------------------------------------
      /// \brief		Affectation operator
      /// \param [in] rhs  A serialized string
      /// \return   Reference to this instance
      //--------------------------------------------------------------
      CDataContainer & operator=(const std::string &rhs);

      //--------------------------------------------------------------
      /// \brief		Output operator (write the serialized container)
      /// \param [in] os  The stream to write
      /// \param [in] dc  The input data container
      /// \return   The stream
      //--------------------------------------------------------------
      friend std::ostream& operator<<(std::ostream& os, const CDataContainer& dc);

      //--------------------------------------------------------------
      /// \brief		Output operator (write the serialized container)
      /// \param [in] os  The stream to write
      /// \param [in] dc  The input data container as shared_ptr
      /// \return   The stream
      //--------------------------------------------------------------
      friend std::ostream& operator<<(std::ostream& os, const boost::shared_ptr<CDataContainer>& dc);

      //--------------------------------------------------------------
      /// \brief		Input operator (read a serialized container)
      /// \param [in] is  The stream to read
      /// \param [in] dc  The output data container
      /// \return   The stream
      //--------------------------------------------------------------
      friend std::istream& operator>>(std::istream& is, CDataContainer& dc);

      //--------------------------------------------------------------
      /// \brief		Input operator (read a serialized container)
      /// \param [in] is  The stream to read
      /// \param [in] dc  The output data container as shared_ptr
      /// \return   The stream
      //--------------------------------------------------------------
      friend std::istream& operator>>(std::istream& is, boost::shared_ptr<CDataContainer>& dc);

      //--------------------------------------------------------------
      /// \brief		Check if the container is empty
      /// \return    true if container is empty
      //--------------------------------------------------------------
      bool empty() const;

      //--------------------------------------------------------------
      /// \brief		Print the content to log
      /// \param [in] os      The out stream where to print out
      //--------------------------------------------------------------
      void printToLog(std::ostream& os) const;

      //--------------------------------------------------------------
      /// \brief		Print size to log
      /// \param [in] os      The out stream where to print out
      //--------------------------------------------------------------
      void printSizeToLog(std::ostream& os) const;

      //--------------------------------------------------------------
      /// \brief	    Merge this container from another one
      ///             Add non-existing (or replace existing) values of "from" container into this container
      ///             Values of this container not present in "from" container will be kept
      /// \param [in] from source container
      //--------------------------------------------------------------
      void mergeFrom(const CDataContainer& from);

      //--------------------------------------------------------------
      /// \brief	    Merge this container from another one
      ///             Add non-existing (or replace existing) values of "from" container into this container
      ///             Values of this container not present in "from" container will be kept
      /// \param [in] from source container
      //--------------------------------------------------------------
      void mergeFrom(boost::shared_ptr<CDataContainer> & from);

      //--------------------------------------------------------------
      //
      //
      //
      // Interface implementations
      //
      //
      //
      //--------------------------------------------------------------

      // IDataSerializable implementation
      std::string serialize() const override;
      void deserialize(const std::string & data) override;
      // [END] IDataSerializable implementation

      // IDataFileSerializable implementation
      void serializeToFile(const std::string & filename) const override;
      void deserializeFromFile(const std::string & filename) override;
      // [END] IDataFileSerializable implementation

      // IDataContainable implementation
      virtual void extractContent(CDataContainer &) const;
      virtual void fillFromContent(const CDataContainer & initialData);
      // [END] IDataContainable implementation



      //--------------------------------------------------------------
      //
      //
      //
      // Static helpers
      //
      //
      //
      //--------------------------------------------------------------


      //--------------------------------------------------------------
      /// \brief		An empty container which could be used as default method parameter
      //--------------------------------------------------------------
      const static CDataContainer EmptyContainer;
      const static boost::shared_ptr<CDataContainer> EmptyContainerSharedPtr;

   protected:
      
	  //--------------------------------------------------------------
	  /// \brief		Find a value from the name
	  /// \param [in] parameterName     The parameter name
     /// \param [in] pathChar          The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
     /// \param [in] createIfNotExists The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
	  //--------------------------------------------------------------
	  boost::json::value* findValue(const std::string& parameterName, char pathChar) const;
     boost::json::value* findValueOrCreate(const std::string& parameterName, char pathChar);

	  //--------------------------------------------------------------
	  /// \brief	Merge two objects
	  /// \param [in] dstObject    Destination object
	  /// \param [in] srcObject    Source object
	  //--------------------------------------------------------------
	  static void mergeObjects(boost::json::object &dstObject, const boost::json::object&srcObject);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a std::string
	  /// \param [in] v    A pointer on the value to convert
	  /// \return The data as std::string
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
	  static std::string convertToString(const boost::json::value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a bool
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as bool (check for "true"/"false", "0"/"1", or ay numeric != 0)
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
	  static bool convertToBool(const boost::json::value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a Int
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as int 
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
	  static int convertToInt(const boost::json::value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a Int64
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as int64 
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
	  static int64_t convertToInt64(const boost::json::value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a char
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as char 
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
	  static char convertToByte(const boost::json::value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a short
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as short 
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
	  static short convertToShort(const boost::json::value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a unsigned int
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as unsigned int 
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
	  static unsigned int convertToUInt(const boost::json::value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a unsigned int64
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as unsigned int64 
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
	  static uint64_t convertToUInt64(const boost::json::value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a unsigned char
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as unsigned char 
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
      static unsigned char convertToUByte(const boost::json::value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a unsigned short
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as unsigned short 
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
	  static unsigned short convertToUShort(const boost::json::value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a float
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as float 
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
      static float convertToFloat(const boost::json::value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a double
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as double 
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
	  static double convertToDouble(const boost::json::value& v);

      //--------------------------------------------------------------
      //
      //
      //
      // Internal get/set methods
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief	    Get parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      T getInternal(const std::string& parameterName, char pathChar) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter value (T is IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter value (T is IDataContainable)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      T getInternalIDataContainable(const std::string& parameterName, char pathChar) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter values
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      std::vector<T> getValuesInternal(const std::string& parameterName, char pathChar) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (T is IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (T is IDataContainable)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      std::vector<T> getValuesInternalIDataContainable(const std::string& parameterName, char pathChar) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (boost::shared_ptr<T>)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (boost::shared_ptr<T>)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      std::vector< boost::shared_ptr<T> > getValuesSPInternal(const std::string& parameterName, char pathChar) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (boost::shared_ptr<T> and T is IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (boost::shared_ptr<T> and T is IDataContainable)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      std::vector< boost::shared_ptr<T> > getValuesSPIDataContainableInternal(const std::string& parameterName, char pathChar) const;

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
      template<class T>
      void setInternal(const std::string& parameterName, const T & value, char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Set parameter value (T is IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter (T is IDataContainable)
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
      template<class T>
      void setInternalIDataContainable(const std::string& parameterName, const T & value, char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
      template<class T>
      void appendArrayInternal(const std::string& parameterName, const T& value, char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Set parameter value (T is IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter (T is IDataContainable)
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
      template<class T>
      void appendArrayInternalIDataContainable(const std::string& parameterName, const T& value, char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Set parameter values
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] values           Values of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
      template<class T>
      void setValuesInternal(const std::string& parameterName, const std::vector<T> & values, char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [out] values          The resulting vector
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      void setValuesInternalIDataContainable(const std::string& parameterName, const std::vector<T> & values, char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (Enum)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (Enum)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      std::vector<T> getValuesInternalEnum(const std::string& parameterName, char pathChar) const;


      //--------------------------------------------------------------
      /// \brief	    Set parameter values (Enum)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] values           Values of the parameter (Enum)
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
      template<class T>
      void setValuesInternalEnum(const std::string& parameterName, const std::vector<T> & values, char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (std::vector< boost::shared_ptr<T> >)
      /// \param [in] parameterName    Name of the parameter
      /// \param [out] values          The resulting vector
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<typename T>
      inline void setValuesSPInternal(const std::string& parameterName, const std::vector< boost::shared_ptr<T> > & values, char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (std::vector< boost::shared_ptr<IDataContainable> >)
      /// \param [in] parameterName    Name of the parameter
      /// \param [out] values          The resulting vector
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      inline void setValuesSPIDataContainableInternal(const std::string& parameterName, const std::vector< boost::shared_ptr<IDataContainable> > & values, char pathChar);


	  //--------------------------------------------------------------
	  /// \brief	Convert a boost::json::value into a typed value (T)
	  /// \param [in] ptrValue    A pointer on the value to convert
	  /// \return The data as T 
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
      template <class T>
      T convert(const boost::json::value* ptrValue) const;

      //--------------------------------------------------------------
      //
      //
      //
      // Internal helpers (allow redirecting to the good get/set internal method)
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with single value type (int, double, std::string,...)
      //--------------------------------------------------------------
      template <class T, class Enable = void>
      struct helper
      {

         //--------------------------------------------------------------
         /// \brief	    GET Method for all standard types (int, double, std::string,...)
         //--------------------------------------------------------------
         static T getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return tree->getInternal<T>(parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for all standard types (int, double, std::string,...)
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const T & value, const char pathChar)
         {
            tree->setInternal<T>(parameterName, value, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    APPEND Array Method for all standard types (int, double, std::string,...)
         //--------------------------------------------------------------
         static void appendArrayInternal(CDataContainer * tree, const std::string& parameterName, const T & value, const char pathChar)
         {
            tree->appendArrayInternal<T>(parameterName, value, pathChar);
         }

      };



      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with boost::shared_ptr
      //--------------------------------------------------------------
      template <typename T>
      struct helper < boost::shared_ptr< T > >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for boost::shared_ptr<T>
         //--------------------------------------------------------------
         static boost::shared_ptr< T > getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return boost::make_shared<T>(helper<T>::getInternal(tree, parameterName, pathChar));
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for boost::shared_ptr<T>
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const boost::shared_ptr< T > & value, const char pathChar)
         {
            helper<T>::setInternal(tree, parameterName, *value.get(), pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    appendArray Method for boost::shared_ptr<T>
         //--------------------------------------------------------------
         static void appendArrayInternal(CDataContainer * tree, const std::string& parameterName, const boost::shared_ptr< T > & value, const char pathChar)
         {
            helper<T>::appendArrayInternal(tree, parameterName, *value.get(), pathChar);
         }
      };

      
      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with boost::optional
      //--------------------------------------------------------------
      template <typename T>
      struct helper < boost::optional< T > >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for boost::shared_ptr<T>
         //--------------------------------------------------------------
         static boost::optional< T > getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            if(tree->containsValue(parameterName, pathChar))
               return boost::optional<T>(helper<T>::getInternal(tree, parameterName, pathChar));
            return boost::optional<T>();
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for boost::shared_ptr<T>
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const boost::optional< T > & value, const char pathChar)
         {
            if (value)
               helper<T>::setInternal(tree, parameterName, *value, pathChar);
            else
               tree->setNull(parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    appendArray Method for boost::shared_ptr<T>
         //--------------------------------------------------------------
         static void appendArrayInternal(CDataContainer * tree, const std::string& parameterName, const boost::optional< T > & value, const char pathChar)
         {
            if (value)
               helper<T>::appendArrayInternal(tree, parameterName, *value, pathChar);
         }
      };

      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with CField<T>
      //--------------------------------------------------------------
      template <typename T>
      struct helper < CField< T > >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for CField<T>
         //--------------------------------------------------------------
         static CField< T > getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return CField< T >(helper<T>::getInternal(tree, parameterName, pathChar));
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for CField<T>
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const CField< T > & value, const char pathChar)
         {
            helper<T>::setInternal(tree, parameterName, value(), pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    appendArray Method for CField<T>
         //--------------------------------------------------------------
         static void appendArrayInternal(CDataContainer * tree, const std::string& parameterName, const CField< T > & value, const char pathChar)
         {
            helper<T>::appendArrayInternal(tree, parameterName, value(), pathChar);
         }
      };



      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with IDataContainable object
      //--------------------------------------------------------------
      template <typename T>
      struct helper < T, typename boost::enable_if< boost::is_base_of< IDataContainable, T > >::type >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for IDataContainable object
         //--------------------------------------------------------------
         static T getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return tree->getInternalIDataContainable<T>(parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for IDataContainable object
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const T & value, const char pathChar)
         {
            tree->setInternalIDataContainable<T>(parameterName, value, pathChar);
         }
         
         //--------------------------------------------------------------
         /// \brief	    appendArray Method for IDataContainable object
         //--------------------------------------------------------------
         static void appendArrayInternal(CDataContainer * tree, const std::string& parameterName, const T & value, const char pathChar)
         {
            tree->appendArrayInternalIDataContainable<T>(parameterName, value, pathChar);
         }

      };


      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with enum
      //--------------------------------------------------------------
      template <typename T>
      struct helper < T, typename boost::enable_if< boost::is_enum< T > >::type >
      {

         //--------------------------------------------------------------
         /// \brief	    GET Method for enumeration
         //--------------------------------------------------------------
         static T getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return static_cast<T>(tree->getInternal<int>(parameterName, pathChar));
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for enumeration
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const T & value, const char pathChar)
         {
            tree->setInternal<int>(parameterName, static_cast<int>(value), pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    appendArray Method for enumeration
         //--------------------------------------------------------------
         static void appendArrayInternal(CDataContainer * tree, const std::string& parameterName, const T & value, const char pathChar)
         {
            tree->appendArrayInternal<int>(parameterName, static_cast<int>(value), pathChar);
         }

      };



      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with an IExtendedEnum object
      //--------------------------------------------------------------
      template <typename T>
      struct helper < T, typename boost::enable_if< boost::is_base_of< shared::enumeration::IExtendedEnum, T > >::type >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for IExtendedEnum object
         //--------------------------------------------------------------
         static T getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return T(tree->getInternal<std::string>(parameterName, pathChar));
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for IExtendedEnum object
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const T & value, const char pathChar)
         {
            tree->setInternal<std::string>(parameterName, static_cast<std::string>(value), pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    appendArray Method for IExtendedEnum object
         //--------------------------------------------------------------
         static void appendArrayInternal(CDataContainer* tree, const std::string& parameterName, const T& value, const char pathChar)
         {
            tree->appendArrayInternal<std::string>(parameterName, static_cast<std::string>(value), pathChar);
         }
      };







      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with vector of value type (vector<int>, vector<double>, vector<class>,...)
      //--------------------------------------------------------------
      template <typename T, class Enable = void >
      // ReSharper disable once IdentifierTypo
      struct vectorhelper
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for std::vector< T >
         //--------------------------------------------------------------
         static std::vector< T > getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return tree->getValuesInternal<T>(parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for std::vector< T >
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const std::vector< T > & value, const char pathChar)
         {
            tree->setValuesInternal(parameterName, value, pathChar);
         }
      };

      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with vector of IDataContainable objects
      //--------------------------------------------------------------
      template <typename T>
      // ReSharper disable once IdentifierTypo
      struct vectorhelper < T, typename boost::enable_if< boost::is_base_of< IDataContainable, T > >::type >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for std::vector< IDataContainable >
         //--------------------------------------------------------------
         static std::vector< T > getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return tree->getValuesInternalIDataContainable<T>(parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for std::vector< IDataContainable >
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const std::vector< T > & value, const char pathChar)
         {
            tree->setValuesInternalIDataContainable(parameterName, value, pathChar);
         }
      };

      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with vector of Enum
      //--------------------------------------------------------------
      template <typename T>
      // ReSharper disable once IdentifierTypo
      struct vectorhelper < T, typename boost::enable_if< boost::is_enum< T > >::type >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for std::vector< Enum >
         //--------------------------------------------------------------
         static std::vector< T > getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return tree->getValuesInternalEnum<T>(parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for std::vector< Enum >
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const std::vector< T > & value, const char pathChar)
         {
            tree->setValuesInternalEnum(parameterName, value, pathChar);
         }
      };


      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with vector of value type (vector<int>, vector<double>, vector<class>,...)
      //--------------------------------------------------------------
      template <typename T>
      struct helper < std::vector< T > >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for std::vector< T >
         //--------------------------------------------------------------
         static std::vector<T> getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return vectorhelper<T>::getInternal(tree, parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for std::vector< T >
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const std::vector<T> & value, const char pathChar)
         {
            vectorhelper<T>::setInternal(tree, parameterName, value, pathChar);
         }
      };






      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with vector of shared_ptr of simple type ( std::vector< boost::shared_ptr<int> >,... )
      //--------------------------------------------------------------
      template <typename T, class Enable = void >
      // ReSharper disable once IdentifierTypo
      struct vhelper
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for std::vector< boost::shared_ptr<T> >
         //--------------------------------------------------------------
         static std::vector< boost::shared_ptr<T> > getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return tree->getValuesSPInternal<T>(parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for std::vector< boost::shared_ptr<T> >
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const std::vector<  boost::shared_ptr<T> > & value, const char pathChar)
         {
            tree->setValuesSPInternal(parameterName, value, pathChar);
         }
      };

      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with vector of shared_ptr of IDataContainable ( std::vector< boost::shared_ptr<IDataContainable> >)
      //--------------------------------------------------------------
      template <typename T>
      // ReSharper disable once IdentifierTypo
      struct vhelper < T, typename boost::enable_if< boost::is_base_of< IDataContainable, T > >::type >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for std::vector< boost::shared_ptr<IDataContainable> >
         //--------------------------------------------------------------
         static std::vector< boost::shared_ptr<T> > getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return tree->getValuesSPIDataContainableInternal<T>(parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for std::vector< boost::shared_ptr<IDataContainable> >
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const std::vector<  boost::shared_ptr<T> > & value, const char pathChar)
         {
            std::vector<  boost::shared_ptr<IDataContainable> > compatibleVector;
            for (typename std::vector<boost::shared_ptr<T>>::const_iterator i = value.begin(); i != value.end(); ++i)
               compatibleVector.push_back(boost::dynamic_pointer_cast<IDataContainable>(*i));
            tree->setValuesSPIDataContainableInternal(parameterName, compatibleVector, pathChar);
         }
      };

      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with vector of value type ( std::vector< boost::shared_ptr<T> > )
      //--------------------------------------------------------------
      template <typename T>
      struct helper < std::vector< boost::shared_ptr<T> > >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for std::vector< boost::shared_ptr<T> >
         //--------------------------------------------------------------
         static std::vector< boost::shared_ptr<T> > getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return vhelper<T>::getInternal(tree, parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for std::vector< boost::shared_ptr<T> >
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const std::vector< boost::shared_ptr<T> > & value, const char pathChar)
         {
            vhelper<T>::setInternal(tree, parameterName, value, pathChar);
         }

      };


   private:
      //--------------------------------------------------------------
      /// \brief	   Generate the right property path
      /// \param [in]	   parameterName     The full parameter name
      /// \param [in]	   pathChar          The character which is interpreted as path separator
      /// \return the path
      //--------------------------------------------------------------
      static std::vector<std::string> generatePath(const std::string & parameterName, char pathChar);

   private:
      //--------------------------------------------------------------
      /// \brief	   The configuration content
      //--------------------------------------------------------------
      boost::json::object m_tree;

      

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the configuration content
      //--------------------------------------------------------------
      mutable boost::mutex m_treeMutex;
   };

   

   //--------------------------------------------------------------
   /// \brief	    Helper structure for get/set with boost::shared_ptr
   //--------------------------------------------------------------
   template <>
   struct CDataContainer::helper < boost::shared_ptr<CDataContainer> >
   {
      //--------------------------------------------------------------
      /// \brief	    GET Method for boost::shared_ptr<T>
      //--------------------------------------------------------------
      static boost::shared_ptr<CDataContainer> getInternal(const CDataContainer* tree, const std::string& parameterName, const char pathChar)
      {
         return tree->getChild(parameterName, pathChar);
      }

      //--------------------------------------------------------------
      /// \brief	    SET Method for boost::shared_ptr<T>
      //--------------------------------------------------------------
      static void setInternal(CDataContainer* tree, const std::string& parameterName, const boost::shared_ptr<CDataContainer>& value, const char pathChar)
      {
         auto& a = *value;
         tree->set(parameterName, a, pathChar);
      }

      //--------------------------------------------------------------
      /// \brief	    appendArray Method for boost::shared_ptr<T>
      //--------------------------------------------------------------
      static void appendArrayInternal(CDataContainer* tree, const std::string& parameterName, const boost::shared_ptr<CDataContainer>& value, const char pathChar)
      {
         auto& a = *value;
         tree->appendArray(parameterName, a, pathChar);
      }
   };


   //--------------------------------------------------------------
   /// \brief	    Helper structure for get/set with vector of value type ( std::vector< boost::shared_ptr<T> > )
   //--------------------------------------------------------------
   template <>
   struct CDataContainer::helper < std::vector< boost::shared_ptr<CDataContainer> > >
   {
      //--------------------------------------------------------------
      /// \brief	    GET Method for std::vector< boost::shared_ptr<T> >
      //--------------------------------------------------------------
      static std::vector< boost::shared_ptr<CDataContainer> > getInternal(const CDataContainer* tree, const std::string& parameterName, const char pathChar)
      {
         return vhelper<CDataContainer>::getInternal(tree, parameterName, pathChar);
      }

      //--------------------------------------------------------------
      /// \brief	    SET Method for std::vector< boost::shared_ptr<T> >
      //--------------------------------------------------------------
      static void setInternal(CDataContainer* tree, const std::string& parameterName, const std::vector< boost::shared_ptr<CDataContainer> >& value, const char pathChar)
      {
         vhelper<CDataContainer>::setInternal(tree, parameterName, value, pathChar);
      }

   };

   //--------------------------------------------------------------
   /// \brief	    The below part of header contains all the function
   ///				definitions (out of class definition for better 
   ///				cross compiler compatibilities).
   ///				-> for template methods
   ///				-> for template specialization
   //--------------------------------------------------------------
   template<class T>
   T CDataContainer::get(const std::string& parameterName, const char pathChar) const
   {
      return helper<T>::getInternal(this, parameterName, pathChar);
   }

   template<>
   inline std::map<std::string, std::string> CDataContainer::get(const std::string& parameterName, const char pathChar) const
   {
      return getAsMap<std::string>(parameterName, pathChar);
   }

   template<>
   inline unsigned long CDataContainer::get(const std::string& parameterName, const char pathChar) const
   {
      //manage "unsigned long" special cases
      //depending on the platform, it maybe 8 or 4 bytes
      //just ensure it's rooted using the real good type.
      //by default this is not the case
      //that's why template specialization is needed
      if (sizeof(unsigned long) == 8)
         return static_cast<unsigned long>(helper<uint64_t>::getInternal(this, parameterName, pathChar));
      return static_cast<unsigned>(helper<unsigned int>::getInternal(this, parameterName, pathChar));
   }

   template<>
   inline long CDataContainer::get(const std::string& parameterName, const char pathChar) const
   {
      //manage "long" special cases
      //depending on the platform, it maybe 8 or 4 bytes
      //just ensure it's rooted using the real good type.
      //by default this is not the case
      //that's why template specialization is needed
      if (sizeof(long) == 8)
         // ReSharper disable once CppUnreachableCode
         return static_cast<long>(helper<int64_t>::getInternal(this, parameterName, pathChar));
      return static_cast<long>(helper<int>::getInternal(this, parameterName, pathChar));
   }

   template<class T>
   T CDataContainer::getWithDefault(const std::string& parameterName, const T & defaultValue, const char pathChar) const
   {
      if (!exists(parameterName, pathChar))
         return defaultValue;
      return get<T>(parameterName, pathChar);
   }


   template<class T>
   void CDataContainer::appendArray(const std::string& parameterName, const T& value, const char pathChar)
   {
      helper<T>::appendArrayInternal(this, parameterName, value, pathChar);
   }

   template<class T>
   void CDataContainer::appendArray(const char* parameterName, const T& value, const char pathChar)
   {
      const std::string strParamName(parameterName);
      appendArray<T>(strParamName, value, pathChar);
   }

   template<>
   inline void CDataContainer::appendArray(const std::string& parameterName, const char& value, const char pathChar)
   {
      std::string s;
      s += value;
      appendArray<std::string>(parameterName, s, pathChar);
   }



   template<class T>
   void CDataContainer::set(const char* parameterName, const T & value, const char pathChar)
   {
      const std::string strParamName(parameterName);
      set<T>(strParamName, value, pathChar);
   }
   

   template<>
   inline void CDataContainer::set(const std::string& parameterName, const unsigned long& value, const char pathChar)
   {
      if(sizeof(unsigned long) == 8)
         // ReSharper disable once CppUnreachableCode
         helper<uint64_t>::setInternal(this, parameterName, value, pathChar);
      else
         helper<unsigned int>::setInternal(this, parameterName, value, pathChar);
   }


   template<>
   inline void CDataContainer::set(const std::string& parameterName, const long& value, const char pathChar)
   {
      if (sizeof(long) == 8)
         // ReSharper disable once CppUnreachableCode
         helper<int64_t>::setInternal(this, parameterName, value, pathChar);
      else
         helper<int>::setInternal(this, parameterName, value, pathChar);
   }



   template<typename T>
   void CDataContainer::set(const std::string& parameterName, const T & value, const char pathChar)
   {
      helper<T>::setInternal(this, parameterName, value, pathChar);
   }

   template<>
   inline void CDataContainer::set(const std::string& parameterName, const char & value, const char pathChar)
   {
      std::string s;
      s+=value;
      set<std::string>(parameterName, s, pathChar);
   }

   template<class T>
   T CDataContainer::getInternal(const std::string& parameterName, const char pathChar) const
   {
      const boost::json::value* found = findValue(parameterName, pathChar);
      if (found)
      {
         T a = convert<T>(found);
         return a;
      }
      throw exception::CInvalidParameter(parameterName + " : is not found");
   }
   
   template<>
   inline CDataContainer CDataContainer::getInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      const boost::json::value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->is_object())
            return CDataContainer(found->as_object());
         throw exception::CInvalidParameter(parameterName + " : is not found");
      }
      throw exception::CInvalidParameter(parameterName + " : is not found");
   }

   


   template<>
   inline boost::posix_time::ptime CDataContainer::getInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      const boost::json::value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->is_string())
            return boost::posix_time::from_iso_string(found->get_string().data());
         else
            throw exception::COutOfRange(parameterName + " can not be converted to string");
      }
      else
      {
         throw exception::CInvalidParameter(parameterName + " : is not found");
      }
   }

   template<class T>
   T CDataContainer::getInternalIDataContainable(const std::string& parameterName, const char pathChar) const
   {
      CDataContainer t2 = getInternal<CDataContainer>(parameterName, pathChar);
      T obj;
      obj.fillFromContent(t2);
      return obj;
   }






   template<class T>
   std::vector<T> CDataContainer::getValuesInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector<T> result;
      const boost::json::value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->is_array())
         {
            auto ar = found->as_array();
			   std::transform(ar.begin(), ar.end(), std::back_inserter(result),
				 [this](auto &v) -> T { return this->convert<T>(&v); });
         }
         else
            throw exception::COutOfRange(parameterName + " is not an array");
      }
      else
      {
         throw exception::CInvalidParameter(parameterName + " : is not found");
      }
      return result;

   }

   template<>
   inline std::vector< boost::shared_ptr<CDataContainer> > CDataContainer::getValuesInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      std::vector< boost::shared_ptr<CDataContainer> > result;
      const boost::json::value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->is_array())
         {
            auto ar = found->as_array();
            std::transform(ar.begin(), ar.end(), std::back_inserter(result),
               [](auto& v) -> boost::shared_ptr<CDataContainer> { return boost::make_shared<CDataContainer>(v.as_object()); });
         }
         else
            throw exception::COutOfRange(parameterName + " is not an array");
      }
      else
      {
         throw exception::CInvalidParameter(parameterName + " : is not found");
      }
      return result;
   }

   template<class T>
   std::vector<T> CDataContainer::getValuesInternalEnum(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector<T> result;
      const boost::json::value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->is_array())
         {
            auto ar = found->as_array();
            std::transform(ar.begin(), ar.end(), std::back_inserter(result),
				 [](auto &v) -> T { return static_cast<T>(v.to_number<int>()); });
         }
         else
            throw exception::COutOfRange(parameterName + " is not an array");
      }
      else
      {
         throw exception::CInvalidParameter(parameterName + " : is not found");
      }
      return result;
   }

   template<class T>
   inline std::vector< boost::shared_ptr<T> > CDataContainer::getValuesSPInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector< boost::shared_ptr<T> > result;
      const boost::json::value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->is_array())
         {
            auto ar = found->as_array();
            for (auto& v : ar)
            {
               boost::shared_ptr<T> sp = boost::make_shared<T>(convert<T>(&v));
               result.push_back(sp);
            }
         }
         else
            throw exception::COutOfRange(parameterName + " is not an array");
      }
      else
      {
         throw exception::CInvalidParameter(parameterName + " : is not found");
      }
      return result;
   }

   template<>
   inline std::vector< boost::shared_ptr<CDataContainer> > CDataContainer::getValuesSPInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector<boost::shared_ptr<CDataContainer> > result;
      const boost::json::value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->is_array())
         {
            auto ar = found->as_array();
            for (auto& v : ar)
            {
               boost::shared_ptr<CDataContainer> sp = make(v.as_object());
               result.push_back(sp);
            }
         }
         else
            throw exception::COutOfRange(parameterName + " is not an array");
      }
      else
      {
         throw exception::CInvalidParameter(parameterName + " : is not found");
      }
      return result;
   }

   template<class T>
   std::vector< boost::shared_ptr<T> > CDataContainer::getValuesSPIDataContainableInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector< boost::shared_ptr<T> > result;
      const boost::json::value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->is_array())
         {
            auto ar = found->as_array();
            for (auto& v : ar)
            {
               boost::shared_ptr<T> sp(new T);
               sp->fillFromContent(CDataContainer(v.as_object()));
               result.push_back(sp);
            }
         }
         else
            throw exception::COutOfRange(parameterName + " is not an array");
      }
      else
      {
         throw exception::CInvalidParameter(parameterName + " : is not found");
      }
      return result;
   }





   template<class T>
   std::vector<T> CDataContainer::getValuesInternalIDataContainable(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector<T> result;
      const boost::json::value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->is_array())
         {
            auto ar = found->as_array();
            for (auto& v : ar)
            {
               T a;
               CDataContainer dc(v.as_object());
               a.fillFromContent(dc);
               result.push_back(a);
            }
         }
         else
            throw exception::COutOfRange(parameterName + " is not an array");
      }
      else
      {
         throw exception::CInvalidParameter(parameterName + " : is not found");
      }
      return result;
   }










   template<class T>
   void CDataContainer::setInternal(const std::string& parameterName, const T & value, const char pathChar)
   {
      boost::json::value* ptr = findValueOrCreate(parameterName, pathChar);
      *ptr = value;
   }

   template<>
   inline void CDataContainer::setInternal(const std::string& parameterName, const std::string & value, const char pathChar)
   {
      boost::json::value* ptr = findValueOrCreate(parameterName, pathChar);
      *ptr = value;
   }


   template<class T>
   void CDataContainer::setInternalIDataContainable(const std::string& parameterName, const T & value, const char pathChar)
   {
      CDataContainer subTree;
      value.extractContent(subTree);
      setInternal(parameterName, subTree, pathChar);
   }



   template<>
   inline void CDataContainer::setInternal(const std::string& parameterName, const CDataContainer & value, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      boost::json::value* ptr = findValueOrCreate(parameterName, pathChar);
      *ptr = value.m_tree;
   }


   template<>
   inline void CDataContainer::setInternal(const std::string& parameterName, const boost::posix_time::ptime & value, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      boost::json::value* ptr = findValueOrCreate(parameterName, pathChar);
      *ptr = boost::posix_time::to_iso_string(value).c_str();;
   }





   template<class T>
   void CDataContainer::setValuesInternal(const std::string& parameterName, const std::vector<T> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      boost::json::array arr;
      for (typename std::vector<T>::const_iterator i = values.begin(); i != values.end(); ++i)
      {
         arr.emplace_back(*i);
      }

      boost::json::value* ptr = findValueOrCreate(parameterName, pathChar);
      *ptr = arr;
   }
   

   template<class T>
   void CDataContainer::setValuesInternalEnum(const std::string& parameterName, const std::vector<T> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      boost::json::array arr;
      for (typename std::vector<T>::const_iterator i = values.begin(); i != values.end(); ++i)
      {
         arr.emplace_back(static_cast<int>(*i));
      }

      boost::json::value* ptr = findValueOrCreate(parameterName, pathChar);
      *ptr = arr;
   }

   template<class T>
   void CDataContainer::setValuesInternalIDataContainable(const std::string& parameterName, const std::vector<T> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      boost::json::array arr;
      for (typename std::vector<T>::const_iterator i = values.begin(); i != values.end(); ++i)
      {
         CDataContainer t;
         i->extractContent(t);
         arr.emplace_back(t.m_tree);
      }

      boost::json::value* ptr = findValueOrCreate(parameterName, pathChar);
      *ptr = arr;


   }



   template<typename T>
   inline void CDataContainer::setValuesSPInternal(const std::string& parameterName, const std::vector< boost::shared_ptr<T> > & values, const char pathChar)
   {
      if (!isArray(parameterName, pathChar))
         createArray(parameterName, pathChar);

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         appendArray(parameterName, *i, pathChar);
      }
   }

   template<class T>
   void CDataContainer::appendArrayInternal(const std::string& parameterName, const T& value, const char pathChar)
   {
      boost::json::value* ptr = findValue(parameterName, pathChar);
      if(ptr && ptr->is_array())
      {
         ptr->as_array().emplace_back(value);
      }
   }
   

   template<>
   inline void CDataContainer::appendArrayInternal(const std::string& parameterName, const CDataContainer& value, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      boost::json::value* ptr = findValue(parameterName, pathChar);
      if (ptr && ptr->is_array())
      {
         ptr->as_array().emplace_back(value.m_tree);
      }

   }


   template<>
   inline void CDataContainer::appendArrayInternal(const std::string& parameterName, const boost::posix_time::ptime& value, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      boost::json::value* ptr = findValue(parameterName, pathChar);
      if (ptr && ptr->is_array())
      {
         ptr->as_array().emplace_back(boost::posix_time::to_iso_string(value));
      }
 
   }

   template<class T>
   void CDataContainer::appendArrayInternalIDataContainable(const std::string& parameterName, const T& value, const char pathChar)
   {
      CDataContainer subTree;
      value.extractContent(subTree);
      appendArrayInternal(parameterName, subTree, pathChar);
   }


   template<class T>
   T CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if (sizeof(T) == 8)
      {
         if(std::is_signed<T>::value)
            return convertToInt64(*ptrValue);
         return convertToUInt64(*ptrValue);
      }
      if (sizeof(T) == 4)
      {
         if (std::is_signed<T>::value)
            return convertToInt(*ptrValue);
         return convertToUInt(*ptrValue);
      }
      if (sizeof(T) == 2)
      {
         if (std::is_signed<T>::value)
            return convertToShort(*ptrValue);
         return convertToUShort(*ptrValue);
      }
      if (sizeof(T) == 1)
      {
         if (std::is_signed<T>::value)
            return convertToByte(*ptrValue);
         return convertToUByte(*ptrValue);
      }

      //pitfall (may succeed)
      if(ptrValue)
         return ptrValue->Get<T>();
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline bool CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if(ptrValue)
         return convertToBool(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline std::string CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if(ptrValue)
         return convertToString(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }
   
   template<>
   inline int CDataContainer::convert(const boost::json::value* ptrValue) const
   {
	   if(ptrValue)
         return convertToInt(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }
   
   template<>
   inline int64_t CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if(ptrValue)
         return convertToInt64(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   } 
   
   template<>
   inline char CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if(ptrValue)
         return convertToByte(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline short CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if(ptrValue)
         return convertToShort(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }   

   template<>
   inline unsigned int CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if(ptrValue)
         return convertToUInt(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }
   
   template<>
   inline uint64_t CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if(ptrValue)
         return convertToUInt64(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   } 
   
   template<>
   inline unsigned char CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if(ptrValue)
         return convertToUByte(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline unsigned short CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if(ptrValue)
         return convertToUShort(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline float CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if(ptrValue)
         return convertToFloat(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline double CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if(ptrValue)
         return convertToDouble(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }


   template<>
   inline CDataContainer CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if (ptrValue)
         return CDataContainer(ptrValue->as_object());
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }



   template<>
   inline boost::shared_ptr<CDataContainer> CDataContainer::convert(const boost::json::value* ptrValue) const
   {
      if (ptrValue)
         return CDataContainer::make(ptrValue->as_object());
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   inline void CDataContainer::setValuesSPIDataContainableInternal(const std::string& parameterName, const std::vector< boost::shared_ptr<IDataContainable> >& values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      boost::json::array arr;
      for (auto i = values.begin(); i != values.end(); ++i)
      {
         CDataContainer t;
         (*i)->extractContent(t);
         arr.emplace_back(t.m_tree);
      }

      boost::json::value* ptr = findValueOrCreate(parameterName, pathChar);
      if (ptr)
         *ptr = arr;

   }


   template<>
   inline void CDataContainer::setValuesInternal(const std::string& parameterName, const std::vector<boost::shared_ptr<CDataContainer>> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      boost::json::array arr;
      for (auto i = values.begin(); i != values.end(); ++i)
      {
         CDataContainer t;
         (*i)->extractContent(t);
         arr.emplace_back((*i)->m_tree);
      }

      boost::json::value* ptr = findValueOrCreate(parameterName, pathChar);
      if (ptr)
         *ptr = arr;

   }



   template<typename EnumType>
   EnumType CDataContainer::getEnumValue(const std::string& parameterName, const EnumValuesNames& valuesNames, const char pathChar) const
   {
      const auto stringValue = get<std::string>(parameterName, pathChar);
      auto it = valuesNames.find(stringValue);
      if (it != valuesNames.end())
         return static_cast<EnumType>(it->second);

      throw exception::COutOfRange(std::string("Value ") + stringValue + " was not found for " + parameterName + " parameter");
   }

   template<typename Type>
   std::map<std::string, Type> CDataContainer::getAsMap(const std::string& parameterName, const char pathChar) const
   {
      std::map<std::string, Type> result;

      const auto found = findValue(parameterName, pathChar);
      if (found)
      {
         const auto path = !parameterName.empty() ? (parameterName + pathChar) : "";

         for (auto i = found->as_object().begin(); i != found->as_object().end(); ++i)
         {
            result[i->key().to_string()] = convert<Type>(&(i->value()));
         }
      }

      return result;
   }

} // namespace shared
