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
        Node[] child = new Node[6];
        ~Node();
        Node* suffixLink;
        int index;
        End* end;
        int start;


    }
    class End{
    public:
        End(int end){
            this->end = end;
        };
        ~End();
        int end;
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
    };



    void startPhase(int i){
        remaining++;
        end->end++;
        while(remaining > 0){
            if(activePoint->activeLength == 0){
                if(getNode(i)!=nullptr){
                    activePoint->activeEdge = i;
                    activePoint->activeLength++;
                    break;
                }
                else{
                    Node* leaf = new Node();
                    leaf->start = i;
                    leaf->end = end;
                    activePoint->activeNode->child[text[i]-'A'] = leaf;
                    remaining--;
                }
                activePoint->activeEdge = index;
            }
        }
    };
    Node* getNode (int index){
        case text[index]{
            'A': return child[0];
            'C': return child[1];
            'G': return child[2];
            'T': return child[3];
            '$': return child[4];
            '#': return child[5];
        }
    }
    

    void extend(int index){

    };
    

}