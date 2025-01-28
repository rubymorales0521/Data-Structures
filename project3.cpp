#include <iostream>
#include <string>

using namespace std;

// Class for holding each individual CPU task.
class CPUJob {
    public:
        int job_id; // Unique identifier for the job
        int priority; // Priority level of the job (1-10)
        int job_type; // Job type (1-10)
        int cpu_time_consumed; // CPU time consumed from performing job
        int memory_consumed; // Total memory consumed (from job)
        CPUJob(); // Default constructor
        CPUJob(int job_id, int priority, int job_type, int cpu_time_consumed, int memory_consumed); // Parameterized constructor
        CPUJob(CPUJob& copy); // Copy constructor
        ~CPUJob(); // Destructor
        void display(); // Display method
};
CPUJob::CPUJob() {} // Default constructor
CPUJob::CPUJob(int job_id, int priority, int job_type, int cpu_time_consumed, int memory_consumed) { // Parameterized constructor
    (*this).job_id = job_id;
    (*this).priority = priority;
    (*this).job_type = job_type;
    (*this).cpu_time_consumed = cpu_time_consumed;
    (*this).memory_consumed = memory_consumed;
}
CPUJob::CPUJob(CPUJob& copy) { // Copy constructor
    (*this).job_id = copy.job_id;
    (*this).priority = copy.priority;
    (*this).job_type = copy.job_type;
    (*this).cpu_time_consumed = copy.cpu_time_consumed;
    (*this).memory_consumed = copy.memory_consumed;
}
CPUJob::~CPUJob() {} // Destructor
void CPUJob::display() { // Display method
    cout << "Job ID: " << job_id << ", Priority: " << priority << ", Job Type: " << job_type << ", CPU Time Consumed: " << cpu_time_consumed << ", Memory Consumed: " << memory_consumed << endl;
}

// Queue data structure (First-in, First-out).
template <class DT>
class Queue {
    public:
        DT JobPointer; // Pointer to a CPUJob
        Queue<DT>* next; // Pointer to the next node in the queue
        Queue(){} // Default constructor
        Queue(DT CPUJob); // Create a new queue object with a CPUJob object
};
template <class DT>
Queue<DT>::Queue(DT CPUJob) { // Create a new queue object with a given CPUJob object
    JobPointer = CPUJob;
}

// Class for managing the CPUJob queue (First-in, First-out).
template <typename DT>
class NovelQueue {
    public:
        Queue<DT>* front; // Pointer to the front of the queue
        Queue<DT>** NodePtrs; // Array to store pointers to queue nodes
        int size; // Number of jobs in the queue
        NovelQueue(); // Default constructor
        ~NovelQueue(); // Destructor
        int enqueue(CPUJob* newJob); // Adds a job to the queue
        CPUJob* dequeue(); // Removes a job from the queue
        int modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed); // Modifies the first job in the queue
        int change(int job_id, int field_index, int new_value); // Updates a specific field of a job
        int promote(int job_id, int positions); // Promotes a job in the queue by a certain number of positions
        void reorder(int attribute_index); // Reorders the queue based on a given attribute
        void display(); // Displays the contents of the queue
        int count() ; // Returns the number of jobs in the queue
        void listJobs() ; // Prints the list of jobs in the queue
        int binarySearch(int key) ; // Performs a binary search on nodeArray and returns the index of the job with the matching key
        int incrementalSearch(int key) ; // Performs an incremental search on nodeArray and returns the index of the job with the matching key
};

// Default constructor for NovelQueue
template <class DT>
NovelQueue<DT>::NovelQueue() { // Default constructor
    front = new Queue<DT>();
    size = 0; // initialized at 0, as queue starts empty
    NodePtrs = new Queue<DT>*[size];
}
// Destructor for NovelQueue
template <typename DT>
NovelQueue<DT>::~NovelQueue() {
    delete front; // Clean up the front pointer
    delete[] NodePtrs; // Delete the node array
}

// Enqueue method: Adds a new job to the queue
template <class DT>
int NovelQueue<DT>::enqueue(CPUJob* newJob) { // Enqueue method
    Queue<DT>** newArray = new Queue<DT>*[size + 1];  // Increase array size by 1
    for (int i = 0; i < size; i++) { // Copy over to newArray
        if (NodePtrs[i]->JobPointer->job_id == newJob->job_id) { // Check to see if newJob is already within NodePtrs
            return -1; // Return -1 so that we can show job is already within queue
        }
        newArray[i] = NodePtrs[i];
    }
    newArray[size] = new Queue<DT>(newJob); // Put newJob into array
    if (size == 0) { // Verifying whether front should be pointing to newJob
        front = newArray[size];
    }
    else { // If it doesn't go at the front then just put it at the end of queue
        newArray[size - 1]->next = newArray[size];
    }
    newArray[size]->next = nullptr; // Set the very end to nullptr
    delete[] NodePtrs; // Delete old NodePtrs array
    NodePtrs = newArray; // Copy new array into NodePtrs
    size++; // Increment size
    return 1; // Return number of nodes enqueued (you can only enqueue one at a time)
} 

