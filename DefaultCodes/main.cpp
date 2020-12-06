#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <math.h>

using namespace std;

struct Nd
{
    long long int No = 0;
    long long int Level = 0;

    bool markedB = false;
    bool markedG = false;

    struct Nd* Parent = nullptr;
    vector<struct Nd*> Connections;

}typedef Node;

struct Ed
{
    long long int No = 0;
    long long int Nf = 0;
    long long int w = 0;

}typedef Edge;

class Tree
{
    public:
        Node *Root;

        vector<Node *> Nodes;
        vector<Edge > Edges;
        vector<Edge > minimalEdges;

        Tree(unsigned long long int N, Edge & conn);
        Node * isNodeAlreadyPresent(long long int Sig);
        long long int isNodeAlreadyBag(long long int Sig);
        bool addEdge(Edge &);
        void printTree();

        ~Tree();
};

Tree::Tree(unsigned long long int N, Edge & conn)
{
    vector<Node *> Temp(N,nullptr);
    this->Nodes = Temp;

    Node * Temp1 = new Node;
    Node * Temp2 = new Node;

    Temp1->No = conn.No;
    Temp2->No = conn.Nf;
    Temp1->Level = 0;
    Temp2->Level = 0;
    Temp1->markedB = true;
    Temp2->markedB = true;

    Temp1->Parent = nullptr;
    Temp2->Parent = Temp1;

    Temp2->Connections.clear();
    Temp1->Connections.clear();

    Temp2->Connections.push_back(Temp1);
    Temp1->Connections.push_back(Temp2);

    this->Root = Temp1;
    this->Edges.push_back(conn);
}

Node * Tree::isNodeAlreadyPresent(long long int Sig)
{
    Node * retValue = nullptr;
    bool flag = false;
    Node * Pointer = this->Root;
    list<Node *> bag;

    if(Pointer->No == Sig)
    {
        retValue = Pointer;
    }else{
        Pointer->markedG = true;
        list<Node *> bag;

        bag.push_back(Pointer);

        while(bag.size()>0)
        {
            Node *nS = bag.front();
            bag.pop_front();

            nS->markedG = true;

            for(auto elem: nS->Connections)
            {
                if(elem->No == Sig)
                {
                    retValue = elem;
                    flag = true;
                    break;
                }else{
                    if(!elem->markedG) bag.push_front(elem);
                }
            }
            if(flag) break;
        }
    }

    bag.push_back(Pointer);
    while(bag.size()>0)
    {
        Node *nS = bag.front();
        nS->markedG = false;

        for(auto elem: nS->Connections)
        {
            if(elem->markedG) bag.push_back(elem);
        }
        bag.pop_front();
    }

    if(!(retValue==nullptr))retValue->markedG = false;

    return retValue;
}

long long int Tree::isNodeAlreadyBag(long long int Sig)
{
    //-1 when it is not present,
    //nonnegative value giving the position of the edge

    int counter = 0;
    bool flag = false;

    for(auto Elem: this->minimalEdges)
    {
        if((Elem.No == Sig)||(Elem.Nf == Sig))
        {
            flag = true;
            break;
        }
        counter++;
    }

    if(!flag) counter = -1;

    return counter;
}

