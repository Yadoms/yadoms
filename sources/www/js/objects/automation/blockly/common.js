
/**
 * Hardcoded interpreter name
 * @type {string}
 */
var BlocklyName = "blockly";

/**
 * Initialize Blockly.Yadoms structure
 * @constructor
 */
Blockly.Yadoms = function () {
};

/**
 * Common HSV hue for all "Yadoms" blocks.
 */
Blockly.Yadoms.blockColour = { HUE : 20 }


/**
 * The active blockly workspace
 */
Blockly.Yadoms.CurrentWorkspace = null;

/**
 * List of operators for numbers
 * @type {*[]}
 * @private
 */
Blockly.Yadoms.NumberOperators_ = Blockly.RTL ? [
    ["=", "EQ"],
    ["\u2260", "NEQ"],
    [">", "LT"],
    ["\u2265", "LTE"],
    ["<", "GT"],
    ["\u2264", "GTE"]
] : [
    ["=", "EQ"],
    ["\u2260", "NEQ"],
    ["<", "LT"],
    ["\u2264", "LTE"],
    [">", "GT"],
    ["\u2265", "GTE"]
];

/**
 * List of operators for strings
 * @type {*[]}
 * @private
 */
Blockly.Yadoms.StringOperators_ = Blockly.RTL ? [
    ["=", "EQ"],
    ["\u2260", "NEQ"],
    [">", "LT"],
    ["\u2265", "LTE"],
    ["<", "GT"],
    ["\u2264", "GTE"]
] : [
    ["=", "EQ"],
    ["\u2260", "NEQ"],
    ["<", "LT"],
    ["\u2264", "LTE"],
    [">", "GT"],
    ["\u2265", "GTE"]
];

/**
 * List of operators for booleans
 * @type {*[]}
 * @private
 */
Blockly.Yadoms.BooleanOperators_ = [
    ["=", "EQ"],
    ["\u2260", "NEQ"]
];


/**
 * Load the blockly language file
 * @param callback The callback used when load is done
 * @constructor
 * @private
 */
Blockly.Yadoms.LoadLanguageScript_ = function (callback) {

    //first try to load user specific language
    var currentLng = i18n.options.lng.substring(0, 2);

    $.getScript("libs/blockly/js/locales/" + currentLng + ".js")
        .done(function () {
            callback();
        })
        .fail(function () {
            console.warning("Fail to load Blockly for " + currentLng);
            //if it fails, load english one
            $.getScript("libs/blockly/js/locales/en.js")
                .done(function () {
                    callback();
                }.fail(function () {
                    callback();
                }));
        });
};

/**
 * Initialize blockly for Yadoms
 * @param $domTarget The div in which inject Blockly
 * @param initialContent The initial blockly content (XML)
 * @param maxTopBlocks The maximum number of TopBlocks
 * @constructor
 */
Blockly.Yadoms.Initialize = function ($domTarget, initialContent, maxTopBlocks) {
    Blockly.Yadoms.LoadDataForBlocklyCustomBlocks_()
    .done(function(data) {
        Blockly.Yadoms.data = data;
        Blockly.Python.INDENT = "\t";

        Blockly.Yadoms.EnumerationsHelper.createBlocks();

        $domTarget.append("<div class=\"blockly-container\"></div>");

        Blockly.Yadoms.LoadLanguageScript_(function() {
            //inject blockly dom+js
            var blocklyArea = $("#automation-rule-editor")[0];
            var blocklyDiv = $domTarget.find("div.blockly-container")[0];
            Blockly.Yadoms.CurrentWorkspace = Blockly.inject(blocklyDiv,
            {
                comments: true,
                disable: true,
                collapse: true,
                grid: {
                    spacing: 25,
                    length: 3,
                    colour: "#ccc",
                    snap: true
                },
                readOnly: false,
                realtime: false,
                scrollbars: true,
                toolbox: Blockly.Yadoms.CreateToolbox_(),
                zoom:
                {
                    enabled: true,
                    controls: true,
                    wheel: true,
                    maxScale: 2,
                    minScale: .1,
                    scaleSpeed: 1.1
                }
            });

            //load initial content if exists
            if (!isNullOrUndefinedOrEmpty(initialContent)) {
                var xml = Blockly.Xml.textToDom(initialContent);
                Blockly.Xml.domToWorkspace(Blockly.Yadoms.CurrentWorkspace, xml);
            }

            //initialize validation
            Blockly.Validation.Init(Blockly.Yadoms.CurrentWorkspace, maxTopBlocks);
        });
    });
};




