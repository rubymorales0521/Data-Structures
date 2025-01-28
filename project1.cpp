#include <iostream>
using namespace std;

class SparseRow {
protected:
    int row;
    int col;
    int value;

public:
    SparseRow() {
        row = -1;
        col = -1;
        value = 0;
    }

    SparseRow(int r, int c, int v) {
        row = r;
        col = c;
        value = v;
    }

    int getRow() const {
        return row;
    }

    int getCol() const {
        return col;
    }

    int getValue() const {
        return value;
    }

    void setRow(int r) {
        row = r;
    }

    void setCol(int c) {
        col = c;
    }

    void setValue(int v) {
        value = v;
    }

    void display() const {
        cout << row << ", " << col << ", " << value << endl;
    }
};

class SparseMatrix {
protected:
    int noRows;
    int noCols;
    int commonValue;
    int noNonSparseValues;
    SparseRow *myMatrix;

public:
    SparseMatrix(int n, int m, int cv, int nsv) {
        noRows = n;
        noCols = m;
        commonValue = cv;
        noNonSparseValues = nsv;
        myMatrix = new SparseRow[noNonSparseValues];
    }

    ~SparseMatrix() {
        delete[] myMatrix;
    }

    void readMatrix() {
        int val;
        int matrixIndex = 0;
        for (int i = 0; i < noRows; i++) {
            for (int j = 0; j < noCols; j++){
                cin >> val;
                if(val>0){
              
                    myMatrix[matrixIndex].setRow(i);
                    myMatrix[matrixIndex].setCol(j);
                    myMatrix[matrixIndex].setValue(val);
                    matrixIndex++;
                }
            }
        }
    }

    void displaySMatrix() const {
        for (int i = 0; i < noNonSparseValues; i++) {
            myMatrix[i].display();
        }
    }

    void displayTMatrix() const {
        for (int i = 0; i < noRows; i++) {
            for (int j = 0; j < noCols; j++) {
                bool found = false;
                for (int k = 0; k < noNonSparseValues; k++) {
                    if (myMatrix[k].getRow() == i && myMatrix[k].getCol() == j) {
                        cout << myMatrix[k].getValue() << " ";
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << commonValue << " ";
                }
            }
            cout << endl;
        }
    }


SparseMatrix* Transpose() {
    // Create a new SparseMatrix for the transposed matrix
    SparseMatrix* transposedMatrix = new SparseMatrix(noCols, noRows, commonValue, noNonSparseValues);

    // Count the number of entries for each column in the transposed matrix
    for (int i = 0; i < noNonSparseValues; i++) {
        transposedMatrix -> myMatrix [i].setRow(myMatrix[i].getCol());
        transposedMatrix -> myMatrix [i].setCol(myMatrix[i].getRow());
        transposedMatrix -> myMatrix [i].setValue(myMatrix[i].getValue());
    }
    return transposedMatrix;
}

SparseMatrix* Add(SparseMatrix& M) {
    if (noRows != M.noRows || noCols != M.noCols) {
        cout << "Matrix addition is not possible" << endl;
        return nullptr;
    }

    SparseMatrix* addedMatrix = new SparseMatrix(noRows, noCols, commonValue, noNonSparseValues + M.noNonSparseValues);
        int index = 0;

        for(int i = 0; i< noNonSparseValues; i++) {
            addedMatrix->myMatrix[index++] = myMatrix[i];
        }

        for (int i=0; i< M.noNonSparseValues; i++) {
            bool found = false;
            for (int j =0; j < noNonSparseValues; j++){
                if (M.myMatrix[i].getRow() == myMatrix [j].getRow() && M.myMatrix[i].getCol() == myMatrix[j].getCol()) {
                    addedMatrix -> myMatrix[j].setValue(myMatrix[j].getValue() + M.myMatrix[i].getValue());
                    found = true;
                    break;
                }

            }
            if (!found) {
                addedMatrix ->myMatrix[index++] = M.myMatrix[i];
            }
        }
        return addedMatrix;
        }
           
SparseMatrix* Multiply(SparseMatrix& M) {
        if (noCols != M.noRows) {
            cout << "Matrix multiplication is not possible" << endl;
            return nullptr;
        }

        SparseMatrix *resultMatrix = new SparseMatrix(noRows, M.noCols, commonValue, noNonSparseValues * M.noNonSparseValues);

        int resultIndex = 0;
        for (int i = 0; i < noRows; i++) {
            for (int j = 0; j < M.noCols; j++) {
                int sum = 0;
                for (int k = 0; k < noCols; k++) {
                    int valA = commonValue, valB = M.commonValue;
                    for (int a = 0; a < noNonSparseValues; a++) {
                        if (myMatrix[a].getRow() == i && myMatrix[a].getCol() == k) {
                            valA = myMatrix[a].getValue();
                            break;
                        }
                    }
                    for (int b = 0; b < M.noNonSparseValues; b++) {
                        if (M.myMatrix[b].getRow() == k && M.myMatrix[b].getCol() == j) {
                            valB = M.myMatrix[b].getValue();
                            break;
                        }
                    }
                    sum += valA * valB;
                }

                if (sum != commonValue) {
                    resultMatrix->myMatrix[resultIndex++] = SparseRow(i, j, sum);
                }
            }
        }

        return resultMatrix;
    }
    };

//define all your classes here (as given above)
//write the methods after the class definition
int main () {
   int n, m, cv, noNSV;
   SparseMatrix* temp;


   std::cin >> n >> m >> cv >> noNSV;
   SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);
   //Write the Statements to read in the first matrix
   firstOne->readMatrix();


   cin >> n >> m >> cv >> noNSV;
   SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);
   //Write the Statements to read in the second matrix
   secondOne->readMatrix();


   cout << "First one in sparse matrix format" << endl;
   firstOne -> displaySMatrix();


    // Transpose of the first matrix
    cout << "After transpose" << endl;
    SparseMatrix *transposeFirst = firstOne->Transpose();
    transposeFirst->displaySMatrix();
  
   cout << "First one in matrix format" << endl;
   firstOne -> displayTMatrix();


   cout << "Second one in sparse matrix format" << endl;
   secondOne -> displaySMatrix();


cout << "After transpose" << endl;
    SparseMatrix *transposeSecond = secondOne->Transpose();
    transposeSecond->displaySMatrix();

   cout << "Second one in matrix format" << endl;
   secondOne->displayTMatrix();


  cout << "Matrix addition result" << endl;
    SparseMatrix *additionResult = firstOne->Add(*secondOne);
    if (additionResult) {
        additionResult->displayTMatrix();
    }

    // Matrix multiplication result
    cout << "Matrix multiplication result" << endl;
    SparseMatrix *multiplicationResult = firstOne->Multiply(*secondOne);
    if (multiplicationResult) {
        multiplicationResult->displayTMatrix();
    }

    // Clean up
    delete firstOne;
    delete secondOne;
    delete transposeFirst;
    delete transposeSecond;
    delete additionResult;
    delete multiplicationResult;

    return 0;
}