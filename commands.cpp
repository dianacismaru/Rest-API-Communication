// Copyright (C) 2023 Cismaru Diana-Iuliana (321CA / 2022-2023)
#include "client.h"

void register_account(int sockfd) {
    // Read username and password
    char username[NMAX], password[NMAX];
    cout << "username=";
    cin.getline(username, NMAX);

    cout << "password=";
    cin.getline(password, NMAX);

    // Spaces are not allowed
    if (strchr(username, ' ') || strchr(password, ' ')) {
        cout << "Invalid credentials! Do NOT use spaces.\n";
        return;
    }

    // Create JSON object
    JSON j;
    j["username"] = username;
    j["password"] = password;

    string jsonString = j.dump();
    char *string = (char *)malloc(jsonString.length());
    strcpy(string, jsonString.c_str());
    char *body_data[] = {string};

    char *request = compute_post_request("34.254.242.81", "/api/v1/tema/auth/register",
                                         "application/JSON", body_data, 1, NULL, 0, NULL);
    send_to_server(sockfd, request);
    free(request);
    free(string);

    char *response = receive_from_server(sockfd);

    char code[4];
    strncpy(code, response + 9, 3);

    if (!strcmp(code, "201") || !strcmp(code, "200")) {
        cout << "200 - Registered successfully!\n";
    } else if (!strcmp(code, "400")) {
        cout << "400 - The username is taken!\n";
    } else {
        cout << "The account can not be registered.\n";
    }
    
    free(response);
}

void login(int sockfd, char *current_cookie) {
    // Read username and password
    char username[NMAX], password[NMAX];
    cout << "username=";
    cin.getline(username, NMAX);

    cout << "password=";
    cin.getline(password, NMAX);

    // Spaces are not allowed
    if (strchr(username, ' ') || strchr(password, ' ')) {
        cout << "Invalid credentials! Spaces are not allowed.\n";
        return;
    }

    // Create JSON object
    JSON j;
    j["username"] = username;
    j["password"] = password;

    // Add JSON object as body data to the post request
    string jsonString = j.dump();
    char *string = (char *)malloc(jsonString.length());
    strcpy(string, jsonString.c_str());
    char *body_data[] = {string};

    char *request = compute_post_request("34.254.242.81", "/api/v1/tema/auth/login",
                                         "application/JSON", body_data, 1, NULL, 0, NULL);
    send_to_server(sockfd, request);

    free(request);
    free(string);

    char *response = receive_from_server(sockfd);

    char code[4];
    strncpy(code, response + 9, 3);

    if (!strcmp(code, "201") || !strcmp(code, "200")) {
        cout << "200 - Logged in successfully!\n";

        // Get the login cookie
        char *cookie = strstr(response, "connect.sid=");
        char *found_semicolon = strchr(cookie, ';');
        int cookie_size = found_semicolon - cookie;

        // Verify consecutive logins and delete the previous cookie
        if (strlen(current_cookie)) {
            memset(current_cookie, 0, strlen(current_cookie));
        }

        strncpy(current_cookie, cookie, cookie_size);

    } else if (!strcmp(code, "400")) {
        cout << "400 - Incorrect credentials!\n";
    } else {
        cout << "The account can not log in.\n";
    }
    
    free(response);
}

void enter_library(int sockfd, char *current_cookie, char *jwt_token) {
    if (!strlen(current_cookie)) {
        cout << "You are not logged in.\n";
        return;
    }

    char *body_data[] = {current_cookie};
    char *request = compute_get_request("34.254.242.81", "/api/v1/tema/library/access",
                                         NULL, body_data, 1, NULL, 0);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);
    
    char code[4];
    strncpy(code, response + 9, 3);

    if (!strcmp(code, "201") || !strcmp(code, "200")) {
        cout << "200 - Entered library successfully!\n";
        char *json_response = basic_extract_json_response(response);
        strcpy(jwt_token, json_response);

    } else if (!strcmp(code, "401")) {
        cout << "401 - Unauthorized. You are not logged in!\n";
    } else {
        cout << "An error occured. You didn't enter the library.\n";
    }

    free(response);
}

void get_books(int sockfd, char *current_cookie, char *jwt_token) {
    if (!strlen(current_cookie)) {
        cout << "You are not logged in.\n";
        return;
    }

    if (!strlen(jwt_token)) {
        cout << "You don't have access to the library.\n";
        return;
    }

    JSON j = JSON::parse(jwt_token);
    string token_string = j["token"];
    char token[LINELEN];
    strcpy(token, token_string.c_str());

    char *request = compute_get_request("34.254.242.81", "/api/v1/tema/library/books",
                                         NULL, NULL, 0, token, 0);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    if (strstr(response, "[]")) {
        cout << "There are no books in the library yet.\n";
    } else {
        cout << "The books in the library are:\n";
        j = JSON::parse(strchr(response, '['));
        cout << j.dump(8) << endl;
    }

    free(response);
}

