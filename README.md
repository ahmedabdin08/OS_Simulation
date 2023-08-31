# OS_Simulation
A simulator for various components of a distributed multi-node operating system includes inter-process communication, scheduling algorithms, and clock synchronization across the nodes. Multithreading simulates the multi-node system and priority queues are used for the various scheduling algorithms. This simulator was developed as part of the course requirements for CSCI3120, taught by Dr. Alex Brodsky.

## Features
### Process primitives: Each process has a set of primitives (pseudo-instructions) 
* DOOP (DOOP Ticks): This primitive is for the amount of time (ticks) a process wants to run on a CPU
* BLOCK (BLOCK Ticks): This primitive is for the amount of time (ticks) a process is blocked
* LOOP (LOOP Iterations): This primitive is for control of flow within a process where a process repeats the loop body till the iterations finish
* END: This primitive is for the end of a loop body
* SEND (SEND Address): This primitive is for sending a message from the sender process to the receiver process; the receiver process's address is specified in the instruction
* RECV (RECV Address): This primitive is for receiving a message from the sender process to the receiver process; the receiver process's address is specified in the instruction
* HALT: This primitive is for the end of a process

### Scheduling Algorithms: Priority queues are used to emulate various scheduling algorithms
* Round-Robin: A CPU quantum is specified in the first line of the input and every process runs for the specified quanta and then gets preempted
* Priority scheduling with preemption: If a process with higher priority becomes ready to run it preempts the currently running process and runs
* Shortest job first: Processes are sorted by the number of ticks in the current DOOP primitive

### Multi-node system
Multithreading is used to emulate a multi-node system, where each node corresponds to a thread. Every node is a self-contained computational unit with its scheduler, resources, and clock.

### Inter-process communication
Processes are allowed to communicate with each other through the SEND and RECV primitives.

### Clock synchronization
Clocks are synchronized across the various nodes using a barrier to ensure synchronous sends and receive. The barrier uses condition variables to enforce that a thread must wait until all other threads have reached the barrier, after which it signals the threads to continue execution
