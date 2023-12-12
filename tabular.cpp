#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

struct item {
    vector<int> minterm;
    string binary;
};
struct groupss {
    vector<item> itemz;
};

void take_input(char*, int&, vector<int>&, vector<int>&);

void Quine_McCluskey_Method(int, vector<int>, vector<int>);
vector<groupss> arrange_minterms(vector<int>&, int);
vector<groupss>  one_bit_change_pair2(vector<groupss>, int, vector<groupss>&);

int main() {
    // Initialize input paramters
    char* input_file = (char*)"input.txt\0";
    int number_of_literals = 0;
    vector<int> minterms;
    vector<int> dont_care_terms;

    // Feed input from files
    take_input(input_file, number_of_literals, minterms, dont_care_terms);

    Quine_McCluskey_Method(number_of_literals, minterms, dont_care_terms);

    system("pause");
}

bool is_num(char c) {
    return (c >= '0' && c <= '9');
}

void take_input(char* input_file, int& number_of_literals, vector<int>& minterms, vector<int>& dont_care_terms) {
    number_of_literals = 0;
    cout << "Reading the input from file" << input_file << "...." << endl;

    std::ifstream file(input_file);
    if (!file.is_open()) {
        cerr << "Error opening file: " << input_file << endl;
        exit(EXIT_FAILURE);
    }

    // Determine the size of the file
    file.seekg(0, std::ios::end);
    streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the entire file into a char array
    char* buffer = new char[fileSize];
    file.read(buffer, fileSize);

    // Close the file
    file.close();

    int line = 0;
    int num = -1;

    for (int i = 0; i < fileSize; i++) {
        char c = buffer[i];
        if (line == 0) {
            if (c == '\n') line++;
            else {
                if (is_num(c)) {
                    number_of_literals *= 10;
                    number_of_literals += c - '0';
                }
            }
        }
        else if (line == 1) {
            if (c == '\n') {
                if (num != -1) {
                    minterms.push_back(num);
                    num = -1;
                }
                line++;
            }
            else {
                if (is_num(c)) {
                    if (num == -1) num = 0;
                    num *= 10;
                    num += c - '0';
                }
                else {
                    if (num != -1)  minterms.push_back(num);
                    num = -1;
                }
            }
        }
        else if (line == 2) {
            if (c == '\n') {
                if (num != -1) {
                    dont_care_terms.push_back(num);
                    num = -1;
                }
                line++;
            }
            else {
                if (is_num(c)) {
                    if (num == -1) num = 0;
                    num *= 10;
                    num += c - '0';
                }
                else if (c == ' ') {
                    if (num != -1)  dont_care_terms.push_back(num);
                    num = -1;
                }
            }
        }
    }

    if (num != -1) dont_care_terms.push_back(num);

    for (int i = 0; i < dont_care_terms.size(); i++) {
        minterms.push_back(dont_care_terms[i]);
    }

    // Release the allocated memory
    delete[] buffer;
    cout << "File read successfully" << endl << endl;
}

string abc(string bin) {
    string res = "";
    for (int i = 0; i < bin.length(); i++) {
        if (bin[i] != '-') {
            res += 65 + i;
            if (bin[i] == '0') res += "'";
        }
    }
    return res;
}

struct isin {
    int minterm;
    vector<int> vals;
};

bool in_vec(int val, vector<int> vec) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == val) return true;
    }
    return false;
}

template <typename T>
void removeElement(std::vector<T>& vec, const T& valueToRemove) {
    auto it = std::find(vec.begin(), vec.end(), valueToRemove);

    if (it != vec.end()) {
        vec.erase(it);
    }
    else {
        std::cout << "Element not found in the vector." << std::endl;
    }
}

bool not_added(const std::vector<std::vector<int>>& all, const std::vector<int>& val) {
    return std::find(all.begin(), all.end(), val) == all.end();
}

bool not_added2(vector<string> str, string val) {
    for (int i = 0; i < str.size(); i++) {
        bool found = true;
        for (int o = 0; o < val.length(); o++) {
            if (val[o] != str[i][o]) found = false;
        }
        if (found == true) return true;
    }
    return false;
}

