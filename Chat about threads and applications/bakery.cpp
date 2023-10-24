#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <algorithm>

using namespace std;
using namespace chrono;


const int NUM_THREADS = 10;  // Number of threads
atomic<int> tickets[NUM_THREADS];  // Tickets for each thread
atomic<bool> choosing[NUM_THREADS];  // Flags for choosing tickets

void lock(int);
void unlock(int);
void criticalSection(int);
void threadFunc(int);

int main() {
    // Initialize the tickets and choosing arrays
    for (int i = 0; i < NUM_THREADS; ++i) {
        tickets[i] = 0;
        choosing[i] = false;
    }

    // Create and launch threads
    vector<thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.push_back(thread(threadFunc, i));
    }

    // Join threads
    for (auto& th : threads) {
        th.join();
    }

    return 0;
}

void lock(int id) {
    choosing[id] = true;
    int max_ticket = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        max_ticket = max(max_ticket, tickets[i].load());
    }
    tickets[id] = max_ticket + 1;
    choosing[id] = false;

    for (int j = 0; j < NUM_THREADS; ++j) {
        while (choosing[j]) {
            this_thread::yield();  // Wait
        }
        while (tickets[j] != 0 &&
              (tickets[j] < tickets[id] || (tickets[j] == tickets[id] && j < id))) {
            this_thread::yield();  // Wait
        }
    }
}

void unlock(int id) {
    tickets[id] = 0;
}

void criticalSection(int id) {
    cout << "Thread " << id << " is entering the critical section." << endl;
    this_thread::sleep_for(milliseconds(100));  // Simulating some work
    cout << "Thread " << id << " is leaving the critical section." << endl;
}

void threadFunc(int id) {
    lock(id);
    criticalSection(id);
    unlock(id);
}