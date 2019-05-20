/**
 * Configuration manager : manage all configurations (server and client)
 * @constructor
 */
function ConfigurationManager() {

    this.load = function () {
        serverConfigurationChanged = false;
        webClientConfigurationChanged = false;

        var deferredArray = [];
        deferredArray.push(loadServerConfiguration());
        deferredArray.push(loadDatabaseVersion());
        var defferedWebClient = loadWebClientConfiguration();
        
        var d = new $.Deferred();
        $.when.apply($,deferredArray)
            .done(function(){
               defferedWebClient.always(d.resolve);
            })
            .fail(d.reject);

        return d.promise();
    }

    this.save = function () {
        var deferredArray = [];
        deferredArray.push(saveServerConfiguration());
        deferredArray.push(saveWebClientConfiguration());

        var d = new $.Deferred();
        $.when.apply($,deferredArray)
            .done(d.resolve)
            .fail(d.reject);

        return d.promise();
    }

    this.reset = function () {
        var deferredArray = [];
        deferredArray.push(resetServerConfiguration());
        deferredArray.push(resetWebClientConfiguration());

        var d = new $.Deferred();
        $.when.apply($,deferredArray)
            .done(d.resolve)
            .fail(d.reject);

        return d.promise();
    }

    this.isServerFirstStart = function () {
        return parseBool(serverConfiguration[items.server.firstStart]);
    };

    this.setServerFirstStartDone = function () {
        if (parseBool(serverConfiguration[items.server.firstStart]) !== false) {
            serverConfiguration[items.server.firstStart] = false;
            serverConfigurationChanged = true;
        }
        return this;
    };

    this.currentLanguage = function () {
        return webClientConfiguration[items.webclient.language];
    };

    this.setCurrentLanguage = function (value) {
        if (webClientConfiguration[items.webclient.language] != value) {
            webClientConfiguration[items.webclient.language] = value;
            webClientConfigurationChanged = true;
        }
    };

    this.dateFormat = function () {
        return webClientConfiguration[items.webclient.dateFormat];
    };

    this.setDateFormat = function (value) {
        if (webClientConfiguration[items.webclient.dateFormat] != value) {
            webClientConfiguration[items.webclient.dateFormat] = value;
            webClientConfigurationChanged = true;
        }
    };

    this.refreshPage = function () {
        return parseBool(webClientConfiguration[items.webclient.refreshPage]);
    };

    this.setRefreshPage = function (active) {
        var currentlyActive = parseBool(webClientConfiguration[items.webclient.refreshPage]);
        if (active !== currentlyActive) {
            webClientConfiguration[items.webclient.refreshPage] = active ? true : false;
            webClientConfigurationChanged = true;
        }
        return this;
    };

    this.databaseVersion = function () {
        return databaseVersion;
    };

    this.advancedParametersActive = function () {
        return parseBool(webClientConfiguration[items.webclient.advancedParametersActive]);
    };

    this.setAdvancedParametersActive = function (active) {
        var currentlyActive = parseBool(webClientConfiguration[items.webclient.advancedParametersActive]);
        if (active !== currentlyActive) {
            webClientConfiguration[items.webclient.advancedParametersActive] = active ? true : false;
            webClientConfigurationChanged = true;
        }
        return this;
    };

    this.location = function () {
        return {
            latitude: serverConfiguration[items.server.locationSection][items.server.location.latitude],
            longitude: serverConfiguration[items.server.locationSection][items.server.location.longitude],
            altitude: serverConfiguration[items.server.locationSection][items.server.location.altitude],
            timezone: serverConfiguration[items.server.locationSection][items.server.location.timezone]
        };
    };

    this.setCurrentLocation = function (latitude, longitude, altitude, timezone) {
        if (isNullOrUndefined(latitude)) {
            latitude = serverConfiguration[items.server.locationSection][items.server.location.latitude];
        }
        if (isNullOrUndefined(longitude)) {
            longitude = serverConfiguration[items.server.locationSection][items.server.location.longitude];
        }
        if (isNullOrUndefined(altitude)) {
            altitude = serverConfiguration[items.server.locationSection][items.server.location.altitude];
        }
        if (isNullOrUndefined(timezone)) {
            timezone = serverConfiguration[items.server.locationSection][items.server.location.timezone];
        }


        if (serverConfiguration[items.server.locationSection][items.server.location.latitude] != latitude ||
            serverConfiguration[items.server.locationSection][items.server.location.longitude] != longitude ||
            serverConfiguration[items.server.locationSection][items.server.location.altitude] != altitude ||
            serverConfiguration[items.server.locationSection][items.server.location.timezone] != timezone) {

            serverConfiguration[items.server.locationSection][items.server.location.latitude] = latitude;
            serverConfiguration[items.server.locationSection][items.server.location.longitude] = longitude;
            serverConfiguration[items.server.locationSection][items.server.location.altitude] = altitude;
            serverConfiguration[items.server.locationSection][items.server.location.timezone] = timezone;
            serverConfigurationChanged = true;
        }
    };

    this.basicAuthentication = function () {
        return {
            active: parseBool(serverConfiguration[items.server.basicAuthenticationSection].active),
            user: serverConfiguration[items.server.basicAuthenticationSection].user,
            password: serverConfiguration[items.server.basicAuthenticationSection].password
        };
    };

    this.setBasicAuthentication = function (active, user, password) {
        var currentlyActive = parseBool(serverConfiguration[items.server.basicAuthenticationSection].active);

        if (currentlyActive != active ||
            serverConfiguration[items.server.basicAuthenticationSection][items.server.basicAuthentication.user] != user ||
            serverConfiguration[items.server.basicAuthenticationSection][items.server.basicAuthentication.password] != password) {

            serverConfiguration[items.server.basicAuthenticationSection].active = active ? true : false;
            serverConfiguration[items.server.basicAuthenticationSection].user = user;
            serverConfiguration[items.server.basicAuthenticationSection].password = password;
            serverConfigurationChanged = true;
        }
    };


    loadServerConfiguration = function () {
        var d = new $.Deferred();

        RestEngine.getJson("rest/configuration/server")
         .done(function (data) {
             serverConfiguration = data;
             d.resolve();
         })
         .fail(d.reject);

        return d.promise();
    }

    saveServerConfiguration = function () {
        var d = new $.Deferred();

        if (!serverConfigurationChanged) {
            d.resolve();
        } else {
            RestEngine.putJson("/rest/configuration/server", {
                    data: JSON.stringify(serverConfiguration)
                })
                .done(function () {
                    serverConfigurationChanged = false;
                    d.resolve();
                })
                .fail(d.reject);
        }

        return d.promise();
    }

    resetServerConfiguration = function () {
        var d = new $.Deferred();

        RestEngine.putJson("/rest/configuration/server/reset")
            .done(function (newConfiguration) {
                serverConfiguration = newConfiguration;
                serverConfigurationChanged = false;
                d.resolve();
            })
            .fail(function () {
                console.error("failed to reset server configuration");
                d.reject();
            })
        return d.promise();
    }


    loadDatabaseVersion = function () {
        var d = new $.Deferred();

        RestEngine.getJson("rest/configuration/databaseVersion")
            .done(function (data) {
                databaseVersion = data;
                d.resolve();
            })
            .fail(d.reject);

        return d.promise();
    }

    loadWebClientConfiguration = function () {
        var d = new $.Deferred();

        RestEngine.getJson("rest/configuration/external/webClient")
            .done(function (data) {
                loadedClientConfiguration = JSON.parse(data);

                // Merge loaded configuration with default one, in case of some new fields not in loaded configuration
                Object.assign(webClientConfiguration, defaultWebClientConfiguration);
                Object.assign(webClientConfiguration, loadedClientConfiguration);

                d.resolve();
            })
            .fail(function () {
                Object.assign(webClientConfiguration, defaultWebClientConfiguration);
                d.reject();
            });

        return d.promise();
    }

    saveWebClientConfiguration = function () {
        var d = new $.Deferred();

        if (!webClientConfigurationChanged) {
            d.resolve();
        } else {
            RestEngine.putJson("/rest/configuration/external/webClient", {
                    data: JSON.stringify(webClientConfiguration)
                })
                .done(function () {
                    webClientConfigurationChanged = false;
                    d.resolve();
                })
                .fail(d.reject);
        }

        return d.promise();
    }

    resetWebClientConfiguration = function () {
        var d = new $.Deferred();        
        
        Object.assign(webClientConfiguration, defaultWebClientConfiguration);
        webClientConfigurationChanged = true;
        saveWebClientConfiguration()
        .done(function(){
            webClientConfigurationChanged = false;
        })
        .always(d.resolve);

        return d.promise(); 
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
    items.webclient.dateFormat = "dateFormat";
    items.webclient.refreshPage = "refreshPage";
    // Associated default values
    const defaultWebClientConfiguration = {};
    defaultWebClientConfiguration[items.webclient.language] = getdefaultLanguageFromSupported();
    defaultWebClientConfiguration[items.webclient.advancedParametersActive] = false;
    defaultWebClientConfiguration[items.webclient.dateFormat] = "LLL";
    defaultWebClientConfiguration[items.webclient.refreshPage] = true;

    // Main configuration instances
    var serverConfiguration;
    var webClientConfiguration = {};
    var databaseVersion;
    var serverConfigurationChanged;
    var webClientConfigurationChanged;
}