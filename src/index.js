const memory = new WebAssembly.Memory({ initial: 100, maximum: 100 });

const dataView = new DataView(memory.buffer);

fetch('./out.wasm')
    .then(response => {
        const environment = {
            memory
        };

        return WebAssembly.instantiateStreaming(response, { env: environment });
    })
    .then(result => {
        const {
            test
        } = result.instance.exports;

        console.log(test());
    });