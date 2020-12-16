const memory = new WebAssembly.Memory({ initial: 100, maximum: 100 });

const container = document.createElement('div');
container.style.position = 'absolute';

document.body.appendChild(container);

let buttonPressHandler;

const controlsEnvironment = initControls(memory, container, id => buttonPressHandler(id));

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
            button_press_handler
        } = result.instance.exports;

        buttonPressHandler = button_press_handler;

        init();
    })
    .catch(err => {
        console.error(err);
    });