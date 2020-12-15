function initControls(memory, container) {
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

    function getLabelSize(textData, textLength, fontFamilyData, fontFamilyLength, fontSize, width, height) {
        textSizeStupidity.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
        textSizeStupidity.style.fontSize = fontSize + 'px';

        textSizeStupidity.innerHTML = decodeString(textData, textLength);

        dataView.setFloat32(width, textSizeStupidity.offsetWidth, true);
        dataView.setFloat32(height, textSizeStupidity.offsetHeight, true);
    }

    return {
        clear_controls: clearControls,
        create_label: createLabel,
        get_label_size: getLabelSize
    };
}