// Dequeue method: Removes and returns the front job from the queue
template <typename DT>
CPUJob* NovelQueue<DT>::dequeue() {
    if (size == 0) // If the queue is empty, return null
        return nullptr;
    CPUJob* result = front->JobPointer; // Store the job to be dequeued

    for (int i = 0; i < size - 1; i++) { // Shift all elements left
        NodePtrs[i] = NodePtrs[i + 1];
    }
    size--; // Decrease the queue size
    if (size > 0) {
        front = NodePtrs[0]; // Update front to the new first element
    } else {
        front = nullptr; // If no elements left, set front to null
    }
    return result; // Return the dequeued job
}

// Modify method: Updates a job's fields based on its job_id
template <typename DT>
int NovelQueue<DT>::modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed) {
    int jobPos = incrementalSearch(job_id); // Find the job's position
    if (jobPos != -1) { // If the job exists, modify it
        NodePtrs[jobPos]->JobPointer = new CPUJob(job_id, new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed); // Update the job fields
    }
    return jobPos; // Return the position of the modified job
}

// Change method: Updates a specific field of a job based on its job_id and the desired field_index
template <typename DT>
int NovelQueue<DT>::change(int job_id, int field_index, int new_value) {
    int jobPos = incrementalSearch(job_id); // Find the job's position
    if (jobPos == -1) {
        return -1; // Return -1 if job is not found
    }

    // Use a switch statement to update the appropriate field
    switch (field_index) {
        case 1: // Update priority
            NodePtrs[jobPos]->JobPointer->priority = new_value;
            break;
        case 2: // Update job type
            NodePtrs[jobPos]->JobPointer->job_type = new_value;
            break;
        case 3: // Update CPU time consumed
            NodePtrs[jobPos]->JobPointer->cpu_time_consumed = new_value;
            break;
        case 4: // Update memory consumed
            NodePtrs[jobPos]->JobPointer->memory_consumed = new_value;
            break;
        default:
            return -1; // Return -1 if an invalid field_index is provided
    }
    return jobPos; // Return the position of the modified job
}
template <class DT>
int NovelQueue<DT>::promote(int job_id, int positions) { // Promote a job in the queue
    int jobPos = incrementalSearch(job_id); // Locate job to promote
    int newPos = jobPos - positions; // Calculate its new position

    Queue<DT>** newArray = new Queue<DT>*[size]; // Create a temporary new array
    for (int i = 0, j = 0; i < size; i++, j++) { // Copy data into new array
        if (i == newPos) { // If the current index matches the new position
            if (i == 0) {
                front = NodePtrs[jobPos]; // Job is now at the front
            }
            newArray[i] = NodePtrs[jobPos]; // Move job to new position
            i++; // Increment to prevent overwriting the position
        }
        if (j == jobPos) { // Skip over the promoted job's original spot
            j++;
        }
        newArray[i] = NodePtrs[j]; // Continue copying other elements
    }
    NodePtrs = newArray; // Update NodePtrs to point to the new array
    return newPos; // Return the new position of the promoted job
}

