const memory = new WebAssembly.Memory({ initial: 100, maximum: 100 });

const container = document.createElement('div');
container.style.position = 'absolute';

document.body.appendChild(container);

const controls_environment = initControls(memory, container);

function debugPrintFloat(value) {
    console.log(value);
}

fetch('./example.wasm')
    .then(response => {
        const environment = {
            memory,
            ...controls_environment,
            debug_print_float: debugPrintFloat
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