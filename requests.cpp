#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(const char *host, const char *url, char *query_params,
							char **cookies, int cookies_count, char *token, int type)
{
	char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

	// Step 1: write the method name, URL, request params (if any) and protocol type
	if (type == 0) {
		if (query_params != NULL) {
			sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
		} else {
			sprintf(line, "GET %s HTTP/1.1", url);
		}
	} else {
		// If type is other than 0, compute a DELETE request
		if (query_params != NULL) {
			sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
		} else {
			sprintf(line, "DELETE %s HTTP/1.1", url);
		}
	}

	compute_message(message, line);

	// Step 2: add the host
	sprintf(line, "Host: %s", host);
	compute_message(message, line);

	// Step 3 (optional): add headers and/or cookies, according to the protocol format
	if (token) {
        memset(line, 0, LINELEN);
        strcat(line, "Authorization: Bearer ");
        strcat(line, token);
		compute_message(message, line);
    }

	if (cookies != NULL) {
		memset(line, 0, LINELEN);
		strcat(line, "Cookie: ");

		for (int i = 0; i < cookies_count - 1; i++) {
			strcat(line, cookies[i]);
			strcat(line, "; ");
		}

		strcat(line, cookies[cookies_count - 1]);
		compute_message(message, line);
	}

	// Step 4: add final new line
	compute_message(message, "");
	free(line);
	return message;
}

char *compute_post_request(const char *host, const char *url,
						   const char* content_type, char **body_data,
						   int body_data_fields_count, char **cookies,
						   int cookies_count, char *token)
{
	char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char *)calloc(LINELEN, sizeof(char));

	// Step 1: write the method name, URL and protocol type
	sprintf(line, "POST %s HTTP/1.1", url);
	compute_message(message, line);
	
	// Step 2: add the host
	sprintf(line, "Host: %s", host);
	compute_message(message, line);

	/* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
			in order to write Content-Length you must first compute the message size
	*/
	if (token) {
        memset(line, 0, LINELEN);
        strcat(line, "Authorization: Bearer ");
        strcat(line, token);
		compute_message(message, line);
    }
	
	sprintf(line, "Content-Type: %s", content_type);
	compute_message(message, line);

	memset(body_data_buffer, 0, LINELEN);

	for (int i = 0; i < body_data_fields_count - 1; i++) {
		strcat(body_data_buffer, body_data[i]);
		strcat(body_data_buffer, "&");
	}

	strcat(body_data_buffer, body_data[body_data_fields_count - 1]);
	sprintf(line, "Content-Length: %lu", strlen(body_data_buffer));
	compute_message(message, line);

	// Step 4 (optional): add cookies
	if (cookies != NULL) {
		memset(line, 0, LINELEN);
		strcat(line, "Cookie: ");

		for (int i = 0; i < cookies_count - 1; i++) {
			strcat(line, cookies[i]);
			strcat(line, ";");
		}

		strcat(line, cookies[cookies_count - 1]);
		compute_message(message, line);
	}

	// Step 5: add new line at end of header
	compute_message(message, "");

	// Step 6: add the actual payload data
	memset(line, 0, LINELEN);
	strcat(message, body_data_buffer);

	free(line);
	free(body_data_buffer);
	return message;
}
