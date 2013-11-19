#pragma once

#include <string>

namespace plugins
{
	//--------------------------------------------------------------
	/// \brief		Container class for information about Hardware plugins
	//--------------------------------------------------------------
    class CHardwarePluginInformation 
    {
    public:
	    //--------------------------------------------------------------
	    /// \brief		Enum for plugin state
	    //--------------------------------------------------------------
        enum EReleaseType
        {
            kStable,    /// \brief		plugin is stable
            kTesting,   /// \brief		plugin is in testing phase
            kBeta       /// \brief		plugin is in beta state
        };


	    //--------------------------------------------------------------
	    /// \brief	    Constructor
        /// \param      name : plugin name
        /// \param      releaseType : plugin state
        /// \param      author : plugin author
        /// \param      url : plugin url
	    //--------------------------------------------------------------
        CHardwarePluginInformation (   const std::string & name , const std::string & version, 
                                       const EReleaseType & releaseType, const std::string & author,
                                       const std::string & url)
            :m_name(name), m_version(version), m_releaseType(releaseType), m_author(author), m_url(url)
        {
        }

 	    //--------------------------------------------------------------
	    /// \brief	    Destructor
 	    //--------------------------------------------------------------
        virtual ~CHardwarePluginInformation ()
        {
        }

    public:
	    //--------------------------------------------------------------
	    /// \brief	    get the plugin Name
        /// \return     the plugin name
	    //--------------------------------------------------------------
        const std::string getName() 
        { 
            return  m_name; 
        }

	    //--------------------------------------------------------------
	    /// \brief	    get the plugin version
        /// \return     the plugin version
	    //--------------------------------------------------------------
        const std::string getVersion() 
        { 
            return  m_version; 
        }

	    //--------------------------------------------------------------
	    /// \brief	    get the plugin release type
        /// \return     the plugin release type
	    //--------------------------------------------------------------
        const EReleaseType getReleaseType() 
        { 
            return m_releaseType; 
        }

	    //--------------------------------------------------------------
	    /// \brief	    get the plugin author
        /// \return     the plugin author
	    //--------------------------------------------------------------
        const std::string getAuthor() 
        { 
            return  m_author; 
        }

	    //--------------------------------------------------------------
	    /// \brief	    get the plugin URL
        /// \return     the plugin URL
	    //--------------------------------------------------------------
        const std::string getUrl() 
        { 
            return  m_url; 
        }

private:
        std::string m_name;
        std::string m_version;
        EReleaseType m_releaseType;
        std::string m_author;
        std::string m_url;
    };
}

