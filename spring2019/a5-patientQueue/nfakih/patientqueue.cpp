/*     HW5: PriorityQueue
 * This class hosts a varity of methods, constructor and deconstructor
 * to create a PatientQueue which places Patient nodes in proper order
 * depending on their assigned priority which represents the urgency of
 * their medical condition.
 * By: Noor Fakih       Class: CS106B       Prof: Marty Stepp
 */
#include "patientqueue.h"

const int DEFAULT_SIZE = 5;

/*              PatientQueue(bool isMinHeap)
 * Constructor for the class. It sets all the private memebers to their intial
 * values, i.e. setting the capacity, making a an array of Patients, setting the
 * size as 0, and using the input bool of isMinHeap to store for future reference
 * for if the queue is ordered by min priority or max priority.
 */
PatientQueue::PatientQueue(bool isMinHeap) {
    _capacity = DEFAULT_SIZE;
    _size = 0;
    _elements = new Patient[_capacity];
    _isMin = isMinHeap;

}

/*              ~PatientQueue()
 * Deconstructor for the class. It makes sure to free the memory
 * used to store the array of Patients. It calls clear() to set
 * the size of the array as 0, and deletes its memory.
 */
PatientQueue::~PatientQueue() {
    clear();
    delete [] _elements;
}

/*          changePriority(string value, int newPriority)
 * It allows the user to change the priority of a Patient named the input value to less or more
 * urgent by input newPriority by looping through the array searching for a Patient with name of
 * value.
 */
void PatientQueue::changePriority(string value, int newPriority) {
    for(int i = 0; i < _size; i++){
        if(_elements[i].name == value){
            int currPrior = _elements[i].priority;
            _elements[i].priority = newPriority;
            if(currPrior < newPriority){
                bubbleDown(i);
            }
            else{
                bubbleUp(i);
            }
            return;
        }
    }
}

/*          clear()
 * The function "clears" the queue by setting the private member
 * variable _size to 0.
 */
void PatientQueue::clear() {
    _size = 0;
}

/*         debug()
 * The function was not utilized.
 */
void PatientQueue::debug() {
    // empty
}

/*           dequeue()
 * The function will returns and removes the name of
 * the first Patient in the vector.
 */
string PatientQueue::dequeue() {
    Patient result = peek();
    Patient lastPatient = _elements[_size - 1];
    _size--;
    _elements[0] = lastPatient;
     bubbleDown(0);

    return result.name;
}

/*          enqueue(string value, int priority)
 * If the queue is maxed out, resize() is called to expand it and double
 * its capcity. It then adds a new Patient with input name of value and the
 * input priority called priority. The size is increased and bubbleUp() is called
 * to make sure the elements are in order.
 */
void PatientQueue::enqueue(string value, int priority) {
    if(_size == _capacity){
        resize();
    }
    Patient newP(value, priority);
    _elements[_size] = newP;

    _size++;

    bubbleUp(_size-1);
}

/*          isEmpty()
 * Returns whether or not the queue is empty by checking
 * its _size.
 */
bool PatientQueue::isEmpty() const {
    return _size == 0;
}

/*          peek()
 * Returns the name of the first Patient in the queue.
 * If empty it will throw a string error instead.
 */
string PatientQueue::peek() const {
    if(isEmpty()){
        throw "Queue is empty.";
    }
    return _elements[0].name;
}

/*          peekPriority()
 * returns the priority of the first Patient in the array.
 */
int PatientQueue::peekPriority() const {
    return _elements[0].priority;
}

/*          size()
 * Returns the size of the queue.
 */
int PatientQueue::size() const {
    return _size;
}

/*
 * The << is overwritten to print the queue in an appropriate form.
 */
ostream& operator <<(ostream& out, const PatientQueue& queue) {
    out << "{";
        for(int i = 0; i < queue.size(); i++){
            out << queue._elements[i];
            if(queue._size != 1 && i != queue.size()-1){
                out << ", ";
            }
        }
    out << "}";
    return out;
}

/*          resize()
 * Called when the queue is at capacity so it can expanded.
 * The capacity is doubled and the memory is properly allocated.
 */
void PatientQueue::resize(){
    _capacity*=2;
    Patient* newPatients = new Patient[_capacity];

    for(int i=0; i < _size; i++){
        newPatients[i] = _elements[i];
    }

    delete [] _elements;
    _elements = newPatients;
}

/*          bubbleUp(int child)
 * This function compares nodes to its parent and swaps them to
 * properly place them in order. It also checks if elements have
 * the same priority, if so it will organize them alphabetically.
 */
void PatientQueue::bubbleUp(int child){
    if(child != 0){
        int parentPos = ((child + 1)/2) - 1;
        Patient childPatient;
        Patient parentPatient;
        if(!_isMin){
             childPatient = _elements[parentPos];
             parentPatient = _elements[child];
        } else{
             childPatient = _elements[child];
             parentPatient = _elements[parentPos];
        }

        //conditions that need to be met to continuing bubbling
        if(childPatient.priority < parentPatient.priority){
            _elements[child] = parentPatient;
            _elements[parentPos] = childPatient;
            bubbleUp(parentPos);
        } else if(childPatient.priority == parentPatient.priority){
            if(childPatient.name < parentPatient.name){
                _elements[child] = parentPatient;
                _elements[parentPos] = childPatient;
                bubbleUp(parentPos);
            }
        }
    }
}

/*          bubbleDown(int parent)
 * This function compares parents to its children and swaps them to
 * properly place them in order. It also checks if elements have
 * the same priority, if so it will organize them alphabetically.
 */
void PatientQueue::bubbleDown(int parent){
    int leftChildPos = 2*(parent + 1) - 1;
    int rightChildPos = 2*(parent + 1);

    if(!_isMin){
        int temp = parent;
        parent = leftChildPos;
        leftChildPos = temp;
    }

    if(leftChildPos > _size - 1){
        return;
    }

    int minChildPos = leftChildPos;

    if(rightChildPos <= _size - 1){
        minChildPos = _elements[leftChildPos] < _elements[rightChildPos] ? leftChildPos : rightChildPos;
    }

    Patient minChildPatient = _elements[minChildPos];
    Patient parentPatient = _elements[parent];

    //conditions that need to be met to continuing bubbling
    if(parentPatient.priority > minChildPatient.priority){
        _elements[parent] = minChildPatient;
        _elements[minChildPos] = parentPatient;
        bubbleDown(minChildPos);
    } else if(parentPatient.priority == minChildPatient.priority){
        if(minChildPatient.name < parentPatient.name){
            _elements[minChildPos] = parentPatient;
            _elements[parent] = minChildPatient;
            bubbleDown(parent);
        }
    }
}
