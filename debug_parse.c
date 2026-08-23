#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/prediction.h"

int main() {
    CareerPath careers[10];
    int count = 0;
    FILE *fp = fopen("data/careers.csv", "r");
    char line[256];
    int first_line = 1;

    while (fgets(line, sizeof(line), fp)) {
        if (first_line) { first_line = 0; continue; }
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        char *token = strtok(line, ",");
        if (!token) continue;
        strcpy(careers[count].name, token);
        
        token = strtok(NULL, ","); if(token) careers[count].career_type = atoi(token);
        token = strtok(NULL, ","); if(token) careers[count].req_programming = atof(token);
        token = strtok(NULL, ","); if(token) careers[count].req_networking = atof(token);
        token = strtok(NULL, ","); if(token) careers[count].req_design = atof(token);
        token = strtok(NULL, ","); if(token) careers[count].req_analytics = atof(token);
        token = strtok(NULL, ","); if(token) careers[count].req_communication = atof(token);
        token = strtok(NULL, ","); if(token) careers[count].req_security = atof(token);

        printf("%s: Prog=%.1f Net=%.1f\n", careers[count].name, careers[count].req_programming, careers[count].req_networking);
        count++;
    }
    return 0;
}
