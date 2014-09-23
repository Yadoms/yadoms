#pragma once

#include "OpenZWaveCommandClass.h"
#include "CommandClass.h"
#include "IOpenZWaveNodeKeyword.h"

//--------------------------------------------------------------
/// \brief     Class used to encapsulate a ZWave network node capacity (from OpenZWave)
///            A capacity match a commandClass and some keywords
//--------------------------------------------------------------
class COpenZWaveNodeCapacity
{
public:
   //--------------------------------------------------------------
   /// \brief	      Constructor
   /// \param [in]   homeId            The home id
   /// \param [in]   nodeId            The node id
   /// \param [in]   classIdentifier   The class identifier (=capacity from OpenZWave library)
   //--------------------------------------------------------------
   COpenZWaveNodeCapacity(const int homeId, const int nodeId, const ECommandClass & classIdentifier);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------   
   virtual ~COpenZWaveNodeCapacity();
   
   //--------------------------------------------------------------
   /// \brief	      Get the commandClass
   /// \return       The commandClass
   //--------------------------------------------------------------        
   boost::shared_ptr<OpenZWave::CommandClass> getCommandClass();
   
   //--------------------------------------------------------------
   /// \brief	      Get the class identifier
   /// \return       the class identifier
   //--------------------------------------------------------------        
   ECommandClass getClassIdentifier();

   //--------------------------------------------------------------
   /// \brief	      Register a keyword
   /// \param [in]   keyword           The keyword to register
   /// \param [in]   value             The ValueID associated to the keyword
   //--------------------------------------------------------------      
   void registerKeyword(const std::string & keyword, OpenZWave::ValueID & value);

   //--------------------------------------------------------------
   /// \brief	      Send a command to a keyword
   /// \param [in]   keyword           The keyword name
   /// \param [in]   commandData       The command data
   //--------------------------------------------------------------   
   bool sendCommand(const std::string & keyword, const std::string & commandData);

   //--------------------------------------------------------------
   /// \brief	      get the last value of a keyword (as string)
   /// \param [in]   keyword           The keyword name
   //--------------------------------------------------------------   
   std::string getLastKeywordValue(const std::string & keyword);

private:
   //--------------------------------------------------------------
   /// \brief	      The class identifier
   //-------------------------------------------------------------- 
   ECommandClass m_classIdentifier;
   
   //--------------------------------------------------------------
   /// \brief	      The commandClass
   //--------------------------------------------------------------    
   boost::shared_ptr<OpenZWave::CommandClass> m_class;
   
   //--------------------------------------------------------------
   /// \brief	      Type for keyword list
   //--------------------------------------------------------------    
   typedef std::map< std::string, boost::shared_ptr<IOpenZWaveNodeKeyword> > KeywordsContainer;

   //--------------------------------------------------------------
   /// \brief	      The keyword list
   //--------------------------------------------------------------    
   KeywordsContainer m_keywords;
};



