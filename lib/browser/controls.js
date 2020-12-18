function initControls(memoryBuffer, frame, buttonPressHandler, frameResizeHandler, textInputChangeHandler) {
    const textDecoder = new TextDecoder('utf-8');

    const textEncoder = new TextEncoder();

    const dataView = new DataView(memoryBuffer);

    function decodeString(data, length) {
        return textDecoder.decode(memoryBuffer.slice(data, data + length));
    }

    function encodeString(string, buffer) {
        new Uint8Array(memoryBuffer).set(textEncoder.encode(string), buffer);
    }

    let nextControlId = 1;
    const controls = {};

    const textSizeStupidity = document.createElement('span');

    textSizeStupidity.style.visibility = 'hidden';
    textSizeStupidity.style.position = 'absolute';

    textSizeStupidity.style.lineHeight = 1;

    frame.appendChild(textSizeStupidity);

    function getTextWidth(textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
        textSizeStupidity.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
        textSizeStupidity.style.fontSize = fontSize + 'px';

        textSizeStupidity.innerHTML = decodeString(textData, textLength);

        return textSizeStupidity.offsetWidth;
    }

    function getFrameSize(width, height) {
        dataView.setFloat32(width, frame.offsetWidth, true);
        dataView.setFloat32(height, frame.offsetHeight, true);
    }

    function clearControls() {
        for(const id in controls) {
            frame.removeChild(controls[id]);

            delete controls[id];
        }
    }

    function createLabel(x, y, textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
        const element = document.createElement('div');

        element.style.position = 'absolute';

        element.style.mozUserSelect = 'none';
        element.style.webkitUserSelect = 'none';
        element.style.msUserSelect = 'none';
        element.style.userSelect = 'none';

        element.style.lineHeight = 1;

        element.style.left = x + 'px';
        element.style.top = y + 'px';

        element.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
        element.style.fontSize = fontSize + 'px';

        element.innerHTML = decodeString(textData, textLength);

        frame.appendChild(element);

        const id = nextControlId;

        controls[id] = element;

        nextControlId += 1;

        return id;
    }

    function createButton(x, y, width, height, textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
        const element = document.createElement('button');

        element.style.position = 'absolute';

        element.style.mozUserSelect = 'none';
        element.style.webkitUserSelect = 'none';
        element.style.msUserSelect = 'none';
        element.style.userSelect = 'none';

        element.style.boxSizing = 'border-box';

        element.style.cursor = 'pointer';

        element.style.padding = '0px';

        element.style.left = x + 'px';
        element.style.top = y + 'px';

        element.style.width = width + 'px';
        element.style.height = height + 'px';

        element.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
        element.style.fontSize = fontSize + 'px';

        element.innerHTML = decodeString(textData, textLength);

        frame.appendChild(element);

        const id = nextControlId;

        element.addEventListener('click', () => buttonPressHandler(id));

        controls[id] = element;

        nextControlId += 1;

        return id;
    }

    function createTextInput(x, y, width, height, textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
        const element = document.createElement('input');

        element.type = 'text';
        element.value = decodeString(textData, textLength);

        element.style.position = 'absolute';

        element.style.boxSizing = 'border-box';

        element.style.cursor = 'text';

        element.style.padding = '0px';

        element.style.left = x + 'px';
        element.style.top = y + 'px';

        element.style.width = width + 'px';
        element.style.height = height + 'px';

        element.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
        element.style.fontSize = fontSize + 'px';

        frame.appendChild(element);

        const id = nextControlId;

        element.addEventListener('change', () => textInputChangeHandler(id));

        controls[id] = element;

        nextControlId += 1;

        return id;
    }

    function getTextInputText(textInputId, buffer, bufferSize) {
        const element = controls[textInputId];

        if(bufferSize < element.value.length) {
            return element.value.length;
        }

        encodeString(element.value, buffer);

        return element.value.length;
    }

    let lastFrameWidth = frame.offsetWidth;
    let lastFrameheight = frame.offsetHeight;

    function resizeProbe() {
        if(frame.offsetWidth != lastFrameWidth || frame.offsetHeight != lastFrameheight) {
            lastFrameWidth = frame.offsetWidth;
            lastFrameheight = frame.offsetHeight;

            frameResizeHandler();
        }

        requestAnimationFrame(resizeProbe);
    }

    requestAnimationFrame(resizeProbe);

    return {
        get_text_width: getTextWidth,
        get_frame_size: getFrameSize,
        clear_controls: clearControls,
        create_label: createLabel,
        create_button: createButton,
        create_text_input: createTextInput,
        get_text_input_text: getTextInputText
    };
}