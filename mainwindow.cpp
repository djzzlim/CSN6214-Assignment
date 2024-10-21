#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Controller.h"
#include "Process.h"
#include "ganttChart.h"
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QIntValidator>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , processCounter(2) // Start with 2 processes
{
    ui->setupUi(this);
    setWindowTitle("CPU Scheduling");

    // Create integer validators
    QIntValidator *intValidator = new QIntValidator(this);
    intValidator->setBottom(0); // Set minimum value for arrival time and burst time

    ui->timeQuantumInput->setValidator(intValidator);

    // Set initial rows
    initializeTable(); // Call the function to initialize the table

    // Hide the vertical header
    ui->processTable->verticalHeader()->setVisible(false);
}

MainWindow::~MainWindow() {
    delete ui;
}


// Function to initialize the table
void MainWindow::initializeTable() {
    ui->processTable->setRowCount(processCounter);
    for (int i = 0; i < processCounter; ++i) {
        // Process ID
        QTableWidgetItem *processIdItem = new QTableWidgetItem(QString::number(i + 1));
        processIdItem->setFlags(processIdItem->flags() & ~Qt::ItemIsEditable); // Make Process ID read-only
        ui->processTable->setItem(i, 0, processIdItem);

        // Set Completion Time, Turnaround Time, and Waiting Time as read-only
        ui->processTable->setItem(i, 4, new QTableWidgetItem("")); // Completion Time
        ui->processTable->setItem(i, 5, new QTableWidgetItem("")); // Turnaround Time
        ui->processTable->setItem(i, 6, new QTableWidgetItem("")); // Waiting Time

        // Make these items read-only
        ui->processTable->item(i, 4)->setFlags(ui->processTable->item(i, 4)->flags() & ~Qt::ItemIsEditable);
        ui->processTable->item(i, 5)->setFlags(ui->processTable->item(i, 5)->flags() & ~Qt::ItemIsEditable);
        ui->processTable->item(i, 6)->setFlags(ui->processTable->item(i, 6)->flags() & ~Qt::ItemIsEditable);
    }

    // Set the column count and header labels
    ui->processTable->setColumnCount(7);
    ui->processTable->setHorizontalHeaderLabels({"Process ID", "Arrival Time", "Burst Time", "Priority", "Completion Time", "Turnaround Time", "Waiting Time"});

    // Set the initial widths of columns
    ui->processTable->setColumnWidth(0, 200); // Process ID
    ui->processTable->setColumnWidth(1, 200); // Arrival Time
    ui->processTable->setColumnWidth(2, 200); // Burst Time
    ui->processTable->setColumnWidth(3, 200); // Priority
    ui->processTable->setColumnWidth(4, 250); // Completion Time
    ui->processTable->setColumnWidth(5, 250); // Turnaround Time
    ui->processTable->setColumnWidth(6, 250); // Waiting Time

    // Adjust the table to fit its contents
    ui->processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void MainWindow::on_addButton_clicked() {
    int currentRowCount = ui->processTable->rowCount();
    ui->processTable->insertRow(currentRowCount); // Add a new row
    ui->processTable->setItem(currentRowCount, 0, new QTableWidgetItem(QString::number(currentRowCount + 1))); // Increment Process ID

    // Set Completion Time, Turnaround Time, and Waiting Time as read-only
    ui->processTable->setItem(currentRowCount, 4, new QTableWidgetItem("")); // Completion Time
    ui->processTable->setItem(currentRowCount, 5, new QTableWidgetItem("")); // Turnaround Time
    ui->processTable->setItem(currentRowCount, 6, new QTableWidgetItem("")); // Waiting Time

    // Make these items read-only
    ui->processTable->item(currentRowCount, 4)->setFlags(ui->processTable->item(currentRowCount, 4)->flags() & ~Qt::ItemIsEditable);
    ui->processTable->item(currentRowCount, 5)->setFlags(ui->processTable->item(currentRowCount, 5)->flags() & ~Qt::ItemIsEditable);
    ui->processTable->item(currentRowCount, 6)->setFlags(ui->processTable->item(currentRowCount, 6)->flags() & ~Qt::ItemIsEditable);
}

void MainWindow::on_deleteButton_clicked() {
    int currentRowCount = ui->processTable->rowCount();
    if (currentRowCount > 2) { // Ensure there are rows to delete
        ui->processTable->removeRow(currentRowCount - 1); // Remove the last row
    } else {
        QMessageBox::warning(this, "Warning", "Cannot delete the last two rows.");
    }
}

void MainWindow::on_resetButton_clicked() {
    ui->processTable->setRowCount(2); // Reset the table to 2 rows
    initializeTable(); // Reinitialize the table data

    // Reset Arrival Time, Burst Time, and Priority columns to empty
    for (int i = 0; i < 2; ++i) {
        ui->processTable->setItem(i, 1, new QTableWidgetItem("")); // Arrival Time
        ui->processTable->setItem(i, 2, new QTableWidgetItem("")); // Burst Time
        ui->processTable->setItem(i, 3, new QTableWidgetItem("")); // Priority

        // Reset Completion Time, Turnaround Time, and Waiting Time to read-only
        ui->processTable->setItem(i, 4, new QTableWidgetItem("")); // Completion Time
        ui->processTable->setItem(i, 5, new QTableWidgetItem("")); // Turnaround Time
        ui->processTable->setItem(i, 6, new QTableWidgetItem("")); // Waiting Time

        // Make these items read-only
        ui->processTable->item(i, 4)->setFlags(ui->processTable->item(i, 4)->flags() & ~Qt::ItemIsEditable);
        ui->processTable->item(i, 5)->setFlags(ui->processTable->item(i, 5)->flags() & ~Qt::ItemIsEditable);
        ui->processTable->item(i, 6)->setFlags(ui->processTable->item(i, 6)->flags() & ~Qt::ItemIsEditable);
    }

    if (ui->ganttChartView->scene() != nullptr) {
        ui->ganttChartView->scene()->clear();
        delete ui->ganttChartView->scene();
    }

    ui->totalTurnaroundTimeOutput->setText("");
    ui->averageTurnaroundTimeOutput->setText("");
    ui->totalWaitingTimeOutput->setText("");
    ui->averageWaitingTimeOutput->setText("");
}

void MainWindow::on_calculateButton_clicked() {
    int timeQuantum = 0;
    int numProcesses = ui->processTable->rowCount();
    std::vector<Process> processes;

    // Validate timeQuantum input
    bool validTimeQuantum = false;
    timeQuantum = ui->timeQuantumInput->text().toInt(&validTimeQuantum);
    if (!validTimeQuantum || timeQuantum <= 0) {
        QMessageBox::warning(this, "Warning", "Please enter a valid positive integer for Time Quantum.");
        return; // Exit the function if the input is invalid
    }

    // Retrieve process data from the table
    for (int i = 0; i < numProcesses; ++i) {
        // Validate Arrival Time
        QTableWidgetItem *arrivalItem = ui->processTable->item(i, 1);
        if (arrivalItem == nullptr || arrivalItem->text().isEmpty()) {
            QMessageBox::warning(this, "Warning", QString("Arrival Time for Process %1 is missing. Please enter a valid non-negative integer.").arg(i + 1));
            return;
        }
        bool validArrivalTime;
        int arrivalTime = arrivalItem->text().toInt(&validArrivalTime);
        if (!validArrivalTime || arrivalTime < 0) {
            QMessageBox::warning(this, "Warning", QString("Invalid Arrival Time for Process %1. Please enter a valid non-negative integer.").arg(i + 1));
            return;
        }

        // Validate Burst Time
        QTableWidgetItem *burstItem = ui->processTable->item(i, 2);
        if (burstItem == nullptr || burstItem->text().isEmpty()) {
            QMessageBox::warning(this, "Warning", QString("Burst Time for Process %1 is missing. Please enter a valid positive integer.").arg(i + 1));
            return;
        }
        bool validBurstTime;
        int burstTime = burstItem->text().toInt(&validBurstTime);
        if (!validBurstTime || burstTime <= 0) {
            QMessageBox::warning(this, "Warning", QString("Invalid Burst Time for Process %1. Please enter a valid positive integer.").arg(i + 1));
            return;
        }

        // Validate Priority
        QTableWidgetItem *priorityItem = ui->processTable->item(i, 3);
        if (priorityItem == nullptr || priorityItem->text().isEmpty()) {
            QMessageBox::warning(this, "Warning", QString("Priority for Process %1 is missing. Please enter a valid non-negative integer.").arg(i + 1));
            return;
        }
        bool validPriority;
        int priority = priorityItem->text().toInt(&validPriority);
        if (!validPriority || priority < 0) {
            QMessageBox::warning(this, "Warning", QString("Invalid Priority for Process %1. Please enter a valid non-negative integer.").arg(i + 1));
            return;
        }

        // Add process to the list
        processes.emplace_back(i + 1, arrivalTime, burstTime, priority);
    }


    Controller controller(processes, timeQuantum);

    QString selectedAlgorithm = ui->algorithmComboBox->currentText();

    // You can now use selectedAlgorithm to switch between different scheduling algorithms
    // For example:
    if (selectedAlgorithm == "Preemptive SJF") {
        controller.executePreemptiveSJF();
    } else if (selectedAlgorithm == "Non Preemptive SJF") {
        controller.executeNonPreemptiveSJF();
    } else if (selectedAlgorithm == "Preemptive Priority") {
        controller.executePreemptivePriority();
    } else if (selectedAlgorithm == "Non Preemptive Priority") {
        controller.executeNonPreemptivePriority();
    } else if (selectedAlgorithm == "Round Robin") {
        controller.executeRoundRobin();
    }


    // Display process information in the table
    for (int i = 0; i < numProcesses; ++i) {
        ui->processTable->item(i, 4)->setText(QString::number(processes[i].getCompletionTime()));   // Completion Time
        ui->processTable->item(i, 5)->setText(QString::number(processes[i].getTurnaroundTime()));  // Turnaround Time
        ui->processTable->item(i, 6)->setText(QString::number(processes[i].getWaitingTime()));      // Waiting Time
    }

    // Remove the existing scene if it exists
    if (ui->ganttChartView->scene() != nullptr) {
        ui->ganttChartView->scene()->clear();
        delete ui->ganttChartView->scene();
    }

    QGraphicsScene *scene = new QGraphicsScene(this);  // Create a scene to hold the Gantt chart
    scene->setSceneRect(0, 0, 955, 225);
    GanttChart chart;
    chart.ganttChart(scene, controller);

    // Display the Gantt chart in the UI's QGraphicsView
    ui->ganttChartView->setScene(scene);

    ui->totalTurnaroundTimeOutput->setText(QString::number(controller.getTotalTurnaroundTime()));
    ui->averageTurnaroundTimeOutput->setText(QString::number(controller.getAverageTurnaroundTime()));
    ui->totalWaitingTimeOutput->setText(QString::number((controller.getTotalWaitingTime())));
    ui->averageWaitingTimeOutput->setText(QString::number(controller.getAverageWaitingTime()));
}


