#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
bool cycle_pairs(int start, int endCase);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }
  
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    for (int i = 0; i < voter_count; i++)
    {
        int ranks[candidate_count];

        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();

    return 0;
}

bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {

        if (strcmp(name, candidates[i]) == 0)
        {

            ranks[rank] = i;

            return true;
        }
    }

    return false;
}

void record_preferences(int ranks[])
{
    
    for (int i = 0; i < candidate_count; i++)
    {

        for (int j = i + 1; j < candidate_count; j++)
        {

            preferences[ranks[i]][ranks[j]]++;
        }
    }

    return;
}


void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {

        for (int j = i + 1; j < candidate_count; j++)
        {

            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {

                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}


void sort_pairs(void)
{

    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = 0; j < pair_count - i - 1; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] <
                preferences[pairs[j + 1].winner][pairs[j + 1].loser])
            {

                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
            }
        }
    }
    return;
}

bool cycle_pairs(int endCase, int start)
{

    if (endCase == start)
    {

        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {


        if (locked[endCase][i])
        {
            if (cycle_pairs(i, start))
            {
                return true;
            }
        }
    }
    return false;
}


void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {

        if (!cycle_pairs(pairs[i].loser, pairs[i].winner))
        {

            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}
// print the winner
void print_winner(void)
{
    
    for (int i = 0; i < candidate_count; i++)
    {

        int falseCount = 0;

        for (int j = 0; j < candidate_count; j++)
        {

            if (locked[j][i] == false)
            {
                falseCount++;
            }
        }

        if (falseCount == candidate_count)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}
