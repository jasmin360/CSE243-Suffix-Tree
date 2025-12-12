#include <iostream>
using namespace std;

class Suffix{
    public:
    Suffix(){
        remaining = 0;
        root = new Node();
        activePoint = new ActivePoint(root);
        globalEnd = new End(-1);

    };
    ~Suffix();
    class End{
    public:
        End(int end){
            this->end = end;
        };
        ~End();
        int end;
    };
    End* globalEnd;
    class Node{
    public:
        Node(){
            suffixLink = nullptr;
            index = -1;
        }
        Node* child[6];
        ~Node();
        Node* suffixLink;
        int index;
        End* end;
        int start;
    };


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
    };
    Node* root;
    string text;
    ActivePoint* activePoint; 
    int remaining;
    void insert(int index){
    };



    void startPhase(int i){
        remaining++;
        (globalEnd->end)++;
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
                    leaf->end = globalEnd;
                    activePoint->activeNode->child[text[i]-'A'] = leaf;
                    remaining--;
                }
                activePoint->activeEdge = i;
            }
        }
    };
    Node* getNode (int index){
        switch (text[index]) {
            case 'A': return activePoint->activeNode->child[0];
            case 'C': return activePoint->activeNode->child[1];
            case 'G': return activePoint->activeNode->child[2];
            case 'T': return activePoint->activeNode->child[3];
            case '$': return activePoint->activeNode->child[4];
            case '#': return activePoint->activeNode->child[5];
        }
    };
    

    void extend(int index){

    };
    

};