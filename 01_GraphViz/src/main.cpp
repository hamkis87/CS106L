#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "SimpleGraph.h"

using namespace std;

void Welcome();
void readGraph(SimpleGraph & graph);
bool wellFormedNodesNumber(ifstream & graphFile, SimpleGraph & graph);
bool wellFormedEdges(ifstream & graphFile, SimpleGraph & graph);
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
    int numberOfNodes;
    ifstream graphFile;
    cout << "Enter the name of graph file: ";
    string graphFileName = GetLine();
    graphFile.open(graphFileName);
    if (!graphFile.is_open()) {
        cerr << "Cannot find the file" << graphFileName << endl;
    }
    else {
        cout << "file read success" << endl;
        if(wellFormedNodesNumber(graphFile, graph)) {
            cout << "number of nodes = " << graph.nodes.size() << endl;
            if(wellFormedEdges(graphFile, graph)) {
                cout << "well formed edges" << endl;
                cout << "No. of Edges: " << graph.edges.size() << endl;
            }
        }
        else {
            cerr << "Number of Nodes is not well-formed." << endl;
        }

    }
}

bool wellFormedNodesNumber(ifstream & graphFile, SimpleGraph & graph) {
    string line;
    bool result = false;
    if(getline(graphFile, line)) {
        stringstream converter;
        converter << line;
        /* Try reading an int, continue if we succeeded. */
        int inputInt;
        if(converter >> inputInt) {
            char remaining;
            if(converter >> remaining) // Something's left, input is invalid
                cout << "Unexpected character: " << remaining << endl;
            else {
                graph.nodes.resize(inputInt);
                result = true;
            }

        }
    }
    return result;
}

bool wellFormedEdges(ifstream & graphFile, SimpleGraph & graph) {
    string line;
    bool result = false;
    while(getline(graphFile, line)) {
        stringstream converter;
        converter << line;
        /* Try reading an int, continue if we succeeded. */
        int start, end;
        if(converter >> start && converter >> end) {
            char remaining;
            if(converter >> remaining) {// Something's left, input is invalid
                cout << "Unexpected character: " << remaining << endl;
                return false;
            }
            else {
                cout << "start: " << start << "end: " << end << endl;
                Edge edge;
                edge.start = start;
                edge.end = end;
                graph.edges.push_back(edge);
                result = true;
            }

        }
        else {
            return false;
        }
    }
    return result;
}

string GetLine() {
    string result;
    getline(cin, result);
    return result;
}
