const net = require('net');
const socket = new net.Socket();

const server = net.createServer((socket) => {
    socket.on('data', (data) => {
        if (data.toString() === 'ping') {
            console.log(`Server: Received ping from client ${socket.remoteAddress}:${socket.remotePort}.`);
            console.log(`Server: Sending pong to client ${socket.remoteAddress}:${socket.remotePort}.`);
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
        console.log(`Listening on 0.0.0.0:${process.env.LOCAL_PORT}`);
    });

// We need to wait a while for both servers to be up before trying to connect.
setTimeout(() => {
    socket.connect(process.env.REMOTE_PORT, process.env.REMOTE_NAME, () => {
        socket.write('ping');

        socket.on('data', (data) => {
            console.log(`Client: Received ${data.toString()} from ${process.env.REMOTE_NAME}:${process.env.REMOTE_PORT}`);
            setTimeout(() => {
                console.log(`Client: Sending ping to ${socket.remoteAddress}:${socket.remotePort}`)
                socket.write('ping');
            }, process.env.WAIT);
        });
        socket.on('error', (error) => {
            console.log('error', error);
        });
    });
}, process.env.WAIT);
