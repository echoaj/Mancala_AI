#pragma warning(disable:4996)
#include "Tree.h"
//#include <algorithm>

MancalaTree::MancalaTree()
{
    root = new Node();
}

MancalaTree::~MancalaTree()
{
    destroyTree(root);
}

void MancalaTree::generateChildren(Node* node)
{
    if (node->childrenGenerated)
        return;

    if (node->playerTurn == 1)
    {
        for (int i = 1; i <= 6; i++){	//6 is the maximum number of legal moves
            if (node->state[i] != 0) { //ignore empty pits

                int stones = node->state[i]; //# of stones in the selected pit

                Node* newNode = new Node();
                newNode->playerTurn = 2; //next players turn
                copy(begin(node->state), end(node->state), begin(newNode->state));//start with last nodes board state
                newNode->state[i] = 0; //empty selected pit

                int placing = i + 1; //begin placement at next pit
                for (int j = 0; j < stones; j++) //placing each stone
                {
                    if (placing >= boardSize) //if we get to the end of the board loop to the beginning
                        placing = 1;
                    if (placing == 0)
                        placing++;

                    newNode->state[placing]++; //increase stones in pit by 1

                    //capture logic
                    if (j == stones - 1 && placing > 0 && placing < 7 && newNode->state[placing] == 1) {
                        newNode->state[7] = newNode->state[7] + newNode->state[placing + (7 - placing)*2]; //Adds stones to players mancala
                        newNode->state[placing + (7 - placing)*2] = 0; //removes stones from oppenent's pit
                    }
                    placing++; //move to next pit
                }

                calcHeuristic(newNode);
                node->children.push_back(newNode);//add child to node
            }
        }
    }
    else if (node->playerTurn == 2) {
        for (int i = 8; i < 14; i++) {
            if (node->state[i] != 0) {

                int stones = node->state[i];

                Node* newNode = new Node();
                newNode->playerTurn = 1;
                copy(begin(node->state), end(node->state), begin(newNode->state));
                newNode->state[i] = 0;

                int placing = i + 1;
                for (int j = 0; j < stones; j++)
                {
                    if (placing == 7)
                        placing++;
                    if (placing >= boardSize)
                        placing = 0;

                    newNode->state[placing]++;

                    if (j == stones - 1 && placing > 7 && newNode->state[placing] == 1) {
                        newNode->state[0] = newNode->state[0] + newNode->state[placing - (placing - 7)*2];
                        newNode->state[placing - (placing - 7)*2] = 0;
                    }

                    placing++;
                }
                calcHeuristic(newNode);
                node->children.push_back(newNode);
            }
        }
    }
}

//prints the mancala board. Goes in counter-clockwise direction
void MancalaTree::printNode(Node * node)
{
    printf("\033[1m");
    printf("   %d  %d  %d  %d  %d  %d\n", node->state[13],node->state[12],node->state[11],node->state[10],node->state[9],node->state[8]);
    printf("%d                     %d\n", node->state[0],node->state[7]);
    printf("   %d  %d  %d  %d  %d  %d\n", node->state[1],node->state[2],node->state[3],node->state[4],node->state[5],node->state[6]);
    printf("\033[0m");
}


void MancalaTree::destroyTree(Node* node)
{
    for (Node* q : node->children)
    {
        destroyTree(q);
    }
    delete node;
}

//Was previously giving errors when game would end.
//Two goal states. When either player 1 or player 2 runs out of stones
bool MancalaTree::goalState(Node *node){
    bool goal = false;

    //When all the pits on player1 side are empty, player1 has won
    if(node->state[1]+node->state[2]+node->state[3]+node->state[4]+node->state[5]+node->state[6] == 0){
        goal = true;
    }
    //When all the pits on player2 side are empty, player2 has won
    if(node->state[8]+node->state[9]+node->state[10]+node->state[11]+node->state[12]+node->state[13] == 0){
        goal = true;
    }

    return goal;
}

//returns a heuristic value we need this or another function
//to return the index tied to the heuristic at the top level
int MancalaTree::minimax(Node *node, int depth, bool max, int alpha, int beta){

    if (depth == 6 || goalState(node)) //depth we want to test to
        return node->heuristic;

    if (!node->childrenGenerated){
        generateChildren(node);
        node->childrenGenerated = true;
    }

    if(max){
        int best = -1000;
        for(Node* q : node->children){
            int val = minimax(q, depth+1, false, alpha, beta);
            best = std::max(best, val);
            alpha = std::max(alpha, best);
            if (beta <= alpha)
                break;
        }
        return best;
    }
    else{
        int best = 1000;
        for (Node* q : node->children){
            int val = minimax(q, depth+1, true, alpha, beta);
            best = std::min(best, val);
            beta = std::min(beta, best);
            if (beta <= alpha)
                break;
        }
        return best;
    }

}

