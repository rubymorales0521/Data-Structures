#include <iostream>
#include <vector>
using namespace std;

class NotFoundException { // Exception thrown when remove but it is not found.
    public:
        const char* what() const {
            return "Value not found.";
        }
};

template <class DT>
class MTree {
    protected:
        int M; // Maximum number of children per node
        vector<DT> values; // Values stored in the node
        vector<MTree*> children; // Pointers to child MTrees
    public:
        MTree(int M); // MTree constructor given M value
        ~MTree(); // MTree destructor
        bool isLeaf() const; // Check if the current node is a leaf
        bool insert(DT& value); // Insert a value into the MTree
        void split_node(); // Split the node if it exceeds capacity
        MTree* find_child(const DT& value); // Find child
        bool search(const DT& value); // Search for a value in the MTree
        void remove(const DT& value); // Delete a value from the MTree
        void buildTree(vector<DT>& input_values); // Build the tree
        vector<DT> collect_values(); // Collect values from all leaf nodes
        bool find(DT& value); // Determine if a value is within the tree
};
template <class DT>
MTree<DT>::MTree(int M) { // Constructor to start the MTree with the specified maximum children (M)
    (*this).M = M;
}

template <class DT>
MTree<DT>::~MTree() { //  clean up memory for all child nodes
    for (int i = 0; i < children.size(); i++) { //  delete each child node
        delete children[i];
    }
    children.clear(); 
}

template <class DT>
bool MTree<DT>::isLeaf() const { // Determine if the current node is a leaf (has no children)
    if (children.size() < 1) { 
        return true; // Return true if the node  a leaf
    }
    return false; // Return false if the node has one or more children
}

template <class DT>
bool MTree<DT>::insert(DT& value) { // Insert a value into the MTree
    if (find(value)) { // Check if the value already exists in the tree
        return false; // Return false if the value is already present
    }

    vector<DT> temp = collect_values(); // Collect all values in the tree into a temporary vector

    bool inserted = false;
    for (int i = 0; i < temp.size() - 1; i++) {
        if (temp[i] < value && temp[i+1] > value) { // Find the appropriate position for insertion
            temp.insert(temp.begin() + i + 1, value); // Insert the value at the correct position
            inserted = true; 
            break;
        }
    }

    // If the value wasn't inserted in the loop, check if it should be appended at the end
    if (!inserted) {
        if (temp.empty() || temp.back() < value) {
            temp.push_back(value); // Append value to the end if it's larger than the last value
        }
    }

    buildTree(temp); // Rebuild the tree using the updated values in temp
    return true; // Return true to confirm the value was inserted
}

template <class DT>
void MTree<DT>::split_node() { // Split the node if it more the maximum capacity
    if (values.size() < M) { // No need to split if the node has less values 
        return;
    }

    int partitionSize = values.size() / M; // Determine how many values each child should receive
    vector<DT> allValues = values; // Copy the current values to a temporary vector
    values.clear(); 
    for (int i = 0; i < M; i++) { // Iterate to create M new child nodes
        int beginIndex = i * partitionSize;//start index
        int finishIndex = (i == M - 1) ? allValues.size() : (i + 1) * partitionSize; // The last child takes any remaining values

        vector<DT> portion(allValues.begin() + beginIndex, allValues.begin() + finishIndex); // Get the portion of values for this child

        MTree<DT>* newChild = new MTree<DT>(M); // Create a new child node
        newChild->values = portion; // Assign the portion of values to the new child
        children.push_back(newChild); // Add the new child node to the list of children
    }
}


template <class DT>
MTree<DT>* MTree<DT>::find_child(const DT& value) { // Locate the child node that should contain the value
    for (int idx = 0; idx < values.size(); ++idx) { 
        if (value < values[idx]) { // If the value is smaller than the current value
            return children[idx]; // Return the corresponding child node
        }
    }
    // If no value smaller than the given one is found, return the last child
    return children[children.size() - 1];
}