template <class DT>
void NovelQueue<DT>::reorder(int attribute_index) { // Reorder queue by a given attribute

    Queue<DT>** newArray = new Queue<DT>*[size]; // Create a new temporary array

    // Each switch case handles sorting based on a specific attribute
    switch (attribute_index) {
        case 1: { // Sort by job_id (no duplicates accounted)
            for (int i = 0; i < size; i++) {
                int minIdx = i;
                for (int j = i + 1; j < size; j++) {
                    if (NodePtrs[j]->JobPointer->job_id < NodePtrs[minIdx]->JobPointer->job_id) {
                        minIdx = j;
                    }
                }
                // Swap elements to sort by job_id
                newArray[i] = NodePtrs[minIdx];
                Queue<DT>* temp = NodePtrs[i];
                NodePtrs[i] = NodePtrs[minIdx];
                NodePtrs[minIdx] = temp;
            }
            break;
        }
        case 2: { // Sort by priority (Stable sort)
            for (int i = 0; i < size; i++) {
                int minIdx = i;
                for (int j = i + 1; j < size; j++) {
                    if (NodePtrs[j]->JobPointer->priority < NodePtrs[minIdx]->JobPointer->priority ||
                        (NodePtrs[j]->JobPointer->priority == NodePtrs[minIdx]->JobPointer->priority &&
                         NodePtrs[j]->JobPointer->job_id < NodePtrs[minIdx]->JobPointer->job_id)) {
                        minIdx = j;
                    }
                }
                // Swap elements to sort by priority
                newArray[i] = NodePtrs[minIdx];
                Queue<DT>* temp = NodePtrs[i];
                NodePtrs[i] = NodePtrs[minIdx];
                NodePtrs[minIdx] = temp;
            }
            break;
        }
        case 3: { // Sort by job_type (Stable sort)
            for (int i = 0; i < size; i++) {
                int minIdx = i;
                for (int j = i + 1; j < size; j++) {
                    if (NodePtrs[j]->JobPointer->job_type < NodePtrs[minIdx]->JobPointer->job_type ||
                        (NodePtrs[j]->JobPointer->job_type == NodePtrs[minIdx]->JobPointer->job_type &&
                         NodePtrs[j]->JobPointer->job_id < NodePtrs[minIdx]->JobPointer->job_id)) {
                        minIdx = j;
                    }
                }
                // Swap elements to sort by job_type
                newArray[i] = NodePtrs[minIdx];
                Queue<DT>* temp = NodePtrs[i];
                NodePtrs[i] = NodePtrs[minIdx];
                NodePtrs[minIdx] = temp;
            }
            break;
        }
        case 4: { // Sort by cpu_time_consumed (Stable sort)
            for (int i = 0; i < size; i++) {
                int minIdx = i;
                for (int j = i + 1; j < size; j++) {
                    if (NodePtrs[j]->JobPointer->cpu_time_consumed < NodePtrs[minIdx]->JobPointer->cpu_time_consumed ||
                        (NodePtrs[j]->JobPointer->cpu_time_consumed == NodePtrs[minIdx]->JobPointer->cpu_time_consumed &&
                         NodePtrs[j]->JobPointer->job_id < NodePtrs[minIdx]->JobPointer->job_id)) {
                        minIdx = j;
                    }
                }
                // Swap elements to sort by cpu_time_consumed
                newArray[i] = NodePtrs[minIdx];
                Queue<DT>* temp = NodePtrs[i];
                NodePtrs[i] = NodePtrs[minIdx];
                NodePtrs[minIdx] = temp;
            }
            break;
        }
        case 5: { // Sort by memory_consumed (Stable sort)
            for (int i = 0; i < size; i++) {
                int minIdx = i;
                for (int j = i + 1; j < size; j++) {
                    if (NodePtrs[j]->JobPointer->memory_consumed < NodePtrs[minIdx]->JobPointer->memory_consumed ||
                        (NodePtrs[j]->JobPointer->memory_consumed == NodePtrs[minIdx]->JobPointer->memory_consumed &&
                         NodePtrs[j]->JobPointer->job_id < NodePtrs[minIdx]->JobPointer->job_id)) {
                        minIdx = j;
                    }
                }
                // Swap elements to sort by memory_consumed
                newArray[i] = NodePtrs[minIdx];
                Queue<DT>* temp = NodePtrs[i];
                NodePtrs[i] = NodePtrs[minIdx];
                NodePtrs[minIdx] = temp;
            }
            break;
        }
    }

    // Replace the old NodePtrs with the sorted array
    delete[] NodePtrs; // Clean up the old array
    NodePtrs = newArray; // Assign sorted array back to NodePtrs
}

// Display all nodes in the queue
template <class DT>
void NovelQueue<DT>::display() {
    for (int i = 0; i < size; ++i) { // Loop through NodePtrs and print each node
        NodePtrs[i]->JobPointer->display();
    }
}

// Return the number of nodes in the queue
template <class DT>
int NovelQueue<DT>::count() {
    return size; // Simply return the size
}

// Perform a binary search for a job by its job_id
template <class DT>
int NovelQueue<DT>::binarySearch(int key) {
    int left = 0;
    int right = size - 1;

    // Continue searching while left index is less than or equal to right index
    while (left <= right) {
        int mid = (left + right) / 2;

        if (NodePtrs[mid]->JobPointer->job_id == key) {
            return mid; // Job found, return the index
        }
        if (NodePtrs[mid]->JobPointer->job_id < key) {
            left = mid + 1; // Adjust the left bound
        } else {
            right = mid - 1; // Adjust the right bound
        }
    }

    return -1; // Return -1 if job not found
}

