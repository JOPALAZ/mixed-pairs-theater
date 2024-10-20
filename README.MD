# Mixed Pairs in Theater

This repository contains a solution to the problem of finding the average number of mixed pairs in a row of seats in a theater. The problem can be stated as follows:

## Problem Statement
Eight men and seven women have purchased tickets for seats in a single row at the theater. The goal is to calculate the expected (average) number of mixed pairs (a man and a woman sitting next to each other) in various random arrangements of these 15 people.

For example, in the sequence `MMWWMMWMWMWWMMW`, there are 9 mixed pairs.

## Solution Overview
The program uses multithreading to generate a large number of random seating arrangements and calculates the number of mixed pairs for each arrangement. It then computes the average number of mixed pairs across all generated arrangements.

### Features
- Uses multithreading to speed up the calculation.
- Allows users to specify the number of iterations for averaging.
- Outputs detailed results of each random arrangement if desired.

## Getting Started

### Prerequisites
- A C++ compiler that supports C++11 or later (e.g., MinGW, GCC).
- CMake (optional, for building).

### Building the Project
To compile the project, run:
```bash
g++ -o mixed_pairs main.cpp -lpthread
