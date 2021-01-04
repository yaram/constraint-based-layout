function initControls(memoryBufferGetter, frame, buttonPressHandler, frameResizeHandler, textInputChangeHandler) {
    const textDecoder = new TextDecoder('utf-8');

    const textEncoder = new TextEncoder();

    function decodeString(data, length) {
        return textDecoder.decode(memoryBufferGetter().slice(data, data + length));
    }

    function encodeString(string, buffer) {
        new Uint8Array(memoryBufferGetter()).set(textEncoder.encode(string), buffer);
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
        const dataView = new DataView(memoryBufferGetter());

        dataView.setFloat32(width, frame.offsetWidth, true);
        dataView.setFloat32(height, frame.offsetHeight, true);
    }

    function packedColorToCSS(packedColor) {
        return `rgba(${packedColor & 0xFF}, ${packedColor >> 8 & 0xFF}, ${packedColor >> 16 & 0xFF}, ${(packedColor >> 24 & 0xFF) / 0xFF * 1})`;
    }

    function clearControls(backgroundColor) {
        for(const id in controls) {
            controls[id].parentNode.removeChild(controls[id]);

            delete controls[id];
        }

        frame.style.backgroundColor = packedColorToCSS(backgroundColor);
    }

    function createContainer(
        parent,
        x,
        y,
        width,
        height,
        backgroundColor,
        borderSize,
        borderColor
    ) {
        const element = document.createElement('div');

        element.style.position = 'absolute';

        element.style.boxSizing = 'border-box';

        element.style.left = x + 'px';
        element.style.top = y + 'px';

        element.style.width = width + 'px';
        element.style.height = height + 'px';

        element.style.backgroundColor = packedColorToCSS(backgroundColor);

        element.style.borderStyle = 'solid';
        element.style.borderWidth = borderSize + 'px';
        element.style.borderColor = packedColorToCSS(borderColor);

        if(parent != 0) {
            controls[parent].appendChild(element);
        } else {
            frame.appendChild(element);
        }

        const id = nextControlId;

        controls[id] = element;

        nextControlId += 1;

        return id;
    }

    function createLabel(
        container,
        x,
        y,
        textData,
        textLength,
        fontFamilyData,
        fontFamilyLength,
        fontSize,
        textColor
    ) {
        const element = document.createElement('div');

        element.style.position = 'absolute';

        element.style.whiteSpace = 'nowrap';

        element.style.mozUserSelect = 'none';
        element.style.webkitUserSelect = 'none';
        element.style.msUserSelect = 'none';
        element.style.userSelect = 'none';

        element.style.lineHeight = 1;

        element.style.left = x + 'px';
        element.style.top = y + 'px';

        element.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
        element.style.fontSize = fontSize + 'px';

        element.style.color = packedColorToCSS(textColor);

        element.appendChild(document.createTextNode(decodeString(textData, textLength)));

        if(container != 0) {
            controls[container].appendChild(element);
        } else {
            frame.appendChild(element);
        }

        const id = nextControlId;

        controls[id] = element;

        nextControlId += 1;

        return id;
    }

    function createButton(
        container,
        x,
        y,
        width,
        height,
        textData,
        textLength,
        fontFamilyData,
        fontFamilyLength,
        fontSize,
        textColor,
        backgroundColor,
        borderSize,
        borderColor
    ) {
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

        element.style.color = packedColorToCSS(textColor);

        element.style.backgroundColor = packedColorToCSS(backgroundColor);

        element.style.borderStyle = 'solid';
        element.style.borderWidth = borderSize + 'px';
        element.style.borderColor = packedColorToCSS(borderColor);

        element.appendChild(document.createTextNode(decodeString(textData, textLength)));

        if(container != 0) {
            controls[container].appendChild(element);
        } else {
            frame.appendChild(element);
        }

        const id = nextControlId;

        element.addEventListener('click', () => buttonPressHandler(id));

        controls[id] = element;

        nextControlId += 1;

        return id;
    }

    function createTextInput(
        container,
        x,
        y,
        width,
        height,
        textData,
        textLength,
        fontFamilyData,
        fontFamilyLength,
        fontSize,
        textColor,
        backgroundColor,
        borderSize,
        borderColor
    ) {
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

        element.style.color = packedColorToCSS(textColor);

        element.style.backgroundColor = packedColorToCSS(backgroundColor);

        element.style.borderStyle = 'solid';
        element.style.borderWidth = borderSize + 'px';
        element.style.borderColor = packedColorToCSS(borderColor);

        if(container != 0) {
            controls[container].appendChild(element);
        } else {
            frame.appendChild(element);
        }

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
        create_container: createContainer,
        create_label: createLabel,
        create_button: createButton,
        create_text_input: createTextInput,
        get_text_input_text: getTextInputText
    };
}