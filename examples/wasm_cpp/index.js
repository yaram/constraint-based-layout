const memory = new WebAssembly.Memory({ initial: 100, maximum: 100 });

const frame = document.createElement('div');
frame.style.position = 'absolute';
frame.style.width = '100%';
frame.style.height = '100%';

document.body.appendChild(frame);

let buttonPressHandler;
let frameResizeHandler;
let textInputChangeHandler;

const controlsEnvironment = initControls(
    memory,
    frame,
    id => buttonPressHandler(id),
    () => frameResizeHandler(),
    id => textInputChangeHandler(id)
);

function debugPrintFloat(value) {
    console.log(value);
}

fetch('./example.wasm')
    .then(response => {
        const environment = {
            memory,
            ...controlsEnvironment,
            debug_print_float: debugPrintFloat
        };

        return WebAssembly.instantiateStreaming(response, { env: environment });
    })
    .then(result => {
        const {
            init,
            button_press_handler,
            frame_resize_handler,
            text_input_change_handler
        } = result.instance.exports;

        buttonPressHandler = button_press_handler;
        frameResizeHandler = frame_resize_handler;
        textInputChangeHandler = text_input_change_handler;

        init();
    })
    .catch(err => {
        console.error(err);
    });