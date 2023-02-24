// This is the H file you will edit and turn in. (TODO: Remove this comment!)

#ifndef _patientqueue_h
#define _patientqueue_h

#include <iostream>
#include <string>
#include "patient.h"
using namespace std;

class PatientQueue {
public:
    PatientQueue(bool isMinHeap = true);
    ~PatientQueue();
    void changePriority(string value, int newPriority);
    void clear();
    void debug();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const PatientQueue& queue);

private:

    Patient* _elements;
    int _capacity;
    int _size;
    bool _isMin;

    void resize();
    void bubbleUp(int child);
    void bubbleDown(int parent);
};

#endif
