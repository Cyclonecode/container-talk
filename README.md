# Example of two containers that can access each other

This is achieved by setting up different network aliases for each container
and then use this alias to establishing connections.

## Running

    docker-compose build
Build service foo and bar.

    docker-compose up -d
Start service foo and bar.

    docker-compose log -f foo
Follow logs for service foo.

    docker-compose log -f bar
Follow logs for service bar.

    docker-compose down
Stop service foo and bar.

## Environment

Create a `.env` file with the following:

    # Server port for 'foo' service
    FOO_PORT=2048
    # Server port for 'bar' service
    BAR_PORT=8000
    # Delay between write calls on socket
    WAIT=1500
