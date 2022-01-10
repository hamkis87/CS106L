#include <iostream>
#include <string>
#include <fstream>
#include "SimpleGraph.h"

using std::cout;	using std::endl;
using std::string;  using std::cin;
using std::ifstream;

void Welcome();
void readGraph(SimpleGraph & graph);
string GetLine();

// Main method
int main() {
    Welcome();
    SimpleGraph myGraph;
    readGraph(myGraph);
    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

void readGraph(SimpleGraph & graph) {
    ifstream graphFile;
    cout << "Enter the name of graph file: ";
    string graphFileName = GetLine();
    graphFile.open(graphFileName);
    if (!graphFile.is_open()) {
        std::cerr << "Cannot find the file" << graphFileName << endl;
    }
    else {


    }
}

string GetLine() {
    string result;
    getline(cin, result);
    return result;
}
