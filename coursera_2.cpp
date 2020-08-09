// Author Sriram Kodey
// for Assignment 2 for C++ for C Programmers, Coursera

// change value of const float node_probe to change the graph density


#include <iostream>
#include <cmath>
#include <time.h>

const float node_prob = .40; // graph density has been defined
const int g_size = 50;       // size of graph has been defined
const int node_edge = static_cast<int>(static_cast<float>(g_size-1)*node_prob); // variable used for construction of random graph
int curr = 0, t_curr = 0;        // variable used for construction of random graph

using namespace std;

int findMinVertex(int* distance, bool* visited, int n)  // function return the minimum distant node from a particular based node
{                                                       // used of implementation of Dijkstra's shortest path algorithm
    int minVertex = -1;
    for (int i=0; i<n; i++)
    {
        if(!visited[i] && (minVertex == -1 || distance[i]<distance[minVertex]))
        {
            minVertex = i;
        }
    }
    return minVertex;  //returning the calculated minVertex
}

class prob   // class used for construction of random bidirectional graph with a particular density or node probability
{
public:
    prob(){} // constructor
    void setup();      // will setup the array required for assigning edges with the required probability or density
    bool fetchFlag(); //  will give information about previously picking the value, ensuring all the values will get picked only once
    ~prob(){} //desrtructor
private:
    bool arr[g_size][2];
};

void prob::setup() // definition of setup function
{
    int counter = 0;
    for (int i=0; i<(g_size-curr); i++)
    {
        if(counter<=(node_edge-t_curr))  // allotting the right number of true values based on the density value
        {
            arr[i][0] = true;
            arr[i][1] = false;
            counter++;
        }
        else
        {
            arr[i][0] = false;
            arr[i][1] = false;
        }
    }
}

bool prob::fetchFlag()  // method that allots edges to the graph
{
    bool collectFlag = false;
    int num = (rand()%(g_size-curr));   // to randomly allot values

    while(!collectFlag)
    {
        if(!arr[num][1])
        {
            arr[num][1] = true;
            return arr[num][0];
        }
        else
        {
            num = (num+1)%(g_size-curr);   // moving linearly to reduce computational time
        }
    }
}

class edge    // the main micro data structure
{
public:
    edge(){}   //constructor
    void setFlag(bool temp); // sets flag data(presence of edge)
    void setDist(float temp); // sets the distance of edge
    bool getFlag();      // reads the private data
    float getDist();    // reads the private data
    ~edge(){}
private:
    bool flag;    // signifies the presence of edge
    float dist;   // mentions the distance or cost
};

void edge::setDist(float temp)
{
    dist = temp;
}

bool edge::getFlag()
{
    return flag;
}

float edge::getDist()
{
    return dist;
}

void edge::setFlag(bool temp)
{
    flag = temp;
}

class graph      // graph class
{
public:
    graph();  // constructor
    void randInit();
    void print();
    void setDist();
    bool isConnected();
    void dijkstraAglo();
    ~graph();   // destructor

private:
    edge** gph;
};

graph::graph()  // constructor, nothing fancy
{
    cout<<"\n"<<"Graph Constructor called"<<"\n"<<endl;

    gph = new edge*[g_size];
    for (int i = 0 ; i < g_size; i++)
    {
        gph[i] = new edge[g_size];
    }
}

void graph::setDist()  // setting distance and making sure that the edge will have same distance in both directions
{
    srand(time(0));
    for(int i=0;i<g_size;i++)
    {
        for(int j=i; j<g_size; j++)
        {
            if(gph[i][j].getFlag())
            {
                if(j==i)
                {
                    gph[i][j].setDist(0);
                    gph[j][i].setDist(0);
                }
                else
                {
                    gph[i][j].setDist((rand()%10)+1); // distance form 1.0 to 10.0
                    gph[j][i].setDist(gph[i][j].getDist());
                }
            }
            else
            {
                gph[i][j].setDist(1000); // making distance infinite
                gph[j][i].setDist(1000);
            }
        }
    }
}

