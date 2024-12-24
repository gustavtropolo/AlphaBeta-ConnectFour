#include <iostream>
#include <cstdint>
#include <array>
#include <bitset>
#include <vector>
#include <cassert>
#include <cmath>
#include <any>
#include <random>
#include <algorithm>
using namespace std;

// 7 x 7
uint64_t tilesUsed = 127;
// uint64_t currPlayer = 127;
const uint64_t bottomMask = 0b0000'0000'0000'0000'0000'0100'0000'1000'0001'0000'0010'0000'0100'0000'1000'0001;
const uint64_t everythingButBottomRow = 0b0000'0000'0000'0001'1111'1011'1111'0111'1110'1111'1101'1111'1011'1111'0111'1110;

uint64_t mask, currP;

template <typename T, size_t Rows, size_t Cols>
void printArray(array<array<T, Rows>, Cols> arr) {
    cout << "Printing:" << endl;
    for (int row = 0; row < Rows; row++) {
        for (int col = 0; col < Cols; col++) {
            cout << arr[row][col];
        }
        cout << endl;
    }
}

uint64_t convToBinary(array<array<int, 7>, 7> nums, bool print = false) {
    uint64_t num = 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000;
    //iterate down and right
    if (print) {
        cout << "printing from " << __func__ << endl;
        for (int r = 0; r < nums.size(); r++) {
            for (int c = 0; c < nums[0].size(); c++) {
                cout << nums[r][c];
            }
            cout << endl;
        }
    }

    for (int col = 0; col < nums[0].size(); col++) {
        for (int row = 0; row < nums.size(); row++) {
            int digit = nums[row][col];
            num = num << 1;
            num = num + digit;
        }
    }
    return num;
}


int indentationLevel = 1;

void setIndentation(int level) {
    indentationLevel = level;
}

ostream& indent(ostream& os) {
    os << string(indentationLevel * 3, ' ');
    return os;
}

template<typename T> 
bool operator == (const array<array<int, 7>, 7> actual, const array<array<int, 7>, 7> expected) {
    if (actual.size() != expected.size() || actual[0].size() != expected[0].size()){
        cout << "Array sizes don't match" << endl;
        return false;
    }
    for (int row = 0; row < actual.size(); row++) {
        for (int col = 0; col < actual[0].size(); col++) {
            if (actual[row][col] != expected[row][col]) {
                return false;
            }
        }
    }
    return true;
}

ostream& operator << (ostream& os, const array<array<int, 7>, 7> arr) {
    os << '\n';
    for (int row = 0; row < arr.size(); row++) {
        os << "[";
        for (int col = 0; col < arr[0].size(); col++) {
            os << arr[row][col] << " ";
        }
        os << "]\n";
    }
    return os;
}

array<array<int, 7>, 7> convToArray(uint64_t num, bool print = false) {
    array<array<int, 7>, 7> nums;
    for (int c = 6; c >= 0; c--) { //changed here
        for (int r = 6; r >= 0; r--) {
            //we start on row 7, iterate up to row 0
            nums[r][c] = num & 1;
            num = num >> 1;
        }
    }
    if (print) {
        cout << "printing from " << __func__ << endl;
        cout << nums;
    }
    
    return nums;
}

template <typename T>
bool result(T actual, T expected, int testNum, bool print = false) {
    if (print) {
        if (actual == expected) {
            cout << indent << "Test " << testNum << "\033[32m passed.\033[0m" << endl;
            cout << "\033[0m";
            return true;
        } else {
            cout << indent << "Test " << testNum << "\033[31m failed.\033[0m" << endl;
            cout << indent << "\033[1;33m" << "Actual: " << actual << "\nExpected: " << expected << "\033[0m" << endl;
        }
    }
    bool res = actual == expected;
    return actual == expected;
}

void printResults(string fName, vector<bool> results) {
    int passed = 0;
    int failed = 0;
    for (bool result : results) {
        if (result) {
            passed++;
        } else {
            failed++;
        }
    }
    int totalWidth = 18; // Total width for alignment
    int prefixWidth = fName.length() + 2; // Including ": "
    int indentWidth = max(0, totalWidth - prefixWidth);
    string prefix = fName + ": " + string(indentWidth, ' ');

    // Output results
    if (passed > 0 && failed == 0) {
        cout << prefix << "All tests \033[32mpassed.\033[0m" << endl;
    } else {
        cout << prefix << passed << " tests \033[32mpassed.\033[0m and "
                  << failed << " tests \033[31mfailed.\033[0m" << endl;
    }
}

