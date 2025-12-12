class Suffix{
    public:
    suffix(){
        remaining = 0;
        root = new Node();
        activePoint = new ActivePoint(root);
        end = new End(-1);

    };
    ~suffix();
    class Node{
    public:
        Node(){
            suffixLink = nullptr;
            index = -1;
        };
        ~Node();
        Node* suffixLink;
        int index;

    }
    class End{
    public:
        End(int end){
            this->end = end;
        };
        ~End();
        int end;
    }
    class Edge{
    public:
        Edge(int start, End* end){
            this->start = start;
            this->end = end;
        };
        ~Edge();
        int start;
        End* end;
    }
    class ActivePoint{
    public:
        ActivePoint(Node* node){
            activeNode = node;
            activeEdge = -1;
            activeLength = 0;
        };
        ~ActivePoint();
        Node* activeNode;
        int activeEdge;
        int activeLength;
    }
    Node* root;
    string text;
    ActivePoint* activePoint; 
    int remaining;
    void insert(int index){
        remaining++;
        end->end++;
        while(remaining > 0){
            if(activePoint->activeLength == 0){
                if()
                activePoint->activeEdge = index;
            }
        }

    };
    void extend(int index){

    };
    

}