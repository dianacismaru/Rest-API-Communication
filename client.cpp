// Copyright (C) 2023 Cismaru Diana-Iuliana (321CA / 2022-2023)
#include "client.h"

int sockfd;

int main(int argc, char *argv[]) {
	char command[CMAX];

	sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		return -1;
	}

	// Read the command
	while (1) {
		scanf("%s", command);

		// Check if the command is valid
		if (!strcmp(command, "exit")) {
			break;

		} else if (!strcmp(command, "register")) {
			register_account();

		} else if (!strcmp(command, "login")) {
			login();

		} else if (!strcmp(command, "enter_library")) {
			enter_library();

		} else if (!strcmp(command, "get_books")) {
			get_books();

		} else if (!strcmp(command, "get_book")) {
			get_book();

		} else if (!strcmp(command, "add_book")) {
			add_book();

		} else if (!strcmp(command, "delete_book")) {
			delete_book();

		} else if (!strcmp(command, "logout")) {
			logout();
		} else {
			// Read the remaining arguments
			fgets(command, CMAX, stdin);
			printf(INVALID_COMMAND);
		}
	}

	close_connection(sockfd);
	return 0;
}
