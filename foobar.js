const net = require('net');
const socket = new net.Socket();

const server = net.createServer((socket) => {
    socket.on('data', (data) => {
        if (data.toString() === 'ping') {
            console.log(`Received ping from client ${socket.remoteAddress}:${socket.remotePort}.`);
            console.log(`Sending pong to client ${socket.remoteAddress}:${socket.remotePort}.`);
            setTimeout(() => {
                socket.write('pong');
            }, process.env.WAIT);
        }
    });
    socket.on('error', (error) => {
        console.log('error', error);
    });
})
    .listen(process.env.LOCAL_PORT, '0.0.0.0', () => {
        console.log(`Listening on ${process.env.LOCAL_PORT}`);
    });

socket.connect(process.env.REMOTE_PORT, process.env.REMOTE_NAME, () => {
    socket.write('ping');

    socket.on('data', (data) => {
        console.log(`Received: ${data.toString()} from ${process.env.REMOTE_NAME}:${process.env.REMOTE_PORT}`);
        setTimeout(() => {
            console.log(`Sending ping to ${socket.remoteAddress}:${socket.remotePort}`)
            socket.write('ping');
        }, process.env.WAIT);
    });
    socket.on('error', (error) => {
        console.log('error', error);
    });
});
