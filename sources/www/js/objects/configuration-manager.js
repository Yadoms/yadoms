/**
 * Configuration manager : manage all configurations (server and client)
 * @constructor
 */
function ConfigurationManager() {

    this.load = function () {
        var deferredArray = [];
        deferredArray.push(loadServerConfiguration());
        deferredArray.push(loadWebClientConfiguration());
        deferredArray.push(loadDatabaseVersion());

        var d = new $.Deferred();
        $.whenAll(deferredArray)
            .done(d.resolve)
            .fail(d.reject);

        return d;
    }

    this.isServerFirstStart = function () {
        return serverConfiguration[items.server.firstStart] === "true";
    };

    this.currentLanguage = function () {
        return webClientConfiguration[items.webclient.language];
    };

    this.refreshPage = function () {
        return webClientConfiguration[items.webclient.refreshPage] === "true";
    };

    this.databaseVersion = function () {
        return databaseVersion;
    };

    this.saveServerFirstStartDone = function () {
        serverConfiguration[items.server.firstStart] = "false";
        return saveServerConfiguration();
    };


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

    saveServerConfiguration = function () {
        return RestEngine.putJson("/rest/configuration/server", {
            data: JSON.stringify(serverConfiguration)
        });
    }


    loadDatabaseVersion = function () {
        var d = new $.Deferred();

        RestEngine.getJson("rest/configuration/DatabaseVersion")
            .done(function (data) {
                databaseVersion = data;
                d.resolve();
            })
            .fail(d.reject);

        return d.promise();
    }

    loadWebClientConfiguration = function () {
        var d = new $.Deferred();

        RestEngine.getJson("rest/configuration/webClient")
            .done(function (data) {
                loadedClientConfiguration = JSON.parse(data);

                // Merge loaded configuration with default one, in case of some new fields not in loaded configuration
                webClientConfiguration = defaultWebClientConfiguration;
                Object.assign(webClientConfiguration, loadedClientConfiguration);

                d.resolve();
            })
            .fail(function () {
                webClientConfiguration = defaultWebClientConfiguration;
                d.reject();
            });

        return d.promise();
    }

    saveWebClientConfiguration = function () {
        return RestEngine.putJson("/rest/configuration/webClient", {
            data: JSON.stringify(webClientConfiguration)
        });
    }
    
    var getdefaultLanguageFromSupported = function () {
        var navigatorLanguage = navigator.language || navigator.userLanguage;
        var langs = window.getSupportedLanguages();
        return isNullOrUndefined(langs[navigatorLanguage]) ? Object.keys(langs)[0] :
            navigatorLanguage;
    }

    
    // Items of server configuration
    var items = {};
    items.serverSection = "server";
    items.server = {};
    items.server.firstStart = "firstStart";
    items.server.timezone = "timezone";

    items.server.locationSection = "location";
    items.server.location = {};
    items.server.location.latitude = "latitude";
    items.server.location.longitude = "longitude";
    items.server.location.altitude = "altitude";
    items.server.location.timezone = "timezone";

    items.server.basicAuthenticationSection = "basicAuthentication";
    items.server.basicAuthentication = {};
    items.server.basicAuthentication.active = "active";
    items.server.basicAuthentication.user = "user";
    items.server.basicAuthentication.password = "password";

    // Items of configuration specific to this web client
    items.webclientSection = "webclient";
    items.webclient = {};
    items.webclient.language = "language";
    items.webclient.advancedParametersActive = "advancedParametersActive";
    items.webclient.dateFormatString = "dateFormatString";
    items.webclient.refreshPage = "refreshPage";
    // Associated default values
    var defaultWebClientConfiguration = {};
    defaultWebClientConfiguration[items.webclient.language] = getdefaultLanguageFromSupported();
    defaultWebClientConfiguration[items.webclient.advancedParametersActive] = "false";
    defaultWebClientConfiguration[items.webclient.dateFormatString] = "LLL";
    defaultWebClientConfiguration[items.webclient.refreshPage] = "false";
    defaultWebClientConfiguration["newData"] = "1234";

    // Main configuration instances
    var serverConfiguration;
    var webClientConfiguration;
    var databaseVersion;
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