# Game of Life

### Overview
The Game of Life, a cellular automaton devised by the mathematician John Horton Conway, simulates a zero-player game where evolution is determined by its initial state, requiring no further input. This project implements Conway's Game of Life in C, allowing users to initiate a state and observe its evolution based on simple rules that create complex behaviors over time.

### Features
- **Dynamic Grid Implementation**: Utilizes a 2D grid where each cell is either alive or dead.
- **Toroidal Option**: Supports both flat and toroidal grid configurations.
- **Customizable Initial States**: Users can set the game's initial state from a file.
- **Generational Evolution**: Simulates the user-defined number of generations.
- **Visualization**: Offers optional visualization using ncurses for real-time simulation.

### Rules of the Game
1. **Survival**: A live cell with two or three live neighbors survives to the next generation.
2. **Birth**: A dead cell with exactly three live neighbors becomes a live cell.
3. **Death**: All other live cells die in the next generation due to underpopulation or overcrowding.

### Setup and Usage
- Compile the game using the provided Makefile.
- Run the game using:
  ```bash
  ./gameoflife -options
  ```
- Options include:
  - `-t`: Run the game on a toroidal universe.
  - `-s`: Silence mode, no ncurses visualization.
  - `-n [generations]`: Set the number of generations.
  - `-i [input file]`: Specify initial configuration.
  - `-o [output file]`: Specify output file for final state.

### How It Works
- The game initializes the universe based on input configurations.
- It computes the next state of each cell based on the rules of survival, birth, and death.
- Updates the grid each generation and optionally displays the grid using ncurses.
- Outputs the final state of the universe after the specified number of generations.

### Building and Running
- **Build**: Use the Makefile for building the project with `make all`.
- **Run**: Execute with `./gameoflife` followed by any required options.


### Documentation
- **README.md**: Instructions and overview.
- **DESIGN.pdf**: Detailed design document describing architecture and implementation.
- **WRITEUP.pdf**: Reflections and learning from the project development.
