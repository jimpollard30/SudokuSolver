// SudokuSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// SudokuSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>


std::vector<int>candidates[9][9];

int puzzle[9][9];

enum Result {
	COMPLETE_AND_VALID,
	INCOMPLETE_AND_VALID,
	INVALID
};

std::vector<std::string> split(const std::string& s, char seperator)
{
	std::vector<std::string> output;
	std::string::size_type prev_pos = 0, pos = 0;
	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring(s.substr(prev_pos, pos - prev_pos));
		output.push_back(substring);
		prev_pos = ++pos;
	}
	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}

int digit_char_to_int(char c) {
	assert('0' <= c && c <= '9');
	return c - '0';
}

void build_initial_candidates()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++) {
			if (puzzle[i][j] == 0) {
				for (int k = 1; k <= 9; k++) {
					candidates[i][j].push_back(k);
				}
			}
		}

	}
}

void remove_candidate(std::vector<int> &candidate_list, int value) {

	for (std::size_t i = 0; i<candidate_list.size(); ++i)
	{
		if (candidate_list[i] == value) {
			candidate_list.erase(candidate_list.begin() + i);
		}
	}
}
void find_group_indices(int i, int *range)
{


	if (i < 3) {

		range[0] = 0;
		range[1] = 2;
	}
	else {
		if (i < 6) {

			range[0] = 3;
			range[1] = 5;

		}
		else {

			range[0] = 6;
			range[1] = 8;
		}
	}

}



void process_puzzle(int puzzle[9][9], std::vector<int>candidates[9][9])
{
	
	bool made_changes = true;
	int iterations = 0;
	while (made_changes)
	{

		made_changes = false;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{

				if (puzzle[i][j] == 0)
				{
					//check row, column and "group of 9" to eliminate from candidates.
					//first, check the other elements on same row

					for (int k = 0; k < 9; k++) {
						if (k != j)
						{
							if (puzzle[i][k] != 0)
							{
								//remove this value from the candidates at puzzle[i][j]
								remove_candidate(candidates[i][j], puzzle[i][k]);
							}
						}
					}
					//next, check other elements up & down the same column
					for (int k = 0; k < 9; k++) {
						if (k != i) {
							if (puzzle[k][j] != 0) {
								//remove from candidates
								remove_candidate(candidates[i][j], puzzle[k][j]);
							}
						}
					}
					//finally, check other elements in the same "group of 9"
					int xrange[2];
					int yrange[2];
					find_group_indices(i, xrange); //row indices
					find_group_indices(j, yrange);

					for (int ii = xrange[0]; ii < xrange[1]; ii++)
					{
						for (int jj = yrange[0]; jj < yrange[1]; jj++)
						{
							if ((ii != i) && (jj != j))
							{
								if (puzzle[ii][jj] != 0)
								{
									remove_candidate(candidates[i][j], puzzle[ii][jj]);
								}
							}
						}
					}
					//if we've eliminated all but 1 candidate, this is the value for this cell of the puzzle, so write it in
					if (candidates[i][j].size() == 1)
					{
						std::vector<int> clist = candidates[i][j];
						puzzle[i][j] = clist.at(0);
						made_changes = true;
						std::cout << "changed puzzle at: " << i << "," << j << " to: " << clist.at(0) << "\n";
						clist.clear();
						candidates[i][j].clear();
					}
				}
			}
		}
		iterations++;
		std::cout << "iteration: " << iterations << "\n";

	}

}


void print_solution(int puzzle[9][9])
{
	int row_count = 0;
	for (int ii = 0; ii < 9; ii++)
	{
		int col_count = 0;
		for (int jj = 0; jj < 9; jj++)
		{
			col_count++;
			if (col_count > 3)
			{
				col_count = 1;
				std::cout << "|" << " ";
			}
			std::cout << puzzle[ii][jj] << " ";
		}
		std::cout << "\n";
		row_count++;
		if (row_count > 2) {
			row_count = 0;
			std::cout << "---------------------\n";
		}
	}
}

bool is_complete(int puzzle[][9])
{
	//puzzle is "complete" if there are no zeroes at any position.
	int local_puzzle[9][9];

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			local_puzzle[i][j] = puzzle[i][j];
		}
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (local_puzzle[i][j] == 0)
				return false;
		}
	}
	return true;
}

bool check_group_of_9(int puzzle[9][9], int i, int j)
{
	for (int digit = 1; digit <= 9; digit++)
	{
		int digit_count = 0;

		for (int ii = i - 1; ii <= i + 1; ii++)
		{
			for (int jj = j - 1; jj <= j + 1; jj++)
			{
				if (puzzle[ii][jj] == digit)
				{
					digit_count++;
				}
			}
		}
		if (digit_count > 1)
			return false;
	}
	return true;
}

