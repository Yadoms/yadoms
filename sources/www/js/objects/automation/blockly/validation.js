


/**
 *
 *
 *
 *  Validation
 *
 *
 *  This feature check is a workspace is completely filled
 *
 *
 *
 */


/**
 * Create the validation subclass
 * @constructor
 */
Blockly.Validation = function () {
};

/**
 * Define the currently list of "error validated" blocks
 * @type {Array}
 */
Blockly.Validation.listErroneousBlocks = [];


/**
 * Define a block as invalid
 * @param block The block to invalidate
 */
Blockly.Validation.setBlockInvalid = function (block) {
    if (block != null && $.isFunction(block.addError)) {
        block.addError();
        Blockly.Validation.listErroneousBlocks.push(block);
    }
};

/**
 * Clear the invalidated blocks list
 */
Blockly.Validation.clearInvalidBlocks = function () {
    $.each(Blockly.Validation.listErroneousBlocks, function (index, value) {
        if (value != null && $.isFunction(value.removeError)) {
            value.removeError();
        }
    });

    //clear the list
    Blockly.Validation.listErroneousBlocks.splice(0, Blockly.Validation.listErroneousBlocks.length);
};

/**
 * Check if a block is valid (all visible inputs are filled)
 * @param block The block to check
 * @returns {boolean} true if block is valid, false if invalid
 */
Blockly.Validation.isBlockValid = function (block) {

    if (block != null) {
        var currentBlockValid = true;

        if ($.isFunction(block.isValid)) {
            currentBlockValid = block.isValid();
            if (!currentBlockValid) {
                Blockly.Validation.setBlockInvalid(block);
            }
        }

         if (currentBlockValid) {
				
		    //check for any empty input
		    $.each(block.inputList, function(index, subBlock) {
			   if (subBlock.connection != null && subBlock.isVisible()) {
			      if ((subBlock.connection.targetConnection == null || subBlock.connection.targetConnection == undefined) && subBlock.type === Blockly.INPUT_VALUE) {
				     Blockly.Validation.setBlockInvalid(subBlock.sourceBlock_);
				     currentBlockValid = false;
			      }
			   }
		    });		  
			 
			 
			//check that all childs are valid
			$.each(block.getChildren(), function(index, subBlock) {
				if(!Blockly.Validation.isBlockValid(subBlock))
					currentBlockValid = false;
            });
        }
		
        return currentBlockValid;
    }
    return false;
};


/**
 * Add the error shpae to the block
 */
Blockly.BlockSvg.prototype.addError = function () {
    Blockly.addClass_(/** @type {!Element} */(this.svgGroup_),
        "blocklySelectedError");
    // Move the selected block to the top of the stack.
    if (!isNullOrUndefined(this.svgGroup_.parentNode))
        this.svgGroup_.parentNode.appendChild(this.svgGroup_);
};

/**
 * Remove the error shape of a block
 */
Blockly.BlockSvg.prototype.removeError = function () {
    Blockly.removeClass_(/** @type {!Element} */(this.svgGroup_),
        "blocklySelectedError");
};

/**
 * Initialize the workspace to allow validation
 * @param workspace The workspace to configure
 * @param maxTopBlocks The maximum number of topblocks allowed
 * @constructor
 */
Blockly.Validation.Init = function (workspace, maxTopBlocks) {
    if (maxTopBlocks != undefined && $.isNumeric(maxTopBlocks))
        Blockly.Validation.maxTopBlocks_ = maxTopBlocks;

    workspace.addChangeListener(function () {
        Blockly.Validation.clearInvalidBlocks();
    });
};

/**
 * The number of allowed top blocks
 * @type {number}
 * @private
 */
Blockly.Validation.maxTopBlocks_ = 1;


/**
 * Get the real topBlocks (without functions)
 * @param workspace The workspace 
 * @return The topBlock list (without functions)
 */
Blockly.Validation.getRealTopBlocks_ = function (workspace) {
    var result = [];
    $.each(workspace.topBlocks_, function (index, value) {
        if (value.type !== "procedures_defnoreturn" && value.type !== "procedures_defreturn")
            result.push(value);
    });
    return result;
};

/**
 * Start a workspace validation
 * @param workspace The workspace to validate
 * @param callback The callback for the validation result
 */
Blockly.Validation.validate = function (workspace, callback) {

    var topBlocks = Blockly.Validation.getRealTopBlocks_(workspace);

    if (topBlocks.length > Blockly.Validation.maxTopBlocks_) {

        $.each(topBlocks, function (index, value) {
            Blockly.Validation.setBlockInvalid(value);
        });
        callback(false, "Only " + Blockly.Validation.maxTopBlocks_ + " max top block is allowed");
    }
    else {
        var result = true;
        $.each(workspace.topBlocks_, function (index, topBlock) {
            if (!Blockly.Validation.isBlockValid(topBlock))
                result = false;
        });

        if (result)
            callback(true, "Workspace is valid");
        else
            callback(false, "Workspace is not valid");
    }
};

/**
 * Start the mainWorkspace validation
 * @param callback The callback for the validation result
 */
Blockly.Validation.validateMainWorkspace = function (callback) {
    Blockly.Validation.validate(Blockly.mainWorkspace, callback);
};

