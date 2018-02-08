#pragma once

#include <shared/Export.h>
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
   ///               //retreive container
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
   class YADOMS_SHARED_EXPORT CDataContainer : public serialization::IDataSerializable, public serialization::IDataFileSerializable /*, public IDataContainable*/
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
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      explicit CDataContainer(const std::string & initialData);     
      
      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial map data
      //--------------------------------------------------------------
      explicit CDataContainer(const std::map<std::string, std::string> & initialData);

      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      CDataContainer(const CDataContainer & initialData);

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
      inline T get(const std::string& parameterName = std::string(), const char pathChar = '.') const; 
      
      //--------------------------------------------------------------
      /// \brief	    Get parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] defaultValue     The default value in case the parameter do not exists
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     The parameter value, or the default value if parameter not found
      //--------------------------------------------------------------
      template<class T>
      inline T getWithDefault(const std::string& parameterName, const T & defaultValue, const char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      //--------------------------------------------------------------
      template<class T>
      inline void set(const std::string& parameterName, const T & value, const char pathChar = '.');
      
      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      //--------------------------------------------------------------
      template<class T>
      inline void set(const char* parameterName, const T & value, const char pathChar = '.');


      //--------------------------------------------------------------
      /// \brief	    Get current parameter key name
      /// \return     The parameter key name
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      std::string getKey() const;


      //--------------------------------------------------------------
      //
      //
      //
      // "Specializations" of get/set method 
      //  for char* (cannot be managed as template specialization easily)
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      //--------------------------------------------------------------
      void set(const std::string & parameterName, const char* value, const char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      //--------------------------------------------------------------
      void set(const char* parameterName, const char* value, const char pathChar = '.');

      //--------------------------------------------------------------
      /// \brief	    Get parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      const char* get(const std::string & parameterName = std::string(), const char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter value as map<string, string>
      /// \param [in] parameterName    Name of the parameter
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      std::map<std::string, std::string> getAsMap(const std::string& parameterName = std::string(), const char pathChar = '.') const;


      //--------------------------------------------------------------
      /// \brief	    Get parameter sub-keys as vector<string>
      /// \param [in] parameterName    Name of the parameter
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      std::vector<std::string> getKeys(const std::string& parameterName = std::string(), const char pathChar = '.') const;


      //--------------------------------------------------------------
      /// \brief	    Find a sub-parameter with criteria
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] whereFct         Criteria : lambda must returns true if item is found
      /// \return     The found parameter
      /// \throw      shared::exception::CEmptyResult if no parameter matching criteria was found
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      CDataContainer find(const std::string& parameterName, boost::function<bool(const CDataContainer&)> whereFct, const char pathChar = '.') const;


      //--------------------------------------------------------------
      /// \brief	    Merge this container from another one
      /// \param [in] from source container
      /// \desc       Add non-exisiting (or replace existing) values of "from" container into this container
      ///             Values of this container not present in "from" container will be kept
      //--------------------------------------------------------------
      void mergeFrom(const CDataContainer& from);



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
      EnumType getEnumValue(const std::string& parameterName, const EnumValuesNames& valuesNames, const char pathChar = '.') const;


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
      bool exists(const std::string& parameterName, const char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Check if a node contains at least a child
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     true if the parameter contains child (CDataContainer), false if it is a termination node with data
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      bool containsChild(const std::string& parameterName = std::string(), const char pathChar = '.') const;

      //--------------------------------------------------------------
      /// \brief	    Check if a node is terminal value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The path spearator to use (default is '.')
      /// \return     true if the path contains only single value, false if it is contains a subtree
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      bool containsValue(const std::string& parameterName = std::string(), const char pathChar = '.') const;

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
      friend YADOMS_SHARED_EXPORT std::ostream& operator<<(std::ostream& os, const CDataContainer& dt);
      
      //--------------------------------------------------------------
      /// \brief		Input operator (read a serialized container)
      /// \param [in] is  The stream to read
      /// \param [in] dt  The output data container
      /// \return   The stream
      //--------------------------------------------------------------
      friend YADOMS_SHARED_EXPORT std::istream& operator>>(std::istream& is, CDataContainer& dt);

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


   protected:
      //--------------------------------------------------------------
      /// \brief		Constructor
      /// \param [in] initialTree    Initial tree for this container
      //--------------------------------------------------------------
      explicit CDataContainer(const boost::property_tree::ptree & initialTree);

      //--------------------------------------------------------------
      /// \brief		Print the content to log
      /// \param [in] tree    The tree to print
      /// \param [in] deep    The current tree deep
      /// \param [in] os      The out stream where to print out
      //--------------------------------------------------------------
      void printToLog(const boost::property_tree::ptree & tree, const int deep, std::ostream& os) const;

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
      inline T getInternal(const std::string& parameterName, const char pathChar) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter value (T is IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter value (T is IDataContainable)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline T getInternalIDataContainable(const std::string& parameterName, const char pathChar) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter values
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline std::vector<T> getValuesInternal(const std::string& parameterName, const char pathChar) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (T is IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (T is IDataContainable)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline std::vector<T> getValuesInternalIDataContainable(const std::string& parameterName, const char pathChar) const;


      //--------------------------------------------------------------
      /// \brief	    Get parameter values (boost::shared_ptr<T>)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (boost::shared_ptr<T>)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline std::vector< boost::shared_ptr<T> > getValuesSPInternal(const std::string& parameterName, const char pathChar) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (boost::shared_ptr<T> and T is IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (boost::shared_ptr<T> and T is IDataContainable)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline std::vector< boost::shared_ptr<T> > getValuesSPIDataContainableInternal(const std::string& parameterName, const char pathChar) const;


      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
      template<class T>
      inline void setInternal(const std::string& parameterName, const T & value, const char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Set parameter value (T is IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter (T is IDataContainable)
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
      template<class T>
      inline void setInternalIDataContainable(const std::string& parameterName, const T & value, const char pathChar);
      
      //--------------------------------------------------------------
      /// \brief	    Set parameter values
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] values           Valuse of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
      template<class T>
      inline void setValuesInternal(const std::string& parameterName, const std::vector<T> & values, const char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (IDataContainable)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (IDataContainable)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline void setValuesInternalIDataContainable(const std::string& parameterName, const std::vector<T> & values, const char pathChar);

		//--------------------------------------------------------------
		/// \brief	    Get parameter values (Enum)
		/// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (Enum)
		/// \throw      shared::exception::COutOfRange if parameter can not be converted
		/// \throw      shared::exception::CInvalidParameter if parameter is not found
		//--------------------------------------------------------------
		template<class T>
      inline std::vector<T> getValuesInternalEnum(const std::string& parameterName, const char pathChar) const;


		//--------------------------------------------------------------
		/// \brief	    Set parameter values (Enum)
		/// \param [in] parameterName    Name of the parameter
		/// \param [in] values           Valuse of the parameter (Enum)
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      //--------------------------------------------------------------
		template<class T>
      inline void setValuesInternalEnum(const std::string& parameterName, const std::vector<T> & values, const char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (std::vector< boost::shared_ptr<T> >)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (std::vector< boost::shared_ptr<T> >)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline void setValuesSPInternal(const std::string& parameterName, const std::vector< boost::shared_ptr<T> > & values, const char pathChar);

      //--------------------------------------------------------------
      /// \brief	    Get parameter values (std::vector< boost::shared_ptr<IDataContainable> >)
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] pathChar         The character used for path separator (default is '.' : standard path, can be 0x00 to disable path, or any char '/', ... )
      /// \return     The parameter values (std::vector< boost::shared_ptr<IDataContainable> >)
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      inline void setValuesSPIDataContainableInternal(const std::string& parameterName, const std::vector< boost::shared_ptr<IDataContainable> > & values, const char pathChar);

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
      };

      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with CField<T>
      //--------------------------------------------------------------
      template <typename T>
      struct helper < CField< T > >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for boost::shared_ptr<T>
         //--------------------------------------------------------------
         static CField< T > getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return CField< T >(helper<T>::getInternal(tree, parameterName, pathChar));
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for boost::shared_ptr<T>
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const CField< T > & value, const char pathChar)
         {
            helper<T>::setInternal(tree, parameterName, value(), pathChar);
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

      };



      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with an IExtendedEnum object
      //--------------------------------------------------------------
      template <typename T>
      struct helper < T, typename boost::enable_if< boost::is_base_of< shared::enumeration::IExtendedEnum, T > >::type >
      {
         //--------------------------------------------------------------
         /// \brief	    GET Method for IDataContainable object
         //--------------------------------------------------------------
         static T getInternal(const CDataContainer * tree, const std::string& parameterName, const char pathChar)
         {
            return T(tree->getInternal<std::string>(parameterName, pathChar));
         }

         //--------------------------------------------------------------
         /// \brief	    SET Method for IDataContainable object
         //--------------------------------------------------------------
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const T & value, const char pathChar)
         {
            tree->setInternal<std::string>(parameterName, static_cast<std::string>(value), pathChar);
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
            typename std::vector<  boost::shared_ptr<T> >::const_iterator i;
            for (i = value.begin(); i != value.end(); ++i)
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
      static boost::property_tree::ptree::path_type generatePath(const std::string & parameterName, const char pathChar);

      static void mergeChildFrom(const boost::property_tree::ptree& from, boost::property_tree::ptree& to);


   private:
      //--------------------------------------------------------------
      /// \brief	   The configuration content
      //--------------------------------------------------------------
      boost::property_tree::ptree m_tree;

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
   inline T CDataContainer::get(const std::string& parameterName, const char pathChar) const
   {
      return helper<T>::getInternal(this, parameterName, pathChar);
   }

   template<>
   inline std::map<std::string, std::string> CDataContainer::get(const std::string& parameterName, const char pathChar) const
   {
      return getAsMap(parameterName, pathChar);
   }

   template<>
   inline const char* CDataContainer::get(const std::string& parameterName, const char pathChar) const
   {
      return (get<std::string>(parameterName, pathChar).c_str());
   }


   template<class T>
   inline T CDataContainer::getWithDefault(const std::string& parameterName, const T & defaultValue, const char pathChar) const
   {
      if (!exists(parameterName, pathChar))
         return defaultValue;
      return get<T>(parameterName, pathChar);
   }


   template<class T>
   inline void CDataContainer::set(const char* parameterName, const T & value, const char pathChar)
   {
      std::string strParamName(parameterName);
      set<T>(strParamName, value, pathChar);
   }




   template<typename T>
   inline void CDataContainer::set(const std::string& parameterName, const T & value, const char pathChar)
   {
      helper<T>::setInternal(this, parameterName, value, pathChar);
   }



   template<class T>
   inline T CDataContainer::getInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         return m_tree.get<T>(generatePath(parameterName, pathChar));
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   } 
   


   template<>
   inline CDataContainer CDataContainer::getInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         return CDataContainer(m_tree.get_child(generatePath(parameterName, pathChar)));
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }


   template<>
   inline boost::posix_time::ptime CDataContainer::getInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         return boost::posix_time::from_iso_string(m_tree.get<std::string>(generatePath(parameterName, pathChar)));
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }

   template<class T>
   inline T CDataContainer::getInternalIDataContainable(const std::string& parameterName, const char pathChar) const
   {
      try
      {
         CDataContainer t2 = getInternal<CDataContainer>(parameterName, pathChar);
         T obj;
         obj.fillFromContent(t2);
         return obj;
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }






   template<class T>
   inline std::vector<T> CDataContainer::getValuesInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector<T> result;
      try
      {
         boost::property_tree::ptree child = m_tree.get_child(generatePath(parameterName, pathChar));

         boost::property_tree::ptree::const_iterator end = child.end();
         for (boost::property_tree::ptree::const_iterator it = child.begin(); it != end; ++it) {
            result.push_back(it->second.get_value<T>());
         }
         return result;
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }


   template<>
   inline std::vector< shared::CDataContainer > CDataContainer::getValuesInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector< shared::CDataContainer > result;
      try
      {
         boost::property_tree::ptree child = m_tree.get_child(generatePath(parameterName, pathChar));

         for (boost::property_tree::ptree::const_iterator it = child.begin(); it != child.end(); ++it)
         {
            result.push_back(shared::CDataContainer(it->second));
         }
         return result;
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }



   template<class T>
   inline std::vector<T> CDataContainer::getValuesInternalEnum(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector<T> result;
      try
      {
         auto child = m_tree.get_child(generatePath(parameterName, pathChar));

         boost::property_tree::ptree::const_iterator end = child.end();
         for (boost::property_tree::ptree::const_iterator it = child.begin(); it != end; ++it) {
            result.push_back(static_cast<T>(it->second.get_value<int>()));
         }
         return result;
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }

   template<class T>
   inline std::vector< boost::shared_ptr<T> > CDataContainer::getValuesSPInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector< boost::shared_ptr<T> > result;
      try
      {
         boost::property_tree::ptree child = m_tree.get_child(generatePath(parameterName, pathChar));

         boost::property_tree::ptree::const_iterator end = child.end();
         for (boost::property_tree::ptree::const_iterator it = child.begin(); it != end; ++it)
         {
            boost::shared_ptr<T> sp = boost::make_shared<T>(it->second.get_value<T>());
            result.push_back(sp);
         }
         return result;
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }

   template<class T>
   inline std::vector< boost::shared_ptr<T> > CDataContainer::getValuesSPIDataContainableInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector< boost::shared_ptr<T> > result;
      try
      {
         boost::property_tree::ptree child = m_tree.get_child(generatePath(parameterName, pathChar));

         boost::property_tree::ptree::const_iterator end = child.end();
         for (boost::property_tree::ptree::const_iterator it = child.begin(); it != end; ++it)
         {
            boost::shared_ptr<T> sp(new T);
            sp->fillFromContent(CDataContainer(it->second));
            result.push_back(sp);
         }
         return result;
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }




   template<>
   inline std::vector< boost::shared_ptr<IDataContainable> > CDataContainer::getValuesSPInternal(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector<boost::shared_ptr<IDataContainable> > result;
      try
      {
         boost::property_tree::ptree child = m_tree.get_child(generatePath(parameterName, pathChar));

         boost::property_tree::ptree::const_iterator end = child.end();
         for (boost::property_tree::ptree::const_iterator it = child.begin(); it != end; ++it)
         {
            boost::shared_ptr<IDataContainable> sp;
            result.push_back(sp);
         }
         return result;
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }



   template<class T>
   inline std::vector<T> CDataContainer::getValuesInternalIDataContainable(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector<T> result;
      try
      {
         boost::property_tree::ptree child = m_tree.get_child(generatePath(parameterName, pathChar));

         boost::property_tree::ptree::const_iterator end = child.end();
         for (boost::property_tree::ptree::const_iterator it = child.begin(); it != end; ++it)
         {
            T a;
            a.fillFromContent(CDataContainer(it->second));
            result.push_back(a);
         }
         return result;
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }










   template<class T>
   inline void CDataContainer::setInternal(const std::string& parameterName, const T & value, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         m_tree.put(generatePath(parameterName, pathChar), value);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }

   template<class T>
   inline void CDataContainer::setInternalIDataContainable(const std::string& parameterName, const T & value, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         CDataContainer subTree;
         value.extractContent(subTree);
         m_tree.add_child(generatePath(parameterName, pathChar), subTree.m_tree);

      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }



   template<>
   inline void CDataContainer::setInternal(const std::string& parameterName, const CDataContainer & value, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         m_tree.add_child(generatePath(parameterName, pathChar), value.m_tree);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }


   template<>
   inline void CDataContainer::setInternal(const std::string& parameterName, const boost::posix_time::ptime & value, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         m_tree.put(generatePath(parameterName, pathChar), boost::posix_time::to_iso_string(value));
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }





   template<class T>
   inline void CDataContainer::setValuesInternal(const std::string& parameterName, const std::vector<T> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         boost::property_tree::ptree innerData;

         typename std::vector<T>::const_iterator i;
         for (i = values.begin(); i != values.end(); ++i)
         {
            boost::property_tree::ptree t;
            t.put("", *i);
            innerData.push_back(std::make_pair("", t));
         }

         m_tree.add_child(generatePath(parameterName, pathChar), innerData);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }

   template<class T>
   inline void CDataContainer::setValuesInternalEnum(const std::string& parameterName, const std::vector<T> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         boost::property_tree::ptree innerData;

         typename std::vector<T>::const_iterator i;
         for (i = values.begin(); i != values.end(); ++i)
         {
            boost::property_tree::ptree t;
            t.put("", static_cast<int>(*i));
            innerData.push_back(std::make_pair("", t));
         }

         m_tree.add_child(generatePath(parameterName, pathChar), innerData);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }

   template<class T>
   inline void CDataContainer::setValuesInternalIDataContainable(const std::string& parameterName, const std::vector<T> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         boost::property_tree::ptree innerData;

         typename std::vector<T>::const_iterator i;
         for (i = values.begin(); i != values.end(); ++i)
         {
            CDataContainer t;
            i->extractContent(t);
            innerData.push_back(std::make_pair("", t.m_tree));
         }

         m_tree.add_child(generatePath(parameterName, pathChar), innerData);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }



   template<class T>
   inline void CDataContainer::setValuesSPInternal(const std::string& parameterName, const std::vector< boost::shared_ptr<T> > & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         boost::property_tree::ptree innerData;

         typename std::vector< boost::shared_ptr<T> >::const_iterator i;
         for (i = values.begin(); i != values.end(); ++i)
         {
            boost::property_tree::ptree t;
            t.put("", *i->get());
            innerData.push_back(std::make_pair("", t));
         }

         m_tree.add_child(generatePath(parameterName, pathChar), innerData);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }

   inline void CDataContainer::setValuesSPIDataContainableInternal(const std::string& parameterName, const std::vector< boost::shared_ptr<IDataContainable> > & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         boost::property_tree::ptree innerData;

         std::vector< boost::shared_ptr<IDataContainable> >::const_iterator i;
         for (i = values.begin(); i != values.end(); ++i)
         {
            CDataContainer t;
            (*i)->extractContent(t);
            innerData.push_back(std::make_pair("", t.m_tree));
         }

         m_tree.add_child(generatePath(parameterName, pathChar), innerData);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }


   template<>
   inline void CDataContainer::setValuesInternal(const std::string& parameterName, const std::vector<CDataContainer> & values, const char pathChar)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         boost::property_tree::ptree innerData;

         std::vector<CDataContainer>::const_iterator i;
         for (i = values.begin(); i != values.end(); ++i)
         {
            innerData.push_back(std::make_pair("", i->m_tree));
         }

         m_tree.add_child(generatePath(parameterName, pathChar), innerData);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }



   template<typename EnumType>
   EnumType CDataContainer::getEnumValue(const std::string& parameterName, const EnumValuesNames& valuesNames, const char pathChar) const
   {
      auto stringValue = get<std::string>(parameterName, pathChar);
      auto it = valuesNames.find(stringValue);
      if (it != valuesNames.end())
         return static_cast<EnumType>(it->second);

      throw exception::COutOfRange(std::string("Value ") + stringValue + " was not found for " + parameterName + " parameter");
   }

} // namespace shared
