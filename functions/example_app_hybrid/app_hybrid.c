#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

#include "unistd.h"

#define BUFFER_SIZE 4096
#define SERVER_NUMBER 10
#define AUTH_SERVER_PORT 80

// Structure to store log event details
typedef struct log_node {
    struct log_node *next;
    char timestamp[100];
    char server_id[100];
    char type[100];
    char details[100];
} log_node;

char event_template[] = 
"%*[^{]"
"\"details\":\"%100[^\"],"
"\"event_type\":\"%100[^\"],"
"\"server_id\":\"%100[^\"],"
"\"timestamp\":\"%100[^\"]"
"}";

int connect_to_server(const char *hostname, int port) {
    struct sockaddr_in server_addr;
    struct hostent *host = gethostbyname(hostname);
    if (!host) {
        perror("Failed to resolve hostname");
        return -1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Socket connection failed");
        close(sockfd);
        return -1;
    }

    return sockfd;
}


int send_http_request(int sockfd, const char *request, char *response, size_t response_size) {
    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("Send failed");
        return -1;
    }

    ssize_t total_received = 0, bytes;
    while ((bytes = recv(sockfd, response + total_received, response_size - total_received - 1, 0)) > 0) {
        total_received += bytes;
        if (total_received >= response_size - 1) break;
    }
    response[total_received] = '\0';
    return total_received > 0 ? 0 : -1;
}


void parse_auth_response(const char *response, char *token) {
    const char *start = strstr(response, "\"token\":");
    if (start) {
        sscanf(start, "\"token\": \"%[^\"]", token);
    }
}

void parse_log_events(const char *response, log_node **log_root) {
    log_node *current = NULL;

    const char *start = strstr(response, "\"events\":");
    if (start) {
        start += 10;
        while (1) {
            current = malloc(sizeof(log_node));
            if (sscanf(start, 
                       event_template, 
                       current->timestamp, 
                       current->server_id, 
                       current->type, 
                       current->details) != 4) {
                free(current);
                break;
            }

            current->next = *log_root;
            *log_root = current;

            start = strstr(start, "},{");
            if (!start) break;
            start += 3;
        }
    }
}

// Render logs to an HTML file
void render_logs_to_html(log_node *log_root) {
    FILE *log_file = fopen("log.html", "w+");
    if (!log_file) {
        perror("Failed to open HTML file");
        return;
    }

    // write preamble 
    fprintf(log_file, "<!DOCTYPE html>\n");
    fprintf(log_file, "<html lang=\"en\">\n");
    fprintf(log_file, "<head>\n");
    fprintf(log_file, "    <meta charset=\"UTF-8\">\n");
    fprintf(log_file, "    <title>Logs</title>\n");
    fprintf(log_file, "</head>\n");
    fprintf(log_file, "<body>\n");
    fprintf(log_file, "    <table>\n");
    fprintf(log_file, "        <tr>\n");
    fprintf(log_file, "            <th>Timestamp</th>\n");
    fprintf(log_file, "            <th>Server ID</th>\n");
    fprintf(log_file, "            <th>Event Type</th>\n");
    fprintf(log_file, "            <th>Details</th>\n");
    fprintf(log_file, "        </tr>\n");

    // write log information
    for(log_node* current = log_root; current != NULL; current = current->next){
        fprintf(log_file, "        <tr>\n");
        fprintf(log_file, "            <th>%s</th>\n", current->timestamp);
        fprintf(log_file, "            <th>%s</th>\n", current->server_id);
        fprintf(log_file, "            <th>%s</th>\n", current->type);
        fprintf(log_file, "            <th>%s</th>\n", current->details);
        fprintf(log_file, "        </tr>\n");
    } 

    // write ending
    fprintf(log_file, "    </table>\n");
    fprintf(log_file, "</body>\n"); 

    fclose(log_file);
}

// Main function to handle authorization, log fetching, and rendering
int main() {
    // Get the authorization server address from the environment variable
    const char *auth_server = getenv("STORAGE_HOST");
    if (!auth_server) {
        fprintf(stderr, "Storage host must be provided with environment variable STORAGE_HOST\n");
        return 1;
    }

    const char *auth_token = "fapw84ypf3984viuhsvpoi843ypoghvejkfld";

    // handle
    int auth_sock = connect_to_server(auth_server, AUTH_SERVER_PORT);
    if (auth_sock < 0) return 1;

    char auth_request[BUFFER_SIZE];
    snprintf(auth_request, sizeof(auth_request),
             "POST /authorize HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %zu\r\n\r\n"
             "{\"token\": \"%s\"}", auth_server, strlen(auth_token) + 12, auth_token);

    char auth_response[BUFFER_SIZE];
    if (send_http_request(auth_sock, auth_request, auth_response, BUFFER_SIZE) < 0) {
        close(auth_sock);
        return 1;
    }
    close(auth_sock);

    char token[256];
    parse_auth_response(auth_response, token);

    // fan_out
    log_node *log_root = NULL;
    char log_request[BUFFER_SIZE];
    char log_response[BUFFER_SIZE];

    for (int i = 0; i < SERVER_NUMBER; ++i) {
        int log_sock = connect_to_server(auth_server, AUTH_SERVER_PORT);
        if (log_sock < 0) continue;

        snprintf(log_request, sizeof(log_request),
                 "GET /logs/%d HTTP/1.1\r\n"
                 "Host: %s\r\n"
                 "Authorization: Bearer %s\r\n\r\n", i, auth_server, token);

        if (send_http_request(log_sock, log_request, log_response, BUFFER_SIZE) == 0) {
            parse_log_events(log_response, &log_root);
        }
        close(log_sock);
    }

    // render
    render_logs_to_html(log_root);

    // Free log nodes
    log_node *current = log_root;
    while (current) {
        log_node *next = current->next;
        free(current);
        current = next;
    }

    return 0;
}