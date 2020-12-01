# Lockless-Message-Queue-Simulator

Mini Project project for UE19CS208: Special Topic

## Index

-   [Getting Started](#getting-started)
-   [Prerequisites](#prerequisites)
-   [Running](#running)

## Getting Started

Clone the repository into your system, by typing the following in your terminal:

```
git clone https://github.com/LaRuim/Lockless-Message-Queue-Sim.git
```

## Prerequisites

-   C++11
-   G++ compiler
-   Python3, IPython (or Jupyter Notebook) and matplotlib for graph visualisation

## Running

To run a simulation, navigate to the folder and type either:

```
make
./locked_mq
```

or

```
make
./atomic_mq
```

in a terminal, depending on the queue chosen.

To benchmark it, run:

```
sudo bash benchmark.sh
```

in a terminal. Make sure to comment and uncomment desired outputs before benchmarking.