A cellular automata engine which uses multithreading on the CPU to generate fast cellular automata simulations.
Uses SDL2 to display the game board. Each pixel on this board represents one cell.
Currently running on a 800x600 board, simulating 480,000 cells each generation.
Currently simulates Conway's Game of Life, however it's easy to switch out rules by simply changing the rule function. 

Built, compiled, and tested on Linux. No code is linux-specific, however, and  should run on any system with SDL2 installed. 
