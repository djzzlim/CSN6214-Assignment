#ifndef GANTTCHART_H
#define GANTTCHART_H

#include <QGraphicsScene>
#include <QVector>
#include <QPair>
#include "Controller.h"

class GanttChart {
public:
    void ganttChart(QGraphicsScene *scene, const Controller& rrController);  // Pass the controller to get the queue data
};

#endif // GANTTCHART_H
