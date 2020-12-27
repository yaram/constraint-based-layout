function getTextWidth(textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
    return controlsEnvironment.get_text_width(textData, textLength, fontFamilyData, fontFamilyLength, fontSize);
}

function getFrameSize(width, height) {
    return controlsEnvironment.get_frame_size(width, height);
}

function clearControls() {
    return controlsEnvironment.clear_controls();
}

function createLabel(x, y, textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
    return controlsEnvironment.create_label(x, y, textData, textLength, fontFamilyData, fontFamilyLength, fontSize);
}

function createButton(x, y, width, height, textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
    return controlsEnvironment.create_button(x, y, width, height, textData, textLength, fontFamilyData, fontFamilyLength, fontSize);
}

function createTextInput(x, y, width, height, textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
    return controlsEnvironment.create_text_input(x, y, width, height, textData, textLength, fontFamilyData, fontFamilyLength, fontSize);
}

function getTextInputText(textInputId, buffer, bufferSize) {
    return controlsEnvironment.get_text_input_text(textInputId, buffer, bufferSize);
}

mergeInto(LibraryManager.library, {
    get_text_width: getTextWidth,
    get_frame_size: getFrameSize,
    clear_controls: clearControls,
    create_label: createLabel,
    create_button: createButton,
    create_text_input: createTextInput,
    get_text_input_text: getTextInputText
});