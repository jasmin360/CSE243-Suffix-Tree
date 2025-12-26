#include <iostream>
using namespace std;

class Suffix{
    public:
    Suffix(){
        remaining = 0;
        root = new Node(0, nullptr);
        activepoint = new ActivePoint(root);
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
        Node(int i, End* e){
            suffixLink = nullptr;
            index = -1;
            start = i;
            end = e;
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
            activelength = 0;
        };
        ~ActivePoint();
        Node* activeNode;
        int activeEdge;
        int activelength;
    };
    Node* root;
    string text;
    ActivePoint* activepoint; 
    int remaining;
    void insert(int index){
    };

    void makeSuffixTree(string str){
        text = str;
        for(int i=0; i<text.length(); i++){
            startPhase(i);
        }
    };

    void startPhase(int i){
       Node * lastnode = NULL;
       globalEnd->end++;
       remaining++;
       while(remaining >0)
       if(activepoint->activelength ==0){
        if(getNode(i) != NULL){
            activepoint->activeEdge =i;
            activepoint->activelength++;
            break;
        }
        else {
            Node* node = new Node(i,globalEnd);
            activepoint->activeNode->child[getIndex(text[i])] = node;
            remaining --;
            if(lastnode != NULL){
                lastnode->suffixLink = activepoint->activeNode;
                lastnode = NULL;
            }
            if(activepoint->activeNode!=root){
                activepoint->activeNode = activepoint->activeNode->suffixLink;
            }
        }
       }
        else{ //activelength != 0; 
        int currentedge = getIndex(text[activepoint->activeEdge]);
        Node * oldnode =  activepoint->activeNode->child[currentedge];//AB
        /*walkdown function*/
        if(text[oldnode->start + activepoint->activelength]== text[i]){
            if(lastnode != NULL){
                lastnode->suffixLink = activepoint->activeNode;
                lastnode = NULL;
            }
            
            activepoint->activelength ++;
            break;
        }
        else{
            End*currentend = new End(activepoint->activelength + oldnode->start -1);
           Node* splitnot = new Node( oldnode->start,currentend);
           Node* newnode = new Node(i,globalEnd);
           activepoint->activeNode->child[currentedge] = splitnot;
           oldnode->start += activepoint->activelength;
           splitnot->child[getIndex(text[i])] = newnode;
           splitnot->child[getIndex(text[oldnode->start])]=oldnode;
           if(lastnode !=NULL){
            lastnode->suffixLink = splitnot;
           }
           remaining--;
           if(activepoint->activeNode == root){
              activepoint->activeEdge++;
              activepoint->activelength--;
           }
           else activepoint->activeNode = activepoint->activeNode->suffixLink;

        }
        
        }
    }
    Node* getNode (int index){
        switch (text[index]) {
            case 'A': return activepoint->activeNode->child[0];
            case 'C': return activepoint->activeNode->child[1];
            case 'G': return activepoint->activeNode->child[2];
            case 'T': return activepoint->activeNode->child[3];
            case '$': return activepoint->activeNode->child[4];
            case '#': return activepoint->activeNode->child[5];
        }
    };

    int getIndex(char c){
        switch (c) {
            case 'A': return 0;
            case 'C': return 1;
            case 'G': return 2;
            case 'T': return 3;
            case '$': return 4;
            case '#': return 5;
        }
    };

    Node* getChildToSplit(){
        return activepoint->activeNode->child[getIndex(text[activepoint->activeEdge])];
    };
    

    void extend(int index){

    };
    

};