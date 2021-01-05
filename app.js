const chalk = require('chalk');
const net = require('net');
const eth0 = require('os').networkInterfaces()['eth0'][0];
const socket = new net.Socket();

net.createServer((socket) => {
    socket.on('data', (data) => {
        if (data.toString() === 'ping') {
            console.log(chalk.green(`Server: Received ping from client ${socket.remoteAddress}:${socket.remotePort}.`));
            console.log(chalk.green(`Server: Sending pong to client ${socket.remoteAddress}:${socket.remotePort}.`));
            setTimeout(() => {
                socket.write('pong');
            }, process.env.WAIT);
        }
    });
    socket.on('error', (error) => {
        console.log(chalk.green('Server: Error', error));
    });
}).listen(process.env.LOCAL_PORT, () => {
        console.log(chalk.green(`Server: Listening on ${eth0.address}:${process.env.LOCAL_PORT}`));
    });

// We need to wait a while for both servers to be up before trying to connect.
setTimeout(() => {
    socket.connect(process.env.REMOTE_PORT, process.env.REMOTE_NAME, () => {
        console.log(chalk.yellow(`Client: Sending ping to ${socket.remoteAddress}:${socket.remotePort}`));
        socket.write('ping');

        socket.on('data', (data) => {
            console.log(chalk.yellow(`Client: Received ${data.toString()} from ${socket.remoteAddress}:${socket.remotePort}`));
            setTimeout(() => {
                console.log(chalk.yellow(`Client: Sending ping to ${socket.remoteAddress}:${socket.remotePort}`));
                socket.write('ping');
            }, process.env.WAIT);
        });
        socket.on('error', (error) => {
            console.log(chalk.yellow('Client: Error', error));
        });
    });
}, process.env.WAIT);
