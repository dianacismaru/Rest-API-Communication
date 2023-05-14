#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
	char *message;
	char *response;
	int sockfd;

	sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		return -1;
	}

	// Ex 1.1: GET dummy from main server
	printf("=============Task1=============\n\n");

	message = compute_get_request("34.254.242.81:8080", "/api/v1/dummy", NULL, NULL, 0);
	printf("Message: %s\n", message);
	send_to_server(sockfd, message);
	free(message);

	response = receive_from_server(sockfd);
	printf("\nResponse is: %s\n", response);
	free(response);

	// Ex 1.2: POST dummy and print response from main server
	char *form_data[] = {"username=student", "password=student"};

	printf("=============Task1.2=============\n\n");
	message = compute_post_request("34.254.242.81:8080", "/api/v1/dummy",
									"application/x-www-form-urlencoded", form_data,
									2, NULL, 0);
	printf("message is: %s\n", message);
	send_to_server(sockfd, message);
	free(message);

	response = receive_from_server(sockfd);
	printf("\nResponse is %s\n", response);
	free(response);

	// Ex 2: Login into main server
	printf("=============Task2=============\n\n");
	message = compute_post_request("34.254.242.81:8080", "/api/v1/auth/login",
									"application/x-www-form-urlencoded", form_data,
									2, NULL, 0);
	printf("Message: %s\n", message);
	send_to_server(sockfd, message);
	free(message);

	response = receive_from_server(sockfd);
	printf("\nResponse is %s\n", response);
	free(response);

	// Ex 3: GET weather key from main server
	printf("=============Task3=============\n\n");
	char *cookies[] = {"connect.sid=s%3Akz3ZIqdWNNb-faLrzYq_2h-gnfTayXeN.uxi8G"
						"%2BZ7nbyKY1YowNj%2FIzAE3Pg%2F6cc96sznryQV2rk; Path=/;"};
	message = compute_get_request("34.254.242.81", "/api/v1/weather/key", NULL, cookies, 1);

	printf("Message: %s\n", message);
	send_to_server(sockfd, message);
	free(message);

	response = receive_from_server(sockfd);
	printf("\nResponse is %s\n", response);
	free(response);

	// Ex 4: GET weather data from OpenWeather API
	printf("=============Task4=============\n\n");
	int weatherfd = open_connection("37.139.20.5", 80, AF_INET, SOCK_STREAM, 0);
	if (weatherfd < 0) {
		return -1;
	}

	// https://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid={API key}
	char *query_param = "lat=44.27&lon=26.10&appid=b912dd495585fbf756dc6d8f415a7649";
	message = compute_get_request("api.openweathermap.org", "/data/2.5/weather",
									query_param, NULL, 0);

	printf("Message: %s\n", message);
	send_to_server(weatherfd, message);
	free(message);

	response = receive_from_server(weatherfd);
	printf("Response is %s\n", response);
	free(response);

	// Ex 5: POST weather data for verification to main server
	printf("=============Task5=============\n\n");

	char *weather_data[] = {"{\"coord\":{\"lon\":26.1,\"lat\":44.27},\"weather\":"
							"[{\"id\":803,\"main\":\"Clouds\",\"description\":"
							"\"broken clouds\",\"icon\":\"04d\"}],\"base\":\"stations\","
							"\"main\":{\"temp\":291.47,\"feels_like\":290.4,\"temp_min\":"
							"291.11,\"temp_max\":291.56,\"pressure\":1023,\"humidity\":40,"
							"\"sea_level\":1023,\"grnd_level\":1016},\"visibility\":10000,"
							"\"wind\":{\"speed\":2.77,\"deg\":68,\"gust\":3.5},\"clouds\":"
							"{\"all\":77},\"dt\":1683991211,\"sys\":{\"type\":2,\"id\":2032494,"
							"\"country\":\"RO\",\"sunrise\":1683946298,\"sunset\":1683999142},"
							"\"timezone\":10800,\"id\":856315,\"name\":\"1 Decembrie\",\"cod\":200}"};


	message = compute_post_request("34.254.242.81", "/api/v1/weather/44.27/26.1",
									"application/json", weather_data, 1, NULL, 0);
	printf("message is: %s\n", message);
	send_to_server(sockfd, message);
	free(message);

	response = receive_from_server(sockfd);
	printf("\nResponse is %s\n", response);
	free(response);

	// // Ex 6: Logout from main server
	printf("=============Task6=============\n\n");
	message = compute_get_request("34.254.242.81:8080", "/api/v1/auth/logout", NULL, NULL, 0);
	printf("Message: %s\n", message);
	send_to_server(sockfd, message);
	free(message);

	response = receive_from_server(sockfd);
	printf("Response is %s\n", response);
	free(response);

	// BONUS: make the main server return "Already logged in!"

	// free the allocated data at the end!
	close(sockfd);
	close(weatherfd);

	return 0;
}
