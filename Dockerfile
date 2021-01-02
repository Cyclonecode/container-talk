FROM node:14.15.3

ARG LOCAL_PORT=1024
ARG REMOTE_PORTE=2048
ARG REMOTE_NAME=
ARG WAIT=500

ENV LOCAL_PORT=$LOCAL_PORT_PORT
ENV REMOTE_PORT=$REMOTE_PORT
ENV REMOTE_NAME=$REMOTE_NAME

EXPOSE $LOCAL_PORT

# This thing first installs npm packages and then it copies the committed files as an overlay
WORKDIR /usr/src/app
COPY package*json ./

RUN npm install --production --no-optional
COPY foobar.js .
CMD ["node", "foobar.js"]