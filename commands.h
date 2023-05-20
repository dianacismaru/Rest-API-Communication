// Copyright (C) 2023 Cismaru Diana-Iuliana (321CA / 2022-2023)
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "client.h"

extern void register_account(int sockfd);
extern void login(int sockfd, char *current_cookie);
extern void enter_library(int sockfd, char *current_cookie, char *jwt_token);
extern void get_books(int sockfd, char *current_cookie, char *jwt_token);
extern void get_book(int sockfd, char *current_cookie, char *jwt_token);
extern void add_book(int sockfd, char *current_cookie, char *jwt_token);
extern void delete_book(int sockfd, char *current_cookie, char *jwt_token);
extern void logout(int sockfd, char *current_cookie, char *jwt_token);

#endif  // COMMANDS_H_