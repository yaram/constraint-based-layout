const memory = new WebAssembly.Memory({ initial: 100, maximum: 100 });

const dataView = new DataView(memory.buffer);

var textDecoder = new TextDecoder('utf-8');

function decodeString(data, length) {
    return textDecoder.decode(memory.buffer.slice(data, data + length));
}

let nextControlId = 1;
const controls = {};

function environmentPanic() {
    throw new Error('panic!');
}

function debugPrintFloat(value) {
    console.log(value);
}

function getTextWidth(textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
    const tempElement = document.createElement('span');

    tempElement.style.visibility = 'hidden';
    tempElement.style.position = 'absolute';

    tempElement.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
    tempElement.style.fontSize = fontSize;

    tempElement.innerHTML = decodeString(textData, textLength);

    document.body.appendChild(tempElement);

    const width = tempElement.offsetWidth;

    document.body.removeChild(tempElement);

    return width;
}

function createLabel(x, y, textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
    const element = document.createElement('div');
    element.style.position = 'absolute';

    element.style.left = x + "px";
    element.style.top = y + "px";

    element.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
    element.style.fontSize = fontSize;

    element.innerHTML = decodeString(textData, textLength);

    document.body.appendChild(element);

    const id = nextControlId;

    controls[id] = element;

    nextControlId += 1;

    return id;
}

function setPosition(controlId, x, y) {
    const element = controls[controlId];

    if(element !== undefined) {
        element.style.left = x + "px";
        element.style.top = y + "px";
    }
}

fetch('./out.wasm')
    .then(response => {
        const environment = {
            memory,
            environment_panic: environmentPanic,
            debug_print_float: debugPrintFloat,
            get_text_width: getTextWidth,
            create_label: createLabel,
            set_position: setPosition
        };

        return WebAssembly.instantiateStreaming(response, { env: environment });
    })
    .then(result => {
        const {
            init
        } = result.instance.exports;

        init();
    })
    .catch(err => {
        console.error(err);
    });