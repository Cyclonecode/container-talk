CC=cc
LDFLAGS=-lc -lpthread
CFLAGS=

app.o : app.c
	${CC} ${CFLAGS} -D WAIT=${WAIT} -D LOCAL_PORT=${LOCAL_PORT} -D REMOTE_PORT=${REMOTE_PORT} -D REMOTE_NAME=\"${REMOTE_NAME}\" -c -o $@ $?

app : app.o
	${CC} ${CFLAGS} -o $@ ${LDFLAGS} $?

clean:
	rm -f app app.o