void graph::randInit() // random initializer
{
    for(int i=0; i<g_size; i++)
    {
        t_curr = 0;
        curr = i;
        prob p1;

        int j=0;

        while(j<i)
        {
            if(gph[i][j].getFlag()==1){t_curr++;}
            ++j;
        }

        gph[i][j].setFlag(false);
        ++j;

        p1.setup();

        while(j<g_size)
        {
            gph[i][j].setFlag(p1.fetchFlag());  //very simple, just assigning values using functions and methods defined earlier
            gph[j][i].setFlag(gph[i][j].getFlag());
            ++j;
        }
    }
}

void graph::dijkstraAglo()  // the algorithm
{
    int avg_dist = 0;
    int avg_nodes = 0;       // initial setup
    int distance[g_size];
    bool visited[g_size];

    int close[g_size][2];
    int open[g_size][3];

    for(int i = 0; i<g_size; i++) // initial setup where the best distance to all known nodes is infinite
    {
        distance[i] = 1000;
        visited[i] = false; // no node has  been visited yet
    }

    distance[0] = 0; // starting at 0 and 0 always connected to 0.

    for(int i=0; i<g_size-1; i++)  // main algo loop
    {
        int minVertex = findMinVertex(distance, visited, g_size);  //calls the minVertex function to get the next node which is least distant from
        visited[minVertex] = true;                                 // from current node, basically replicating a priority queue
        for(int j=0; j<g_size; j++)
        {
            if(gph[minVertex][j].getDist()!=0 && !visited[j])     // checking if the new node has been visited or has a better path
            {
                int dist = distance[minVertex] + gph[minVertex][j].getDist();   // calculating the new distance
                if(dist<distance[j])
                {
                    distance[j] = dist;    //updating distance if an improved path is provided
                }
            }
        }
    }

    for(int i=0; i<g_size; i++)
    {
        cout<< i << "    " << distance[i] << endl; // output the best distance for each node from 0 node.
    }
    for(int i=0; i<g_size; i++) // loop to calculate the average distance
    {
        if(distance[i]!=1000 && distance[i]>0)
        {
            avg_dist = avg_dist + distance[i];
            ++avg_nodes;
        }
    }
    cout<<"\n"<<"Average Distance = "<<static_cast<float>(avg_dist)/ static_cast<float>(avg_nodes)<<endl; // output average distance
}

void graph::print() // for printing the graph, Matrix mode
{
    cout<<"Graph size is = "<<g_size<<"\n"<<endl;
    for (int i=0; i<g_size; i++)
    {
        for (int j=0; j<g_size; j++)
        {
            cout << gph[i][j].getDist() << "  ";
        }
        cout << endl;
    }
}

bool graph::isConnected() // the isConnected Algorithm, not necessary
{
    int old_size = 0, c_size = 0;
    bool* close = new bool[g_size];
    bool* open = new bool[g_size];
    for(int i = 0; i<g_size; i++)
    {
        open[i] = close[i] = false;
    }
    open[0] = true; // starting at 0 and 0 always connected to 0.

    while(c_size<g_size)
    {
        for(int i=0; i<g_size; i++)
        {
            old_size = c_size;

            if(open[i]&&(close[i] == false))
            {
                close[i] = true;
                c_size++;

                for(int j=0; j<g_size; j++)
                {
                    open[j] = open[j] || gph[i][j].getFlag();
                }
            }
        }
        if(c_size == g_size)
            return true;
        if(old_size == c_size)
            return false;
    }
}

graph::~graph() // destructor for graph class
{
    for(int i=0; i<g_size; i++)
    {
        delete[] gph[i];  // delete method to deallocate heap memory
    }
    cout<<"\n"<<"Graph Destructor called"<<"\n"<<endl;
}


int main()
{
    srand(time(0));    // seed the random number generator
    graph g1;         // create graph object g1
    g1.randInit();   // initializes the graph with random values but still meeting all the requirements of the problem statement.
    g1.setDist();    // sets the distances between nodes randomly between 1 to 10
    g1.print();      // prints the graph in Matrix mode; comment this line to AVOID Printing.
    if(g1.isConnected())
    {
        cout<<endl<<"Connected Graph"<<endl<<endl;
    }
    else{
        cout<<"\n"<<"Graph NOT Connected"<<endl;
    }

    g1.dijkstraAglo(); // call the Dijkstra's shortest path algorithm implementation method of g1 object of graph class

    return 0;
}
