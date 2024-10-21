#include "Controller.h"
#include <algorithm>
#include <queue>

bool Controller::allProcessesFinished() {
    for (const auto& process : processes) {
        if (!process.finished()) {
            return false;
        }
    }
    return true;
}

void Controller::executeRoundRobin() {
    int currentTime = 0;

    stable_sort(processes.begin(), processes.end(), [](const Process &p1, const Process &p2)
                {
                    if (p1.getArrivalTime() == p2.getArrivalTime()) {
                        return p1.getPriority() < p2.getPriority();
                    }
                    return p1.getArrivalTime() < p2.getArrivalTime();
                });

    while (!allProcessesFinished()) {
        bool processExecuted = false;
        for (auto& process : processes) {
            if (process.getArrivalTime() <= currentTime && !process.finished()) {
                processExecuted = true;
                int timeSlice = min(quantumTime, process.getRemainingBurstTime());

                ganttChartQueue.push({process.getProcessID(), currentTime, timeSlice});

                currentTime += timeSlice;
                process.reduceRemainingBurstTime(timeSlice);

                if (process.getRemainingBurstTime() == 0) {
                    process.setCompletionTime(currentTime);
                    process.markAsFinished();
                }
            }
        }
        if (!processExecuted) {
            currentTime++;
        }
    }

    for (auto& process : processes) {
        process.calculateTurnaroundTime();
        process.calculateWaitingTime();
    }

    for (auto& process : processes) {
        totalTurnaroundTime += process.getTurnaroundTime();
        totalWaitingTime += process.getWaitingTime();
    }

    averageTurnaroundTime = static_cast<double>(totalTurnaroundTime) / static_cast<double>(processes.size());
    averageWaitingTime = static_cast<double>(totalWaitingTime) / static_cast<double>(processes.size());
}

void Controller::executeNonPreemptivePriority() {
    int currentTime = 0;

    stable_sort(processes.begin(), processes.end(), [](const Process &p1, const Process &p2) {
        if (p1.getArrivalTime() == p2.getArrivalTime()) {
            return p1.getPriority() < p2.getPriority();
        }
        return p1.getArrivalTime() < p2.getArrivalTime();
    });

    while (!allProcessesFinished()) {
        auto nextProcess = processes.end();
        for (auto it = processes.begin(); it != processes.end(); ++it) {
            if (it->getArrivalTime() <= currentTime && !it->finished()) {
                if (nextProcess == processes.end() || it->getPriority() < nextProcess->getPriority()) {
                    nextProcess = it;
                }
            }
        }

        if (nextProcess != processes.end()) {
            // Execute the selected process to completion
            int timeSlice = nextProcess->getRemainingBurstTime(); // Total burst time for Gantt entry
            ganttChartQueue.push({nextProcess->getProcessID(), currentTime, timeSlice});
            currentTime += timeSlice;
            nextProcess->setCompletionTime(currentTime);
            nextProcess->markAsFinished();
        } else {
            currentTime++;
        }
    }

    for (auto& process : processes) {
        process.calculateTurnaroundTime();
        process.calculateWaitingTime();
    }

    for (auto& process : processes) {
        totalTurnaroundTime += process.getTurnaroundTime();
        totalWaitingTime += process.getWaitingTime();
    }

    averageTurnaroundTime = static_cast<double>(totalTurnaroundTime) / static_cast<double>(processes.size());
    averageWaitingTime = static_cast<double>(totalWaitingTime) / static_cast<double>(processes.size());
}

