# IRC Server

A lightweight Internet Relay Chat server written in C++98.

The project implements a network server from the socket layer upward, including TCP connections, non-blocking I/O, client management, IRC command parsing, channels, permissions, and message routing.

## Overview

The server accepts a port and password, listens for TCP connections, maintains connected clients, parses IRC commands, and dispatches those commands to the appropriate handlers.

At its core, the server uses:

- TCP sockets
- `poll()` for multiplexing client connections
- Non-blocking sockets
- C++98
- Explicit client/channel state management
- Command parsing and dispatch

## Architecture

```text
                    ┌─────────────────┐
                    │   IRC Client    │
                    └────────┬────────┘
                             │ TCP
                             ▼
                    ┌─────────────────┐
                    │   IRC Server    │
                    │                 │
                    │ socket + poll() │
                    └────────┬────────┘
                             │
                ┌────────────┼────────────┐
                ▼            ▼            ▼
           Client state   Channels     Parser
                │            │            │
                └────────────┴────────────┘
                             │
                             ▼
                     Command handlers
```

The server keeps track of connected clients and channels while dispatching incoming IRC messages through dedicated command handlers.

## Supported commands

The implementation includes handlers for commands such as:

- `PASS`
- `NICK`
- `USER`
- `JOIN`
- `PART` / `QUIT`
- `PRIVMSG`
- `TOPIC`
- `KICK`
- `INVITE`
- `MODE`
- `WHOIS`
- `PING`
- `CAP`

Channel functionality includes operator and permission handling, channel topics, invitations, passwords, and user limits through the supported mode commands.

## Server design

The main server loop is built around `poll()`.

The general lifecycle is:

```text
Create TCP socket
      │
      ▼
Resolve address
      │
      ▼
Bind + listen
      │
      ▼
poll() for events
      │
      ├── New connection
      │      └── accept client
      │
      └── Client event
             ├── receive data
             ├── parse command
             └── dispatch handler
```

Sockets are configured for non-blocking operation, allowing a single event loop to manage multiple connected clients.

## Code organisation

```text
irc_gang/
├── include/
│   ├── Server.hpp
│   ├── Client.hpp
│   ├── Channel.hpp
│   └── command headers
├── src/
│   ├── main.cpp
│   ├── Server.cpp
│   ├── Client.cpp
│   ├── Channel.cpp
│   ├── Parse.cpp
│   └── commands/
│       ├── Commands.cpp
│       ├── Pass.cpp
│       ├── Join.cpp
│       ├── Nick.cpp
│       ├── User.cpp
│       ├── Privmsg.cpp
│       ├── Topic.cpp
│       ├── Mode.cpp
│       └── ...
└── Makefile
```

This separation keeps transport/server state separate from protocol parsing and individual command implementations.

## Build

The project targets **C++98** and uses strict compiler warnings.

```bash
make
```

The resulting executable is:

```text
ircserv
```

Clean the build with:

```bash
make clean
make fclean
make re
```

## Run

The server expects a port and server password:

```bash
./ircserv <port> <password>
```

For example:

```bash
./ircserv 6667 secret
```

An IRC client can then connect to the configured port.

## Engineering focus

This project was primarily an exercise in network and systems programming.

The main engineering problems were:

- managing multiple TCP clients without blocking the server loop
- maintaining client and channel state
- parsing a line-oriented network protocol
- routing messages between users and channels
- implementing channel permissions
- handling socket lifecycle and cleanup
- keeping protocol-specific behaviour separated into command handlers

## Project context

Built as part of the 42 Abu Dhabi curriculum.
