#include <iostream>
using namespace std;

class Suffix{
    public:
    Suffix(){
        remaining = 0;
        root = new Node(0, nullptr);
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

    void makeSuffixTree(string str){
        text = str;
        for(int i=0; i<text.length(); i++){
            startPhase(i);
        }
    };

    void startPhase(int i){
        Node* lastCreatedInternalNode = nullptr;
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
                    Node* leaf = new Node(i, globalEnd);
                    activePoint->activeNode->child[getIndex(text[i])] = leaf;
                    remaining--;

                }
                
            }
            else{
                Node* node = getChildToSplit();
                int ogStart = node->start;
                node->start = ogStart + activePoint->activeLength;
                Node* internalNode = new Node(ogStart, new End(ogStart + activePoint->activeLength -1));
                Node* leaf = new Node(i, globalEnd);
                internalNode->child[getIndex(text[i])] = leaf;
                internalNode->child[getIndex(text[ogStart + activePoint->activeLength])] = node;
                activePoint->activeNode->child[getIndex(text[ogStart])] = internalNode;
                //handle index??
                if(lastCreatedInternalNode != nullptr){
                    lastCreatedInternalNode->suffixLink = internalNode;
                }
                lastCreatedInternalNode = internalNode;
                internalNode->suffixLink = root;
                if(activePoint->activeNode != root){
                    activePoint->activeNode = activePoint->activeNode->suffixLink;
                }
                else{
                    activePoint->activeEdge ++;
                    activePoint->activeLength --;
                }
                remaining--;
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
        return activePoint->activeNode->child[getIndex(text[activePoint->activeEdge])];
    };
    

    void extend(int index){

    };
    

};