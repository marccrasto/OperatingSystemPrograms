#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Process {
    int id;
    bool arrivalTimeTaken;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;
    bool arrived; 
    bool completed; 
} Process;

int main() {
    FILE* file;
    char line[100];
    file = fopen("sjf_input.txt", "r");
    int testCaseNum = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("Test case #%d: %s", testCaseNum, line);
        int totalBurstTime = 0;
        int numOfProcesses = 0;
        int length = strlen(line);
        for (int i = 0; i < length; i++) {
            if (line[i] == 'p') {
                numOfProcesses++;
            }
        }

        Process processes[numOfProcesses];
        for (int i = 0; i < numOfProcesses; i++) {
            processes[i].arrivalTimeTaken = false;
            processes[i].arrived = false;
            processes[i].completed = false;
            processes[i].waitingTime = 0;
            processes[i].turnaroundTime = 0;
        }
        printf("Number of Processes: %ld\n", sizeof(processes) / sizeof(processes[0]));
        int processCount = 0;

        for (int i = 0; i < length; i++) {
            if (line[i] == 'p') {
                char idString[4];
                int idStringPos = 0;
                i = i + 1;
                while (isdigit(line[i])) {
                    idString[idStringPos] = line[i];
                    idStringPos++;
                    i = i + 1;
                }
                idString[idStringPos] = '\0';
                processes[processCount].id = atoi(idString);
            }

            if (isdigit(line[i])) {
                char numString[4];
                int numStringPos = 0;
                while (line[i] != ' ' && line[i] != '\n' && line[i] != '\0') {
                    numString[numStringPos] = line[i];
                    numStringPos++;
                    i = i + 1;
                }

                numString[numStringPos] = '\0';
                int numInteger = atoi(numString);
                if (processes[processCount].arrivalTimeTaken == false) {
                    processes[processCount].arrivalTime = numInteger;
                    processes[processCount].arrivalTimeTaken = true;
                }
                else {
                    processes[processCount].burstTime = numInteger;
                    totalBurstTime = totalBurstTime + numInteger;
                    processCount++;
                }
            }
        }

        for (int i = 1; i < numOfProcesses; i++) {
            for (int j = i; j < numOfProcesses; j++) {
                if (processes[j].burstTime < processes[i].burstTime) {
                    Process tempProcess = processes[i];
                    processes[i] = processes[j];
                    processes[j] = tempProcess;
                }
            }
        }
        //string has been parsed and processes have been created.


        totalBurstTime = totalBurstTime + processes[0].arrivalTime;
        printf("Process Scheduling Started:\n");

        bool working = false;
        int workingTime = 0;
        processCount = 0;
        for (int i = 0; i <= totalBurstTime; i++) {
            printf("CPU Time %d: ", i);

            for (int j = 0; j < numOfProcesses; j++) {
                if (processes[j].arrivalTime == i) {
                    printf("[P%d Arrived] ", processes[j].id);
                    processes[j].arrived = true;
                    working = true;
                }
            }


            if (processes[processCount].arrived == true) {
                printf("P%d [%d/%d]", processes[processCount].id, workingTime, processes[processCount].burstTime);
                workingTime++;
            }

            if (workingTime > processes[processCount].burstTime) {
                processes[processCount].turnaroundTime = i - processes[processCount].arrivalTime;
                printf("\nProcess P%d completed with Turnaround Time: %d, Waiting Time: %d", processes[processCount].id, processes[processCount].turnaroundTime, processes[processCount].waitingTime);
                processes[processCount].completed = true;
                processCount++;
                workingTime = 0;
                processes[processCount].waitingTime = i - processes[processCount].arrivalTime;
                i = i - 1;
            }

            if (working == false) {
                printf("None");
            }

            printf("\n");
            if (processes[numOfProcesses - 1].completed == true) {
                break;
            }
        }

        float totalTurnaroundTime = 0;
        float totalWaitingTime = 0;
        for (int i = 0; i < numOfProcesses; i++) {
            totalTurnaroundTime = totalTurnaroundTime + processes[i].turnaroundTime;
            totalWaitingTime = totalWaitingTime + processes[i].waitingTime;
        }
        printf("Process scheduling completed with Avg Turnaround Time: %.2f, Avg Waiting Time: %.2f\n", totalTurnaroundTime / numOfProcesses, totalWaitingTime / numOfProcesses);

        printf("\n");
        testCaseNum++;
    }
    fclose(file);
}