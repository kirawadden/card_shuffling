#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

static unsigned long num_cards;
static unsigned long num_piles;

typedef struct node node;

struct node
{
    int val;
    node* next;
};

node* make_deck(void)
{
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
    return head;
}

int shuffle_cards(node* head)
{
    if (!head) return -1;
    unsigned long rounds = 0;
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

        // sanity check - iterate through all cards to check if they're in order
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

    node* deck = make_deck();

    unsigned long nrounds = shuffle_cards(deck);
    printf("Number of rounds = %lu\n", nrounds);

    // free memory
    node* temp = deck;
    while (temp != NULL)
    {
        node* t2 = temp;
        temp = temp->next;
        free(t2);
    }

    return 0;
}
