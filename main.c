#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define CAND_NUM 14
#define VOTER_NUM 2000

void fill_voter_arr(int voter_arr[VOTER_NUM][CAND_NUM]) {
	srand(time(NULL));
	int place = 0;
	for (int i = 0; i < VOTER_NUM; i++) {
		int candidate[CAND_NUM];
		for (int j = 0; j < CAND_NUM; j++)
			candidate[j] = j;

		for (int j = CAND_NUM; j > 0; j--) {
			place = rand() % j;
			voter_arr[i][CAND_NUM - j] = candidate[place];
			//printf("%d: %d -> %d\n", CAND_NUM - j, candidate[place], candidate[j - 1]);
			candidate[place] = candidate[j - 1];
		}
	}
}


void fptp(int voter_arr[VOTER_NUM][CAND_NUM], int results[CAND_NUM][2]) {
	int candidates[CAND_NUM] = {0};

	for (int i = 0; i < VOTER_NUM; i++)
		candidates[voter_arr[i][0]]++;

	for (int i = 0; i < CAND_NUM; i++) {
		int support = -1;
		int pos = 0;

		for (int j = 0; j < CAND_NUM; j++)
			if (candidates[j] > support) {
				support = candidates[j];
				pos = j;
			}

		results[i][0] = pos;
		results[i][1] = support;
		candidates[pos] = -1;
	}
}


void borda(int voter_arr[VOTER_NUM][CAND_NUM], int results[CAND_NUM][2]) {
	int candidates[CAND_NUM] = {0};

	for (int i = 0; i < VOTER_NUM; i++)
		for (int j = 0; j < CAND_NUM; j++)
			candidates[voter_arr[i][j]] += CAND_NUM - j - 1;

	for (int i = 0; i < CAND_NUM; i++) {
		int support = -1;
		int pos = 0;

		for (int j = 0; j < CAND_NUM; j++)
			if (candidates[j] > support) {
				support = candidates[j];
				pos = j;
			}

		results[i][0] = pos;
		results[i][1] = support;
		candidates[pos] = -1;
	}
}


void borda_nauru(int voter_arr[VOTER_NUM][CAND_NUM], double results[CAND_NUM][2]) {
	double candidates[CAND_NUM] = {0.0};

	for (int i = 0; i < VOTER_NUM; i++)
		for (int j = 0; j < CAND_NUM; j++)
			candidates[voter_arr[i][j]] += 1.0 / (double)(j + 1);

	for (int i = 0; i < CAND_NUM; i++) {
		double support = -1;
		int pos = 0;

		for (int j = 0; j < CAND_NUM; j++)
			if (candidates[j] > support) {
				support = candidates[j];
				pos = j;
			}

		results[i][0] = (double)pos;
		results[i][1] = support;
		candidates[pos] = -1;
	}
}


void instant_runoff(int voter_arr[VOTER_NUM][CAND_NUM], int results[CAND_NUM]) {
	int candidates[CAND_NUM] = {0};
	int voter_choice[VOTER_NUM] = {0};
	
	for (int k = CAND_NUM - 1; 1; k--) {
		for (int i = 0; i < VOTER_NUM; i++)
			candidates[voter_arr[i][voter_choice[i]]]++;
		
		int winner = 0;
		for (int i = 0; i < CAND_NUM; i++)
			if (candidates[i] > (VOTER_NUM / 2))
				winner = 1;
		if (winner) {
			for (int i = 0; i < k + 1; i++) {
				
				int support = -1;
				int pos = 0;

				for (int j = 0; j < CAND_NUM; j++)
					if (candidates[j] > support) {
						support = candidates[j];
						pos = j;
					}
				results[i] = pos;
				candidates[pos] = -1;
			}
			break;
		}

		int support = VOTER_NUM;
		int loser;
		for (int i = 1; i < CAND_NUM; i++)
			if (candidates[i] < support && candidates[i] > -1) {
				loser = i;
				support = candidates[i];
			}
		
		candidates[loser] = -1;
		results[k] = loser;
		
		for (int i = 0; i < VOTER_NUM; i++)
			while (candidates[voter_arr[i][voter_choice[i]]] == -1)
				voter_choice[i]++; //does not work with partial ballots, will cause array out of bounds exception

		for (int i = 0; i < CAND_NUM; i++)
			if (candidates[i] != -1)
				candidates[i] = 0;
	}
}


void coombs_rule(int voter_arr[VOTER_NUM][CAND_NUM], int results[CAND_NUM]) {
	int candidates[CAND_NUM] = {0};
	int voter_choice[VOTER_NUM] = {0};
	
	for (int k = CAND_NUM - 1; 1; k--) {
		for (int i = 0; i < VOTER_NUM; i++)
			candidates[voter_arr[i][voter_choice[i]]]++;
		
		int winner = 0;
		for (int i = 0; i < CAND_NUM; i++)
			if (candidates[i] > (VOTER_NUM / 2))
				winner = 1;
		if (winner) {
			for (int i = 0; i < k + 1; i++) {
				
				int support = -1;
				int pos = 0;

				for (int j = 0; j < CAND_NUM; j++)
					if (candidates[j] > support) {
						support = candidates[j];
						pos = j;
					}
				results[i] = pos;
				candidates[pos] = -1;
			}
			break;
		}
		for (int i = 0; i < CAND_NUM; i++)
			if (candidates[i] != -1)
				candidates[i] = 0;

		for (int i = 0; i < VOTER_NUM; i++) {
			int j = CAND_NUM - 1;
			while (candidates[voter_arr[i][j]] == -1)
				j--;

			candidates[voter_arr[i][j]]++;
		}

		int support = -1;
		int pos = 0;

		for (int j = 0; j < CAND_NUM; j++)
			if (candidates[j] > support) {
				support = candidates[j];
				pos = j;
			}
		
		candidates[pos] = -1;
		results[k] = pos;
		
		for (int i = 0; i < VOTER_NUM; i++)
			while (candidates[voter_arr[i][voter_choice[i]]] == -1)
				voter_choice[i]++; //does not work with partial ballots, will cause array out of bounds exception

		for (int i = 0; i < CAND_NUM; i++)
			if (candidates[i] != -1)
				candidates[i] = 0;
	}
}

