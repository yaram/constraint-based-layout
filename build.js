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

const units = [
    { source: 'main.cpp', object: 'main.o' },
    { source: 'constraint_arithmetic.cpp', object: 'constraint_arithmetic.o' },
    { source: 'solver.cpp', object: 'solver.o' },
];

for(const unit of units) {
    const { status, stderr } = spawnSync(
        'clang++',
        [
            '-c',
            `-O${config === 'debug' ? '0' : '2' }`,
            '--target=wasm32',
            ...config === 'debug' ? ['-g'] : [],
            `-I${join(scriptPath, 'thirdparty')}`,
            '-o', join(buildDirectory, unit.object),
            join(sourceDirectory, unit.source)
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
            ...units.map(unit => join(buildDirectory, unit.object))
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