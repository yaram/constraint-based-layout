const memory = new WebAssembly.Memory({ initial: 100, maximum: 100 });

const dataView = new DataView(memory.buffer);

var textDecoder = new TextDecoder('utf-8');

function decodeString(data, length) {
    return textDecoder.decode(memory.buffer.slice(data, data + length));
}

let nextControlId = 1;
const controls = {};

const container = document.createElement('div');
container.style.position = 'absolute';

document.body.appendChild(container);

const textWidthStupidity = document.createElement('span');
textWidthStupidity.style.visibility = 'hidden';
textWidthStupidity.style.position = 'absolute';

container.appendChild(textWidthStupidity);

function environmentPanic() {
    throw new Error('panic!');
}

function debugPrintFloat(value) {
    console.log(value);
}

function getTextWidth(textData, textLength, fontFamilyData, fontFamilyLength, fontSize) {
    textWidthStupidity.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
    textWidthStupidity.style.fontSize = fontSize;

    textWidthStupidity.innerHTML = decodeString(textData, textLength);

    const width = textWidthStupidity.offsetWidth;

    return width;
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

    element.style.left = x + 'px';
    element.style.top = y + 'px';

    element.style.fontFamily = decodeString(fontFamilyData, fontFamilyLength);
    element.style.fontSize = fontSize;

    element.innerHTML = decodeString(textData, textLength);

    container.appendChild(element);

    const id = nextControlId;

    controls[id] = element;

    nextControlId += 1;

    return id;
}

function setPosition(controlId, x, y) {
    const element = controls[controlId];

    if(element !== undefined) {
        element.style.left = x + 'px';
        element.style.top = y + 'px';
    }
}

fetch('./out.wasm')
    .then(response => {
        const environment = {
            memory,
            environment_panic: environmentPanic,
            debug_print_float: debugPrintFloat,
            get_text_width: getTextWidth,
            clear_controls: clearControls,
            create_label: createLabel,
            set_position: setPosition
        };

        return WebAssembly.instantiateStreaming(response, { env: environment });
    })
    .then(result => {
        const {
            init,
            update
        } = result.instance.exports;

        init();

        function frame() {
            update();

            requestAnimationFrame(frame);
        }

        requestAnimationFrame(frame);
    })
    .catch(err => {
        console.error(err);
    });