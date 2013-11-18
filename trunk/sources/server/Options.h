//
// Options.h
//
// Yadoms-configuration management singleton
//
#pragma once

#include <boost/log/trivial.hpp>

//TODO : à mettre dans le main :
main()
{
   if (COptions::getInstance()->load(argc, argv))
      // Sortie en erreur

   // ... Boucle principale ...
   if (COptions::getInstance()->getLogLevel() == boost::log::trivial::severity_level::fatal)
      // Fais qqc

   if (!COption::getInstance()->save())
      // Juste une trace d'erreur
}


class COptions
{
private:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \note       Declared as private to implement singleton
   //--------------------------------------------------------------
   COptions() {}

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COptions() {}

public:
   //--------------------------------------------------------------
   /// \brief	    Get (create if necessary) the singleton instance
   /// \return     The COptions singleton
   //--------------------------------------------------------------
   static COptions* getInstance();

   //--------------------------------------------------------------
   /// \brief	            Load configuration (from config file and commande line)
   /// \param[in]  argc    Main argc parameter (from command line)
   /// \param[in]  argv    Main argv parameter (from command line)
   /// \return             Return false if load fails
   //--------------------------------------------------------------
   bool load(int argc, char** argv);

   //--------------------------------------------------------------
   /// \brief	            Save configuration in config file
   /// \return             Return false if save fails
   //--------------------------------------------------------------
   bool save();

   //--------------------------------------------------------------
   /// \brief	    Get the log level
   /// \return     Configured log level
   //--------------------------------------------------------------
   boost::log::trivial::severity_level getLogLevel() { return m_Loglevel; }


protected:
   //--------------------------------------------------------------
   /// \brief	    Deserialization operator
   //--------------------------------------------------------------
   std::istream& operator>>(std::istream& in, Length::Unit& unit) const;

private:
   //--------------------------------------------------------------
   /// \brief	    The singleton instance
   /// \note       We don't want COptions to be create at application
   ///             loading, to be sure to get the command line parameters.
   //--------------------------------------------------------------
   static COptions* m_instance;


   //--------------------------------------------------------------
   /// \brief	    Log level
   //--------------------------------------------------------------
   boost::log::trivial::severity_level m_Loglevel;
};

// TODO : les règles de codage me permettent-elles de faire ça ? Faut déjà que ça marche
// L'idée étant d'appeler Options->load() au lieu de COptions::getInstance()->load()
extern COptions* Options;