template <class DT>
int NovelQueue<DT>::incrementalSearch(int key) { // Performs an incremental search on NodePtrs and returns index of the key
    for (int i = 0; i < size; i++) {
        if (NodePtrs[i]->JobPointer->job_id == key) {
            return i; // Returns index when found
        }
    }

    return -1; // Returns -1 if unfound
}


int main() {
    int n; // Number of commands
    cin >> n; // Read in the number of commands

    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>(); // Instantiate a NovelQueue for CPUJob pointers

    char command; // Variable to store the command type

    // Job attribute declaration placeholders
    int job_id, priority, job_type, cpu_time_consumed, memory_consumed;

    // Job attribute modification placeholders
    int new_priority, new_job_type, new_cpu_time_consumed;
    int new_memory_consumed;
    int field_index, new_value;

    int positions; // Variable to store number of positions in the 'Promote' command
    int attribute_index; // Variable for the 'Reorder' command

    for (int i = 0; i < n; i++) {
        cin >> command; // Read in command type
        // Here we are parsing through the commands to determine which one should be performed based on the command type
        switch (command) {
            case 'A': { // Command to add a new CPUJob to queue (enqueue)
                cin >> job_id >> priority >> job_type >> cpu_time_consumed >> memory_consumed; // Takes input to initialize new CPUJob object
                CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed); // Creates new CPUJob object from input
                int jobsAdded = (*myNovelQueue).enqueue(newJob); // Adds this newJob to the queue
                if (jobsAdded < 1) { // Determine if job is already within queue (does not get added)
                    cout << "Job ID " << newJob->job_id << " already exists!" << endl;
                    break;
                }
                cout << "Enqueued Job: " << endl;
                myNovelQueue->NodePtrs[((myNovelQueue->size) - 1)]->JobPointer->display(); // Prints enqueued job
                
                cout << "Jobs after enqueue:" << endl;
                myNovelQueue->display(); // Uses display() method to print all nodes in queue
                break; // End case
            }
            case 'R': { // Command to remove a CPUJob from queue (dequeue)
                CPUJob* removedJob = (*myNovelQueue).dequeue(); // Create placeholder for dequeue'd job
                if (removedJob) { // Determines whether dequeue() returns anything
                    cout << "Dequeued Job: " << endl;
                    (*removedJob).display(); // Prints removedJob
                    delete removedJob; // Deletes removedJob to free up memory
                }
                cout << "Jobs after dequeue:" << endl;
                myNovelQueue->display(); // Uses display() method to print all nodes in queue
                break; // End case
            }
            case 'M': {
                cin >> job_id >> priority >> job_type >> cpu_time_consumed >> memory_consumed; // Takes input to modify a CPUJob object
                int pos = myNovelQueue->modify(job_id, priority, job_type, cpu_time_consumed, memory_consumed); // Changes job's priority, job_type, cpu_time_consumed, and memory_consumed. Returns index within queue
                cout << "Modified Job ID " << job_id << ":" << endl;
                myNovelQueue->NodePtrs[pos]->JobPointer->display(); // Print modified job
                cout << "Jobs after modification:" << endl;
                myNovelQueue->display(); // Uses display() method to print all nodes in queue
                break;
            }
            case 'C': {
                cin >> job_id >> field_index >> new_value; // Takes input to modify a specific field of a CPUJob object
                int pos = myNovelQueue->change(job_id, field_index, new_value); // Changes job's field_index with new_value and returns index within queue
                if (pos == -1) { // Determine if job was found within queue
                    cout << "Job with ID " << job_id << " not found in the queue." << endl;
                    break;
                }
                cout << "Changed Job ID " << job_id << " field " << field_index << " to " << new_value << ":" << endl;
                myNovelQueue->NodePtrs[pos]->JobPointer->display(); // Print changed job
                cout << "Jobs after changing field:" << endl;
                myNovelQueue->display(); // Uses display() method to print all nodes in queue
                break;
            }
            case 'P': {
                cin >> job_id >> positions; // Takes input to promote a certain job within the queue
                int pos = myNovelQueue->promote(job_id, positions); // Promotes job within queue and returns index of job
                if (pos != -1) { // Determines if job was actually found within queue
                    cout << "Promoted Job ID " << job_id << " by " << positions << " Position(s):" << endl;
                    myNovelQueue->NodePtrs[pos]->JobPointer->display(); // Print promoted job
                    cout << "Jobs after promotion:" << endl;
                    myNovelQueue->display(); // Uses display() method to print all nodes in queue
                    break;
                }
                else { // If job not found do nothing
                    break;
                }
            }
            case 'O': {
                cin >> attribute_index; // Takes input to determine which field to reorder the queue by
                myNovelQueue->reorder(attribute_index); // Reorders queue nodes based on attribute index
                cout << "Reordered Queue by attribute " << attribute_index << ":" << endl;
                myNovelQueue->display(); // Uses display() method to print all nodes in queue
                break;
            }
            case 'D': { // Displays all jobs within queue
                cout << "Displaying all jobs in the queue:" << endl;
                myNovelQueue->display(); // Uses display() method to print all nodes in queue
                break;
            }
            case 'N': { // Prints number of nodes in the queue
                int size = myNovelQueue->count(); // Grabs amount of nodes in queue
                cout << "Number of elements in the queue: " << size << endl;
                break;
            }
            case 'L': { // Lists jobs sorted by job IDs
                myNovelQueue->reorder(1); // Reorders queue based on field 1 (job_ID)
                cout << "List of jobs sorted by job IDs:" << endl;
                myNovelQueue->display(); // Uses display() method to print all nodes in queue
                break;
            }
            default: // Default case for undefined commands
                cout << "Invalid command" << endl;
        }
    }

    delete myNovelQueue; // Delete NovelQueue after program completion

    return 0; // End program
} 

