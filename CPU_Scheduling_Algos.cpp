#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <map>
#include <climits>
#include <ctime>
#include <vector>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class Process {
    int id;
    int burstTime;
    int arrivalTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int remainingTime;
public:
    int getId() {
        return id;
    }
    int getburstTime() {
        return burstTime;
    }
    int getarrivalTime() {
        return arrivalTime;
    }
    int getcompletionTime() {
        return completionTime;
    }
    int getTurnaroundTime() {
        return turnaroundTime;
    }
    int getwaitingTime() {
        return waitingTime;
    }
    int getRemainingTime() {
        return remainingTime;
    }
    void setId(int id) {
        this->id = id;
    }
    void setburstTime(int burstTime) {
        this->burstTime = burstTime;
    }
    void setarrivalTime(int arrivalTime) {
        this->arrivalTime = arrivalTime;
    }
    void setcompletionTime(int completionTime) {
        this->completionTime = completionTime;
    }
    void setTurnaroundTime(int turnaroundTime) {
        this->turnaroundTime = turnaroundTime;
    }
    void setwaitingTime(int waitingTime) {
        this->waitingTime = waitingTime;
    }
    void setRemainingTime(int remainingTime) {
        this->remainingTime = remainingTime;
    }
};

void FCFS(Process P[], int numProcesses) {
    sort(P, P + numProcesses, [](Process p, Process q) { return p.getarrivalTime() < q.getarrivalTime(); });
    int currentTime = 0;
    float totalWaitTime = 0, totalTurnaroundTime = 0;

    for (int i = 0; i < numProcesses; i++) {
        if (currentTime < P[i].getarrivalTime()) {
            currentTime = P[i].getarrivalTime();
        }
        currentTime += P[i].getburstTime();
        P[i].setcompletionTime(currentTime);
        P[i].setTurnaroundTime(P[i].getcompletionTime() - P[i].getarrivalTime());
        P[i].setwaitingTime(P[i].getTurnaroundTime() - P[i].getburstTime());
        totalWaitTime += P[i].getwaitingTime();
        totalTurnaroundTime += P[i].getTurnaroundTime();
    }

    float avgWaitTime = totalWaitTime / numProcesses;
    float avgTurnaroundTime = totalTurnaroundTime / numProcesses;
   json result;
    for (int i = 0; i < numProcesses; i++) {
        result["processes"].push_back({
            {"id", P[i].getId()},
            {"arrivalTime", P[i].getarrivalTime()},
            {"burstTime", P[i].getburstTime()},
            {"completionTime", P[i].getcompletionTime()},
            {"turnaroundTime", P[i].getTurnaroundTime()},
            {"waitingTime", P[i].getwaitingTime()}
        });
    }
    result["avgWaitTime"] = avgWaitTime;
    result["avgTurnaroundTime"] = avgTurnaroundTime;
    cout << result.dump(4) << endl; // Output the result as JSON
    
}

void SJF_Preemptive(Process P[], int numProcesses) {
    int currentTime = 0;
    int completed = 0;
    float totalWaitTime = 0, totalTurnaroundTime = 0;

    while (completed != numProcesses) {
        int idx = -1;
        int minRemainingTime = INT_MAX;

        for (int i = 0; i < numProcesses; i++) {
            if (P[i].getarrivalTime() <= currentTime && P[i].getRemainingTime() > 0 && P[i].getRemainingTime() < minRemainingTime) {
                minRemainingTime = P[i].getRemainingTime();
                idx = i;
            }
        }

        if (idx != -1) {
            currentTime++;
            P[idx].setRemainingTime(P[idx].getRemainingTime() - 1);

            if (P[idx].getRemainingTime() == 0) {
                completed++;
                P[idx].setcompletionTime(currentTime);
                P[idx].setTurnaroundTime(P[idx].getcompletionTime() - P[idx].getarrivalTime());
                P[idx].setwaitingTime(P[idx].getTurnaroundTime() - P[idx].getburstTime());
                totalWaitTime += P[idx].getwaitingTime();
                totalTurnaroundTime += P[idx].getTurnaroundTime();
            }
        } else {
            currentTime++;
        }
    }

    float avgWaitTime = totalWaitTime / numProcesses;
    float avgTurnaroundTime = totalTurnaroundTime / numProcesses;
    json result;
    for (int i = 0; i < numProcesses; i++) {
        result["processes"].push_back({
            {"id", P[i].getId()},
            {"arrivalTime", P[i].getarrivalTime()},
            {"burstTime", P[i].getburstTime()},
            {"completionTime", P[i].getcompletionTime()},
            {"turnaroundTime", P[i].getTurnaroundTime()},
            {"waitingTime", P[i].getwaitingTime()}
        });
    }
    result["avgWaitTime"] = avgWaitTime;
    result["avgTurnaroundTime"] = avgTurnaroundTime;
    cout << result.dump(4) << endl; // Output the result as JSON

}

