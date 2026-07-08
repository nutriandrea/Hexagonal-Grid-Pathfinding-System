[![C](https://img.shields.io/badge/C-11-A8B9CC)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![Algorithms](https://img.shields.io/badge/A*-Pathfinding-FF6F00)](https://en.wikipedia.org/wiki/A*_search_algorithm)
[![Hex Grid](https://img.shields.io/badge/Grid-Hexagonal-4CAF50)](https://www.redblobgames.com/grids/hexagons/)
[![License](https://img.shields.io/badge/License-MIT-yellow)](LICENSE)
[![Polimi](https://img.shields.io/badge/Politecnico_Milano-API_2024--2025-900000)](https://www.polimi.it/)

# Hexagonal Grid Pathfinding System

A comprehensive C implementation of pathfinding algorithms on hexagonal grids with dynamic cost modification and air-route management.

Developed as the final project for **Algoritmi e Principi dell'Informatica** at Politecnico di Milano (2024–2025).

## Overview

This single-file C program (~760 lines) provides a complete hexagonal grid pathfinding system with three algorithms:

- **A\*** — Heuristic search (optimal, fastest for most cases)
- **Dijkstra** — Uniform-cost search (optimal, no heuristic)
- **BFS** — Breadth-first search (unweighted, good for uniform terrains)

The system supports hexagonal grids with proper neighbor computation (odd/even row offset), air routes for long-distance jumps, and dynamic cost modification.

## Features

| Feature | Description |
|---|---|
| **Hexagonal grid** | N×M grid with proper hexagonal neighbor coordinates (odd/even row offset) |
| **3 pathfinding algorithms** | A\*, Dijkstra, BFS — selectable at runtime |
| **Air routes** | Predefined teleport connections between distant cells with configurable cost |
| **Dynamic cost modification** | Set per-cell movement cost with `cambia_costo` command |
| **Interactive CLI** | Command-based interface: `inizializza`, `aggiungi_rotta`, `cambia_costo`, `cammino` |
| **Overflow-safe** | Size-checked memory allocation, input validation |
| **Performance counters** | Nodes visited per search, algorithm comparison |

## Algorithm Comparison

| Algorithm | Optimal | Heuristic | Best For |
|---|---|---|---|
| **A\*** | Yes | Yes (Manhattan on hex grid) | General pathfinding, most efficient |
| **Dijkstra** | Yes | No | Uniform exploration, all-paths analysis |
| **BFS** | No (unweighted only) | No | Unweighted grids, connectivity checks |

## Hexagonal Grid Geometry

The grid uses an **odd-row offset** coordinate system:

```
Row 0 (even):  □ - □ - □ - □
                \ / \ / \ / \
Row 1 (odd):     □ - □ - □ - □
                  \ / \ / \ / \
Row 2 (even):      □ - □ - □ - □
```

Each cell has **6 neighbors** with coordinates computed based on whether the current row is even or odd. The neighbor-finding function `trova_vicini` correctly handles edge cells at grid boundaries.

## Air Routes

Air routes (`rotte`) allow teleport-like connections between non-adjacent cells with a specified cost. Up to **5 air routes per cell**. Useful for modeling:
- Long-distance travel in game maps
- Portal mechanics
- Preferential paths in logistics
- Terrain shortcuts

## Commands

| Command | Syntax | Description |
|---|---|---|
| `inizializza c r` | `inizializza 5 5` | Create an N×M hexagonal grid |
| `aggiungi_rotta x1 y1 x2 y2 costo` | `aggiungi_rotta 0 0 4 4 3` | Add an air route between two cells |
| `cambia_costo x y costo` | `cambia_costo 2 3 5` | Change the movement cost of a cell |
| `cammino x1 y1 x2 y2` | `cammino 0 0 4 4` | Find shortest path (A\* by default) |
| `cammino x1 y1 x2 y2 -dijkstra` | `cammino 0 0 4 4 -dijkstra` | Find path using Dijkstra |
| `cammino x1 y1 x2 y2 -bfs` | `cammino 0 0 4 4 -bfs` | Find path using BFS |

## Getting Started

### Prerequisites

- GCC or any C11-compatible compiler
- Make (optional)

### Compile & Run

```bash
# Compile
gcc -std=c11 -O2 -o hex-pathfinding 24.c -lm

# Run
./hex-pathfinding
```

### Example Session

```
comando: inizializza 5 5
OK

comando: aggiungi_rotta 0 0 4 4 2
OK

comando: cammino 0 0 4 4
OK
Percorso: (0,0) -> (1,0) -> (2,1) -> (3,2) -> (4,3) -> (4,4)
Costo totale: 7

comando: cambia_costo 2 1 10
OK

comando: cammino 0 0 4 4
OK
Percorso: (0,0) -> (0,1) -> (1,2) -> (2,3) -> (3,4) -> (4,4)
Costo totale: 9
```

## Project Structure

```
├── 24.c         # Single-file implementation (~763 lines)
├── README.md    # This file
```

## Implementation Details

### Data Structures

- **`Mappa`**: Grid container with dimensions and dynamic cell array
- **`Cella`**: Cell with movement cost and up to 5 air routes
- **`Rotta`**: Air route with destination coordinates, cost, and active flag
- **`Queue`**: BFS queue implementation
- **`MinHeap`**: Priority queue for A\* and Dijkstra
- **`HeapNode`**: Priority queue node with coordinates and cumulative cost

### A\* Heuristic

Uses Manhattan distance adapted for the hexagonal grid coordinate system as the admissible heuristic, ensuring optimal paths.

### Memory Safety

- Overflow-checked multiplication for grid allocation
- `NULL` checks after every `malloc`
- Proper `free` on grid re-initialization
- Input bounds validation on all coordinates

## License

MIT — academic project, Politecnico di Milano 2024–2025.
