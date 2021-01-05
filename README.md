# Container communication

An example of two containers running both a client and server, continuously
sending ping and pong to each other.

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

## Shortcuts

For convenience there are a couple of npm scripts that can be used:

    npm run build
Build service foo and bar.

    npm run logs:foo
Follow logs for service foo.

    npm run logs:bar
Follow logs for service bar.

    npm run exec:foo
Run a shell in foo.

    npm run exec:bar
Run a shell in bar.

    npm up
Start service foo and bar.

    npm down
Stop service foo and bar.

## Environment

Environment variables can be added to a `.env` file.

| Name          | Description        | Example      |
| ------------- |:------------------:|:-------------:
| FOO_PORT      | foo server port    | 2048         |
| BAR_PORT      | bar server port    | 8000         |
| WAIT          | delay between send | 1500         |