void SJF_NonPreemptive(Process P[], int numProcesses) {
    sort(P, P + numProcesses, [](Process p, Process q) { return p.getarrivalTime() < q.getarrivalTime(); });
    int currentTime = 0;
    float totalWaitTime = 0, totalTurnaroundTime = 0;

    for (int i = 0; i < numProcesses; i++) {
        int shortestIndex = i;
        for (int j = i + 1; j < numProcesses; j++) {
            if (P[j].getarrivalTime() <= currentTime && P[j].getburstTime() < P[shortestIndex].getburstTime()) {
                shortestIndex = j;
            }
        }
        swap(P[i], P[shortestIndex]);
        if (currentTime < P[i].getarrivalTime()) {
            currentTime = P[i].getarrivalTime();
        }
        currentTime += P[i].getburstTime();
        P[i].setcompletionTime(currentTime);
        P[i].setTurnaroundTime(P[i].getcompletionTime() - P[i].getarrivalTime());
        P[i].setwaitingTime(P[i].getTurnaroundTime() - P[i].getburstTime());
        totalWaitTime += P[i].getwaitingTime();
        totalTurnaroundTime += P[i].getTurnaroundTime();
    }

    float avgWaitTime = totalWaitTime / numProcesses;
    float avgTurnaroundTime = totalTurnaroundTime / numProcesses;
    json result;
    for (int i = 0; i < numProcesses; i++) {
        result["processes"].push_back({
            {"id", P[i].getId()},
            {"arrivalTime", P[i].getarrivalTime()},
            {"burstTime", P[i].getburstTime()},
            {"completionTime", P[i].getcompletionTime()},
            {"turnaroundTime", P[i].getTurnaroundTime()},
            {"waitingTime", P[i].getwaitingTime()}
        });
    }
    result["avgWaitTime"] = avgWaitTime;
    result["avgTurnaroundTime"] = avgTurnaroundTime;
    cout << result.dump(4) << endl; // Output the result as JSON
}

void RoundRobin(Process P[], int numProcesses, int quantum) {
    queue<int> q;
    int currentTime = 0;
    int remainingProcesses = numProcesses;
    float totalWaitTime = 0, totalTurnaroundTime = 0;
    bool inQueue[numProcesses] = {false};

    for (int i = 0; i < numProcesses; i++) {
        if (P[i].getarrivalTime() <= currentTime) {
            q.push(i);
            inQueue[i] = true;
        }
    }

    while (remainingProcesses > 0) {
        if (q.empty()) {
            currentTime++;
            for (int i = 0; i < numProcesses; i++) {
                if (P[i].getarrivalTime() <= currentTime && P[i].getRemainingTime() > 0 && !inQueue[i]) {
                    q.push(i);
                    inQueue[i] = true;
                }
            }
            continue;
        }
        
        int i = q.front();
        q.pop();
        inQueue[i] = false;

        int timeSlice = min(quantum, P[i].getRemainingTime());
        currentTime += timeSlice;
        P[i].setRemainingTime(P[i].getRemainingTime() - timeSlice);

        for (int j = 0; j < numProcesses; j++) {
            if (P[j].getarrivalTime() <= currentTime && P[j].getRemainingTime() > 0 && !inQueue[j]) {
                q.push(j);
                inQueue[j] = true;
            }
        }

        if (P[i].getRemainingTime() == 0) {
            remainingProcesses--;
            P[i].setcompletionTime(currentTime);
            P[i].setTurnaroundTime(P[i].getcompletionTime() - P[i].getarrivalTime());
            P[i].setwaitingTime(P[i].getTurnaroundTime() - P[i].getburstTime());
            totalWaitTime += P[i].getwaitingTime();
            totalTurnaroundTime += P[i].getTurnaroundTime();
        } else {
            q.push(i);
            inQueue[i] = true;
        }
    }

    float avgWaitTime = totalWaitTime / numProcesses;
    float avgTurnaroundTime = totalTurnaroundTime / numProcesses;
   json result;
    for (int i = 0; i < numProcesses; i++) {
        result["processes"].push_back({
            {"id", P[i].getId()},
            {"arrivalTime", P[i].getarrivalTime()},
            {"burstTime", P[i].getburstTime()},
            {"completionTime", P[i].getcompletionTime()},
            {"turnaroundTime", P[i].getTurnaroundTime()},
            {"waitingTime", P[i].getwaitingTime()}
        });
    }
    result["avgWaitTime"] = avgWaitTime;
    result["avgTurnaroundTime"] = avgTurnaroundTime;
    cout << result.dump(4) << endl; // Output the result as JSON
}

int main(int argc, char * argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <schedulerType> <quantum> <processes>" << endl;
        return 1;
    }

    string schedulerType = (string)argv[1];
      int quantum = (int)argv[2];
    json processesJson = json::parse(argv[3]);
    int numProcesses = processesJson.size();
    Process P[numProcesses];

    for (int i = 0; i <= numProcesses; i++) {
        P[i].setId(processesJson[i]["id"]);
        P[i].setarrivalTime(processesJson[i]["arrivalTime"]);
        P[i].setburstTime(processesJson[i]["burstTime"]);
        P[i].setRemainingTime(processesJson[i]["burstTime"]); // Initialize remaining time
    }

    if (schedulerType == "FCFS") {
         FCFS(P, numProcesses);

     } else if (schedulerType == "SJF_Preemptive") {
        SJF_Preemptive(P, numProcesses);
   } else if (schedulerType == "SJF_NonPreemptive") {
        SJF_NonPreemptive(P, numProcesses);
     } else if (schedulerType == "RoundRobin") {
        RoundRobin(P, numProcesses, quantum);
     } else {
         cerr << "Invalid scheduler type!" << endl;
         
     }
    return 0;
 
}
