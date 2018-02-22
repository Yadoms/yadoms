//Here is the list of items of server configuration
ConfigurationManager.items = {};
ConfigurationManager.items.serverSection = "server";
ConfigurationManager.items.server = {};
ConfigurationManager.items.server.firstStart = "firstStart";
ConfigurationManager.items.server.timezone = "timezone";

ConfigurationManager.items.server.locationSection = "location";
ConfigurationManager.items.server.location = {};
ConfigurationManager.items.server.location.latitude = "latitude";
ConfigurationManager.items.server.location.longitude = "longitude";
ConfigurationManager.items.server.location.altitude = "altitude";
ConfigurationManager.items.server.location.timezone = "timezone";

ConfigurationManager.items.server.basicAuthenticationSection = "basicAuthentication";
ConfigurationManager.items.server.basicAuthentication = {};
ConfigurationManager.items.server.basicAuthentication.active = "active";
ConfigurationManager.items.server.basicAuthentication.user = "user";
ConfigurationManager.items.server.basicAuthentication.password = "password";

//Here is the list of items of configuration specific to this web client
ConfigurationManager.items.webclientSection = "webclient";
ConfigurationManager.items.webclient = {};
ConfigurationManager.items.webclient.language = "language";
ConfigurationManager.items.webclient.advancedParametersActive = "advancedParametersActive";
ConfigurationManager.items.webclient.dateFormatString = "dateFormatString";
ConfigurationManager.items.webclient.refreshPage = "refreshPage";


/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function ConfigurationManager() {
    var serverConfiguration;
    var webClientConfiguration;

    this.load = function () {
        var deferredArray = [];
        deferredArray.push(loadServerConfiguration());
        deferredArray.push(loadWebClientConfiguration());
    
        var d = new $.Deferred();
        $.whenAll(deferredArray)
            .done(d.resolve)
            .fail(d.reject);
    
        return d;
    }

    loadServerConfiguration = function () {
        var d = new $.Deferred();
    
        RestEngine.getJson("rest/configuration/server")
            .done(function (data) {
                serverConfiguration = JSON.parse(data);
                d.resolve();
            })
            .fail(d.reject);
    
        return d.promise();
    }
    
    loadWebClientConfiguration = function () {
        var d = new $.Deferred();
    
        RestEngine.getJson("rest/configuration/webClient")
            .done(function (data) {
                webClientConfiguration = JSON.parse(data);
                d.resolve();
            })
            .fail(d.reject);
    
        return d.promise();
    }

    this.isServerFirstStart = function () {
        return serverConfiguration[ConfigurationManager.items.server.firstStart] === "true";
    };
    
    this.currentLanguage = function () {
        return webClientConfiguration[ConfigurationManager.items.webclient.language] || getdefaultLanguageFromSupported();
    };

    this.refreshPage = function () {
        return webClientConfiguration[ConfigurationManager.items.webclient.refreshPage] === "true";
    };

    getdefaultLanguageFromSupported = function() {
        var navigatorLanguage = navigator.language || navigator.userLanguage;
        var langs = window.getSupportedLanguages();
        return isNullOrUndefined(langs[navigatorLanguage]) ? Object.keys(langs)[0] :
              navigatorLanguage;
  }
}













// TODO faire le ménage (fonctions obsolètes)

/**
 * Create a ConfigurationItem from json content
 * @param {Json} json The configuration item as json
 * @returns {ConfigurationItem} The newly created configuration item
 */
//TODO utile ?
ConfigurationManager.factory = function (json) {
    assert(!isNullOrUndefined(json), "json must be defined");
    assert(!isNullOrUndefined(json.section), "json.section must be defined");
    assert(!isNullOrUndefined(json.value), "json.value must be defined");
    assert(!isNullOrUndefined(json.lastModificationDate), "json.lastModificationDate must be defined");

    return new ConfigurationItem(json.section, json.name, ConfigurationItem.decodeValue(json.value),
        ConfigurationItem.decodeValue(json.defaultValue), json.description, json.securityAccess, json.lastModificationDate);
};





/**
 * Update a configuration item
 * @param {Object} configurationItem The configuration item to update
 * @returns {Promise} 
 */
ConfigurationManager.updateToServer = function (configurationItem) {
    assert(!isNullOrUndefined(configurationItem), "configurationItem must be defined");
    return RestEngine.putJson("/rest/configuration/" + configurationItem.section + "/" + configurationItem.name, {
        data: JSON.stringify(configurationItem)
    });
};


/**
 * Save a configuration item 
 * @param {String} section The configuration item section
 * @param {String} value The value 
 * @returns {Promise} 
 */
ConfigurationManager.createToServer = function (section, value) {
    assert(!isNullOrUndefined(section), "section must be defined");
    assert(!isNullOrUndefined(value), "value must be defined");

    return RestEngine.putJson("/rest/configuration/" + section, {
        data: JSON.stringify({
            "section": section,
            "value": value
        })
    });
};

ConfigurationManager.saveSystemConfiguration = function (newConfiguration) {
    assert(!isNullOrUndefined(newConfiguration), "newConfiguration must be defined");
    return RestEngine.putJson("/rest/configuration/server", {
        data: JSON.stringify(newConfiguration)
    });
};

ConfigurationManager.ServerConfiguration = function () {
    if (isNullOrUndefined(ServerConfiguration))
        console.error("Configuration not already loaded, call ConfigurationManager.loadServerConfiguration before accessing configuration");
    return ServerConfiguration;
}

ConfigurationManager.resetSystemConfiguration = function () {
    var d = new $.Deferred();

    RestEngine.putJson("/rest/configuration/server/reset")
        .done(function (newConfiguration) {
            ServerConfiguration = newConfiguration;
            d.resolve();
        })
        .fail(function () {
            console.error("failed to reset server configuration");
            d.reject();
        })
    return d;
}

ConfigurationManager.saveDefaultLanguage = function (defaultLanguage) {
    return ConfigurationManager.createToServer(ConfigurationManager.items.systemSection,
        ConfigurationManager.items.server.language,
        defaultLanguage,
        "",
        "language used by default");
};

ConfigurationManager.saveFirstStart = function (arrayOfDeffered) {
    return ConfigurationManager.createToServer(ConfigurationManager.items.installSection,
        ConfigurationManager.items.install.firstStart,
        "false",
        "true",
        "First start of Web app has been done");
};