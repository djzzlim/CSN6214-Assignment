#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <queue>
#include "Process.h"

using namespace std;

struct GanttChartEntry {
    int processID;  // ID or identifier of the process
    int startTime;  // When the process starts execution
    int duration;   // How long the process executes in this round
};

class Controller
{
private:
    vector<Process>& processes;
    int quantumTime;
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;
    double averageTurnaroundTime = 0.0;
    double averageWaitingTime = 0.0;

    queue<GanttChartEntry> ganttChartQueue;

public:
    Controller(vector<Process>& processes, int quantumTime)
        : processes(processes), quantumTime(quantumTime) {}

    void executeRoundRobin();
    void executeNonPreemptivePriority();
    void executeNonPreemptiveSJF();
    void executePreemptivePriority();
    void executePreemptiveSJF();

    bool allProcessesFinished();

    const queue<GanttChartEntry>& getGanttChartData() const { return ganttChartQueue; }

    int getTotalTurnaroundTime() const { return totalTurnaroundTime; }
    int getTotalWaitingTime() const { return totalWaitingTime; }
    double getAverageTurnaroundTime() const { return averageTurnaroundTime; }
    double getAverageWaitingTime() const { return averageWaitingTime; }
};

#endif
