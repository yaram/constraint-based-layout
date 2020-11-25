const { spawnSync } = require('child_process');
const { existsSync, mkdirSync, copyFileSync } = require('fs');
const { join } = require('path');

let config = 'debug';

if(process.argv.length >= 3) {
    config = process.argv[2];
}

const scriptPath = __dirname;

const sourceDirectory = join(scriptPath, 'src');

const buildDirectory = join(scriptPath, 'build');

if(!existsSync(buildDirectory)) {
    mkdirSync(buildDirectory);
}

const publicDirectory = join(scriptPath, 'public');

if(!existsSync(publicDirectory)) {
    mkdirSync(publicDirectory);
}

{
    const { status, stderr } = spawnSync(
        'clang++',
        [
            '-c',
            `-O${config === 'debug' ? '0' : '2' }`,
            '--target=wasm32',
            `-I${join(scriptPath, 'thirdparty')}`,
            '-o', join(buildDirectory, 'main.o'),
            join(sourceDirectory, 'main.cpp')
        ]
    );

    if(status != 0) {
        console.error(stderr.toString('utf8'));

        process.exit(1);
    }
}

{
    const { status, stderr } = spawnSync(
        'wasm-ld',
        [
            '--no-entry',
            '--export-all',
            '--import-memory',
            '--allow-undefined',
            '-o', join(publicDirectory, 'out.wasm'),
            join(buildDirectory, 'main.o')
        ]
    );

    if(status != 0) {
        console.error(stderr.toString('utf8'));

        process.exit(1);
    }
}

const files_to_copy = [
    'index.html',
    'index.js'
];

files_to_copy.forEach(file => copyFileSync(join(sourceDirectory, file), join(publicDirectory, file)));