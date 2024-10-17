#include <iostream>
#include <queue>
#include <string>

using namespace std;

// Structure to represent a client
struct Client {
    string name;
    string vehicleBody;
    int priority;
};

// Custom comparator for priority queue
struct ComparePriority {
    bool operator()(const Client& c1, const Client& c2) {
        // Higher priority value means lower priority in the queue
        return c1.priority > c2.priority;
    }
};

// Function to add a client to the queue
void enqueueClient(priority_queue<Client, vector<Client>, ComparePriority>& clientQueue) {
    Client client;
    cout << "Enter client name: ";
    cin >> client.name;
    cout << "Enter vehicle body type (sedan, SUV, truck, motorcycle): ";
    cin >> client.vehicleBody;
    cout << "Enter priority (1 or 2): ";
    cin >> client.priority;

    // Input validation for priority
    while (client.priority != 1 && client.priority != 2) {
        cout << "Invalid priority. Please enter 1 or 2: ";
        cin >> client.priority;
    }

    clientQueue.push(client);
    cout << "Client added to the queue.\n";
}

// Function to check the next client in the queue
void checkQueue(priority_queue<Client, vector<Client>, ComparePriority>& clientQueue) {
    if (clientQueue.empty()) {
        cout << "The queue is empty.\n";
        return;
    }

    Client nextClient = clientQueue.top();
    cout << "Next client in queue:\n";
    cout << "Name: " << nextClient.name << endl;
    cout << "Vehicle Body: " << nextClient.vehicleBody << endl;
    cout << "Priority: " << nextClient.priority << endl;
}

// Function to dequeue the next client
void dequeueClient(priority_queue<Client, vector<Client>, ComparePriority>& clientQueue) {
    if (clientQueue.empty()) {
        cout << "The queue is empty.\n";
        return;
    }

    Client dequeuedClient = clientQueue.top();
    clientQueue.pop();
    cout << "Client " << dequeuedClient.name << " dequeued.\n";
}

int main() {
    priority_queue<Client, vector<Client>, ComparePriority> clientQueue;

    int choice;
    do {
        cout << "\nGarage Management System\n";
        cout << "1. Enqueue Client\n";
        cout << "2. Check Queue\n";
        cout << "3. Dequeue Client\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                enqueueClient(clientQueue);
                break;
            case 2:
                checkQueue(clientQueue);
                break;
            case 3:
                dequeueClient(clientQueue);
                break;
            case 4:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}