// Copyright (C) 2023 Cismaru Diana-Iuliana (321CA / 2022-2023)
#ifndef CLIENT_H_
#define CLIENT_H_

// Maximum size of a command
#define CMAX 12

#define INVALID_COMMAND "The given command does not exist. Try again!\n"

// Libraries
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>

// Headers
#include "helpers.h"
#include "requests.h"
#include "commands.h"

#endif  // CLIENT_H_