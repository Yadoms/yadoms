#pragma once
#define RAPIDJSON_VALUE_DEFAULT_OBJECT_CAPACITY 1
#include <shared/exception/OutOfRange.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include "serialization/IDataSerializable.h"
#include "serialization/IDataFileSerializable.h"
#include "IDataContainable.h"
#include "enumeration/IExtendedEnum.h"
#include "Field.hpp"

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace shared
{
   //--------------------------------------------------------------
   /// \brief	This class should be used as a data container
   ///			The aim is to handle any data; like configuration
   ///			
   ///			The container should contains data by a get/set method
   ///			Each data is associated to a key name
   ///			The data could be a simple type, a list of data, or a DataContainer child
   ///
   ///			The container has the serialize feature
   ///			This is done be serializing/deserializing a string,
   ///			or a file
   ///			
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
   //--------------------------------------------------------------

   typedef boost::shared_ptr<CDataContainer> CDataContainerSharedPtr;
   typedef boost::shared_ptr<const CDataContainer> CDataContainerSharedPtrConst;

   #define new_CDataContainerSharedPtr() boost::make_shared<shared::CDataContainer>()
   #define new_CDataContainerSharedPtrOptimized(estimatedDataSize,estimatedItemCount) boost::make_shared<shared::CDataContainer>(estimatedDataSize,estimatedItemCount)
   #define new_CDataContainerSharedPtrP(P) boost::make_shared<shared::CDataContainer>(P)

   class CDataContainer : public serialization::IDataSerializable, public serialization::IDataFileSerializable /*, public IDataContainable*/
   {
   public:
      //--------------------------------------------------------------
      //
      //
      //
      // Constructors & destructor
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief		Constructor. Empty data
      //--------------------------------------------------------------
      CDataContainer();
      
      //--------------------------------------------------------------
      /// \brief		Constructor. Empty data
      ///            With preallocated buffer (!! important if use of
      ///            huge number of items)
      //--------------------------------------------------------------
      CDataContainer(unsigned int estimatedDataSize, unsigned int estimatedItemCount);

      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      explicit CDataContainer(const std::string & initialData);

      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial map data
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
	  /// \param [in] d    Initial data for this container (rapidjson::Value&)
	  //--------------------------------------------------------------
      explicit CDataContainer(rapidjson::Value & d);

	  //--------------------------------------------------------------
	  /// \brief		Constructor
	  /// \param [in] d    Initial data for this container (rapidjson::Value*)
	  //--------------------------------------------------------------
	  explicit CDataContainer(rapidjson::Value * d);
      
	  //--------------------------------------------------------------
	  /// \brief		Constructor
	  /// \param [in] d    Initial data for this container (rapidjson::Document&)
	  //--------------------------------------------------------------
	  explicit CDataContainer(rapidjson::Document & d);

   public:
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
      /// \param [in] pathChar         The path spearator to use (default is '.')
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
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     The parameter value, or the default value if parameter not found
      //--------------------------------------------------------------
      template<class T>
      T getWithDefault(const std::string& parameterName, const T & defaultValue, char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Get child value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     The child as CDataContainerSharedPtr
      //--------------------------------------------------------------
      CDataContainerSharedPtr getChild(const std::string& parameterName, char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      //--------------------------------------------------------------
      template<class T>
      void set(const std::string& parameterName, const T & value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      //--------------------------------------------------------------
      template<class T>
      void set(const char* parameterName, const T & value, char pathChar = '.');


      //--------------------------------------------------------------
      /// \brief	    Append value to an already existing array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      //--------------------------------------------------------------
      template<class T>
      void appendArray(const std::string& parameterName, const T& value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Append value to an already existing array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
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
      /// \param [in] pathChar         The path spearator to use (default is '.')
      //--------------------------------------------------------------
      void set(const std::string & parameterName, const char* value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      //--------------------------------------------------------------
      void set(const char* parameterName, const char* value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Append a value to an array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      //--------------------------------------------------------------
      void appendArray(const char* parameterName, const char* value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Append a value to an array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      //--------------------------------------------------------------
      void appendArray(const std::string& parameterName, const char* value, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Get parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      std::string get(const std::string & parameterName = std::string(), char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter value as map<string, string>
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<typename Type>
      std::map<std::string, Type> getAsMap(const std::string& parameterName = std::string(), char pathChar = '.') const;


      //--------------------------------------------------------------
      /// \brief	    Get parameter sub-keys as vector<string>
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      std::vector<std::string> getKeys(const std::string& parameterName = std::string(), char pathChar = '.') const;


      //--------------------------------------------------------------
      /// \brief	    Find a sub-parameter with criteria
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] where_fct         Criteria : lambda must returns true if item is found
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     The found parameter
      /// \throw      shared::exception::CEmptyResutl if no parameter matching criteria was found
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      CDataContainer find(const std::string& parameterName, boost::function<bool(const CDataContainer&)> where_fct, char pathChar = '.') const;


      //--------------------------------------------------------------
      /// \brief	    Make a full copy of current container
      /// \return     The newly created container
      //--------------------------------------------------------------
      CDataContainerSharedPtr copy() const;

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
      void initializeWith(const CDataContainerSharedPtr& rhs);

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
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     true if the parameter contains child (CDataContainer), false if it is a termination node with data
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      bool containsChildArray(const std::string& parameterName = std::string(), char pathChar = '.') const;
      
	  //--------------------------------------------------------------
      /// \brief	    Check if a node contains at least a child object (with a value : {...})
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     true if the parameter contains child (CDataContainer), false if it is a termination node with data
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      bool containsChild(const std::string& parameterName = std::string(), char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Check if a node is terminal value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     true if the path contains only single value, false if it is contains a subtree
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      bool containsValue(const std::string& parameterName = std::string(), char pathChar = '.') const;

	   //--------------------------------------------------------------
	   /// \brief	  Set a value as null
	   /// \param [in] parameterName    Name of the parameter
	   /// \param [in] pathChar         The path spearator to use (default is '.')
	   //--------------------------------------------------------------
      void setNull(const std::string& parameterName = std::string(), char pathChar = '.') const;

	   //--------------------------------------------------------------
	   /// \brief	  Check if a value is null
	   /// \param [in] parameterName    Name of the parameter
	   /// \param [in] pathChar         The path spearator to use (default is '.')
	   /// \return  true is the value is null
	   //--------------------------------------------------------------
	   bool isNull(const std::string& parameterName = std::string(), char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	  Create an empty array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      //--------------------------------------------------------------
      bool createArray(const std::string& parameterName, char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	  Check if targetted value is an array
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
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
      /// \param [in] dt  The input data container
      /// \return   The stream
      //--------------------------------------------------------------
      friend std::ostream& operator<<(std::ostream& os, const CDataContainer& dt);

      //--------------------------------------------------------------
      /// \brief		Input operator (read a serialized container)
      /// \param [in] is  The stream to read
      /// \param [in] dt  The output data container
      /// \return   The stream
      //--------------------------------------------------------------
      friend std::istream& operator>>(std::istream& is, CDataContainer& dt);

      //--------------------------------------------------------------
      /// \brief		Check if the container is empty
      /// \return    true if contanier is empty
      //--------------------------------------------------------------
      bool empty() const;

      //--------------------------------------------------------------
      /// \brief		Print the content to log
      /// \param [in] os      The out stream where to print out
      //--------------------------------------------------------------
      void printToLog(std::ostream& os) const;

      //--------------------------------------------------------------
      /// \brief		Print sizeto log
      /// \param [in] os      The out stream where to print out
      //--------------------------------------------------------------
      void printSizeToLog(std::ostream& os) const;

      //--------------------------------------------------------------
      /// \brief	    Merge this container from another one
      /// \param [in] from source container
      /// \desc       Add non-exisiting (or replace existing) values of "from" container into this container
      ///             Values of this container not present in "from" container will be kept
      //--------------------------------------------------------------
      void mergeFrom(const CDataContainer& from);

      //--------------------------------------------------------------
      /// \brief	    Merge this container from another one
      /// \param [in] from source container
      /// \desc       Add non-exisiting (or replace existing) values of "from" container into this container
      ///             Values of this container not present in "from" container will be kept
      //--------------------------------------------------------------
      void mergeFrom(CDataContainerSharedPtr & from);

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
      const static CDataContainerSharedPtr EmptyContainerSharedPtr;

   protected:
      //--------------------------------------------------------------
      /// \brief		Constructor
      /// \param [in] initialTree    Initial tree for this container
      //--------------------------------------------------------------
      explicit CDataContainer(const rapidjson::Document & initialTree);

	  //--------------------------------------------------------------
	  /// \brief		Find a value from the name
	  /// \param [in] parameterName    The parameter name
	  /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
	  //--------------------------------------------------------------
	  rapidjson::Value* findValue(const std::string& parameterName, char pathChar) const;

	  //--------------------------------------------------------------
	  /// \brief	Merge two objects
	  /// \param [in] dstObject    Destination value
	  /// \param [in] srcObject    Source value
	  /// \param [in] allocator    The allocator
	  //--------------------------------------------------------------
	  static void mergeObjects(rapidjson::Value &dstObject, const rapidjson::Value &srcObject, rapidjson::Document::AllocatorType &allocator);

	  //--------------------------------------------------------------
	  /// \brief	Get pointer on the top root object
	  /// \return   A pointer on the rapidjson::Document
	  //--------------------------------------------------------------
	  rapidjson::Document * getPointer() const;

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a std::string
	  /// \param [in] v    A pointer on the value to convert
	  /// \return The data as std::string
	  /// \throw exception::CInvalidParameter if value is null or in an incompatible type
	  //--------------------------------------------------------------
	  static std::string convertToString(const rapidjson::Value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a bool
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as bool (check for "true"/"false", "0"/"1", or ay numeric != 0)
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
	  static bool convertToBool(const rapidjson::Value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a Int
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as int 
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
	  static int convertToInt(const rapidjson::Value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a Int64
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as int64 
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
	  static int64_t convertToInt64(const rapidjson::Value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a char
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as char 
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
	  static char convertToByte(const rapidjson::Value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a short
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as short 
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
	  static short convertToShort(const rapidjson::Value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a unsigned int
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as unsigned int 
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
	  static unsigned int convertToUInt(const rapidjson::Value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a unsigned int64
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as unsigned int64 
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
	  static uint64_t convertToUInt64(const rapidjson::Value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a unsigned char
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as unsigned char 
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
      static unsigned char convertToUByte(const rapidjson::Value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a unsigned short
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as unsigned short 
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
	  static unsigned short convertToUShort(const rapidjson::Value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a float
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as float 
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
      static float convertToFloat(const rapidjson::Value& v);

	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a double
	  /// \param [in] v    A reference on the value to convert
	  /// \return The data as double 
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
	  static double convertToDouble(const rapidjson::Value& v);

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
      /// \param [in] values           Valuse of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
      template<class T>
      void setValuesInternal(const std::string& parameterName, const std::vector<T> & values, char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [out] values          The resulting vector
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (IDataContainable)
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
      /// \param [in] values           Valuse of the parameter (Enum)
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
      template<class T>
      void setValuesInternalEnum(const std::string& parameterName, const std::vector<T> & values, char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (std::vector< boost::shared_ptr<T> >)
      /// \param [in] parameterName    Name of the parameter
      /// \param [out] values          The resulting vector
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (std::vector< boost::shared_ptr<T> >)
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
      /// \return     The parameter values (std::vector< boost::shared_ptr<IDataContainable> >)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      inline void setValuesSPIDataContainableInternal(const std::string& parameterName, const std::vector< boost::shared_ptr<IDataContainable> > & values, char pathChar);


	  //--------------------------------------------------------------
	  /// \brief	Convert a rapidjson::Value into a typed value (T)
	  /// \param [in] ptrValue    A pointer on the value to convert
	  /// \return The data as T 
	  /// \throw exception::CInvalidParameter if value is null or in an incomptible type
	  //--------------------------------------------------------------
      template <class T>
      T convert(rapidjson::Value* ptrValue) const;

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
      template <typename T, class Enable = void>
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
      /// \brief	    Helper structure for get/set with boost::shared_ptr
      //--------------------------------------------------------------
      template <>
      struct helper < CDataContainerSharedPtr >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for boost::shared_ptr<T>
         //--------------------------------------------------------------
         static CDataContainerSharedPtr getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return tree->getChild(parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for boost::shared_ptr<T>
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const CDataContainerSharedPtr & value, const char pathChar)
         {
            auto & a = *value.get();
            tree->set(parameterName, a, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    appendArray Method for boost::shared_ptr<T>
         //--------------------------------------------------------------
         static void appendArrayInternal(CDataContainer * tree, const std::string& parameterName, const CDataContainerSharedPtr& value, const char pathChar)
         {
            auto& a = *value.get();
            tree->appendArray(parameterName, a, pathChar);
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

      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with vector of value type ( std::vector< boost::shared_ptr<T> > )
      //--------------------------------------------------------------
      template <>
      struct helper < std::vector< CDataContainerSharedPtr > >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for std::vector< boost::shared_ptr<T> >
         //--------------------------------------------------------------
         static std::vector< CDataContainerSharedPtr > getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return vhelper<CDataContainer>::getInternal(tree, parameterName, pathChar);
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for std::vector< boost::shared_ptr<T> >
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const std::vector< CDataContainerSharedPtr > & value, const char pathChar)
         {
            vhelper<CDataContainer>::setInternal(tree, parameterName, value, pathChar);
         }

      };

   private:
      //--------------------------------------------------------------
      /// \brief	   Generate the right property path
      /// \param [in]	   parameterName     The full parameter name
      /// \param [in]	   pathChar          The character which is interpreted as path separator
      /// \return the path
      //--------------------------------------------------------------
      static std::string generatePath(const std::string & parameterName, char pathChar);

   private:
      //--------------------------------------------------------------
      /// \brief	   The rapidjson tree allocator initial buffer
      //--------------------------------------------------------------
      void* m_tree_allocator_initial_buffer;

      //--------------------------------------------------------------
      /// \brief	   The rapidjson tree allocator
      //--------------------------------------------------------------
      rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> * m_tree_allocator;

      //--------------------------------------------------------------
      /// \brief	   The configuration content
      //--------------------------------------------------------------
      rapidjson::Document m_tree;

      

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the configuration content
      //--------------------------------------------------------------
      mutable boost::mutex m_treeMutex;
   };

   


   //--------------------------------------------------------------
   /// \brief	    The below part of header contains all the function
   ///				definitions (out of class definition for better 
   ///				cross compiler compatibilities).
   ///				-> for template methods
   ///				-> for template specialization
   //--------------------------------------------------------------
   template<typename T>
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
   inline const char* CDataContainer::get(const std::string& parameterName, const char pathChar) const
   {
      return (get<std::string>(parameterName, pathChar).c_str());
   }

   template<>
   inline unsigned long CDataContainer::get(const std::string& parameterName, const char pathChar) const
   {
      return get<unsigned int>(parameterName, pathChar);
   }

   template<>
   inline long CDataContainer::get(const std::string& parameterName, const char pathChar) const
   {
      return get<int>(parameterName, pathChar);
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
   inline void CDataContainer::appendArray(const std::string& parameterName, const unsigned long& value, const char pathChar)
   {
      helper<unsigned int>::appendArrayInternal(this, parameterName, value, pathChar);
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
   


   template<typename T>
   void CDataContainer::set(const std::string& parameterName, const T & value, const char pathChar)
   {
      helper<T>::setInternal(this, parameterName, value, pathChar);
   }


   template<>
   inline void CDataContainer::set(const std::string& parameterName, const unsigned long & value, const char pathChar)
   {
      helper<unsigned int>::setInternal(this, parameterName, value, pathChar);
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
      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
         return convert<T>(found);
      throw exception::CInvalidParameter(parameterName + " : is not found");
   }
   
   template<>
   inline CDataContainer CDataContainer::getInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
         return CDataContainer(found);
      throw exception::CInvalidParameter(parameterName + " : is not found");
   }

   


   template<>
   inline boost::posix_time::ptime CDataContainer::getInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->IsString())
            return boost::posix_time::from_iso_string(found->GetString());
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
      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->IsArray())
         {
			 std::transform(found->GetArray().begin(), found->GetArray().end(), std::back_inserter(result),
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
   /*
   template<>
   inline std::vector< shared::CDataContainer > CDataContainer::getValuesInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
#pragma messge ("Avoid use of std::vector<shared::CDataContainer>")
      std::vector< shared::CDataContainer > result;
      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->IsArray())
         {
			 std::transform(found->GetArray().begin(), found->GetArray().end(), std::back_inserter(result),
				 [](auto &v) -> shared::CDataContainer { return shared::CDataContainer(v); });
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
   */

   template<>
   inline std::vector< CDataContainerSharedPtr > CDataContainer::getValuesInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      std::vector< CDataContainerSharedPtr > result;
      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->IsArray())
         {
            std::transform(found->GetArray().begin(), found->GetArray().end(), std::back_inserter(result),
               [](auto& v) -> CDataContainerSharedPtr { return boost::make_shared<CDataContainer>(v); });
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
      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->IsArray())
         {
			 std::transform(found->GetArray().begin(), found->GetArray().end(), std::back_inserter(result),
				 [](auto &v) -> T { return static_cast<T>(v.GetInt()); });
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
      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->IsArray())
         {
            for (auto& v : found->GetArray())
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
   inline std::vector< CDataContainerSharedPtr > CDataContainer::getValuesSPInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector<CDataContainerSharedPtr > result;
      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->IsArray())
         {
            for (auto& v : found->GetArray())
            {
               CDataContainerSharedPtr sp = boost::make_shared<CDataContainer>(&v);
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
      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->IsArray())
         {
            for (auto& v : found->GetArray())
            {
               boost::shared_ptr<T> sp(new T);
               sp->fillFromContent(CDataContainer(v));
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
      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
      {
         if (found->IsArray())
         {
            for (auto& v : found->GetArray())
            {
               T a;
               CDataContainer dc(v);
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
      if (!m_tree.IsObject())
         m_tree.SetObject();

      rapidjson::Pointer(generatePath(parameterName, pathChar)).Set(m_tree, value);
   }

   template<>
   inline void CDataContainer::setInternal(const std::string& parameterName, const std::string & value, const char pathChar)
   {
      if (!m_tree.IsObject())
         m_tree.SetObject();

      rapidjson::Value v(value, m_tree.GetAllocator());
      rapidjson::Pointer(generatePath(parameterName, pathChar)).Set(m_tree, v);
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
      rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Set(m_tree, value.m_tree);
   }


   template<>
   inline void CDataContainer::setInternal(const std::string& parameterName, const boost::posix_time::ptime & value, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Set(m_tree, boost::posix_time::to_iso_string(value).c_str());
   }





   template<class T>
   void CDataContainer::setValuesInternal(const std::string& parameterName, const std::vector<T> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::Value & v = rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Create(m_tree).SetArray();
      rapidjson::Document::AllocatorType& allocator = m_tree.GetAllocator();
      for (typename std::vector<T>::const_iterator i = values.begin(); i != values.end(); ++i)
      {
         v.PushBack(*i, allocator);
      }
   }

   template<>
   inline void CDataContainer::setValuesInternal(const std::string& parameterName, const std::vector<std::string> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::Value & v = rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Create(m_tree).SetArray();
      rapidjson::Document::AllocatorType& allocator = m_tree.GetAllocator();
      for (auto i = values.begin(); i != values.end(); ++i)
      {
         rapidjson::Value val(i->c_str(), i->size(), allocator);
         v.PushBack(val, allocator);
      }
   }

   template<class T>
   void CDataContainer::setValuesInternalEnum(const std::string& parameterName, const std::vector<T> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::Value & v = rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Create(m_tree).SetArray();
      rapidjson::Document::AllocatorType& allocator = m_tree.GetAllocator();
      for (typename std::vector<T>::const_iterator i = values.begin(); i != values.end(); ++i)
      {
         v.PushBack(static_cast<int>(*i), allocator);
      }
   }

   template<class T>
   void CDataContainer::setValuesInternalIDataContainable(const std::string& parameterName, const std::vector<T> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::Value & v = rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Create(m_tree).SetArray();
      rapidjson::Document::AllocatorType& allocator = m_tree.GetAllocator();
      for (typename std::vector<T>::const_iterator i = values.begin(); i != values.end(); ++i)
      {
         CDataContainer t;
         i->extractContent(t);

         rapidjson::Value a;
         a.CopyFrom(t.m_tree, allocator);
         v.PushBack(a, allocator);
      }
   }



   template<typename T>
   inline void CDataContainer::setValuesSPInternal(const std::string& parameterName, const std::vector< boost::shared_ptr<T> > & values, const char pathChar)
   {
      if (!isArray(parameterName, pathChar))
         createArray(parameterName, pathChar);

      for (typename std::vector<boost::shared_ptr<T>>::const_iterator i = values.begin(); i != values.end(); ++i)
      {
         appendArray(parameterName, *i, pathChar);
      }
   }

   /*
   template<>
   inline void CDataContainer::setValuesSPInternal(const std::string& parameterName, const std::vector< CDataContainerSharedPtr > & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::Value & v = rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Create(m_tree).SetArray();
      rapidjson::Document::AllocatorType& allocator = m_tree.GetAllocator();
      for (std::vector<CDataContainerSharedPtr>::const_iterator i = values.begin(); i != values.end(); ++i)
      {
         rapidjson::Value a;
         a.CopyFrom((*i).get()->m_tree, allocator);
         v.PushBack(a, allocator);
      }
   }*/





   template<class T>
   void CDataContainer::appendArrayInternal(const std::string& parameterName, const T& value, const char pathChar)
   {
      if (!m_tree.IsObject())
         m_tree.SetObject();

      auto ptr = rapidjson::Pointer(generatePath(parameterName, pathChar)).Get(m_tree);
      if(ptr != NULL && ptr->IsArray())
      {
         ptr->PushBack(value, m_tree.GetAllocator());
      }
   }

   template<>
   inline void CDataContainer::appendArrayInternal(const std::string& parameterName, const std::string& value, const char pathChar)
   {
      if (!m_tree.IsObject())
         m_tree.SetObject();

      rapidjson::Value v(value, m_tree.GetAllocator());
      auto ptr = rapidjson::Pointer(generatePath(parameterName, pathChar)).Get(m_tree);
      if (ptr != NULL && ptr->IsArray())
      {
         ptr->PushBack(v, m_tree.GetAllocator());
      }

   }


   template<>
   inline void CDataContainer::appendArrayInternal(const std::string& parameterName, const CDataContainer& value, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      auto ptr = rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Get(m_tree);
      if (ptr != NULL && ptr->IsArray())
      {
         rapidjson::Document::AllocatorType& allocator = m_tree.GetAllocator();
//         CDataContainer t;
  //       value.extractContent(t);
    //     value.m_tree.CopyFrom();

         rapidjson::Value a;
         a.CopyFrom(value.m_tree, allocator);
         ptr->PushBack(a, allocator);
      }
   }


   template<>
   inline void CDataContainer::appendArrayInternal(const std::string& parameterName, const boost::posix_time::ptime& value, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      auto ptr = rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Get(m_tree);
      if (ptr != NULL && ptr->IsArray())
      {
         const std::string s = boost::posix_time::to_iso_string(value);
         rapidjson::Value v(s, m_tree.GetAllocator());
         ptr->PushBack(v, m_tree.GetAllocator());
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
   T CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return ptrValue->Get<T>();
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline bool CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToBool(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline std::string CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToString(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }
   
   template<>
   inline int CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToInt(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }
   
   template<>
   inline int64_t CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToInt64(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   } 
   
   template<>
   inline char CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToByte(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline short CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToShort(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }   

   template<>
   inline unsigned int CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToUInt(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }
   
   template<>
   inline uint64_t CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToUInt64(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   } 
   
   template<>
   inline unsigned char CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToUByte(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline unsigned short CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToUShort(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline float CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToFloat(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }

   template<>
   inline double CDataContainer::convert(rapidjson::Value* ptrValue) const
   {
      if(ptrValue)
         return convertToDouble(*ptrValue);
      throw exception::CInvalidParameter("Fail to convert NULL value");
   }


   inline void CDataContainer::setValuesSPIDataContainableInternal(const std::string& parameterName, const std::vector< boost::shared_ptr<IDataContainable> > & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::Value & v = rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Create(m_tree).SetArray();
      rapidjson::Document::AllocatorType& allocator = m_tree.GetAllocator();
      std::vector< boost::shared_ptr<IDataContainable> >::const_iterator i;
      for (i = values.begin(); i != values.end(); ++i)
      {
         CDataContainer t;
         (*i)->extractContent(t);
         rapidjson::Value a;
         a.CopyFrom(t.m_tree, allocator);
         v.PushBack(a, allocator);
      }
   }


   template<>
   inline void CDataContainer::setValuesInternal(const std::string& parameterName, const std::vector<CDataContainerSharedPtr> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::Value & v = rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Create(m_tree).SetArray();
      rapidjson::Document::AllocatorType& allocator = m_tree.GetAllocator();
      for (const auto& value : values)
      {
         rapidjson::Value a;
         a.CopyFrom(value->m_tree, allocator);
         v.PushBack(a, allocator);
      }
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
         for (rapidjson::Value::ConstMemberIterator i = found->MemberBegin(); i != found->MemberEnd(); ++i)
         {
            result[i->name.GetString()] = get<Type>(path + i->name.GetString(), pathChar);
         }
      }

      return result;
   }

} // namespace shared
