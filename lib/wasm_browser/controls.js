function initControls(memory, container) {
    var textDecoder = new TextDecoder('utf-8');

    function decodeString(data, length) {
        return textDecoder.decode(memory.buffer.slice(data, data + length));
    }

    let nextControlId = 1;
    const controls = {};

    const textWidthStupidity = document.createElement('span');
    textWidthStupidity.style.visibility = 'hidden';
    textWidthStupidity.style.position = 'absolute';

    container.appendChild(textWidthStupidity);

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

    function getLabelWidth(textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
        textWidthStupidity.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
        textWidthStupidity.style.fontSize = fontSize + 'px';

        textWidthStupidity.innerHTML = decodeString(textData, textLength);

        const width = textWidthStupidity.offsetWidth;

        return width;
    }

    return {
        clear_controls: clearControls,
        create_label: createLabel,
        get_label_width: getLabelWidth
    };
}