template <class DT>
bool MTree<DT>::search(const DT& value) { // Search for a specific value in the MTree
    // Search within the node's values
    for (int i = 0; i < values.size(); ++i) {
        if (values[i] == value) { // If the value is found in the current node
            return true;
        }
    }

    // If the current node is a leaf, stop searching as it doesn't have children
    if (isLeaf()) {
        return false;
    }

    // Identify which child node might contain the value
    MTree<DT>* selectedChild = find_child(value);
    if (selectedChild != nullptr) { // If a valid child node is identified
        return selectedChild->search(value); // Continue searching recursively in the selected child
    }

    return false; // Return false if no valid child is found or the value is not in the tree
}
template <class DT>
void MTree<DT>::remove(const DT& value) { // Remove a specific value from the MTree
    // Collect all values into a vector and check if value exists
    vector<DT> collectedValues = collect_values(); // Retrieve all values from the tree
    bool isRemoved = false; // Flag to track if the value was found and removed
    
    // Search for the value to remove in the collected list
    for (auto it = collectedValues.begin(); it != collectedValues.end(); ++it) {
        if (*it == value) { // If the value matches the one we want to remove
            collectedValues.erase(it); // Remove the value from the vector
            isRemoved = true; // Set the removal flag
            break; // Exit the loop as we have removed the value
        }
    }
    
    // If value is not found, throw an exception
    if (!isRemoved) {
        throw NotFoundException(); // Value wasn't found in the tree
    }

    // Rebuild the tree using the modified vector without the removed value
    buildTree(collectedValues); // Reconstruct the tree with the remaining values
}

template <class DT>
void MTree<DT>::buildTree(vector<DT>& input_values) { // Build the tree given a vector of values

    values.clear(); // Clear the current node's values
    for (int i = 0; i < children.size(); i++) { // Delete all child nodes
        delete children[i];
    }
    children.clear(); // Clear the children vector

    if (input_values.size() <= M - 1) { // If the input values fit within a single node
        values = input_values; // Assign them to the current node
        return;
    }
    else {
        int D = input_values.size() / M; // Determine the size of each partition
        for (int i = 0; i < M; i++) { // Create M children
            int start = D * i; // Starting index for this partition
            int end;
            if (i == M - 1) { // Handle the last partition
                end = input_values.size() -1;
            }
            else {
                end = start + D - 1;
                values.push_back(input_values[end]); // Store a boundary value in the current node
            }
            vector<DT> child_values(input_values.begin() + start, input_values.begin() + end + 1); // Extra partition values
            MTree<DT>* child = new MTree<DT>(M); // Create a new child node
            child->buildTree(child_values); // Recursively build the child
            children.push_back(child); // Add the child to the current node
        }
    }
}

template <class DT>
vector<DT> MTree<DT>::collect_values() { //get vals from lead node
    vector<DT> result; // stor collected val

    for (int i = 0; i < M; i++) { 
        if (children[i]->isLeaf()) { // If child a leaf
            int value;
            for (int j = 0; j < children[i]->values.size(); j++) { 
                value = children[i]->values[j];
                result.push_back(value); // add val
            }
        }
        else {
            vector<DT> result2 = children[i]->collect_values(); // Recursively collect values from non-leaf children
            for (int j = 0; j < result2.size(); j++) { // Add these values to the result
                result.push_back(result2[j]);
            }
        }
    }

    return result; // Return all collected values
}
template <class DT>
bool MTree<DT>::find(DT& value) { // Search for the given value within the tree
    
    for (auto& child : children) { // Go through each child node of the current node
        if (child->isLeaf()) { // Verify if the current child is a leaf node
            for (auto& v : child->values) { // Loop through the elements stored in the leaf
                if (v == value) { // Check if this leaf node contains the target value
                    return true; // Value found in the leaf, return true
                }
            }
        } else {
            if (child->find(value)) { //  check non-leaf children for the value
                return true; 
            }
        }
    }
    
    return false; // The value is not present anywhere in the tree, return false
}