//Calls minimax and returns a childs index
int MancalaTree::chooseMoveAI(Node* node, bool max){

    int index = 0;

    if (!node->childrenGenerated){
        generateChildren(node);
        node->childrenGenerated = true;
    }

    if(max){
        int best = -1000;
        int x;
        for (int i = 0; i < node->children.size(); i++){
            x = minimax(node->children[i], 0, false, -1000, 1000);
            if (best < x){
                best = x;
                index = i;
            }
        }
    }
    else{
        int best = 1000;
        int x;
        for (int i = 0; i < node->children.size(); i++){
            x = minimax(node->children[i], 0, true, -1000, 1000);
            if (best > x){
                best = x;
                index = i;
            }
        }
    }

    return index;
}

void MancalaTree::calcHeuristic(Node* node){
//int heu = node->state[8] - node->state[0];
    /* Part 1 of Heuristic:
     * Takes the sum of the opponents board and subtracts the sum of the players board  => heuristic*/
    int opponent = node->state[8]+node->state[9]+node->state[10]+node->state[11]+node->state[12]+node->state[13];
    int player = node->state[1]+node->state[2]+node->state[3]+node->state[4]+node->state[5]+node->state[6];
    int difference = opponent - player; //result

    /* Part 2 of Heuristic:
     * ANALYSIS IF THERE A SITUATION WHERE OPPONENT STONES CAN BE CAPTURED.
     * 1)Checks to see if there are any empty pits on the player's side
     * 2)If so, checks to see if there a non-empty pit on the player side
     * in which the last piece would land on the empty pit
     * 3)If so, checks to see if there are more than 0 pieces directly across
     * from that empty slot on the opponent's side.
     * 4)If so, that means these pieces can be captured
     * therefore we add the number of pieces that can be captured to the heuristic value */
    int capture = 0;
    if(node->state[2] == 0 && node->state[1] == 1 && node->state[12] > 0){
        capture = node->state[12];
    }else if(node->state[3] == 0 && (node->state[2] == 1 || node->state[1] == 2) && node->state[11] > 0){
        capture = node->state[11];
    }else if(node->state[4] == 0 && (node->state[3] == 1 || node->state[2] == 2 || node->state[1] == 3) && node->state[10] > 0) {
        capture = node->state[10];
    }else if(node->state[5] == 0 && (node->state[4] == 1 || node->state[3] == 2 || node->state[2] == 3 || node->state[1] == 4) && node->state[9] > 0){
        capture = node->state[9];
    }else if(node->state[6] == 0 && (node->state[5] == 1 || node->state[4] == 2 || node->state[3] == 3 || node->state[2] == 4 || node->state[1] == 5) && node->state[8] > 0) {
        capture = node->state[8];
    }

    /* Part 3 of Heuristic:
     * pits with a amount of stones are considered bad
     * because sooner or later it is likely they opponent will capture it
     * Also, the opponent may win prematurely
     * This code checks to see if there are pits with too many peices and assignes a "danger value.
     * It subtracts this danger value from the heuristic
     */
    int danger = 0;
    if(node->state[1] > 8 || node->state[2] > 8 ||node->state[3] > 8 ||node->state[4] > 8 ||node->state[5] > 8 ||node->state[6] > 8){
        danger = 3;
    }else if(node->state[1] > 7 || node->state[2] > 7 ||node->state[3] > 7 ||node->state[4] > 7 ||node->state[5] > 7 ||node->state[6] > 7){
        danger = 2;
    }else if(node->state[1] > 6 || node->state[2] > 6 ||node->state[3] > 6 ||node->state[4] > 6 ||node->state[5] > 6 ||node->state[6] > 6){
        danger = 1;
    }

    int heuristic = difference + capture - danger;
    heuristic = heuristic * -1;

    node->heuristic = heuristic;
}


//New Function
/*Error checking for when the player enters a number
 * that is above the amount of available pits on their side*/
int openSpaces(Node *node, string player){
    int num = 0;
    int start = 1;
    int end = 7;

    if(player == "p2") {
        start = 8;
        end = 14;
    }
    for (int i = start; i < end; ++i) {
        if(node->state[i] != 0){
            num = num + 1;
        }
    }
    return num;
}

//New Function
/* Determins the winner by taking the total of the player mancalas
 * plus the peices on the opponents mancalas, compares them and prints out who won. */
void printWinner(Node *node){
    int player1Total = 0;
    int player2Total = 0;
    //Player1 total = # peieces in pit + # of peices remaining on Player2 side.
    for(int i = 7; i < 14; i++)
        player1Total = player1Total + node->state[i];

    //Player2 total = # peieces in pit + # of peices remaining on Player1 side.
    for(int i = 0; i < 7; i++)
        player2Total = player2Total + node->state[i];

    //player1Total = node->state[7];    //without adding up opponent's side
    //player2Total = node->state[0];

    cout << endl;
    cout << "Final Score Player 1: " << player1Total << endl;
    cout << "Final Score Player 2: " << player2Total << endl;

    printf("\033[1m");
    if (player1Total > player2Total){
        cout << "\tCongratulations!\n\t Player 1 Wins" << endl;
    }else if(player1Total < player2Total){
        cout << "\tCongratulations!\n\t Player 2 Wins" << endl;
    }else
        cout << "The game is a tie!" << endl;
    printf("\033[0m");
}