void bucklin(int voter_arr[VOTER_NUM][CAND_NUM], int results[CAND_NUM][2]) {
	int candidates[CAND_NUM] = {0};
	int winner = 0;

	for (int i = 0; !(winner); i++) {
		for (int j = 0; j < VOTER_NUM; j++)
			candidates[voter_arr[j][i]]++;

		for (int j = 0; j < CAND_NUM; j++)
			if (candidates[j] > VOTER_NUM / 2)
				winner = 1;
	}

	for (int i = 0; i < CAND_NUM; i++) {
		int support = -1;
		int pos = 0;

		for (int j = 0; j < CAND_NUM; j++)
			if (candidates[j] > support) {
				support = candidates[j];
				pos = j;
			}

		results[i][0] = pos;
		results[i][1] = support;
		candidates[pos] = -1;
	}
}


void condorcet(int voter_arr[VOTER_NUM][CAND_NUM], double results[CAND_NUM][2]) {
	double candidates[CAND_NUM] = {0.0};

	for (int i = 0; i < CAND_NUM; i++)
		for (int j = i + 1; j < CAND_NUM; j++) {
			int pairwise_support[] = {0, 0};
			for (int k = 0; k < VOTER_NUM; k++)
				for (int l = 0; l < CAND_NUM; l++)
					if (voter_arr[k][l] == i) {
						pairwise_support[0]++;
						break;
					} else if (voter_arr[k][l] == j) {
						pairwise_support[1]++;
						break;
					}
			if (pairwise_support[0] > pairwise_support[1])
				candidates[i]++;
			else if (pairwise_support[1] > pairwise_support[0])
				candidates[j]++;
			else {
				candidates[i] += 0.5;
				candidates[j] += 0.5;
			}
		}

	for (int i = 0; i < CAND_NUM; i++) {
		double support = -1;
		int pos = 0;

		for (int j = 0; j < CAND_NUM; j++)
			if (candidates[j] > support) {
				support = candidates[j];
				pos = j;
			}

		results[i][0] = (double)pos;
		results[i][1] = support;
		candidates[pos] = -1;
	}
}

int sequential_pairwise(int voter_arr[VOTER_NUM][CAND_NUM]) {
	int i = 0;
	for (int j = 1; j < CAND_NUM; j++) {
		int pairwise_support[] = {0, 0};
		for (int k = 0; k < VOTER_NUM; k++)
			for (int l = 0; l < CAND_NUM; l++)
				if (voter_arr[k][l] == i) {
					pairwise_support[0]++;
					break;
				} else if (voter_arr[k][l] == j) {
					pairwise_support[1]++;
					break;
				}
		if (pairwise_support[0] < pairwise_support[1])
			i = j;
	}

	return i;
}

int main() {
	int voter_arr[VOTER_NUM][CAND_NUM];

	fill_voter_arr(voter_arr);

	int fptp_results[CAND_NUM][2];
	fptp(voter_arr, fptp_results);
	int borda_results[CAND_NUM][2];
	borda(voter_arr, borda_results);
	double borda_nauru_results[CAND_NUM][2];
	borda_nauru(voter_arr, borda_nauru_results);
	int instant_runoff_results[CAND_NUM];
	instant_runoff(voter_arr, instant_runoff_results);
	int coombs_rule_results[CAND_NUM];
	coombs_rule(voter_arr, coombs_rule_results);
	int bucklin_results[CAND_NUM][2];
	bucklin(voter_arr, bucklin_results);
	double condorcet_results[CAND_NUM][2];
	condorcet(voter_arr, condorcet_results);
	
	printf("\nFPTP\n");

	for (int i = 0; i < CAND_NUM; i++)
		printf("%d: %d\n", fptp_results[i][0], fptp_results[i][1]);

	printf("\nBorda\n");

	for (int i = 0; i < CAND_NUM; i++)
		printf("%d: %d\n", borda_results[i][0], borda_results[i][1]);

	printf("\nBorda Nauru\n");

	for (int i = 0; i < CAND_NUM; i++)
		printf("%d: %f\n", (int)borda_nauru_results[i][0], borda_nauru_results[i][1]);

	printf("\nInstant Runoff\n");

	for (int i = 0; i < CAND_NUM; i++)
		printf("#%d: %d\n", i + 1, instant_runoff_results[i]);
	
	printf("\nCoombs Rule\n");

	for (int i = 0; i < CAND_NUM; i++)
		printf("#%d: %d\n", i + 1, coombs_rule_results[i]);

	printf("\nBucklin\n");

	for (int i = 0; i < CAND_NUM; i++)
		printf("%d: %d\n", bucklin_results[i][0], bucklin_results[i][1]);

	printf("\nCondorcet\n");

	for (int i = 0; i < CAND_NUM; i++)
		printf("%d: %.1f\n", (int)condorcet_results[i][0], condorcet_results[i][1]);

	printf("\nSequential Pairwise\nThe winner is: %d\n", sequential_pairwise(voter_arr));

	return 0;
}
