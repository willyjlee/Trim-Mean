#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

// printing the usage message
void usagemsg(){
    char msg[] = "usage: ./trimmean [--help] <filename> <percentage>\n" \
    "percentage: e.g. 1.00 or 0.89. Must be between 0 and 1\n";
    cout << msg;
}

// parsing a double from a string but printing usage message on failure
double parseDouble(const char* str) {
    char* res = nullptr;
    double percentage = strtod(str, &res);
    
    // check for error on converting and for overflow
    if((percentage == 0 && res != nullptr) || percentage == HUGE_VAL) {
        
        // print usage message and exit
        usagemsg();
        exit(0);
    }
    return percentage;
}

// find the kth smallest value in vector vec.
// Assume left <= k <= right
double kthSmallest(vector<double>& vec, int left, int right, int k, int& numright, int& numleft) {
    
    // base case where only one element of vec considered
    if(left == right)
        return vec[left];
    
    // take the pivot as leftmost element
    double pivot = vec[left];
    int i = left + 1;
    double temp;
    
    // scan left to right and swap with element at i if less than pivot
    for(int j = left + 1; j <= right; j++) {
        if(vec[j] < pivot) {
            temp = vec[i];
            vec[i] = vec[j];
            vec[j] = temp;
            i++;
        }
    }
    
    // swap the pivot value to where it belongs
    temp = vec[left];
    vec[left] = vec[i - 1];
    vec[i - 1] = temp;
    
    int begin = i - 1;;
    for(int j = i; j <= right; j++) {
        if(vec[j] == pivot) {
            temp = vec[i];
            vec[i] = vec[j];
            vec[j] = temp;
            i++;
        }
    }
    
    // after the loop, all elements are partitioned:
    // all elemenets on left are less than pivot except for at the left index
    
    // vec[i-1] is now in place
    if(begin <= k && k <= i - 1) {
        
        // find number of equal elements on either side of the pivot
        numright = i - 1 - k;
        numleft = k - begin;
        // this is the element at position k if vec were sorted
        return vec[k];
    }
    else if (k < begin) {
        
        // find the kth smallest element only considering left side of pivot
        return kthSmallest(vec, left, begin - 1, k, numright, numleft);
    }
    // find the kth smallest element only considering right side of pivot
    return kthSmallest(vec, i, right, k, numright, numleft);
}

int main(int argc, char* argv[]){
    
    // handle the help option
    string option;
    for(int i = 1; i < argc; i++) {
        option = argv[i];
        if(option == "--help") {
            usagemsg();
            return 0;
        }
    }
    if(argc != 3) {
        usagemsg();
        return 0;
    }
    
    // open file argv[1] with argv[2] percentage
    // read argv[2] for a double value
    double percentage = parseDouble(argv[2]);
    if(percentage < 0 || percentage > 1) {
        usagemsg();
        return 0;
    }
    
    ifstream finput(argv[1]);
    if(!finput.is_open()) {
        cout << "error opening file" <<endl;
        return 1;
    }
    
    vector<double> set;
    string line;
    double d;
    
    // read the file into a vector to apply select method
    while(getline(finput, line)) {
        if(line.empty())
            continue;
        d = parseDouble(line.c_str());
        set.push_back(d);
    }
    finput.close();
    
    // find number of elements to trim off both sides
    int trim = ((int)(set.size() * percentage))/2;
    
    int index = trim - 1;
    bool hasBounds = (index >= 0 && index < set.size());
    
    double min;
    double max;
    
    
    int numrightmin = -1;
    int numleftmin = -1;
    int numrightmax = -1;
    int numleftmax = -1;
    // if trimming is possible
    if(hasBounds) {
        min = kthSmallest(set, 0, set.size() - 1, index, numrightmin, numleftmin);
        max = kthSmallest(set, 0, set.size() - 1, set.size() - 1 - index, numrightmax, numleftmax);
    }
    
    // numSet is number of elements in the set to take the mean
    int numSet = set.size() - 2 * trim;
    double average = 0;
    
    double add;
    // loop all values in vec that are within the range more than min but less than max
    for(int i = 0; i < set.size(); i++) {
        add = set[i]/numSet;

        // no bounds
        if(!hasBounds) {
            average += add;
        } else {

            // numbers are in between min and max
            if(set[i] > min && set[i] < max)
                average += add;

            // adding numbers that are thrown away because of the comparison without index
            if(set[i] == min && numrightmin > 0) {
                average += add;
                numrightmin--;
            }

            // adding numbers that are thrown away because of the comparison without index
            if(set[i] == max && numleftmax > 0) {
                average += add;
                numleftmax--;
            }
        }
    }
    cout << average << endl;
    
}




