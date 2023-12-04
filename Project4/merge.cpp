#include <iostream>
#include <fstream>
#include <cstring>

// Global matrix for dynamic programming
bool dp[1001][1001];

int main() {
    std::string inputFile, outputFile, A, B, C;
    std::ifstream readInput;
    std::ofstream readOutput;

    std::cout << "Enter name of input file: ";
    std::cin >> inputFile;
    std::cout << "Enter name of output file: ";
    std::cin >> outputFile;

    readInput.open(inputFile.c_str());
    readOutput.open(outputFile.c_str());

    while (readInput >> A && readInput >> B && readInput >> C) {
        // Length of A+B should match the length of C
        if ((A.length() + B.length()) != C.length()) {
            readOutput << "*** NOT A MERGE ***" << std::endl;
            continue;
        }

        // Initialize first row and column of the dp matrix
        for (int i = 0; i <= B.length(); ++i) {
            for (int j = 0; j <= A.length(); ++j) {
                // Initialize all elements to false
                dp[i][j] = false;

                // Initialize the first row with chars from A
                if (i == 0 && j > 0) {
                    if (C[j-1] == A[j-1]) {
                        if (j == 1) {
                            dp[i][j] = true;
                        } 
                        else {
                            dp[i][j] = dp[i][j-1];
                        }
                    }
                }

                // Initialize the first column with chars from B
                else if (j == 0 && i > 0) {
                    if (C[i-1] == B[i-1]) {
                        if (i == 1) {
                            dp[i][j] = true;
                        } 
                        else {
                            dp[i][j] = dp[i-1][j];
                        }
                    }
                }
            }
        }

        // Update the rest of the matrix based on previous computations
        for (int i = 1; i <= B.length(); ++i) {
            for (int j = 1; j <= A.length(); ++j) {
                if (dp[i-1][j] && C[i+j-1] == B[i-1]) {
                    dp[i][j] = true;
                } else if (dp[i][j-1] && C[i+j-1] == A[j-1]) {
                    dp[i][j] = true;
                }
            }
        }

        // Check if the last element in the matrix is true
        if (dp[B.length()][A.length()]) {
            int m = B.length();
            int n = A.length();
            // Backtrack through the matrix to capitalize letters from A
            while (n > 0 && m >= 0) {
                if (dp[m][n] && (m == 0 || !dp[m-1][n])) {
                    C[m+n-1] = std::toupper(C[m+n-1]);
                    n--;
                } else {
                    m--;
                }
            }
            readOutput << C << std::endl;
        } else {
            readOutput << "*** NOT A MERGE ***" << std::endl;
        }
    }

    readInput.close();
    readOutput.close();
    return 0;
}
