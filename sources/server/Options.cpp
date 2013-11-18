//
// Options.cpp
//
#pragma once

#include <Options.h>
#include <boost/program_options.hpp>

static const std::string ConfigFile("yadoms.cfg");

namespace po = boost::program_options;

COptions::m_instance = NULL;
COptions* Options = COptions::getInstance();


COptions* COptions::instance()
{
   if (m_instance == NULL)
      m_instance = new COptions;
   return m_instance;
}

std::istream& COptions::operator>>(std::istream& in, boost::log::trivial::severity_level& logLevel) const//TODO voir si réellement nécessaire (si c'est pas déjà implémenté dans le type severity_level (m'étonnerai, ça doit être un enum tout bête m'enfin faut vérifier. Et là je ferme toutes les parenthèses. Nico, si tu me lis, sache que tu perds ton temps à relire tout mon code, il compilera et marchera parfaitement ! ... un jour. Tu vois, je te l'avais dit, tu perds ton temps... Cette fois je les ferme les parenthèses. ) )
{
    std::string token;
    in >> token;
    if      (token == "trace"  ) logLevel = boost::log::trivial::trace;
    else if (token == "debug"  ) logLevel = boost::log::trivial::debug;
    else if (token == "info"   ) logLevel = boost::log::trivial::info;
    else if (token == "warning") logLevel = boost::log::trivial::warning;
    else if (token == "error"  ) logLevel = boost::log::trivial::error;
    else if (token == "fatal"  ) logLevel = boost::log::trivial::fatal;
    else throw po::validation_error("Invalid log Level");
    return in;
}

bool COptions::load(int argc, char** argv)
{
   try
   {
      // Configuration description
      po::options_description desc("Allowed options");
      desc.add_options()
         ("help", "produce help message")
         ("logLevel", po::value<boost::log::trivial::severity_level>(&m_logLevel)->default_value(boost::log::trivial::warning),
            "set log level, accepted values are : trace, debug, info, warning, error, fatal")
         ;

      // Load configuration from command line and complete with config file (second data read don't overwrite first)
      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, options), vm);
      po::store(parse_config_file(ConfigFile, desc), vm);
      po::notify(vm);
   }
   catch(po::validation_error &exception)
   {
      BOOST_LOG_TRIVIAL(fatal) << "Configuration error : " << exception.what();
      return false;
   }

   return true;
}

bool COptions::save()
{
   // TODO : C'est moche, il n'y a rien pour enregistrer le fichier de conf
   // En même temps, ça serait merdique vu qu'aucune différence n'est faite entre les données venant de la ligne de commande
   // et celle venant du fichier de config.
}