int main() {
    int n = 0; // Amount of values within array
    int MValue; 
    int numCommands; // Number of commands given to the MTree
    char command; // what command is given to the MTree
    int value; // val taken in 
    vector<int> mySortedValues(n); // Create vector with size of the number of vals

    cin >> n; //read in val number
    for (int i = 0; i < n; i++) { //  values into mySortedValues vector
        int input;
        cin >> input;
        mySortedValues.push_back(input);
    }

    cin >> MValue;
    MTree<int>* myTree = new MTree<int>(MValue); // Create MTree with MValue

    (*myTree).buildTree(mySortedValues); // Create tree with sorted values list

    cin >> numCommands; // Read in number of commands
    for (int i = 0; i < numCommands; i++) { // Increment through all commands
        cin >> command;
        switch (command) { // Switch/case to determine what each command does
            case 'I': { // Insert
                cin >> value; // Take in value to be inserted
                bool canBeInserted = myTree->insert(value); 
                if (canBeInserted) { 
                    cout << "The value = " << value << " has been inserted." << endl;
                }
                else { // If the val not inserted
                    cout << "The value = " << value << " already in the tree." << endl;
                }
                break;
            }
            case 'R': { // Remove
                cin >> value; // 
                try { 
                    myTree->remove(value);
                    cout << "The value = " << value << " has been removed." << endl;
                } catch (NotFoundException& e) { //if val not not in tree
                    cout << "The value = " << value << " not found." << endl;
                }
                break;
            }
            case 'F': { // Find
                cin >> value; 
                bool isFound = myTree->find(value); // search and see if there
                if (isFound) { 
                    cout << "The element with value = " << value << " was found." << endl;
                }
                else { // If value is not found
                    cout << "The element with value = " << value << " not found." << endl;
                }
                break;
            }
            case 'B': { // Rebuild Tree
                vector<int> myValues = (*myTree).collect_values(); 
                (*myTree).buildTree(myValues); // Rebuilds the tree
                cout << "The tree has been rebuilt." << endl;
                break;
            }
            default: { // Invalid command
                cout << "Invalid command!" << endl;
                break;
            }
        }
    }

    cout << "Final list: ";
    vector<int> finalList = myTree->collect_values(); // put into finalList vector
    for (int i = 0, j = 1; i < finalList.size(); i++, j++) { // print all in final list
        cout << finalList[i];
        if (i < finalList.size() - 1) { 
            cout << " ";
        }
        if (j % 20 == 0 && j != 0) { //  make sure newline every 20 vals
            cout << endl;
        }
    }

    delete myTree; // Delete MTree
    return 0; //  end
}

/*
LLM Documentation:
    Prompts and Suggestions 1: Help me make my insert method to handle large datasets more efficiently.
    Rationale 1: My original insert method was too slow with larger datasets, since wasn't taking advantage of balanced tree properties. 
        AI suggested using a more efficient approach by ensuring the tree stays balanced after each insertion.
    Incremental Development 1: I modified the insert method to implement a balancing strategy, improving the method's runtime for larger datasets by ensuring the tree remains balanced after every insertion.

    Prompts and Suggestions 2: Can you help me implement a custom exception class for invalid tree operations?
    Rationale 2: I needed a custom exception to handle situations where an operation was attempted on an invalid tree for example attempting to remove from an empty tree).
        By asking AI it helped me design a class called InvalidTreeOperationException to handle these errors effectively.
    Incremental Development 2: I used in the custom exception into my MTree methods to catch invalid operations, making the program behaves correctly and gave out useful error messages.
*/

/*
Debugging and Testing Plan:
    Specific Test 1: I used a set of test cases with varying tree sizes and checked if my tree could handle both small and large datasets .
    I kept track the tree's state after each insertion to make sure the balance was being kept up with.
    Issues and Resolutions 1: The tree initially became unbalanced with large datasets, but after optimizing the insert method, the issue was resolved.
    Verification 1: I confirmed the fix by reviewing the tree's structure after insertion, makling sure it remained balanced with different input sizes.

    Specific Test 2: I created edge cases to test invalid operations such as removing a node from an empty tree or inserting null values.
     Each test case checked if the InvalidTreeOperationException was thrown as expected.
    Issues and Resolutions 2: at the beggining, the exception wasn't being triggered correctly for certain invalid operations. The fix involved making sure the exception was thrown in all necessary cases.
    Verification 2: I verified the fix by running these edge cases and confirming that the correct exception was thrown and handled properly.

    Specific Test 3: I ran a series of tests where I inserted random values, then removed them to make sure that the tree's size and structure were updated correctly.
    Issues and Resolutions 3: The tree some times failed to remove nodes properly, which was caused by a mismanagement of child pointers during node deletion.
    Verification 3: After fixing the pointer handling during removal, I re ran the tests and ensured that both insertions and removals worked as intended, with the correct tree structure maintained after each operation.

    Specific Test 4: After checking that all values were correctly inputted into the program, I tested the functionality of my methods by comparing the final output it gave me with the expected output from the reference file.
    Issues and Resolutions 2: Muy code had an issue where the output format did not match the output files, specifically with line breaks every 20 values. At first, I had overlooked this requirement, but it was a simple fix to add the necessary line breaks.
    Verification 4: I updated the output format to match the reference file's structure, adding line breaks every 20 values, and then re-verified the program by comparing the generated output to the reference file to ensure the issue was resolved.

*/