void Quine_McCluskey_Method(int no_of_literals, vector<int> minterms, vector<int> dont_care_terms) {
    cout << "Solving..." << endl << endl;
    // Step 1. Arrange the given minterms according to the number of ones present in their binary representation in ascending order.
    vector<groupss> arranged_groups = arrange_minterms(minterms, no_of_literals);
    vector<groupss> prime_implicants;
    vector<groupss> new_grp = one_bit_change_pair2(arranged_groups, no_of_literals, prime_implicants);
    cout << endl << endl;
    while (new_grp[0].itemz.size() > 0) {
        for (int i = 0; i < new_grp.size(); i++) {
            for (int k = 0; k < new_grp[i].itemz.size(); k++) {
                cout << "(";
                for (int l = 0; l < new_grp[i].itemz[k].minterm.size(); l++) {
                    cout << new_grp[i].itemz[k].minterm[l] << ", ";
                }
                cout << ") " << new_grp[i].itemz[k].binary << endl;
            }
            cout << "################################" << endl << endl;
        }
        new_grp = one_bit_change_pair2(new_grp, no_of_literals, prime_implicants);
    }
    cout << endl;
    for (int i = 0; i < new_grp.size(); i++) {
        for (int k = 0; k < new_grp[i].itemz.size(); k++) {
            cout << "matched: (";
            for (int l = 0; l < new_grp[i].itemz[k].minterm.size(); l++) {
                cout << new_grp[i].itemz[k].minterm[l] << ", ";
            }
            cout << ") " << new_grp[i].itemz[k].binary << endl;
        }
        cout << "################################" << endl << endl;
    }

    new_grp = one_bit_change_pair2(new_grp, no_of_literals, prime_implicants);

    //vector<isin> minc;
    map<int, vector<int>> minc;
    map<int, item> minc2;

    for (int i = 0; i < dont_care_terms.size(); i++) {
        removeElement(minterms, dont_care_terms[i]);
    }

    vector<vector<int>> all;
    vector<string> aray;

    for (int i = 0; i < prime_implicants.size(); i++) {
        for (int l = 0; l < prime_implicants[i].itemz.size(); l++) {

            if (not_added2(aray, prime_implicants[i].itemz[l].binary) == false) {
                aray.push_back(prime_implicants[i].itemz[l].binary);
                cout << "prime implicant: (";
                for (int k = 0; k < prime_implicants[i].itemz[l].minterm.size(); k++) {
                    cout << prime_implicants[i].itemz[l].minterm[k] << ",";
                    if (in_vec(prime_implicants[i].itemz[l].minterm[k], minterms)) {
                        minc[prime_implicants[i].itemz[l].minterm[k]].push_back(0);
                        minc2[prime_implicants[i].itemz[l].minterm[k]] = prime_implicants[i].itemz[l];
                    }
                }
                cout << ") " << abc(prime_implicants[i].itemz[l].binary) << endl;
            }



        }
        cout << endl << endl;
    }

    aray.resize(0);

    cout << "simplifed equation: ";
    for (int i = 0; i < minterms.size(); i++) {
        if (minc[minterms[i]].size() == 1) {
            if (not_added2(aray, minc2[minterms[i]].binary) == false) {
                cout << abc(minc2[minterms[i]].binary) << " + ";
                aray.push_back(minc2[minterms[i]].binary);
            }
        }
    }

    cout << endl << "Solved!" << endl;
}

int countOnes(int num) {
    int count = 0;
    while (num > 0) {
        count += num % 2;
        num /= 2;
    }
    return count;
}

int countSetBits(int n) {
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}
std::string intToBinary(int n) {
    if (n == 0) return "0";
    std::string binary = "";
    while (n > 0) {
        binary = (n % 2 == 0 ? "0" : "1") + binary;
        n /= 2;
    }
    return binary;
}

struct groups {
    vector<vector<int>> term;
    string binary;
};

vector<groupss> arrange_minterms(vector<int>& minterms, int no) {
    sort(minterms.begin(), minterms.end(), [](int a, int b) {
        return countOnes(a) < countOnes(b);
        });

    vector<vector<int>> mintermGroups;

    // Create a map to store minterms based on the count of set bits
    map<int, vector<int>> groupsMap;

    // Iterate through each minterm and group them based on the count of set bits
    for (int minterm : minterms) {
        int count = countSetBits(minterm);
        groupsMap[count].push_back(minterm);
    }

    // Convert the map to a vector of vectors
    for (const auto& group : groupsMap) {
        mintermGroups.push_back(group.second);
    }

    vector<groupss> ret;

    for (int i = 0; i < mintermGroups.size(); i++) {
        groupss temp;
        for (int o = 0; o < mintermGroups[i].size(); o++) {
            vector<int> k;
            k.push_back(mintermGroups[i][o]);
            string bin = intToBinary(mintermGroups[i][o]);
            while (bin.length() < no) bin.insert(0, "0");
            temp.itemz.push_back({ k, bin });
            cout << mintermGroups[i][o] << ", ";
        }
        cout << endl;
        ret.push_back(temp);
    }
    return ret;
}

bool one_bit(string str1, string str2) {
    int c = 0;
    for (int i = 0; i < str1.length(); i++) {
        if (str1[i] != str2[i]) c++;
    }
    return c <= 1;
}

string match(string str1, string str2) {
    string str = "";
    for (int i = 0; i < str1.length(); i++) {
        if (str1[i] != str2[i]) str += '-';
        else str += str1[i];
    }
    return str;
}


vector<groupss> one_bit_change_pair2(vector<groupss> grps, int no_lit, vector<groupss>& prime_implicants) {
    vector<groupss> new_grp;
    vector<item> curr_prime_imp;
    vector<vector<int>> all;
    for (int i = 0; i < grps.size(); i++) {
        groupss temp;
        groupss temp2;
        for (int o = 0; o < grps[i].itemz.size(); o++) {
            if (i + 1 < grps.size()) {
                bool found = false;
                for (int k = 0; k < grps[i + 1].itemz.size(); k++) {
                    if (one_bit(grps[i + 1].itemz[k].binary, grps[i].itemz[o].binary)) {
                        found = true;
                        vector<int> aray;
                        for (int l = 0; l < grps[i + 1].itemz[k].minterm.size(); l++) {
                            aray.push_back(grps[i + 1].itemz[k].minterm[l]);
                        }
                        all.push_back(aray);
                        for (int l = 0; l < grps[i].itemz[o].minterm.size(); l++) {
                            aray.push_back(grps[i].itemz[o].minterm[l]);
                        }
                        all.push_back(aray);
                        temp.itemz.push_back({ aray, match(grps[i + 1].itemz[k].binary, grps[i].itemz[o].binary) });
                    }
                }
                if (!found) {
                    vector<int> aray;
                    for (int l = 0; l < grps[i].itemz[o].minterm.size(); l++) {
                        aray.push_back(grps[i].itemz[o].minterm[l]);
                    }
                    if (not_added(all, aray))
                        temp2.itemz.push_back({ aray, grps[i].itemz[o].binary });
                }
            }
        }
        new_grp.push_back(temp);
        if (temp2.itemz.size() > 0) {
            prime_implicants.push_back(temp2);
        }
    }
    return new_grp;
}