void Controller::executeNonPreemptiveSJF() {
    int currentTime = 0;

    stable_sort(processes.begin(), processes.end(), [](const Process &p1, const Process &p2) {
        if (p1.getArrivalTime() == p2.getArrivalTime()) {
            return p1.getPriority() < p2.getPriority();
        }
        return p1.getArrivalTime() < p2.getArrivalTime();
    });

    while (!allProcessesFinished()) {
        auto nextProcess = processes.end();
        for (auto it = processes.begin(); it != processes.end(); ++it) {
            if (it->getArrivalTime() <= currentTime && !it->finished()) {
                if (nextProcess == processes.end() || it->getInitialBurstTime() < nextProcess->getInitialBurstTime()) {
                    nextProcess = it;
                }
            }
        }

        if (nextProcess != processes.end()) {
            int timeSlice = nextProcess->getInitialBurstTime(); // Total burst time for Gantt entry
            ganttChartQueue.push({nextProcess->getProcessID(), currentTime, timeSlice});
            currentTime += timeSlice;
            nextProcess->setCompletionTime(currentTime);
            nextProcess->markAsFinished();
        } else {
            currentTime++;
        }
    }

    for (auto& process : processes) {
        process.calculateTurnaroundTime();
        process.calculateWaitingTime();
    }

    for (auto& process : processes) {
        totalTurnaroundTime += process.getTurnaroundTime();
        totalWaitingTime += process.getWaitingTime();
    }

    averageTurnaroundTime = static_cast<double>(totalTurnaroundTime) / static_cast<double>(processes.size());
    averageWaitingTime = static_cast<double>(totalWaitingTime) / static_cast<double>(processes.size());
}

void Controller::executePreemptivePriority() {
    int currentTime = 0;
    Process* currentProcess = nullptr;

    while (!allProcessesFinished()) {
        auto nextProcess = processes.end();
        for (auto it = processes.begin(); it != processes.end(); ++it) {
            if (it->getArrivalTime() <= currentTime && !it->finished()) {
                if (nextProcess == processes.end() || it->getPriority() < nextProcess->getPriority()) {
                    nextProcess = it;
                }
            }
        }

        if (nextProcess != processes.end()) {
            if (!currentProcess || nextProcess->getPriority() < currentProcess->getPriority()) {
                // If currentProcess is running, push its Gantt entry
                if (currentProcess) {
                    ganttChartQueue.push({currentProcess->getProcessID(), currentTime - 1, 1}); // Log the previous process
                }
                currentProcess = &(*nextProcess);
            }

            currentProcess->reduceRemainingBurstTime(1);
            currentTime++;

            if (currentProcess->getRemainingBurstTime() == 0) {
                currentProcess->setCompletionTime(currentTime);
                currentProcess->markAsFinished();
                ganttChartQueue.push({currentProcess->getProcessID(), currentTime - 1, 1}); // Log the finished process
                currentProcess = nullptr;
            }
        } else {
            currentTime++;
        }
    }

    for (auto& process : processes) {
        process.calculateTurnaroundTime();
        process.calculateWaitingTime();
    }

    for (auto& process : processes) {
        totalTurnaroundTime += process.getTurnaroundTime();
        totalWaitingTime += process.getWaitingTime();
    }

    averageTurnaroundTime = static_cast<double>(totalTurnaroundTime) / static_cast<double>(processes.size());
    averageWaitingTime = static_cast<double>(totalWaitingTime) / static_cast<double>(processes.size());
}

void Controller::executePreemptiveSJF() {
    int currentTime = 0;

    while (!allProcessesFinished()) {
        auto shortestJob = processes.end();
        for (auto it = processes.begin(); it != processes.end(); ++it) {
            if (it->getArrivalTime() <= currentTime && !it->finished()) {
                if (shortestJob == processes.end() || it->getRemainingBurstTime() < shortestJob->getRemainingBurstTime()) {
                    shortestJob = it;
                }
            }
        }

        if (shortestJob != processes.end()) {
            ganttChartQueue.push({shortestJob->getProcessID(), currentTime, 1}); // Log the process execution for 1 unit
            shortestJob->reduceRemainingBurstTime(1);
            currentTime++;

            if (shortestJob->getRemainingBurstTime() == 0) {
                shortestJob->setCompletionTime(currentTime);
                shortestJob->markAsFinished();
            }
        } else {
            currentTime++;
        }
    }

    for (auto& process : processes) {
        process.calculateTurnaroundTime();
        process.calculateWaitingTime();
    }

    for (auto& process : processes) {
        totalTurnaroundTime += process.getTurnaroundTime();
        totalWaitingTime += process.getWaitingTime();
    }

    averageTurnaroundTime = static_cast<double>(totalTurnaroundTime) / static_cast<double>(processes.size());
    averageWaitingTime = static_cast<double>(totalWaitingTime) / static_cast<double>(processes.size());
}

