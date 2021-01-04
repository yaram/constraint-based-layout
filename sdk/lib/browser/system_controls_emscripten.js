function getTextWidth(...args) {
    return controlsEnvironment.get_text_width(...args);
}

function getFrameSize(...args) {
    return controlsEnvironment.get_frame_size(...args);
}

function clearControls(...args) {
    return controlsEnvironment.clear_controls(...args);
}

function createContainer(...args) {
    return controlsEnvironment.create_container(...args);
}

function createLabel(...args) {
    return controlsEnvironment.create_label(...args);
}

function createButton(...args) {
    return controlsEnvironment.create_button(...args);
}

function createTextInput(...args) {
    return controlsEnvironment.create_text_input(...args);
}

function getTextInputText(...args) {
    return controlsEnvironment.get_text_input_text(...args);
}

mergeInto(LibraryManager.library, {
    get_text_width: getTextWidth,
    get_frame_size: getFrameSize,
    clear_controls: clearControls,
    create_container: createContainer,
    create_label: createLabel,
    create_button: createButton,
    create_text_input: createTextInput,
    get_text_input_text: getTextInputText
});