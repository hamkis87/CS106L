#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <ctime>
#include "SimpleGraph.h"

using namespace std;

#define kPi 3.14159265358979323
#define kRepel 0.001
#define kAttract 0.001

void Welcome();
void readGraph(SimpleGraph & graph);
int getSimulationDuration();
bool wellFormedNodesNumber(ifstream & graphFile, SimpleGraph & graph);
bool wellFormedEdges(ifstream & graphFile, SimpleGraph & graph);
void initiallyPositionNodes(SimpleGraph & myGraph);
void computeRepulsiveForce(const Node & node0, const Node & node1,
                           double & dx0, double & dy0,
                           double & dx1, double & dy1);
void computeAttractForce(const Node & node0, const Node & node1,
                         double & dx0, double & dy0,
                         double & dx1, double & dy1);
void moveNodes(SimpleGraph & graph);
string GetLine();

// Main method
int main() {
    Welcome();
    SimpleGraph myGraph;
    InitGraphVisualizer(myGraph);
    readGraph(myGraph);
    int simulationDuration = getSimulationDuration();
    initiallyPositionNodes(myGraph);
    time_t startTime = time(NULL);
    while(difftime(time(NULL), startTime) < simulationDuration) {
        moveNodes(myGraph);
        DrawGraph(myGraph);
    }
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
    //int numberOfNodes;
    ifstream graphFile;
    cout << "Enter the name of graph file: ";
    string graphFileName = GetLine();
    graphFile.open(graphFileName);
    if (!graphFile.is_open()) {
        cerr << "Cannot find the file" << graphFileName << endl;
    }
    else {
        if(!(wellFormedNodesNumber(graphFile, graph) &&
           wellFormedEdges(graphFile, graph))) {
                cerr << "file is not well-formed." << endl;
        }
    }
}

int getSimulationDuration() {
    int duration;            // duration in seconds
    cout << "Number of seconds to run the algorithm: ";
    string durationAsString = GetLine();
    stringstream converter;
    converter << durationAsString;
    /* Try reading an int, continue if we succeeded. */
    if(converter >> duration) {
        char remaining;
        if(converter >> remaining) { // Something's left, input is invalid
            cout << "Unexpected character: " << remaining << endl;
            return 0;
        }
        else
            return duration;
    }
    else
        return 0;
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

void initiallyPositionNodes(SimpleGraph & myGraph) {
    int numberOfNodes = myGraph.nodes.size();
    for (int k = 0; k < numberOfNodes; ++k) {
        double angle = 2 * kPi * (k + 1) / numberOfNodes;
        myGraph.nodes.at(k).x = cos(angle);
        myGraph.nodes.at(k).y = sin(angle);
    }
}

void computeRepulsiveForce(const Node & node0, const Node & node1,
                           double & dx0, double & dy0,
                           double & dx1, double & dy1) {
    double x0 = node0.x;
    double y0 = node0.y;
    double x1 = node1.x;
    double y1 = node1.y;
    double xDiff = x1 - x0;
    double yDiff = y1 - y0;
    double fRepel = kRepel / sqrt(xDiff * xDiff + yDiff * yDiff);
    double theta = atan2(yDiff, xDiff);
    double xComponent = fRepel * cos(theta);
    double yComponent = fRepel * sin(theta);
    dx0 -=  xComponent;
    dy0 -=  yComponent;
    dx1 +=  xComponent;
    dy1 +=  yComponent;
}

void computeAttractForce(const Node & node0, const Node & node1,
                         double & dx0, double & dy0,
                         double & dx1, double & dy1) {
    double x0 = node0.x;
    double y0 = node0.y;
    double x1 = node1.x;
    double y1 = node1.y;
    double xDiff = x1 - x0;
    double yDiff = y1 - y0;
    double fAttract = kAttract * (xDiff * xDiff + yDiff * yDiff);
    double theta = atan2(yDiff, xDiff);
    double xComponent = fAttract * cos(theta);
    double yComponent = fAttract * sin(theta);
    dx0 +=  xComponent;
    dy0 +=  yComponent;
    dx1 -=  xComponent;
    dy1 -=  yComponent;
}

void moveNodes(SimpleGraph & graph) {
    std::vector<Node> nodesLocationsDelta;   // this vector stores the cumulative dx and dy for each node
    int numberOfNodes = graph.nodes.size();
    int numberOfEdges = graph.edges.size();
    nodesLocationsDelta.resize(numberOfNodes);
    for(int i = 0; i < numberOfNodes; ++i) { // initialize dx and dy for all nodes
        nodesLocationsDelta.at(i).x = 0.0;
        nodesLocationsDelta.at(i).y = 0.0;

    }
    for(int i = 0; i < numberOfNodes - 1; ++i) {  // update dx and dy for all nodes based on repulsive force
        Node node0 = graph.nodes.at(i);
        for(int j = i+1; j < numberOfNodes; ++j) {
            //if(i != j) {
                Node node1 = graph.nodes.at(j);
                computeRepulsiveForce(node0, node1,
                                      nodesLocationsDelta.at(i).x,
                                      nodesLocationsDelta.at(i).y,
                                      nodesLocationsDelta.at(j).x,
                                      nodesLocationsDelta.at(j).y);
            //}
        }
    }
    for(int k = 0; k < numberOfEdges; ++k) { // update dx and dy for all nodes based on attract force
        Edge e = graph.edges.at(k);
        int i = e.start;
        int j = e.end;
        Node node0 = graph.nodes.at(i);
        Node node1 = graph.nodes.at(j);
        computeAttractForce(node0, node1,
                            nodesLocationsDelta.at(i).x,
                            nodesLocationsDelta.at(i).y,
                            nodesLocationsDelta.at(j).x,
                            nodesLocationsDelta.at(j).y);
    }
    for(int i = 0; i < numberOfNodes; ++i) { // update each node location according to its dx and dy
        graph.nodes.at(i).x += nodesLocationsDelta.at(i).x;
        graph.nodes.at(i).y += nodesLocationsDelta.at(i).y;
    }
}

string GetLine() {
    string result;
    getline(cin, result);
    return result;
}
