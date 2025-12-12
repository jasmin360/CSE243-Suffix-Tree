//ukonens algorithm 

class suffix{
public:
    suffix();
    ~suffix();
    class Node{
    public:
        Node();
        ~Node();
        Node* suffixLink;
        int index;

    }
    class End{
    public:
        End(int end);
        ~End();
        int end;
    }
    class Edge{
    public:
        Edge(int start, End* end);
        ~Edge();
        int start;
        End* end;
    }
    class ActivePoint{
    public:
        ActivePoint(Node* node);
        ~ActivePoint();
        Node* activeNode;
        int activeEdge;
        int activeLength;
    }
    Node* root;
    string text;
    ActivePoint* activePoint; 
    int remaining;
    void insert(int index);
    void extend(int index);
    

}