bool Tree::addEdge(Edge & conn)
{
	bool retVal = false;
    Node * Temp1 = new Node;
    Node * TempA = new Node;
    Edge TempEdge;

    Node * res1 = isNodeAlreadyPresent(conn.No);
    Node * res2 = isNodeAlreadyPresent(conn.Nf);

    //CHECK ON THE BAG OF MINIMUM EDGES
    long long int IndexA = isNodeAlreadyBag(conn.No);
    long long int IndexB = isNodeAlreadyBag(conn.Nf);
    long long int Index = max(IndexA,IndexB);
    long long int NodeToSeek = -1;

    if(Index >= 0)
    {
        TempEdge = this->minimalEdges[Index];
    }

    if((res1!=nullptr)&&(res2!=nullptr))
    {
        //cout << "Edge "<< conn.No <<"<->"<< conn.Nf <<" no se puede incluir"<<endl;
    }else
    {
        this->Edges.push_back(conn);

        if((res1==nullptr)&&(res2==nullptr))
        {
            this->minimalEdges.push_back(conn);
        }else{
        	retVal = true;
            if(res1!=nullptr)
            {
                if(Index>=0)//Create a node with conn.Nf and attach it to res1
                {
                    if(TempEdge.No == conn.Nf)
                    {
                        TempA->No = TempEdge.Nf;
                        NodeToSeek = TempEdge.Nf;//TempEdge.No;
                    }else
                    {
                        TempA->No = TempEdge.No;
                        NodeToSeek = TempEdge.No;//TempEdge.Nf;
                    }

                    if(isNodeAlreadyPresent(TempA->No)==nullptr)
                    {
                        TempA->Level =  res1->Level+2;
                        TempA->Parent = Temp1;
                        TempA->Connections.push_back(Temp1);
                        Temp1->Connections.push_back(TempA);
                    }
                    this->minimalEdges[Index].No = -1;
                    this->minimalEdges[Index].Nf = -1;
                    this->minimalEdges[Index].w = -1;

                }
                Temp1->No = conn.Nf;
                Temp1->Level = res1->Level+1;
                Temp1->Parent = res1;
                Temp1->Connections.push_back(res1);
                res1->Connections.push_back(Temp1);
            }

            if(res2!=nullptr)
            {
                if(Index>=0)//Create a node with conn.Nf and attach it to res1
                {
                    if(TempEdge.No == conn.No)
                    {
                        TempA->No = TempEdge.Nf;
                        NodeToSeek = TempEdge.Nf;//TempEdge.No;
                    }else
                    {
                        TempA->No = TempEdge.No;
                        NodeToSeek = TempEdge.No;//TempEdge.Nf;
                    }

                    if(isNodeAlreadyPresent(TempA->No)==nullptr)
                    {
                        TempA->Level =  res2->Level+2;
                        TempA->Parent = Temp1;
                        TempA->Connections.push_back(Temp1);
                        Temp1->Connections.push_back(TempA);
                    }

                    this->minimalEdges[Index].No = -1;
                    this->minimalEdges[Index].Nf = -1;
                    this->minimalEdges[Index].w = -1;

                }

                Temp1->No = conn.No;
                Temp1->Level = res2->Level+1;
                Temp1->Parent = res2;
                Temp1->Connections.push_back(res2);
                res2->Connections.push_back(Temp1);
            }

            if(Index>=0)
            {
                Edge TempEdge;
                long long int Var = isNodeAlreadyBag(NodeToSeek);

                if(Var>=0)
                {
                    TempEdge = this->minimalEdges[Var];

                    this->minimalEdges[Var].No = -1;
                    this->minimalEdges[Var].Nf = -1;
                    this->minimalEdges[Var].w = -1;

                    addEdge(TempEdge);
                }


            }

        }
    }
    return retVal;
}

void Tree::printTree()
{
cout <<endl<<"printing tree "<<endl;

    Node *Pointer = this->Root;
    list<Node *> bag;
    long long int level = 0;

    bag.push_back(Pointer);

    while(bag.size()>0)
    {
        Node *nS = bag.front();

        if(level < nS->Level) cout<<endl;
        level = nS->Level;
//        cout<< nS->No<<"("<< nS->Level <<") ";

        cout<< nS->No;
        if(nS->Parent!=nullptr)
        {
            cout<<"("<< nS->Parent->No <<") ";
        }else{
            cout<<"(-1) ";
        }


        nS->markedG = true;

        for(auto &elem: nS->Connections)
        {
            if(!elem->markedG)
            {
                elem->Level = nS->Level + 1;
                bag.push_back(elem);
            }
        }
        bag.pop_front();
    }

    //cleaning flags
    bag.push_back(Pointer);
    while(bag.size()>0)
    {
        Node *nS = bag.front();
        bag.pop_front();

        nS->markedG = false;

        for(auto elem: nS->Connections)
        {
            if(elem->markedG) bag.push_front(elem);
        }
    }
    cout << endl;
}

Tree::~Tree()
{
}

int main()
{
    unsigned long long int T = 0;
    unsigned long long int N = 0;
    unsigned long long int M = 0;

    unsigned long long int ai = 0;
    unsigned long long int bi = 0;
    unsigned long long int ci = 0;

    unsigned long long int count = 0;

    multimap<unsigned long long int,Edge > Country;
    Tree *Arbol;
    Edge Temp;

    cout<< "BEGINNING"<<endl;
    cin >> T;

    for(unsigned long long int tt = 0; tt < T; tt++)
    {

        cin >> N >> M;

        for(unsigned long long int mm = 0; mm < M; mm++)
        {
            cin >> ai >> bi >> ci;

            Temp.No = ai;
            Temp.Nf = bi;
            Temp.w = ci;
            Country.insert(pair<unsigned long long int,Edge>(ci,Temp));
        }
    }

    cout<<"Processing the tree"<<endl;
    for(auto Elem:Country)
    {
        Edge TT = Elem.second;
        cout<<endl<<"Node0 = "<< TT.No<<", Node1 = "<< TT.Nf<<", w = "<< TT.w<<endl;

        if(count == 0)
        {
            Arbol = new Tree(N,TT);
        }else{
            Arbol->addEdge(TT);
        }
        Arbol->printTree();

        count++;
    }

    cout<<"Arbol Finale"<<endl;
    Arbol->printTree();
}
