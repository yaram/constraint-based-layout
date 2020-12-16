function initControls(memory, container, buttonPressHandler) {
    const textDecoder = new TextDecoder('utf-8');

    const dataView = new DataView(memory.buffer);

    function decodeString(data, length) {
        return textDecoder.decode(memory.buffer.slice(data, data + length));
    }

    let nextControlId = 1;
    const controls = {};

    const textSizeStupidity = document.createElement('span');
    textSizeStupidity.style.visibility = 'hidden';
    textSizeStupidity.style.position = 'absolute';

    container.appendChild(textSizeStupidity);

    function getTextSize(textData, textLength, fontFamilyData, fontFamilyLength, fontSize, width, height) {
        textSizeStupidity.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
        textSizeStupidity.style.fontSize = fontSize + 'px';

        textSizeStupidity.innerHTML = decodeString(textData, textLength);

        dataView.setFloat32(width, textSizeStupidity.offsetWidth, true);
        dataView.setFloat32(height, textSizeStupidity.offsetHeight, true);
    }

    function clearControls() {
        for(const id in controls) {
            container.removeChild(controls[id]);

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

        element.style.left = x + 'px';
        element.style.top = y + 'px';

        element.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
        element.style.fontSize = fontSize + 'px';

        element.innerHTML = decodeString(textData, textLength);

        container.appendChild(element);

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

        element.style.cursor = 'pointer';

        element.style.padding = '0px';

        element.style.left = x + 'px';
        element.style.top = y + 'px';

        element.style.width = width + 'px';
        element.style.height = height + 'px';

        element.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
        element.style.fontSize = fontSize + 'px';

        element.innerHTML = decodeString(textData, textLength);

        container.appendChild(element);

        const id = nextControlId;

        element.addEventListener('click', () => buttonPressHandler(id));

        controls[id] = element;

        nextControlId += 1;

        return id;
    }

    return {
        get_text_size: getTextSize,
        clear_controls: clearControls,
        create_label: createLabel,
        create_button: createButton
    };
}