/*

LLM and AI Usage Documentation:
    Prompt and Suggestion 1: I asked the AI, "Generate a method for the enqueue operation in my NovelQueue class."
    Rationale 1: This prompt helped me create a foundational method for my queue implementation.
    Incremental Development 1: The AI provided a basic enqueue method that I modified to include checks for an empty queue and proper pointer management.

    Prompt and Suggestion 2: I asked, "How can I implement the dequeue method for a queue using a circular linked list?"
    Rationale 2: This prompt was to help me efficiently implement the dequeue operation.
    Incremental Development 2: The guidance helped me optimize the dequeue logic to correctly update the front and rear pointers.

    Prompt and Suggestion 3: I asked, "What are common problems when managing memory in C++ with linked lists?"
    Rationale 3: This helped me understand potential memory management issues in my project.
    Incremental Development 3: I used this information to implement proper destructor logic and avoid memory leaks.

    Prompt and Suggestion 4: I queried, "Why is my output correct but not in the right order?"
    Rationale 4: I needed to understand why the elements were not dequeuing in the expected sequence.
    Incremental Development 4: The AI suggested reviewing the enqueue and dequeue logic to ensure proper ordering based on how elements were inserted.

    Prompt and Suggestion 5: I asked, "Why does my program sometimes produce infinite output?"
    Rationale 5: I needed to identify why my loop occasionally did not terminate correctly.
    Incremental Development 5: The AI recommended checking the loop conditions and ensuring that the pointers were updated correctly to prevent infinite loops.

Debugging and Testing Plan:
    Specific Test 1: I verified that the enqueue method correctly added elements by checking the queue size after multiple insertions.
    Issue and Resolution 1: I found that the size was not updating correctly because I didn't increment it. I fixed this in the enqueue method.
    Verification 1: After the fix, I printed the queue size after each insertion to confirm accuracy.

    Specific Test 2: I made sure the dequeue method returned the correct element and maintained queue integrity.
    Issue and Resolution 2: The dequeue method initially returned unexpected values due to pointer issues. I added checks to update the front pointer properly.
    Verification 2: I ran several dequeue operations and printed the results to ensure they matched the expected order.

    Specific Test 3: I tested the queue's stability by enqueueing and dequeueing many elements.
    Issue and Resolution 3: I encountered segmentation faults due to memory access issues. I adjusted the pointer logic to prevent this.
    Verification 3: After debugging the code, I ran the tests multiple times without faults, confirming the queue's robustness.

    Specific Test 4: I checked the order of elements after multiple enqueue and dequeue operations.
    Issue and Resolution 4: The output order was incorrect because of how elements were managed in the enqueue and dequeue methods. I revised the logic to ensure proper ordering.
    Verification 4: After implementing the changes, I validated that elements were dequeued in the correct sequence.

    Specific Test 5: I looked out in the program for infinite output during stress testing.
    Issue and Resolution 5: I discovered that the loop conditions were not terminating correctly. I refined the conditions and ensured pointers were updated properly to avoid infinite loops.
    Verification 5: After making the adjustments, I ran the program under various scenarios and confirmed it terminated as expected.


*/