# Hexagonal Grid Pathfinding System

A comprehensive C implementation of pathfinding algorithms on hexagonal grids with dynamic cost modification and air route management.[1]

##  Overview

This project implements a sophisticated pathfinding system designed for hexagonal grids, featuring **Dijkstra's algorithm** for optimal path finding, **BFS** for distance calculations, and a complete command-line interface for interactive grid manipulation. Originally developed as part of an Algorithms and Computer Science Principles project at Politecnico di Milano.[1]

##  Key Features

- **Hexagonal Grid System**: Native support for hexagonal coordinate systems with proper neighbor detection
- **Dijkstra's Algorithm**: Optimal pathfinding with custom cost functions and priority queue implementation
- **Dynamic Cost Modification**: Real-time adjustment of cell traversal costs with radius-based effects
- **Air Route Management**: Create and toggle aerial connections between distant cells
- **Min-Heap Priority Queue**: Efficient data structure for optimal pathfinding performance
- **BFS Distance Calculation**: Fast hexagonal distance computation for grid operations
- **Memory Management**: Robust allocation and deallocation with overflow protection

##  Architecture

### Core Data Structures

| Structure | Purpose | Description |
|-----------|---------|-------------|
| `Rotta` | Air Routes | Manages aerial connections between cells |
| `Cella` | Grid Cells | Individual hexagonal cells with costs and routes |
| `Mappa` | Grid Map | Main grid structure with dimensions and cell array |
| `MinHeap` | Priority Queue | Efficient min-heap for Dijkstra's algorithm |
| `Queue` | BFS Queue | FIFO queue for breadth-first search operations |

### Key Algorithms

- **Dijkstra's Algorithm**: Finds shortest paths with custom cost functions[2]
- **Breadth-First Search**: Calculates hexagonal distances efficiently[3]
- **Hexagonal Coordinate System**: Handles even/odd row neighbor calculations[4]
- **Min-Heap Operations**: Maintains priority queue for optimal performance

##  Compilation

```bash
gcc -Wall -Werror -std=gnu11 -O2 -l```4.c -o hexpath
```

**Requirements:**
- GCC 11 or compatible C compiler
- Standard C library with math functions (`-lm`)

##  Usage

The program accepts commands via standard input with the following interface:

### Available Commands

| Command | Syntax | Description |
|---------|--------|-------------|
| `init` | `init <columns> <rows>` | Initialize hexagonal grid |
| `travel_cost` | `travel_cost <x1> <y1> <x2> <y2>` | Calculate shortest path cost |
| `change_cost` | `change_cost <x> <y> <value> <radius>` | Modify cell costs in radius |
| `toggle_air_route` | `toggle_air_route <x1> <y1> <x2> <y2>` | Add/remove air routes |

### Example Usage

```bash
# Initialize a 10x8 hexagonal grid
init``` 8

# Calculate travel```st from (0,0) to```,3)
travel_cost 0 ``` 3

# Increase costs around (3,2) with```dius 2
change_cost 3```5 2

# Create```r route from (1,1) to (7```
toggle_air_route ``` 7 5
```

##  Command Details

### Grid Initialization
```bash
init <columns> <rows>
```
Creates a hexagonal grid with specified dimensions. All cells start with default exit cost of 1.[1]

### Pathfinding
```bash
travel_cost <x1> <y1> <x2> <y2>
```
Calculates the minimum cost path between two points using Dijkstra's algorithm. Returns `-1` if no path exists.[1]

### Cost Modification
```bash
change_cost <x> <y> <value> <radius>
```
Modifies cell costs in a radius around the specified point. Effect decreases with distance from center.[1]

### Air Route Management
```bash
toggle_air_route <x1> <y1> <x2```y2>
```
Creates or removes aerial connections between distant cells. Air routes have costs based on average of existing routes.[1]

##  Algorithm Complexity

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Dijkstra's Algorithm | O(V log V + E) | O(V) |
| BFS Distance | O(V + E) | O(V) |
| Cost Modification | O(V + E) | O(V) |
| Air Route Toggle | O(1) | O(1) |

Where V = number of cells, E = number of connections.[5]

##  Configuration

### Grid Constraints
- **Maximum Routes per Cell**: 5 air routes
- **Cost Range**: 0-100 (cells with cost 0 are impassable)
- **Value Change Range**: -10 to +10 for cost modifications
- **Grid Size**: Limited by available memory

### Coordinate System
The implementation uses **offset coordinates** for hexagonal grids with special handling for even/odd rows :[3]
- Even rows: Standard neighbor pattern
- Odd rows: Shifted neighbor pattern for proper hexagonal geometry

##  Implementation Highlights

### Hexagonal Neighbor Detection
```c
// Different neighbor patterns for even/odd rows
if``` % 2 == 0)```    // Even row neighbors```else {
    // Odd row```ighbors (shifted pattern)
}
```

### Memory Safety
- Overflow protection in grid allocation
- Proper cleanup of dynamic memory
- Bounds checking for all operations

### Performance Optimizations
- Min-heap priority queue for Dijkstra's algorithm
- Efficient neighbor lookup tables
- Single-pass BFS for radius operations

##  Error Handling

The system provides robust error handling:
- **Input Validation**: All coordinates and parameters are validated
- **Memory Management**: Graceful handling of allocation failures
- **Boundary Checks**: Prevents access to invalid grid positions
- **Overflow Protection**: Safe arithmetic operations

##  Performance Characteristics

Optimized for competitive programming environments with:
- Fast pathfinding on grids up to 1000x1000 cells
- Efficient memory usage with minimal overhead
- Robust handling of edge cases and invalid inputs
- Suitable for real-time interactive applications

##  Academic Context

Developed as part of the **Algorithms and Computer Science Principles** course at Politecnico di Milano (2024-2025), demonstrating:
- Advanced data structure implementation
- Graph algorithm optimization
- Memory management in C
- Hexagonal geometry calculations
- Interactive command-line interfaces

##  Dependencies

- **Standard C Library**: `stdio.h`, `stdlib.h`, `string.h`, `stdbool.h`
- **Math Library**: `math.h` (requires `-lm` flag)
- **Memory Management**: Dynamic allocation with `malloc`/`free`

##  License

Academic project developed for educational purposes at Politecnico di Milano. All rights reserved by the original author.
