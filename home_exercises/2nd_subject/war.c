#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define CARDS   52


struct Player {
    int deck[CARDS];
    int war_queue[CARDS];
    int deck_start;
    int deck_length;
    int war_queue_length;
};

struct Player create_player() {
    struct Player player;
    player.deck_start = 0;
    player.deck_length = 0;
    player.war_queue_length = 0;
    for (int i = 0; i < CARDS; i++) {
        player.deck[i] = 0;
        player.war_queue[i] = 0;
    }
    return player;
}

void print_deck(struct Player * player) {
    for (int i = 0; i < player->deck_length; i++) {
        if (i + player->deck_start < CARDS) printf("%d ", player->deck[i + player->deck_start]);
        else printf("%d ", player->deck[i + player->deck_start - CARDS]);
    }
}

int war_queue_push(struct Player * player, int card) {
    if (player->war_queue_length < CARDS) {
        player->war_queue[player->war_queue_length] = card;
        player->war_queue_length = player->war_queue_length + 1;
        return 0;
    }
    return 1;
}

int war_queue_top(struct Player * player) {
    return player->war_queue[player->war_queue_length - 1];
}

int war_queue_read(struct Player * player, int index) {
    return player->war_queue[index];
}

int war_queue_length(struct Player * player) {
    return player->war_queue_length;
}

void empty_war_queue(struct Player * player) {
    for (int i = 0; i < CARDS; i++) {
        player->war_queue[i] = 0;
    }
    player->war_queue_length = 0;
}

int is_out_of_cards(struct Player * player) {
    return player->deck_length == 0;
}

int push_card(struct Player * player, int card) {
    if (player->deck_length >= CARDS) {
        return 1;
    }
    if (player->deck_start + player->deck_length < CARDS) player->deck[player->deck_start + player->deck_length] = card;
    else player->deck[player->deck_start + player->deck_length - CARDS] = card;
    player->deck_length++;
    return 0;
}

int pop_card(struct Player * player) { 
    if (player->deck_length <= 0) {
        return -1;
    }
    player->deck_start++;
    player->deck_length--;
    return player->deck[player->deck_start - 1];
}

int play_card(struct Player * player) {
    int card = pop_card(player);
    if (card < 0) return 1;
    return war_queue_push(player, card);
}

int compare_cards(struct Player * playerA, struct Player * playerB) {
    int cardA = war_queue_top(playerA);
    int cardB = war_queue_top(playerB);
    return (cardA >> 2) - (cardB >> 2);
}

int collect_cards(struct Player * winner, struct Player * loser) {
    int queue_index = 0;
    int winner_war_queue_length = war_queue_length(winner);
    while (winner_war_queue_length != queue_index) {
        int is_full = push_card(winner, war_queue_read(winner, queue_index));
        queue_index++;
        if (is_full) return 1;
    }
    empty_war_queue(winner);
    
    queue_index = 0;
    int loser_war_queue_length = war_queue_length(loser);
    while (loser_war_queue_length != queue_index) {
        int is_full = push_card(winner, war_queue_read(loser, queue_index));
        queue_index++;
        if (is_full) return 1;
    }
    empty_war_queue(loser);
    return 0;
}

int fight(struct Player * playerA, struct Player * playerB, int * conflicts, int max_conflicts, int simple) {
    if (*conflicts == max_conflicts) return 1;

    if (is_out_of_cards(playerA) || is_out_of_cards(playerB)) return 2;
    
    *conflicts = *conflicts + 1;

    play_card(playerA);
    play_card(playerB);

    int outcome = compare_cards(playerA, playerB);

    if (outcome > 0) {
        // playerA wins
        int win = collect_cards(playerA, playerB);
        return win;
    }

    if (outcome < 0) {
        // playerB wins
        int win = collect_cards(playerB, playerA);
        return win;
    }

    if (simple) {
        collect_cards(playerA, playerA);
        collect_cards(playerB, playerB);
        return 0;
    }

    // there is a war

    if (is_out_of_cards(playerA) || is_out_of_cards(playerB)) return 2;

    play_card(playerA);
    play_card(playerB);

    return fight(playerA, playerB, conflicts, max_conflicts, simple);
}

int rand_from_interval(int a, int b) {
    if (a == b) {
        return a;
    }
    if (a > b) {
        return INT_MIN;
    }
    if (b - a > RAND_MAX) {
        return INT_MAX;
    }
    int range = b - a + 1;
    return a + rand() % range;
}

void bogosort(int n, int tab[]) {
    for (int i = 0; i < n; i++){
        tab[i] = i;
    }
    for (int i = 0; i < n - 1; i++){
        int k = rand_from_interval(i, n - 1);
        int placeholder = tab[i];
        tab[i] = tab[k];
        tab[k] = placeholder;
    }
}

int main(void) {
    int seed;
    int version;
    int max_conflicts;
    int conflicts = 0;
    int outcome = 0;

    scanf("%d %d %d", &seed, &version, &max_conflicts);
    srand(seed);

    // create and shuffle the deck
    int deck[CARDS];

    for (int i = 0; i < CARDS; i++) {
        deck[i] = i;
    }

    bogosort(CARDS, deck);

    // initialize players and give them the cards
    struct Player playerA = create_player();
    struct Player playerB = create_player();

    for (int i = 0; i < CARDS / 2; i++) {
        push_card(&playerA, deck[i]);
    }
    
    for (int i = CARDS / 2; i < CARDS; i++) {
        push_card(&playerB, deck[i]);
    }
    
    for (int i = CARDS / 2; i < CARDS; i++) {
        playerA.deck[i] = 0;
        playerB.deck[i] = 0;
    }

    while (max_conflicts > conflicts) {
        outcome = fight(&playerA, &playerB, &conflicts, max_conflicts, version);
        if (outcome != 0) break;
    }

    if (conflicts == max_conflicts) {
        printf("0 %d %d\n", playerA.deck_length + playerA.war_queue_length, playerB.deck_length + playerB.war_queue_length);
        return 0;
    }

    if (playerA.deck_length == 52) {
        printf("2 %d\n", conflicts);
        return 0;
    }

    if (playerB.deck_length == 52) {
        printf("3\n");
        print_deck(&playerB);
        return 0;
    }

    if (outcome == 2) {
        printf("1 %d %d\n", playerA.deck_length + playerA.war_queue_length, playerB.deck_length + playerB.war_queue_length);
        return 0;
    }

    return 0;
}