/** @module Recipient class */


/**
 * Creates an instance of Recipient
 * @param id The recipient ID
 * @param firstName the first name
 * @param lastName the last name
 * @param fields all additional fields (email, mobile phone,....)
 * @constructor
 */
function Recipient(id, firstName, lastName, fields) {
    assert(firstName !== undefined, "firstName of a recipient must be defined");
    assert(lastName !== undefined, "lastName of a recipient must be defined");

    this.id = id;
    this.firstName = firstName;
    this.lastName = lastName;
    this.fields = fields;
}

/**
 * Give a string representation of a recipient (concatenate first and last names)
 * @return {string}
 */
Recipient.prototype.toString = function() {
    return this.firstName + " " + this.lastName;
};

/**
 * Convert the recipient to JSON format
 * @return {{id: *, firstName: string, lastName: string, fields: *}}
 */
Recipient.prototype.toJSON = function () {
    return {
        id : this.id,
        firstName: this.firstName,
        lastName: this.lastName,
        fields : this.fields
    };
};


/**
 * Define the systemFields
 * @type {string[]}
 *
 * A simple international regex : the number begin by +, no space, no -, no ()
 */
Recipient.systemFields = {
    "email": "^[a-zA-Z0-9_.-]+@[a-zA-Z0-9-]+.[a-zA-Z0-9-.]+$",
    "mobile": "^\\+(?:[0-9]?){6,14}[0-9]$"
};


/**
 * Merge current recipient data with plugin fields
 * @param allPlugins
 */
Recipient.prototype.mergeFields = function() {
    var self = this;

    var d = $.Deferred();

    var definedFields = [];
    $.each(this.fields, function(index, f) {
        definedFields.push(f);
    });

    this.fields = [];
    //add system fields
    $.each(Recipient.systemFields, function(key, value) {
        var currentField = {
            fieldName: key,
            pluginType: "system",
            pluginDefaultDisplayName: "system",
            name: $.t("objects.recipient.fields." + key + ".name"),
            description: $.t("objects.recipient.fields." + key + ".description"),
            regexErrorMessage: $.t("objects.recipient.fields." + key + ".regexErrorMessage"),
            regex: value,
            value: ""
        };

        //search the field value in definedFields (search if the recipient is already configured for this field)
        $.each(definedFields, function(index, field) {
            if (field.pluginType.toLowerCase() == "system" && field.fieldName.toLowerCase() == key.toLowerCase()) {
                //the field match an already saved one, just reuse the value
                currentField.value = field.value;
            }
        });

        self.fields.push(currentField);
    });

    //add plugin fields from the cache array
    //we take only plugin that have at least one instance
    //we dynamically ask for plugins (async)
    PluginInstanceManager.getAll().done(function(allInstances) {
        var instanciatedTypes = [];
        $.each(allInstances, function(index, value) {
            if (!PluginInstanceManager.isSystemCategory(value))
                instanciatedTypes.push(value.type);
        });

        instanciatedTypes = removeDuplicates(instanciatedTypes);

        $.each(instanciatedTypes, function(key, value) {
            //for each recipient fields in the plugin
            var plugin = PluginManager.packageList[value];
            if (plugin) {
                var recipientFieldsFromPlugin = plugin.getRecipientFields();
                for (var recipientField in recipientFieldsFromPlugin) {
                    //make a copy of the object
                    var currentField = $.extend(true, {}, recipientFieldsFromPlugin[recipientField]);

                    //force the field value to empty string
                    currentField.value = currentField.defaultValue;

                    //search the field value in definedFields (search if the recipient is already configured for this field)
                    $.each(definedFields, function(index, field) {
                        if (field.pluginType.toUpperCase() === plugin.type.toUpperCase() && field.fieldName.toUpperCase() === recipientField.toUpperCase()) {
                            //the field match an already saved one, just reuse the value
                            currentField.value = field.value;
                        }
                    });

                    currentField.fieldName = recipientField;
                    currentField.pluginType = plugin.type;
                    currentField.pluginDefaultDisplayName = plugin.package.name;

                    self.fields.push(currentField);
                }
            }
        });
        d.resolve();
    });

    return d.promise();
}
