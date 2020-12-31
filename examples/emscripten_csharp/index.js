Module.onRuntimeInitialized = () => {
    const frame = document.createElement('div');
    frame.style.position = 'absolute';
    frame.style.overflow = 'hidden';
    frame.style.width = '100%';
    frame.style.height = '100%';

    document.body.appendChild(frame);

    window.controlsEnvironment = initControls(
        () => Module.HEAPU8.buffer,
        frame,
        id => Module.ccall('button_press_handler', 'void', ['i32'], [id]),
        () => Module.ccall('frame_resize_handler', 'void', [], []),
        id => Module.ccall('text_input_change_handler', 'void', ['i32'], [id])
    );
};