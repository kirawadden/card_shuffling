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
#include <stdbool.h>
#include <signal.h>

static unsigned long num_cards;
static unsigned long num_piles;

typedef struct node node;

unsigned long rounds = 0;

void handler(int i)
{
    printf("Rounds completed %lu\n", rounds);
    exit(1);
}

struct node
{
    int val;
    node* next;
};

int shuffle_cards(node* head)
{
    if (!head) return -1;
    // static unsigned long rounds = 0;
    bool in_order = false;

    while (!in_order)
    {
        node* temp = NULL;
        // create a "ring buffer" to keep track of the references to the head + tails of piles of cards
        // first half of the buffer is the heads, second half is the tails
        node* head_tail_buf[num_piles*2];
        memset(head_tail_buf, 0, sizeof(head_tail_buf));

        int pile = 0;

        // shuffle the cards into piles
        while (head != NULL)
        {
            if (head_tail_buf[pile] != NULL)
            {
                temp = head_tail_buf[pile];
            }
            else
            {
                // assign tail
                head_tail_buf[pile + num_piles] = head;
            }
            head_tail_buf[pile] = head;
            head = head->next;
            head_tail_buf[pile]->next = temp;
            pile = (pile + 1) % num_piles;
            temp = NULL;
        }

        // assign head to the top pile
        head = head_tail_buf[0];

        // stack the piles
        for (int i = 0; i < num_piles - 1; i++)
        {
            head_tail_buf[i + num_piles]->next = head_tail_buf[i + 1];
        }

        rounds++;
        temp = head;

        // sanity check - cards should only be in order once card 1 is back on top of the pile
        // iterate through all cards to check
        for (int i = 0; i < num_cards; i++)
        {
            if (temp->val == (i+1)) in_order = true;
            else
            {
                in_order = false;
                break;
            }
            temp = temp->next;
        }
    }
    return rounds;

}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        printf("Usage: %s -N <number of cards> -Y <number of piles>\n", argv[0]);
        return -1;
    }

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
            printf("Unknown option - abort\n");
            return -1;
        }
    }

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
        return 0;
    }

    signal(SIGINT, handler);

    node* head = NULL;
    node* temp = NULL;

    // put the cards into a deck
    for (int i = 0; i < num_cards; i++)
    {
        node* card = malloc(sizeof(node));
        card->val = i+1;
        card->next = NULL;
        if (i == 0)
        {
            head = card;
            temp = card;
            continue;
        }
        temp->next = card;
        temp = card;
    }
    unsigned long nrounds = shuffle_cards(head);

    // free memory
    temp = head;
    while (temp != NULL)
    {
        node* t2 = temp;
        // printf("%d\n", temp->val);
        temp = temp->next;
        free(t2);
    }

    printf("Number of rounds = %lu\n", nrounds);
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