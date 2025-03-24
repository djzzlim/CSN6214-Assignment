# OS Scheduling Calculator  

This project is a **Java-based Operating System Scheduling Simulator** that allows users to visualize different CPU scheduling algorithms. The application provides a graphical user interface (GUI) to help users understand scheduling concepts through Gantt charts and process statistics.  

## Features  

- Supports multiple scheduling algorithms:
  - **First-Come, First-Served (FCFS)**
  - **Shortest Job First (SJF) / Shortest Job Next (SJN)**
  - **Round Robin (RR)**
  - **Priority Scheduling (Preemptive & Non-Preemptive)**
- Displays a **Gantt Chart** for scheduling visualization.
- Calculates **Turnaround Time, Waiting Time, and Response Time**.
- User-friendly **Graphical Interface** built using Java Swing.

## Files Overview  

- **`Main.java`** – Entry point of the application.  
- **`MainWindow.java`** – Handles the GUI and user interactions.  
- **`Controller.java`** – Implements the scheduling logic and manages process execution.  
- **`GanttChart.java`** – Generates and displays the Gantt chart for visual representation.  
- **`Process.java`** – Represents a process with attributes like burst time, arrival time, and priority.  

## Getting Started  

### 1. Clone the Repository  

```bash
git clone https://github.com/djzzlim/CSN6214-Assignment.git
cd CSN6214-Assignment
```

### 2. Compile the Java Files  

Ensure you have **JDK 8 or later** installed. Compile the program using:  

```bash
javac *.java
```

### 3. Run the Application  

Execute the main class:  

```bash
java Main
```

The GUI should open, allowing you to input process details and select a scheduling algorithm.  

## Example Usage  

1. Enter the number of processes.  
2. Input **arrival time, burst time, and priority (if applicable)**.  
3. Select a **CPU scheduling algorithm** from the GUI.  
4. Click **"Run"** to see the Gantt Chart and computed statistics.  

## Contributing  

Feel free to fork this repository and improve the scheduler by adding more features or optimizing the algorithms.  

## License  

This project is licensed under the MIT License.  

---