void get_book(int sockfd, char *current_cookie, char *jwt_token) {
    char id[NMAX];
    cout << "id=";
    cin.getline(id, NMAX);

    if (!strlen(current_cookie)) {
        cout << "You are not logged in.\n";
        return;
    }

    if (!strlen(jwt_token)) {
        cout << "You don't have access to the library.\n";
        return;
    }

    if (!isNumber(id)) {
        cout << "ID is not valid.\n";
        return;
    }

    JSON j = JSON::parse(jwt_token);
    string token_string = j["token"];
    char token[LINELEN];
    strcpy(token, token_string.c_str());

    char url[NMAX] = "/api/v1/tema/library/books/";
    strcat(url, id);
    char *request = compute_get_request("34.254.242.81", url, NULL, NULL, 0, token, 0);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    char code[4];
    strncpy(code, response + 9, 3);

    if (!strcmp(code, "201") || !strcmp(code, "200")) {
        cout << "The requested book is:\n";
        char *json_response = basic_extract_json_response(response);
        j = JSON::parse(json_response);
        cout << j.dump(8) << endl;

    } else if (!strcmp(code, "404")) {
        cout << "404 - Invalid ID. No book was found.\n";
    } else {
        cout << "The book cannot be shown.\n";
    }

    free(response);
}

void add_book(int sockfd, char *current_cookie, char *jwt_token) {
    if (!strlen(current_cookie)) {
        cout << "You are not logged in.\n";
        return;
    }
    
    if (!strlen(jwt_token)) {
        cout << "You don't have access to the library.\n";
        return;
    }

    char title[NMAX], author[NMAX], genre[NMAX], page_count_string[NMAX], publisher[NMAX];
    cout << "title=";
    cin.getline(title, NMAX);

    cout << "author=";
    cin.getline(author, NMAX);

    cout << "genre=";
    cin.getline(genre, NMAX);

    cout << "publisher=";
    cin.getline(publisher, NMAX);

    cout << "page_count=";
    cin.getline(page_count_string, NMAX);

    // Empty fields verification
    if (!strlen(title) || !strlen(author) || !strlen(genre) ||
        !strlen(publisher) || !strlen(page_count_string)) {
        cout << "You can't leave empty fields!\n";
        return;
    }

    // Page count verification
    if (!isNumber(page_count_string)) {
        cout << "Invalid page count!\n";
        return;
    }

    // Convert string to integer
    int page_count = atoi(page_count_string);

    // Create JSON object
    JSON j = {{"title", title}, {"author", author}, {"genre", genre},
              {"page_count", page_count}, {"publisher", publisher}};

    string jsonString = j.dump();
    char *book = (char *)malloc(jsonString.length());
    strcpy(book, jsonString.c_str());
    char *body_data[] = {book};

    j = JSON::parse(jwt_token);
    string token_string = j["token"];
    char token[LINELEN];
    strcpy(token, token_string.c_str());

    char *request = compute_post_request("34.254.242.81", "/api/v1/tema/library/books",
                                         "application/JSON", body_data, 1, NULL, 0, token);

    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    char code[4];
    strncpy(code, response + 9, 3);

    if (!strcmp(code, "201") || !strcmp(code, "200")) {
        cout << "The book was successfully added.\n";
    } else {
        cout << "The book was not added.\n";
    }

    free(response);
}

void delete_book(int sockfd, char *current_cookie, char *jwt_token) {
    char id[NMAX];
    cout << "id=";
    cin.getline(id, NMAX);

    if (!strlen(current_cookie)) {
        cout << "You are not logged in.\n";
        return;
    }

    if (!strlen(jwt_token)) {
        cout << "You don't have access to the library.\n";
        return;
    }

    if (!isNumber(id)) {
        cout << "ID is not valid.\n";
        return;
    }

    JSON j = JSON::parse(jwt_token);
    string token_string = j["token"];
    char token[LINELEN];
    strcpy(token, token_string.c_str());

    char url[NMAX] = "/api/v1/tema/library/books/";
    strcat(url, id);
    char *request = compute_get_request("34.254.242.81", url, NULL, NULL, 0, token, 1);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    char code[4];
    strncpy(code, response + 9, 3);

    if (!strcmp(code, "201") || !strcmp(code, "200")) {
        cout << "Book successfully deleted!\n";
    } else if (!strcmp(code, "404")) {
        cout << "404 - Invalid ID. No book was deleted!\n";
    } else {
        cout << "The book cannot be deleted.\n";
    }

    free(response);
}

void logout(int sockfd, char *current_cookie, char *jwt_token) {
    char *body_data[] = {current_cookie};
    char *request = compute_get_request("34.254.242.81", "/api/v1/tema/auth/logout",
                                         NULL, body_data, 1, NULL, 0);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);
    
    char code[4];
    strncpy(code, response + 9, 3);

    if (!strcmp(code, "201") || !strcmp(code, "200")) {
        cout << "200 - Logged out successfully!\n";
        memset(current_cookie, 0, strlen(current_cookie));
        memset(jwt_token, 0, strlen(jwt_token));

    } else if (!strcmp(code, "400")) {
        cout << "400 - You are not logged in!\n";
    } else {
        cout << "An error occured. You didn't log out\n";
    }

    free(response);
}
