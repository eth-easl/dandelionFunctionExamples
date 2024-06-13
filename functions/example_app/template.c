#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "unistd.h"

typedef struct log_node{
   struct log_node* next; 
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

int main(int argc, char const *argv[]) {

    log_node* log_root = NULL;

    // read all events and put the in list sorted by timestamps
    DIR* log_dir = opendir("/responses");
    if (log_dir == NULL) {
        perror("Could not open log directory");
        return -1;
    }
    
    struct dirent* entry;
    while((entry = readdir(log_dir))) {
        if(entry->d_type == DT_REG) {
            char filepath[256] = {0}; 
            if(snprintf(filepath, 256, "/responses/%s", entry->d_name) < 0){
                perror("Failed to format log filepath\n");
                return -1;
            }
            FILE* log_file = fopen(filepath, "r");
            size_t line_size = 0;
            char* line_buffer = NULL;
            // read until and including initial [
            if(getdelim(&line_buffer, &line_size,'[', log_file) < 0){
                perror("Failed to read initial json {");
                return -1;
            }
            free(line_buffer);
            // keep reading until we find no more }
            int items_found = 0;
            log_node* current = malloc(sizeof(log_node));
            while(
                (items_found =
                    fscanf(log_file,
                    event_template,
                    current->details,
                    current->type,
                    current->server_id,
                    current->timestamp)) == 4){
                current->next = NULL;
                if(log_root == NULL) {
                    log_root = current;
                } else {
                    if (strcmp(current->timestamp, log_root->timestamp) <= 0){
                        current->next = log_root;
                        log_root = current;
                    } else {
                        log_node* previous = log_root;
                        while(previous->next != NULL) {
                            log_node* next = previous->next;
                            if(strcmp(current->timestamp, next->timestamp) <= 0){
                                current->next = next;
                                break; 
                            }
                            previous = next;
                        }
                        previous->next = current;
                    }
                }
                current = malloc(sizeof(log_node));
            }
            free(current);
        }
    } 

    // open output file
    FILE *log_file = fopen("/requests/log.txt", "w+");
    if (log_file == NULL) {
        perror("Failed to open file to get auth server\n");
        return -1;
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

    return -1;
}
