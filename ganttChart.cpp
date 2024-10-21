#include "ganttChart.h"
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>

void GanttChart::ganttChart(QGraphicsScene *scene, const Controller& rrController) {
    int height = 30;         // Height of each bar
    int currentX = 50;       // Start drawing from this X position
    int scaleFactor = 20;    // Scale factor to enlarge the rectangles
    int lastFinishTime = 0;  // Variable to keep track of the finish time of the last process

    // Retrieve the queue containing Gantt chart data from the RoundRobinController
    std::queue<GanttChartEntry> ganttChartQueue = rrController.getGanttChartData();

    // Process the Gantt chart queue
    while (!ganttChartQueue.empty()) {
        // Get the next entry from the queue
        GanttChartEntry entry = ganttChartQueue.front();
        ganttChartQueue.pop();

        QString processName = QString("P%1").arg(entry.processID);  // Example process name (P1, P2, etc.)
        int duration = entry.duration * scaleFactor;  // Scale the duration
        int arrivalTime = entry.startTime;  // Get the arrival (start) time for this process

        // Add the horizontal bar to the scene with white fill and black border
        scene->addRect(currentX, 50, duration, height, QPen(Qt::black), QBrush(Qt::white));

        // Add text for the process name above the rectangle
        QGraphicsTextItem *processTextItem = scene->addText(processName);
        // Center the text horizontally and set Y position above the rectangle
        processTextItem->setPos(currentX + (duration / 2) - 10, 25);

        // Add text for the arrival time at the start of the rectangle (aligned with the left edge)
        QGraphicsTextItem *arrivalTextItem = scene->addText(QString::number(arrivalTime));
        // Position the arrival time at the left edge of the rectangle (currentX) and below the rectangle
        arrivalTextItem->setPos(currentX - 6, 85);

        // Update the finish time
        lastFinishTime = arrivalTime + entry.duration;

        // Update the X position for the next rectangle
        currentX += duration;
    }

    // After processing all entries, display the finish time for the last process
    QGraphicsTextItem *finishTextItem = scene->addText(QString::number(lastFinishTime));
    // Position the finish time below the last rectangle
    finishTextItem->setPos(currentX - 6, 85);  // Align with the end of the last bar
}