/**
 * Function which convert a yadoms type, to a blockly type
 * @param yadomsKeywordType  The yadoms type
 * @returns {*} The blockly type matching yadoms type
 * @constructor
 * @param yadomsTypeName The yadoms type name. In case of enum, this is the internal enum name
 */
Blockly.Yadoms.GetBlocklyType_ = function (yadomsKeywordType, yadomsTypeName) {
    switch (yadomsKeywordType.toLowerCase()) {
        case "numeric":
            return "Number";
        case "string":
        case "json":
            return "String";
        case "bool":
            return "Boolean";
        case "datetime":
            return "datetime";
        case "enum":
            return "enum_" + yadomsTypeName;
        default:
            return null; //is not known allow any type
    }
};


/**
 * Function which convert a yadoms type, to a blockly type
 * @param keyword  The keyword
 * @returns {*} The blockly type matching yadoms type
 * @constructor
 * @param yadomsTypeName The yadoms type name. In case of enum, this is the internal enum name
 */
Blockly.Yadoms.GetDefaultBlock_ = function (keyword, workspace) {
	var createdBlock = null;
	
    switch (keyword.type.toLowerCase()) {
        case "numeric":
			createdBlock = Blockly.Block.obtain(workspace, 'math_number');
			//childBlock.setFieldValue('NUM', 42);
			break;
        case "string":
			createdBlock = Blockly.Block.obtain(workspace, 'text');
			createdBlock.setFieldValue('Hello', 'TEXT');		
			break;

        case "bool":
			createdBlock = Blockly.Block.obtain(workspace, 'logic_boolean');
            break;
			
        case "enum":
			createdBlock = Blockly.Block.obtain(workspace, 'yadoms_enumeration');
			var device = Blockly.Yadoms.data.devices[keyword.deviceId];
			var plugin = Blockly.Yadoms.data.plugins[device.pluginId];
			createdBlock.updateEnumeration(keyword.id, device.id, plugin.id);
		    break
			
        case "datetime":
			createdBlock = Blockly.Block.obtain(workspace, 'yadoms_date_datetime');
            break;

        default:
            break; //all other blocks do nothing
    }
	return createdBlock;
};



/**
 * Update a block colour depending on its type
 * @param self The block
 * @param type The block type
 * @constructor
 */
Blockly.Yadoms.UpdateBlockColour_ = function (self, type) {
    if (type === "String")
        self.setColour(Blockly.Blocks.texts.HUE);
    else if (type === "Boolean")
        self.setColour(Blockly.Blocks.logic.HUE);
    else if (type === "Number")
        self.setColour(Blockly.Blocks.math.HUE);
    else if (type === "datetime")
        self.setColour(Blockly.Yadoms.Dates.datetime.HUE);
    else //if (type === "null" || type == null)
        self.setColour(Blockly.Blocks.math.HUE);
};


/**
 * Dropdown refresh method
 * The function update the dropdown values, select the first one and call the change handler
 * -> Main blockly hack (not using only public memebers)
 * @param data The new dropdown values
 * @param tryToSelectValue The value of new values to select
 */
Blockly.FieldDropdown.prototype.refresh = function (data, tryToSelectValue) {
    if (data != undefined && data.length > 0) {
        this.menuGenerator_ = data;

        var valueFound = false;
        $.each(data, function (index, val) {
            if (val[1] === tryToSelectValue)
                valueFound = true;
        });

        if (isNullOrUndefined(tryToSelectValue) || !valueFound)
            this.setValue(data[0][1]);
        else
            this.setValue(tryToSelectValue);
    }
};


/**
 * Get the blockly result (xml, python)
 * @param language Output langugae (python, dart, js, ...)
 * @param callback A callback to receive results (xml, python)
 * @constructor
 */
Blockly.Yadoms.GetResult = function (language, callback) {
    assert(!isNullOrUndefined(callback), "callback must be defined");

    //get xml code
    var xmlDomString = Blockly.Xml.workspaceToDom(Blockly.Yadoms.CurrentWorkspace);
    var xmlString = Blockly.Xml.domToText(xmlDomString);

    var generatedCode = "";
    switch(language) {
        case "python":
            generatedCode = Blockly.Yadoms.GetPythonOutput_();
            break;

        default:
            console.error("Unsupported generation language : " + language);
            break;
    }
    callback(xmlString, generatedCode);
};
