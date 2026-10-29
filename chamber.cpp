/*
 * Archaeological Chamber Exploration and Excavation Planning System
 * Course  : Data Structures (E1CSA25479)
 * Language: C++ (console based)
 *
 * Data structures used:
 *   vector  -> stores chamber records
 *   graph   -> adjacency list, stores tunnels between chambers
 *   stack   -> decides exploration order (depth-first)
 *   queue   -> keeps excavation tasks in first-in first-out order
 */

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <limits>
using namespace std;


// ---------------------------------------------------------------
// STEP 1: Data model
// ---------------------------------------------------------------
struct Chamber {
    int    id;
    string name;
    bool   visited;
};

vector<Chamber>     chambers;          // all chamber records
vector<vector<int>> adj;               // adj[i] = indexes of chambers joined to chamber i
queue<string>       excavationTasks;   // pending excavation work


// ---------------------------------------------------------------
// STEP 2: Helper - find the position of a chamber using its ID
// ---------------------------------------------------------------
int findIndex(int id) {
    for (size_t i = 0; i < chambers.size(); i++) {
        if (chambers[i].id == id) {
            return (int)i;
        }
    }
    return -1;                         // not found
}


// ---------------------------------------------------------------
// STEP 3: Add a chamber (vector)
// ---------------------------------------------------------------
void addChamber() {
    Chamber c;

    cout << "Enter chamber ID: ";
    cin >> c.id;

    if (findIndex(c.id) != -1) {
        cout << "Chamber ID already exists.\n";
        return;
    }

    cout << "Enter chamber name: ";
    cin >> c.name;

    c.visited = false;
    chambers.push_back(c);
    adj.push_back(vector<int>());      // new chamber starts with no tunnels

    cout << "Chamber added.\n";
}


// ---------------------------------------------------------------
// STEP 4: Connect two chambers with a tunnel (graph edge)
// ---------------------------------------------------------------
void connectChambers() {
    int a, b;
    cout << "Enter two chamber IDs to connect: ";
    cin >> a >> b;

    int i = findIndex(a);
    int j = findIndex(b);

    if (i == -1 || j == -1 || i == j) {
        cout << "Invalid chamber IDs.\n";
        return;
    }

    for (int x : adj[i]) {
        if (x == j) {
            cout << "Already connected.\n";
            return;
        }
    }

    adj[i].push_back(j);               // tunnels are two-way
    adj[j].push_back(i);

    cout << "Tunnel created between " << a << " and " << b << ".\n";
}


// ---------------------------------------------------------------
// STEP 5: Display all chambers with status and connections
// ---------------------------------------------------------------
void displayChambers() {
    if (chambers.empty()) {
        cout << "No chambers stored.\n";
        return;
    }

    cout << "ID\tName\t\tStatus\t\tConnected to\n";
    for (size_t i = 0; i < chambers.size(); i++) {
        cout << chambers[i].id << "\t"
             << chambers[i].name << "\t\t"
             << (chambers[i].visited ? "Explored" : "Unexplored") << "\t";

        for (int x : adj[i]) {
            cout << chambers[x].id << " ";
        }
        cout << "\n";
    }
}


// ---------------------------------------------------------------
// STEP 6: Explore chambers using a stack (depth-first traversal)
// ---------------------------------------------------------------
void exploreChambers() {
    int startId;
    cout << "Enter starting chamber ID: ";
    cin >> startId;

    int start = findIndex(startId);
    if (start == -1) {
        cout << "Chamber not found.\n";
        return;
    }

    for (auto &c : chambers) {
        c.visited = false;             // fresh exploration
    }

    stack<int> st;
    st.push(start);

    cout << "Exploration order: ";
    while (!st.empty()) {
        int cur = st.top();
        st.pop();

        if (chambers[cur].visited) {
            continue;                  // already explored
        }

        chambers[cur].visited = true;
        cout << chambers[cur].name << " ";

        // push in reverse so the first tunnel is followed first
        for (int k = (int)adj[cur].size() - 1; k >= 0; k--) {
            int next = adj[cur][k];
            if (!chambers[next].visited) {
                st.push(next);
            }
        }
    }
    cout << "\n";

    for (auto &c : chambers) {
        if (!c.visited) {
            cout << "Note: " << c.name
                 << " is not reachable from the start chamber.\n";
        }
    }
}


// ---------------------------------------------------------------
// STEP 7: Excavation tasks using a queue (first in, first out)
// ---------------------------------------------------------------
void addTask() {
    string task;
    cout << "Enter excavation task: ";
    cin >> task;

    excavationTasks.push(task);
    cout << "Task queued.\n";
}

void processTask() {
    if (excavationTasks.empty()) {
        cout << "No pending excavation tasks.\n";
        return;
    }

    cout << "Processing task: " << excavationTasks.front() << "\n";
    excavationTasks.pop();
}


// ---------------------------------------------------------------
// STEP 8: Search a chamber by ID
// ---------------------------------------------------------------
void searchChamber() {
    int id;
    cout << "Enter chamber ID to search: ";
    cin >> id;

    int i = findIndex(id);
    if (i == -1) {
        cout << "Chamber not found.\n";
        return;
    }

    cout << "Found: " << chambers[i].name
         << " (" << (chambers[i].visited ? "Explored" : "Unexplored") << ")"
         << ", tunnels: " << adj[i].size() << "\n";
}


// ---------------------------------------------------------------
// STEP 9: Main menu - joins all the modules together
// ---------------------------------------------------------------
int main() {
    int choice;

    do {
        cout << "\n--- Archaeological Chamber Planning System ---\n"
             << "1. Add chamber\n"
             << "2. Connect chambers\n"
             << "3. Display chambers\n"
             << "4. Explore chambers (Stack)\n"
             << "5. Add excavation task\n"
             << "6. Process next task (Queue)\n"
             << "7. Search chamber\n"
             << "0. Exit\n"
             << "Choice: ";

        if (!(cin >> choice)) {        // guard against letters / bad input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice) {
            case 1: addChamber();      break;
            case 2: connectChambers(); break;
            case 3: displayChambers(); break;
            case 4: exploreChambers(); break;
            case 5: addTask();         break;
            case 6: processTask();     break;
            case 7: searchChamber();   break;
            case 0: cout << "Exiting.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
