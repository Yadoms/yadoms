/** @module Device class */

/**
 * Creates an instance of Keyword
 * @constructor
 */
function Keyword(id, deviceId, capacityName, accessMode, name, friendlyName, type, units, details, typeInfo, blacklist) {
   assert(!isNullOrUndefined(id), "id of a Keyword must be defined");
   assert(!isNullOrUndefined(deviceId), "deviceId of a Keyword must be defined");
   assert(!isNullOrUndefined(capacityName), "capacityName of a Keyword must be defined");
   assert(!isNullOrUndefined(accessMode), "accessMode of a Keyword must be defined");
   assert(!isNullOrUndefined(name), "name of a Keyword must be defined");
   assert(!isNullOrUndefined(friendlyName), "friendlyName of a Keyword must be defined");
   assert(!isNullOrUndefined(type), "type of a Keyword must be defined");
   assert(!isNullOrUndefined(units), "units of a Keyword must be defined");
   assert(!isNullOrUndefined(blacklist), "blacklist of a Keyword must be defined");

   this.id = id;
   this.deviceId = deviceId;
   this.capacityName = capacityName;
   this.accessMode = accessMode;
   this.name = name;
   this.friendlyName = friendlyName;
   this.type = type;
   this.units = units;
   this.details = details;
   this.typeInfo = typeInfo;
   this.blacklist = blacklist;
}

Keyword.prototype.toJSON = function () {
   return {
      id: this.id,
      deviceId: this.deviceId,
      capacityName: this.capacityName,
      accessMode: this.accessMode,
      name: this.name,
      friendlyName: this.friendlyName,
      type: this.type,
      units: this.units,
      details: this.details,
      typeInfo : this.typeInfo,
	  blacklist : this.blacklist
   };
};

/**
 * Check if the keyword is writable
 * @returns {boolean}
 */
Keyword.prototype.isWritable = function() {
  return this.accessMode.toLowerCase() === "getset";
};