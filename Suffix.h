//ukonens algorithm 
#include <iostream>
using namespace std;
class Suffix{
public:
    Suffix();
    ~Suffix();
    
    class End{
    public:
        End(int end);
        ~End();
        int end;
    };
    End* globalEnd;
    class Node{
    public:
        Node();
        ~Node();
        Node* suffixLink;
        Node* child[6];
        int index;
        End* end;
        int start;


    };

    
    class ActivePoint{
    public:
        ActivePoint(Node* node);
        ~ActivePoint();
        Node* activeNode;
        int activeEdge;
        int activelength;
    };
    Node* root;
    string text;
    ActivePoint* activepoint; 
    int remaining;
    void insert(int index);
    void extend(int index);
    

};