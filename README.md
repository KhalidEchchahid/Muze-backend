# Maze Pathfinding Visualizer

## Overview
This project is a web-based **pathfinding visualizer** that allows users to explore and compare two well-known pathfinding algorithms:
- **Dijkstra’s Algorithm**
- **Breadth-First Search (BFS)**

The application provides a graphical representation of a **maze**, where the algorithms navigate from a given **start** position to an **end** position. The backend logic for the pathfinding is implemented in **C++**, while the frontend and API layer are developed using **Next.js**.

---

## Features
- **Maze Visualization:** Displays a grid-based environment where the pathfinding algorithms operate.
- **Dijkstra vs. BFS Comparison Mode:** Runs both algorithms side by side and highlights the differences.
- **Performance Metrics:** Provides statistics on:
  - Number of **cells visited**
  - **Path length** (shortest path found)
  - **Execution time**
- **Web-based Interface:** Built with **React.js (Next.js)** for seamless user interaction.
- **Backend in C++:** Handles pathfinding logic efficiently.
- **Logging Mechanism:** Stores execution logs for debugging and analysis.

---

## Project Architecture

### 1. Frontend (Next.js)
The frontend is responsible for user interaction, visualization, and API communication. It sends a request to the backend with the maze grid and algorithm selection.

### 2. Backend API (Next.js with Node.js & Child Process)
The backend API in Next.js acts as a bridge between the frontend and the C++ executable. It:
- Accepts POST requests containing **grid data, start position, end position, and the selected algorithm**.
- Saves the data into a **temporary JSON file**.
- Executes the **C++ program** (`Muze-backend.exe`) using the **Node.js child process**.
- Reads the output from the C++ program and returns it as a JSON response to the frontend.

### 3. Pathfinding Logic (C++ Backend)
The C++ program receives the JSON input file from the API, processes the grid data, and executes the selected algorithm (**Dijkstra or BFS**).

---

## Installation & Setup

### Prerequisites
Ensure you have the following installed:
- **Node.js** & **npm** (for the Next.js frontend & API layer)
- **C++ Compiler** (for backend logic)

### Steps
1. **Clone the repository:**
   ```bash
   git clone https://github.com/khalidEchchahid/Muze-Dijkstra-BFS-nextjs-c-.git
   ```
2. **Install dependencies:**
   ```bash
   npm install
   ```

3. **Start the Next.js server:**
   ```bash
   npm run dev
   ```
4. Open your browser and go to `http://localhost:3000`

---

## Usage
- **Select a start and end point** on the grid.
- **Choose an algorithm** (Dijkstra or BFS) from the options.
- Click **Run Algorithm** to visualize the pathfinding process.
- Compare performance metrics displayed in real-time.
You can also click on Start tutrial  in the  navbar to learn more 
---

## Technologies Used
- **Frontend:** Next.js 
- **Backend API:** Next.js API Routes (Node.js, child_process)
- **Pathfinding Logic:** C++
- **Data Format:** JSON

---

## Contributing
If you want to contribute, follow these steps:
1. Fork the repository.
2. Create a new branch: `git checkout -b feature-name`
3. Commit your changes: `git commit -m 'Add feature-name'`
4. Push to the branch: `git push origin feature-name`
5. Open a Pull Request

![image](https://github.com/user-attachments/assets/271f3da6-5b6b-457c-ac2d-84bba3f6256d)
![image](https://github.com/user-attachments/assets/d7e19ff6-a763-41b5-b92b-061dbfa73fd5)
