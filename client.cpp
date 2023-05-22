// Copyright (C) 2023 Cismaru Diana-Iuliana (321CA / 2022-2023)
#include "client.h"

int sockfd;
char current_cookie[LINELEN];
char jwt_token[LINELEN];

int main(int argc, char *argv[]) {
	char command[CMAX];

	// Read the command
	while (1) {
		sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			return -1;
		}

		cout << "\n";
		cin.getline(command, CMAX);

		if (!strcmp(command, "exit")) {
			break;

		} else if (!strcmp(command, "register")) {
			register_account(sockfd);

		} else if (!strcmp(command, "login")) {
			login(sockfd, current_cookie);

		} else if (!strcmp(command, "enter_library")) {
			enter_library(sockfd, current_cookie, jwt_token);

		} else if (!strcmp(command, "get_books")) {
			get_books(sockfd, current_cookie, jwt_token);

		} else if (!strcmp(command, "get_book")) {
			get_book(sockfd, current_cookie, jwt_token);

		} else if (!strcmp(command, "add_book")) {
			add_book(sockfd, current_cookie, jwt_token);

		} else if (!strcmp(command, "delete_book")) {
			delete_book(sockfd, current_cookie, jwt_token);

		} else if (!strcmp(command, "logout")) {
			logout(sockfd, current_cookie, jwt_token);
			
		} else {
			printf(INVALID_COMMAND);
		}

		close_connection(sockfd);
	}

	return 0;
}
