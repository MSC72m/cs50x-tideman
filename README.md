# Tideman Voting System

## Project Description
This project is a solution to a CS50x problem set that involves implementing the Tideman voting system. The program allows users to vote for candidates by ranking them in order of preference. It then determines the winner using the Tideman algorithm, which ensures that the candidate with the broadest support wins.

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Algorithm Explanation](#algorithm-explanation)
- [Code Explanation](#code-explanation)

## Installation
No special installation is required for this project. Ensure you have a C compiler like `gcc` installed and the CS50 library available.

## Usage
To compile and run the project, use the following commands:
make tideman
./tideman candidate1 candidate2 ... candidateN
You will be prompted to enter the number of voters and then to rank candidates in order of preference for each voter.

## Algorithm Explanation
The Tideman voting algorithm (also known as Ranked Pairs) is a voting system that prioritizes candidates who win the strongest pairwise contests. The algorithm works as follows:

Record Preferences:

For each voter's ranking, update the preferences to reflect the number of voters who prefer candidate A over candidate B.
### Create Pairs:

Identify all pairs of candidates where one candidate is preferred over the other.
### Sort Pairs:

Sort the pairs in decreasing order by the strength of the victory. The strength of a victory is determined by the number of voters who prefer one candidate over the other.
### Lock Pairs:

Lock pairs into the candidate graph without creating cycles. This step ensures that the final graph represents a valid ordering of candidates.
### Print the Winner:

The winner is the candidate who is not locked in by any other candidate. This candidate is the "source" of the graph.
### Example Walkthrough
Suppose there are three candidates: Alice, Bob, and Charlie. The voters' preferences are recorded, pairs are created, and sorted based on the strength of victory. The pairs are then locked without creating cycles, and the winner is determined.

## Code Explanation
Constants and Structures
``` C
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
```
#include <cs50.h>, #include <stdio.h>, and #include <string.h> include the necessary libraries for CS50 functions, standard input/output functions, and string handling functions respectively.
#define MAX 9 sets the maximum number of candidates to 9.
int preferences[MAX][MAX] is a 2D array that keeps track of voter preferences between pairs of candidates.
bool locked[MAX][MAX] is a 2D array that keeps track of which candidates are locked in over others.
typedef struct { int winner; int loser; } pair; defines a structure for a pair of candidates.
string candidates[MAX] is an array that holds the candidate names.
pair pairs[MAX * (MAX - 1) / 2]; is an array that holds all pairs of candidates.
int pair_count and int candidate_count keep track of the number of pairs and candidates respectively.
Function Prototypes
``` C
bool vote(int rank, string name, int ranks[]);
bool cycle_pairs(int start, int endCase);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
```
These are the prototypes for the functions used in the program.
Main Function
``` C
int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
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

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
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
```
if (argc < 2) checks if there are enough command-line arguments (at least one candidate) and prints usage instructions if not.
candidate_count = argc - 1; sets the number of candidates.
if (candidate_count > MAX) checks if the number of candidates exceeds the maximum allowed and prints an error message if true.
The for loop populates the candidates array with candidate names.
The nested for loop clears the locked array by setting all values to false.
int voter_count = get_int("Number of voters: "); prompts the user to enter the number of voters.
The second for loop iterates over each voter, prompting them to rank candidates. If vote(j, name, ranks) returns false, it prints "Invalid vote." and exits with an error code.
record_preferences(ranks);, add_pairs();, sort_pairs();, lock_pairs();, and print_winner(); are called to process the votes, sort pairs, lock pairs, and print the winner.
Vote Function
``` C
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
```
The for loop iterates over each candidate.
if (strcmp(name, candidates[i]) == 0) checks if the given name matches a candidate's name.
If a match is found, the candidate's index is recorded in the ranks array at the given rank position and the function returns true.
If no match is found, the function returns false.
Record Preferences Function
``` C
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
```
The outer for loop iterates over each candidate.
The inner for loop updates the preferences array to reflect the preferences of the voters.
Add Pairs Function
``` C
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
```
The nested for loops iterate over each pair of candidates.
If one candidate is preferred over the other, the pair is added to the pairs array.
Sort Pairs Function
``` C
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = 0; j < pair_count - i - 1; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[j + 1].winner][pairs[j + 1].loser])
            {
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
            }
        }
    }
    return;
}
```
The nested for loops sort the pairs array in decreasing order of the strength of victory using a bubble sort algorithm.
Cycle Pairs Function
``` C
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
```
The function checks for cycles in the graph. If adding a pair creates a cycle, the function returns true.
Lock Pairs Function
``` C
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
```
The for loop locks pairs into the locked array, ensuring no cycles are created.
Print Winner Function
``` C
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
```
The for loop iterates over each candidate and checks if they are not locked in by any other candidate. If a candidate is found who is not locked in by any other candidate, they are declared the winner.
