#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

typedef struct card card;

card* make_card(int val);
card* make_deck(int32_t num_cards);
int shuffle_cards(card* deck, int32_t num_cards, int32_t num_piles);

struct card
{
    int val;
    card* next;
};

// Helper function to allocate card memory. Memory must be freed by caller
card* make_card(int val)
{
    card* c;
    c = malloc(sizeof(card));
    c->val = val;
    c->next = NULL;
    return c;
}

card* make_deck(int32_t num_cards)
{
    card* deck = NULL;
    card* temp = NULL;

    card* c = make_card(1);
    deck = c;
    temp = c;
    // put the cards into a deck
    for (int i = 1; i < num_cards; i++)
    {
        c = make_card(i+1);
        temp->next = c;
        temp = c;
    }
    return deck;
}

int shuffle_cards(card* deck, int32_t num_cards, int32_t num_piles)
{
    unsigned long rounds = 0;
    bool in_order = false;
    card* temp = NULL;

    if (!deck) return -1;

    while (!in_order)
    {
        temp = NULL;
        // create a "ring buffer" to keep track of the references to the head + tails of piles of cards
        // first half of the buffer is the heads, second half is the tails
        card* head_tail_buf[num_piles*2];
        memset(head_tail_buf, 0, sizeof(head_tail_buf));

        int pile = 0;

        // shuffle the cards into piles
        while (deck)
        {
            if (head_tail_buf[pile])
            {
                temp = head_tail_buf[pile];
            }
            else
            {
                // assign tail
                head_tail_buf[pile + num_piles] = deck;
            }
            head_tail_buf[pile] = deck;
            deck = deck->next;
            head_tail_buf[pile]->next = temp;
            pile = (pile + 1) % num_piles;
            temp = NULL;
        }

        // assign head to the top pile
        deck = head_tail_buf[0];

        // stack the piles
        for (int i = 0; i < num_piles - 1; i++)
        {
            head_tail_buf[i + num_piles]->next = head_tail_buf[i + 1];
        }

        rounds++;
        temp = deck;

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
    // Use signed integers to catch if user inputted negative numbers as arguments
    int32_t num_cards;
    int32_t num_piles;
    int c = 0;
    card* deck;
    card* temp;
    unsigned long nrounds;

    if (argc == 1)
    {
        printf("Usage: %s -N <number of cards> -Y <number of piles>\n", argv[0]);
        return -1;
    }

    while ((c = getopt(argc, argv, "N:Y:")) != - 1)
    {
        switch(c) {
        case 'N':
            num_cards = atoi(optarg);
            break;
        case 'Y':
            num_piles = atoi(optarg);
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    if (num_cards <= 0 || num_piles <= 0)
    {
        printf("Number of cards and piles must be greater than zero\n");
        return EXIT_FAILURE;
    }
    else if (num_cards < num_piles)
    {
        printf("Number of cards must be greater than or equal to number of piles\n");
        return EXIT_FAILURE;
    }
    else if (num_cards == num_piles)
    {
        printf("Number of rounds to return to original order: %d\n", num_cards);
        return EXIT_SUCCESS;
    }

    deck = make_deck(num_cards);

    nrounds = shuffle_cards(deck, num_cards, num_piles);
    printf("Number of rounds = %lu\n", nrounds);

    // free memory
    temp = deck;
    while (temp)
    {
        card* t2 = temp;
        temp = temp->next;
        free(t2);
    }

    return EXIT_SUCCESS;
}
