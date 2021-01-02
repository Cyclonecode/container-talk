const net = require('net');

const server = net.createServer((socket) => {
    socket.on('data', (data) => {
        console.log('data', data);
        socket.write('hello');
        socket.end();
        socket.destroy();
    });
    socket.on('close', (data) => {
        console.log('close', data);
    });
})
    .listen(2050, () => {
        console.log('listening on 2050');
    })
