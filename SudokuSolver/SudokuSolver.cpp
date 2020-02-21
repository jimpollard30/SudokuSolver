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

bool evaluate_puzzle()
{
	bool complete = true;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (puzzle[i][j] == 0)
			{
				complete = false;
				break;
			}
		}
	}
	return complete;
}
void solve_puzzle()
{
	build_initial_candidates();
	bool made_changes = true;
	int iterations = 0;
	while (made_changes )
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
				int cccc = 0;
			}
		}
		iterations++;
		std::cout << "iteration: " << iterations << "\n";

	}
	bool puzzle_complete = evaluate_puzzle();
}

void print_solution()
{
	for (int ii = 0; ii < 9; ii++)
	{
		for (int jj = 0; jj < 9; jj++)
		{
			std::cout << puzzle[ii][jj] << " ";
		}
		std::cout << "\n";
	}
}

void build_puzzle()
{
	std::ifstream file("puzzle_input.txt");
	std::string str;
	int i, j = 0;
	while (std::getline(file, str))
	{
		//std::cout << str << "\n";
		std::vector<std::string> row = split(str, ',');
		//std::cout << row.size() << "\n";
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
}
int main()
{

	build_puzzle();
	std::cout << "initial puzzle:" << "\n";
	print_solution();
	
	solve_puzzle();
	std::cout << "\n" << "final puzzle" << "\n";
	print_solution();

	bool bstatus = evaluate_puzzle();
	if (evaluate_puzzle()) {
		std::cout << " puzzle complete " << "\n";
	}
	else
	{
		std::cout << " puzzle incomplete " << "\n";
	}
	std::cin ;
    return 0;
}

