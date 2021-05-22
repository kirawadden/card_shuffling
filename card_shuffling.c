// =======================
// Question 1

// You are given a deck containing N cards.  While holding the deck facedown:

// 1. Deal all the cards facedown onto a table into Y piles like you would if you were playing with a group of people (i.e. card 1 to P1, card 2 to P2, ..., card Y to PY, card Y + 1 to P1, etc).
// 2. Combine all the piles into a deck by placing P1 onto P2, then P1+P2 onto P3, and so on. This is a round.
// 3. Pick up the deck from the table and repeat steps 1-2 until the deck is in the original order.
// 4. For each round, vary the pile count according to a repeating pattern. Start with 3 piles, then 4, then 5, then loop back to 3, then 4 and so on.

// Write a program to determine how many rounds it will take to put a deck back into the original order.  This will involve creating a data structure to represent the order of the cards. Do not use an array. This program should be written in C only. It should take a number of cards in the deck as a command line argument and write the result to stdout.  Please ensure the program compiles and runs correctly (no pseudo-code).  This isn't a trick question; it should be fairly straightforward.

// Bonus: Output how many rounds should be completed before the deck is adequately shuffled from the original deck for a person who is casually playing a game with cards. Provide your methodology in a comment block.

// =======================
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static unsigned long num_cards;
static unsigned long num_piles;

typedef struct node node;

struct node
{
    int val;
    node* next;
};

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        printf("Usage: %s -N <number of cards> -Y <number of piles>\n", argv[0]);
        return -1;
    }
    // num_cards = 0;
    // num_piles = 0;
    int c = 0;
    while ((c = getopt(argc, argv, "N:Y:")) != - 1)
    {
        switch(c) {
        case 'N':
            num_cards = strtoul(optarg, NULL, 10);
            break;
        case 'Y':
            num_piles = strtoul(optarg, NULL, 10);
            break;
        default:
            printf("Unknown option\n");
            return -1;
        }
    }
    printf("N: %lu, Y: %lu\n", num_cards, num_piles);
    if (num_cards <= 0 || num_piles <= 0)
    {
        printf("Number of cards and piles must be greater than zero\n");
        return -1;
    }
    else if (num_cards < num_piles)
    {
        printf("Number of cards must be greater than or equal to number of piles\n");
        return -1;
    }
    else if (num_cards == num_piles)
    {
        printf("Number of rounds to return to original order: %lu\n", num_cards);
    }

    node* head = NULL;
    node* tail = NULL;

    node* temp = NULL;


    for (int i = 0; i < num_cards; i++)
    {
        node* list_node = malloc(sizeof(node));
        list_node->val = i+1;
        printf("Creating node i: %d\n", list_node->val);
        list_node->next = NULL;
        if (i == 0)
        {
            head = list_node;
        }
        else
        {
            temp->next = list_node;
            tail = list_node;
        }
        temp = list_node;
    }

    // create a "ring buffer" to keep track of head + tails of LL stacks
    node* head_buf[num_piles];
    memset(head_buf, 0, sizeof(node)*num_piles);
    // printf("memset\n");

   
    int pile = 0;
    temp = NULL;
    // if (head_buf[0] == NULL) printf("It's null\n");
    while (head != NULL)
    {
        if (head_buf[pile] != NULL)
        {
            temp = head_buf[pile];
        }
        // else
        // {
        //     // assign tail
        //     printf("assign tail\n");
        //     head_buf[pile + num_piles] = head;
        // }
        head_buf[pile] = head;
        head = head->next;
        head_buf[pile]->next = temp;
        pile = (pile + 1) % num_piles;
    }
    printf("Stack piles complete\n");
    // printf("Tails for piles 1, 2, 3: %d, %d, %d\n", head_buf[3]->val, head_buf[4]->val, head_buf[5]->val);
    temp = head_buf[0];
    while (temp != NULL)
    {
        printf("Pile 1 value -> %d\n", temp->val);
        node* t2 = temp;
        temp = temp->next;
        free(t2);
    }

    temp = head_buf[1];
    while (temp != NULL)
    {
        printf("Pile 2 value -> %d\n", temp->val);
        node* t2 = temp;
        temp = temp->next;
        free(t2);
    }

    temp = head_buf[2];
    while (temp != NULL)
    {
        printf("Pile 3 value -> %d\n", temp->val);
        node* t2 = temp;
        temp = temp->next;
        free(t2);
    }

    return 0;
}

// node* h1 = NULL;
// node* h2 = NULL;
// node* h3 = NULL;

// while (head != NULL)
// {
//     if (pile == 0)
//     {
//         if (h1 != NULL)
//         {
//             temp = h1;
//         }
//         h1 = head;
//         head = head->next;
//         h1->next = temp;
//     }
//     else if (pile == 1)
//     {
//         if (h2 != NULL)
//         {
//             temp = h2;
//         }
//         h2 = head;
//         head = head->next;
//         h2-> next = temp;
//     }
//     else
//     {
//         if (h3 != NULL)
//         {
//             temp = h3;
//         }
//         h3 = head;
//         head = head->next;
//         h3->next = temp;
//     }
//     pile = (pile + 1) % num_piles;
// }