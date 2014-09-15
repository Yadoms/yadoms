/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function DeviceManager(){}

DeviceManager.factory = function(json) {
   assert(!isNullOrUndefined(json), "json must be defined");
   assert(!isNullOrUndefined(json.id), "json.id must be defined");
   assert(!isNullOrUndefined(json.pluginId), "json.pluginId must be defined");
   assert(!isNullOrUndefined(json.name), "json.name must be defined");
   assert(!isNullOrUndefined(json.friendlyName), "json.friendlyName must be defined");
   assert(!isNullOrUndefined(json.model), "json.model must be defined");

   return new Device(json.id, json.pluginId, decodeURIComponent(json.name), decodeURIComponent(json.friendlyName), json.model);
};

DeviceManager.get = function (deviceId, callback) {
   assert(!isNullOrUndefined(deviceId), "deviceId must be defined");
   assert($.isFunction(callback), "callback must be a function");

   $.getJSON("rest/device/" + deviceId)
      .done(function( data ) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.generic.errorGetting", {objectName : "Device with Id = " + deviceId}), JSON.stringify(data));
            return;
         }
         callback(DeviceManager.factory(data.data));
      })
      .fail(function() {notifyError($.t("switch:errorDuringGettingDeviceInformation"));});
}

DeviceManager.getAttachedPlugin = function(device, callback) {
   assert(!isNullOrUndefined(device), "device must be defined");
   //we ask for the status of current device
   $.getJSON("/rest/plugin/" + device.pluginId)
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.deviceManager.errorGettingAttachedPlugin", {deviceName : device.friendlyName}), JSON.stringify(data));
            return;
         }

         //we save the attachedPlugin into the object
         device.attachedPlugin = PluginInstanceManager.factory(data.data);
         
         if ($.isFunction(callback))
            callback();

      })
      .fail(function() { notifyError($.t("objects.deviceManager.errorGettingAttachedPlugin", {deviceName : device.friendlyName})); });
};

DeviceManager.getKeywords = function(device, callback) {
   assert(!isNullOrUndefined(device), "device must be defined");

   if (isNullOrUndefined(device.keywords)) {
      //we ask for the keywords of current device
      $.getJSON("/rest/device/" + device.id + "/keyword")
         .done(function(data) {
            //we parse the json answer
            if (data.result != "true")
            {
               notifyError($.t("objects.deviceManager.errorGettingDeviceDetails", {deviceName : device.friendlyName}), JSON.stringify(data));
               return;
            }

            //we save the keyword list into the object
            device.keywords = [];
            $.each(data.data.keyword, function (index, value) {
               device.keywords.push(KeywordManager.factory(value));
            });

            if ($.isFunction(callback))
               callback();
         })
         .fail(function() { notifyError($.t("objects.deviceManager.errorGettingDeviceDetails", {deviceName : device.friendlyName})); });
   }
   else {
      //keyword has already been gotten
      callback();
   }
};

DeviceManager.deleteFromServer = function(device, callback) {
   assert(!isNullOrUndefined(device), "device must be defined");
   $.ajax({
      type: "DELETE",
      url: "/rest/device/" + device.id,
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.deviceManager.errorDeleting", {deviceName : device.friendlyName}), JSON.stringify(data));
            return;
         }

         if ($.isFunction(callback))
            callback();
      })
      .fail(function() {notifyError($.t("objects.deviceManager.errorDeleting", {deviceName : device.friendlyName}));});
};

DeviceManager.createVirtual = function(device, callback) {
   assert(!isNullOrUndefined(device), "device must be defined");
   $.ajax({
      type: "POST",
      url: "/rest/device",
      data: JSON.stringify({ pluginId: device.pluginId, name: device.name, friendlyName: device.friendlyName, model: device.model}),
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.deviceManager.errorCreating", {deviceName : device.friendlyName}), JSON.stringify(data));
            //launch callback with false as ko result
            if ($.isFunction(callback))
               callback(false);
            return;
         }

         //we update our information from the server
         device.id = data.data.id;
         device.pluginId = data.data.pluginId;
         device.name = data.data.name;
         device.friendlyName = data.data.friendlyName;
         device.model = data.data.model;

         if ($.isFunction(callback))
            callback(true);
      })
      .fail(function() {
         notifyError($.t("objects.deviceManager.errorCreating", {deviceName : device.friendlyName}));
         //launch callback with false as ko result
         if ($.isFunction(callback))
            callback(false);
      });
};

DeviceManager.updateToServer = function(device, callback) {
   assert(!isNullOrUndefined(device), "device must be defined");
   $.ajax({
      type: "PUT",
      url: "/rest/device/" + device.id,
      data: JSON.stringify(device),
      contentType: "application/json; charset=utf-8",
      dataType: "json"
   })
      .done(function(data) {
         //we parse the json answer
         if (data.result != "true")
         {
            notifyError($.t("objects.deviceManager.errorUpdating", {deviceName : device.friendlyName}), JSON.stringify(data));
            //launch callback with false as ko result
            if ($.isFunction(callback))
               callback(false);
            return;
         }
         //it's okay
         //we update our information from the server
         device = DeviceManager.factory(data.data);

         //we call the callback with true as a ok result
         if ($.isFunction(callback))
            callback(true);
      })
      .fail(function() {
         notifyError($.t("objects.deviceManager.errorUpdating", {deviceName : device.friendlyName}));
         //launch callback with false as ko result
         if ($.isFunction(callback))
            callback(false);
      });
};