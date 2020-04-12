/*********  Mancala AI  ********** California Sate University San Marcos  **********
 *
 * AUTHORS:
 * Alexander Joslin,
 * Colby Schexnayder,
 * Sean Seaver
 *
 * DESCRIPTION:
 * This is a mancala game where the players compete against the computer AI.
 * The AI uses the Minimax search algorithm with alpha-beta pruning to
 * predict the best possible move 6 moves ahead.  It uses 3 heuristics to
 * to determine a move's value.
 *
 * DATE:
 * March 1, 2019
 */

#include "Tree.h"
#include "Tree.cpp"
#include <string>
#include <iostream>

using namespace std;
int main() {
    MancalaTree tree;
    Node *p = tree.root;  //p is the object containing the board, heuristic, player's turn
    cout << "Who are you?" << endl
         << "1) Player 1" << endl
         << "2) Player 2" << endl;
    int hPlayer;          //player number
    int turnCounter = 1;  //will be used to display the turn number
    int inp;              //stores the player and AI's pit choice
    cin >> hPlayer;
    int available = 0;    //Will store the amount of available pits

    while (true) {                                        //player1
        cout << "\nTurn " << turnCounter << endl;
        tree.printNode(p);

        if (tree.goalState(p)) {                         //Check to see if an end goal has been met
            printWinner(p);                             //print who is the winner
            break;
        }

        if (hPlayer == 1) {
            tree.generateChildren(p);                   //generateChildren

            if (!p->children.empty()) {
                available = openSpaces(p, "p1");         //checks to see how many pits are available
                do {
                    cin >> inp;
                    if (inp > available || inp <= 0)     //error checking
                        cout << "Error! Invalid Input. \nTry a number from 1 - " << available << endl;
                } while (inp > available || inp <= 0);    //loop if input is invalid
                p = p->children[inp - 1];
            }
            turnCounter++;                              //Next turn
            cout << "\nTurn " << turnCounter << endl;
            tree.printNode(p);

            if (tree.goalState(p)) {                     //check if an end goal has been met
                printWinner(p);                         //print who is the winner
                break;
            }

            inp = tree.chooseMoveAI(p, true);
            if (!p->children.empty())
                p = p->children[inp];
            turnCounter++;
            } else {                                           //player 2

            inp = tree.chooseMoveAI(p, false);
            if (!p->children.empty())
                p = p->children[inp];
            turnCounter++;
            cout << "\nTurn " << turnCounter << endl;
            tree.printNode(p);

            if (tree.goalState(p)) {
                printWinner(p);
                break;
            }

            tree.generateChildren(p);

            if (!p->children.empty()) {
                available = openSpaces(p, "p2");
                do {
                    cin >> inp;
                    if (inp > available || inp <= 0)       //error checking
                        cout << "Error! Invalid Input. \nTry a number from 1 - " << available << endl;
                } while (inp > available || inp <= 0);
                p = p->children[inp - 1];
            }
            turnCounter++;

        }
    }

}   