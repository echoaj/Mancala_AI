#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;


struct Node
{
    int state [14] = { 0, 4, 4, 4, 4, 4, 4, 0,
                       4, 4, 4, 4, 4, 4 }; //initial state, state[7] is player1's mancala; state[0] is player2's

    int heuristic = 0;                //heuristic value
    int playerTurn = 1;               //Alternates between 1 and 2 depnding on player
    bool childrenGenerated = false;
    vector<Node*> children;
};

class MancalaTree
{
public:
    MancalaTree();
    ~MancalaTree();
    bool goalState(Node* node);
    int chooseMoveAI(Node* node, bool max);
    int minimax(Node* node, int depth, bool max, int alpha, int beta);
    void generateChildren(Node* node);
    void calcHeuristic(Node* node);
    void printNode(Node* node);
    Node * root;
private:

    const int boardSize = 14;
    void destroyTree(Node* node);
};

int openSpaces(Node *node, string player);  //Returns available pits
void printWinner(Node *node); //Prints who wins