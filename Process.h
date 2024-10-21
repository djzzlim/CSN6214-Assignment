#ifndef PROCESS_H
#define PROCESS_H

class Process
{
private:
    int processID;          
    int arrivalTime;        
    int initialBurstTime;   
    int remainingBurstTime; 
    int completionTime;     
    int turnaroundTime;     
    int waitingTime;        
    bool isFinished;        
    int priority;           

public:
    Process(int processID, int arrivalTime, int initialBurstTime, int priority)
    {
        this->processID = processID;
        this->arrivalTime = arrivalTime;
        this->initialBurstTime = initialBurstTime;
        this->priority = priority;
        remainingBurstTime = initialBurstTime;
        completionTime = 0;
        turnaroundTime = 0;
        waitingTime = 0;
        isFinished = false;
    }

    // Getters
    int getProcessID() const { return processID; }
    int getArrivalTime() const { return arrivalTime; }
    int getInitialBurstTime() const { return initialBurstTime; }
    int getRemainingBurstTime() const { return remainingBurstTime; }
    int getCompletionTime() const { return completionTime; }
    int getTurnaroundTime() const { return turnaroundTime; }
    int getWaitingTime() const { return waitingTime; }
    bool finished() const { return isFinished; }
    int getPriority() const { return priority; }

    // Setters
    void reduceRemainingBurstTime(int quantum) {
        remainingBurstTime -= quantum;
        if (remainingBurstTime < 0) remainingBurstTime = 0;
    }
    void setCompletionTime(int time) { completionTime = time; }
    void calculateTurnaroundTime() { turnaroundTime = completionTime - arrivalTime; }
    void calculateWaitingTime() { waitingTime = turnaroundTime - initialBurstTime; }
    void markAsFinished() { isFinished = true; }
};

#endif
