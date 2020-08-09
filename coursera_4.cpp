#include <iostream>
#include <cmath>
#include <time.h>
#include <bits/stdc++.h>

const float node_prob = .60; // graph density has been defined
const int g_size = 20;       // size of graph has been defined
const int q_size = (g_size*(g_size-1))/2;
const int node_edge = static_cast<int>(static_cast<float>(g_size-1)*node_prob); // variable used for construction of random graph
int curr = 0, t_curr = 0;        // variable used for construction of random graph

using namespace std;

class prob   // class used for construction of random bidirectional graph with a particular density or node probability
{
public:
    prob(){} // constructor
    void setup();      // will setup the array required for assigning edges with the required probability or density
    bool fetchFlag(); //  will give information about previously picking the value, ensuring all the values will get picked only once
    ~prob(){} //destructor
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
    void fileInit(string filename);
    void print();
    void print_dist();
    void setDist();
    bool isConnected();
    void dijkstraAglo();
    void minSpanTree();
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

    for(int i=0; i<g_size; i++)
    {
        for(int j=0; j<g_size; j++)
        {
            gph[i][j].setFlag(false);
        }
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
                    gph[i][j].setDist(1000);
                    gph[j][i].setDist(1000);
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

void graph::fileInit(string filename)
{
    ifstream g_file(filename);
    istream_iterator<int> start(g_file), end;

    vector<int> nums(start,end);

    cout<<"Graph Size = "<<nums[0]<<endl;

    for(int i=1; i<nums.size(); i++)
    {
        gph[nums[i]][nums[i+1]].setDist(nums[i+2]);
        gph[nums[i]][nums[i+1]].setFlag(true);

        i += 2;
    }

    for(int i=0; i<g_size; i++)
    {
        gph[i][i].setDist(1000);
        gph[i][i].setFlag(false);

        for(int j=i+1; j<g_size; j++)
        {
            if(!gph[i][j].getFlag())
            {
                gph[i][j].setDist(1000);
            }
        }
    }
}

void graph::print_dist() // for printing the graph, Matrix mode
{
    cout<<endl;
    for (int i=0; i<g_size; i++)
    {
        for (int j=0; j<g_size; j++)
        {
            if(gph[i][j].getDist()==1000)
            {
                cout<<0<<"\t";
            }
            else
            {
                cout<<gph[i][j].getDist()<<"\t";
            }
        }
        cout << endl;
    }
}

void graph::print()
{
    cout<<"Graph size is = "<<g_size<<"\n"<<endl;
    for (int i=0; i<g_size; i++)
    {
        for (int j=0; j<g_size; j++)
        {
            cout<<gph[i][j].getFlag()<<"\t";
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

struct q_element
{
    q_element(int dist, int v1, int v2):dist(dist),v1(v1),v2(v2){}
    int dist;
    int v1,v2;
};

struct Compare_qele
{
    bool operator()(q_element q1, q_element q2)
    {
        return q1.dist>q2.dist;
    }
};

bool connected(vector<q_element> mst, int s, int e, int prev)
{
    bool flag = false;
    for(int i=0; i<mst.size(); i++)
    {
        if((mst[i].v1==s||mst[i].v1==e) && (mst[i].v2==s||mst[i].v2==e))
        {
            return true;  //  check if the edge already exists
        }
    }
    for(int i=0; i<mst.size(); i++) // if it doesn't already exist
    {
        if(mst[i].v1==s && mst[i].v2!=prev)
        {
            flag = flag||connected(mst,mst[i].v2, e, mst[i].v1); // will trace back to a node and recursively apply until
        }                                                        // the end node is reached or it decides the loop is not formed
        else if(mst[i].v2==s && mst[i].v1!=prev)
        {
            flag = flag||connected(mst,mst[i].v1, e, mst[i].v2);
        }
        if(flag==true)
        {
            return flag;
        }
    }
    return flag;
}

void graph::minSpanTree()   // kruskal's algorithm used
{
    priority_queue<q_element, vector<q_element>, Compare_qele> pq;  //priority queue for kruskal's

    vector<q_element> mst; // the mst stored in vector

    int tab[g_size]; // array to keep track of number of nodes included in mst
    int weight = 0; // weight of mst

    for(int i=0;i<g_size;i++){tab[i]=false;} // initialize the array

    int counter = 0;  // counter
    for(int i=0;i<g_size;i++)
    {
        for(int j=i;j<g_size;j++)
        {
            if(gph[i][j].getFlag()) // checking the number of edges
            {
                counter++;
            }
        }
    }

    for(int i=0;i<g_size;i++)
    {
        for(int j=i;j<g_size;j++)
        {
            if(gph[i][j].getFlag())
            {
                pq.push(q_element(gph[i][j].getDist(), i, j));  // adding all the edges to priority queue
            }
        }
    }
    int x=0;
    counter = 0;
    while(counter<g_size-1)  // creating the mst
    {
        q_element temp = pq.top();
        if((tab[temp.v1]==0)||(tab[temp.v2]==0)) // if both the nodes are already a part of the tree
        {
            mst.push_back(temp);
            tab[temp.v1] = true;
            tab[temp.v2] = true;
            counter++;
        }
        else if(!connected(mst, temp.v1, temp.v2, g_size+1)) // connected() is gonna tell if the nodes will form a loop if connected
        {
            mst.push_back(temp);
            tab[temp.v1] = true;
            tab[temp.v2] = true;
            counter++;
        }
        x++;
        pq.pop();
    }

    cout<<endl<<"Minimum Spanning Tree is : "<<endl<<endl;

    for(int i=0; i<g_size-1; i++)
    {
        weight += mst[i].dist;
        cout<<mst[i].v1<<"\t"<<mst[i].v2<<"\t"<<mst[i].dist<<endl;
    }

    cout<<endl<<"Weight of MST = "<<weight<<endl;
}

int main()
{
    srand(time(0));    // seed the random number generator

    graph g1;         // create graph object g1

    //g1.randInit();   // initializes the graph with random values but still meeting all the requirements of the problem statement.
    //g1.setDist();    // sets the distances between nodes randomly between 1 to 10

    g1.fileInit("graph_file.txt");  //Please change this name accordingly. Initializes with a file

    //g1.print();      // prints the graph in Matrix mode; comment this line to AVOID Printing.
    //g1.print_dist();

    if(g1.isConnected())
    {
        cout<<endl<<"Connected Graph"<<endl<<endl;

        g1.minSpanTree();
    }
    else
    {
        cout<<"\n"<<"Graph NOT Connected"<<endl;
    }
    return 0;
}