bool is_valid(int puzzle[9][9])
{
	//puzzle is "valid" if each digit, if it appears, appears once and only once in each column, row, and "group of 9"
	//puzzle can be "incomplete" and still valid.  

	//check each row...

	int local_puzzle[9][9];

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			local_puzzle[i][j] = puzzle[i][j];
		}
	}

	for (int i = 0; i < 9; i++)
	{
		int digit_count = 0;
		for (int digit = 1; digit <= 9; digit++)
		{
			digit_count = 0;
			for (int j = 0; j < 9; j++)
			{
				if (local_puzzle[i][j] == digit)
				{
					digit_count++;
				}
			}
			if (digit_count > 1)
				return false;
		}
	}
	//check each column
	for (int i = 0; i < 9; i++)
	{
		int digit_count = 0;
		for (int digit = 1; digit <= 9; digit++)
		{
			digit_count = 0;
			for (int j = 0; j < 9; j++)
			{
				if (local_puzzle[j][i] == digit)
				{
					digit_count++;
				}
			}
			if (digit_count > 1)
				return false;
		}
	}
	//check each group of 9
	if (!check_group_of_9(local_puzzle, 1, 1))
		return false;
	if (!check_group_of_9(local_puzzle, 1, 4))
		return false;
	if (!check_group_of_9(local_puzzle, 1, 7))
		return false;
	if (!check_group_of_9(local_puzzle, 4, 1))
		return false;
	if (!check_group_of_9(local_puzzle, 4, 4))
		return false;
	if (!check_group_of_9(local_puzzle, 4, 7))
		return false;
	if (!check_group_of_9(local_puzzle, 7, 1))
		return false;
	if (!check_group_of_9(local_puzzle, 7, 4))
		return false;
	if (!check_group_of_9(local_puzzle, 7, 7))
		return false;

	return true;
}



Result solve_puzzle(int puzzle[][9], std::vector<int>candidates[][9])
{
	int local_puzzle[9][9];
	//make a copy of the puzzle and candidate arrays for use of this level of the recursion
	std::vector<int>localcandidates[9][9];
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			local_puzzle[i][j] = puzzle[i][j];
			std::vector<int>clist = candidates[i][j];
			for (size_t k = 0; k < clist.size(); k++)
			{
				localcandidates[i][j].push_back(clist.at(k));
			}
		}
	}
	process_puzzle(local_puzzle, localcandidates);
	std::cout << "after process: \n";
	print_solution(local_puzzle);
	bool bvalid = is_valid(local_puzzle);
	bool bcomplete = is_complete(local_puzzle);
	if (bvalid && bcomplete)
	{
		return COMPLETE_AND_VALID;
		
	}
	if (bvalid && !bcomplete)
	{
		std::cout << "before guess: " << "\n";
		print_solution(puzzle);
		//find first non-empty candidate cell  (contains list of candidates)
		//for every candidate in that list:
		//...write that candidate in the cell location
		//...empty that candidate cell
		//...recurse:  call solve_puzzle with the local copy of the puzzle and candidates.
		//...it will either solve the puzzle, or determine that there is NO valid solution
		//...based on that particular candidate in that particular cell.  
		//...if you get to the end of the candidate list for that cell, then bubble up INVALID up the recursion stack.
		
		std::vector<int>cell_candidates;
		int trial_x;
		int trial_y;

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (localcandidates[i][j].size() > 0)
				{
					assert(localcandidates[i][j].size() > 1);
					for (size_t kk = 0; kk < localcandidates[i][j].size(); kk++)
					{
						cell_candidates.push_back(localcandidates[i][j].at(kk));
					}
					trial_x = i;
					trial_y = j;
					
					localcandidates[i][j].clear();

					goto bail;
				}
			}
		}
	bail:
		//iterate over "cell_candidates" here.
		for (int ii = 0; ii < cell_candidates.size(); ii++)
		{
			local_puzzle[trial_x][trial_y] = cell_candidates.at(ii);
			std::cout << "guessing: " << cell_candidates.at(ii) << " at position: " << trial_x << " , " << trial_y << "\n";
			Result result = solve_puzzle(local_puzzle, localcandidates);
			if (result == COMPLETE_AND_VALID)
				return result;
		}
		
	}
	else
		return INVALID;
}



void build_puzzle()
{
	std::ifstream file("puzzle_input.txt");
	std::string str;
	int i, j = 0;
	while (std::getline(file, str))
	{
		std::vector<std::string> row = split(str, ',');
		assert(row.size() == 9);
		for (i = 0; i < 9; i++)
		{
			std::string cell = row[i];
			assert(cell.length() == 1);
			const char *ccell = cell.c_str();
			if (*ccell == 'x')
				puzzle[j][i] = 0;
			else
				puzzle[j][i] = digit_char_to_int(*ccell);
		}
		j++;
	}
	build_initial_candidates();
}


int main()
{
	build_puzzle();
	std::cout << "initial puzzle:" << "\n";
	print_solution(puzzle);

	Result result = solve_puzzle(puzzle, candidates);
	if (result == COMPLETE_AND_VALID)
		std::cout << "puzzle solved \n";

	std::cin;
	return 0;
}