array<array<int, 7>, 7> randBoard(int num) {
    array<array<int, 7>, 7> board = {0};
    for (int i = 0; i < num; i++) {
        board[1 + rand() % 6][rand() % 7] = 1; //6 rows, 7 cols
    } //avoid row 0
    return board;    
}

void testconvToBinary(bool print = false) {
    vector<bool> results;
    bool res;

    array<array<int, 7>, 7> arr = {0};
    arr[6][6] = 1;
    bool actual = (convToArray(convToBinary(arr)) == arr);
    bool expected = true;
    results.push_back(result(actual, expected, 1, print));

    for (auto& row : arr) {
        std::fill(row.begin(), row.end(), 0);
    }
    for (int i = 2; i < 7; i++) {
        arr[i][floor(i/2)] = 1;
    }
    actual = (convToArray(convToBinary(arr)) == arr);
    expected = true;
    results.push_back(result(actual, expected, 2, print));

    
    for (auto& row : arr) {
        std::fill(row.begin(), row.end(), 0);
    }
    arr = randBoard(28);
    actual = (convToArray(convToBinary(arr)) == arr);
    expected = true;
    results.push_back(result(actual, expected, 3, print));
    

    printResults(__func__, results);
}

bool checkForWin(array<array<int, 7>, 7> arr) {
    int counter = 0;
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 7; col++) {
            if (arr[row][col] == 1) {
                counter++;
            } else {
                counter = 0;
            }
            if (counter == 4) {
                return true;
            }
        }
        counter = 0;
    }
    counter = 0;
    for (int col = 0; col < 7; col++) {
        for (int row = 0; row < 7; row++) {
            if (arr[row][col] == 1) {
                counter++;
            } else {
                counter = 0;
            }
            if (counter == 4) {
                return true;
            }
        }
        counter = 0;
    }
    //diagonals
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 7; col++) {
            if (row + 3 < 7 && col + 3 < 7) {
                if (arr[row][col] == 1 && //down to right
                arr[row + 1][col + 1] == 1 &&
                arr[row + 2][col + 2] == 1 &&
                arr[row + 3][col + 3] == 1) {
                    return true;
                }
            }
            if (row + 3 < 7 && col - 3 >= 0) {
                if (arr[row][col] == 1 && //down to left
                arr[row + 1][col - 1] == 1 &&
                arr[row + 2][col - 2] == 1 &&
                arr[row + 3][col - 3] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool isWin(uint64_t binary, bool printVert = false, bool printHoriz = false, bool printDLeft = false, bool printDRight = false) {
    uint64_t temp = binary;
    temp = (temp << 2) & temp;
    temp = (temp << 1) & temp;
    bool vertWin = temp;

    temp = binary;
    temp = (temp << 14) & temp;
    temp = (temp << 7) & temp;
    bool horizWin = temp;

    temp = binary;
    temp = (temp << 16) & temp; //up to the left
    temp = (temp << 8) & temp;
    bool diagWin = temp;
    temp = binary;
    temp = (temp << 12) & temp; //down to the left
    temp = (temp << 6) & temp;
    diagWin = temp || diagWin;

    return vertWin || horizWin || diagWin;
}

void testWin(bool print = false) {
    vector<bool> results;

    array<array<int, 7>, 7> arr;
    bool actual, expected, res;

    for (int i = 1; i <= 1000; i++) {
        arr = randBoard(15);
        actual = isWin(convToBinary(arr));
        expected = checkForWin(arr);
        res = result(actual, expected, i, print);
        if (res == false) {
            printArray<int, 7, 7>(arr);
            isWin(convToBinary(arr), false, false, false, false); //cols, rows, DL, DR
        }
        results.push_back(res);
    }
    printResults(__func__, results);
}

bool isLegalMove(int col) {
    return mask & (uint64_t(1) << ((col * 7) + 6));
}

array<array<int, 7>, 7> generatePosition() {
    array<array<int, 7>, 7> pos = {0};
    int numAttempts = rand() % 40;
    if (numAttempts % 2 == 1) {numAttempts++;}
    for (int i = 0; i < numAttempts; i++) {
        for (int x = 0; x < 1000; x++) {
            int row = 1 + rand() % 6;
            int col = rand() % 7;
            if ((row == 6 && pos[row][col] == 0) || (row + 1 <= 6 && pos[row + 1][col] != 0 && pos[row][col] == 0)) {
                if (i % 2 == 0) {
                    pos[row][col] = 1;
                    break;
                } else {
                    pos[row][col] = 2;
                    break;
                }
            }
        }
    }
    return pos;
}


array<array<int, 7>, 7> getMask(array<array<int, 7>, 7> arr) {
    array<array<int, 7>, 7> newArr = {0};
    for (int row = 0; row < arr.size(); row++) {
        for (int col = 0; col < arr[0].size(); col++) {
            if (arr[row][col] != 0) {
                newArr[row][col] = 1;
            }
        }
    }
    for (int row = 0; row < arr.size(); row++) {
        for (int col = 0; col < arr[0].size(); col++) {
            if (row == 6 || (newArr[row + 1][col] != 0 && newArr[row][col] == 0)) {
                newArr[row][col] = 1;
            }
        }
    }
    return newArr;
}

array<array<int, 7>, 7> getCurrPlayer(array<array<int, 7>, 7> arr) {
    array<array<int, 7>, 7> newArr = {0};
    for (int row = 0; row < arr.size(); row++) {
        for (int col = 0; col < arr[0].size(); col++) {
            if (arr[row][col] == 1) {
                newArr[row][col] = 1;
            }
        }
    }
    return newArr;
}

array<array<int, 7>, 7> makeMoveOld(int colMove, array<array<int, 7>, 7> pos) {
    array<array<int, 7>, 7> newPos;
    for (int row = 0; row < pos.size(); row++) {
        for (int col = 0; col < pos[0].size(); col++) {
            if (col == colMove && (row == 6 || pos[row + 1][col] != 0) && pos[row][col] == 0) {
                newPos[row][col] = 1;
            } else {
                newPos[row][col] = pos[row][col];
            }
        }
    }
    for (int row = 0; row < newPos.size(); row++) {
        for (int col = 0; col < newPos[0].size(); col++) {
            if (newPos[row][col] == 2) {
                newPos[row][col] = 0;
            }
        }
    }
    return newPos;

}

void makeMove(int col) { //returns currP but later will just update globally
    uint64_t maskShiftedDown = ((mask & everythingButBottomRow) >> 1);
    uint64_t otherPlayer = currP ^ maskShiftedDown;
    mask = mask | (mask + (uint64_t(1) << (7 * (6 - col))));
    maskShiftedDown = ((mask & everythingButBottomRow) >> 1);
    currP = maskShiftedDown - otherPlayer;
}

void testMakeMove(bool print = false) {
    vector<bool> results;
    array<array<int, 7>, 7> pos, actual, expected;
    int col;
    bool res;

    for (int i = 1; i <= 1; i++) {
        col = rand() % 7;
        pos = generatePosition();
        while (pos[1][col] != 0) {
            col = rand() % 7; //get a empty col
        }
        currP = convToBinary(getCurrPlayer(pos));
        mask = convToBinary(getMask(pos));
        makeMove(col);
        actual = convToArray(currP);
        expected = makeMoveOld(col, pos);
        res = result(actual, expected, i, print);
        results.push_back(res);
    }
    printResults(__func__, results);
}

void undoMove(int col) {
    //remove from currP and mask
    //switch back to the other player
    currP = currP ^ ((mask & everythingButBottomRow) >> 1); //switch back to last player
    uint64_t newCol = (mask & (0b1111110 << (7 * (6 - col)))) >> 1; //just the column
    //get all of the mask except this col
    mask = mask & ~(0b1111111 << (7 * (6 - col))); //everything but the column we are replacing
    mask = mask | newCol;
}

void testUndoMove(bool print = false) {
    vector<bool> results;
    array<array<int, 7>, 7> pos, actual, expected, actual2, expected2;
    uint64_t mask, currP;
    int col;
    bool res, res2;

    for (int i = 1; i <= 100; i++) {
        col = rand() % 7;
        pos = generatePosition();
        while (pos[1][col] != 0) {
            col = rand() % 7; //get a not full col
        }
        currP = convToBinary(getCurrPlayer(pos));
        mask = convToBinary(getMask(pos));
        makeMove(col);
        undoMove(col);
        actual = convToArray(currP);
        expected = getCurrPlayer(pos);
        actual2 = convToArray(mask);
        expected2 = getMask(pos);
        res = result(actual, expected, i, print);
        res2 = result(actual2, expected2, 2, print);
        results.push_back(res);
        results.push_back(res2);
    }
    printResults(__func__, results);
}

void testAll() {
    cout << "\nTesting testconvToBinary..." << endl;
    testconvToBinary(true);
    testWin(false);
    testMakeMove(true);
    testUndoMove(false);
    cout << endl;
}


int main()
{
    testAll();
    convToArray(bottomMask);
    convToArray(mask);
    return 0;

}



