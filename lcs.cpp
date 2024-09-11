#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Function to find the longest common subsequence
std::string longestCommonSubsequence(std::string &str1, std::string &str2) {
    int size1 = str1.size();
    int size2 = str2.size();
    std::vector<std::vector<int>> lcsTable(size1 + 1, std::vector<int>(size2 + 1));

    // Building the LCS table
    for (int i = 0; i <= size1; i++) {
        for (int j = 0; j <= size2; j++) {
            if (i == 0 || j == 0)
                lcsTable[i][j] = 0;
            else if (str1[i - 1] == str2[j - 1])
                lcsTable[i][j] = lcsTable[i - 1][j - 1] + 1;
            else
                lcsTable[i][j] = std::max(lcsTable[i - 1][j], lcsTable[i][j - 1]);
        }
    }

    // Reconstructing the LCS from the LCS table
    int index = lcsTable[size1][size2];
    std::string longestSubsequence(index, ' ');
    int i = size1, j = size2;
    while (i > 0 && j > 0) {
        if (str1[i - 1] == str2[j - 1]) {
            longestSubsequence[index - 1] = str1[i - 1];
            i--;
            j--;
            index--;
        } else if (lcsTable[i - 1][j] > lcsTable[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    return longestSubsequence;
}

int main(int argc, char **argv) {
    if (argc > 3) {
        std::cerr << "Error: Too many input strings" << std::endl;
        return -1;
    }

    std::string str1(argv[1]);
    std::string str2(argv[2]);

    if (str1.size() > 100 || str2.size() > 100) {
        std::cerr << "The string length is too long" << std::endl;
        return -2;
    }

    std::string result = longestCommonSubsequence(str1, str2);

    std::cout << "Length of LCS: " << result.length() << std::endl;
    std::cout << "LCS: " << result << std::endl;

    return 0;
}
