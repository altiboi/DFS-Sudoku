#include <iostream>
#include <array>
#include <stack>
#include <cctype>
#include <set>
#include <string>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;

void print(array<array<char, 9>, 9> sudoku);

array<array<char, 9>, 9> grid()
{
    array<array<char, 9>, 9> sudoku;

    for(int i = 0; i < 9; i++){
     for(int k = 0; k < 9; k++){
        cin >> sudoku[i][k];
    }
    }

    return sudoku;
}

int empty_blocks(array<array<char, 9>, 9> puzzle)
{
    int count = 0;

    for(int i = 0; i < 9; i++){
    for(int k = 0; k < 9; k++){
        if(puzzle[i][k] == '0'){
            count++;
        }
    }
    }

    return count;
}

set<string> exists(array<array<char, 9>, 9> puzzle)
{
    set<string> used;

    for(int i = 0; i < 9; i++){
    for(int k = 0; k < 9; k++){
       if(puzzle[i][k] != '0')
       {
        string value;
        value.append(1, puzzle[i][k]);
        used.insert(value + " in row " + to_string(i));
        used.insert(value + " in column " + to_string(k));
        used.insert(value + " in block " + to_string(i/3) + "-" + to_string(k/3));
       }
    }
    }

    return used;
}

pair<int, int> next_zero(array<array<char, 9>, 9> puzzle, int row, int col)
{

    while(row <=8  && puzzle[row][col] != '0')
    {
        if(col == 8)
        {
           if(row < 8){
            row++;
            col = 0;
           }
           else if(row == 8){
               break;
           }
            continue;
        }
        col++;
    }
    
    return make_pair(row,col);
}

void remove_from_set(int row, int col, char v, set<string> &used)
{
    string val;
    val.append(1,v);
    used.erase(val + " in row " + to_string(row));
    used.erase(val + " in column " + to_string(col));
    used.erase(val + " in block " + to_string(row/3) + "-" + to_string(col/3));
}

bool invalid(int row, int col, char v, set<string> &used)
{
   string val;
   val.append(1,v);
   if(v == '0')
   {
       return true;
   }
   else if(used.find(val + " in row " + to_string(row)) == used.end() &&
        used.find(val + " in column " + to_string(col)) == used.end() &&
        used.find(val + " in block " + to_string(row/3) + "-" + to_string(col/3)) == used.end())
        {
            used.insert(val + " in row " + to_string(row));
            used.insert(val + " in column " + to_string(col));
            used.insert(val + " in block " + to_string(row/3) + "-" + to_string(col/3));
            return false;
        } 
    return true;
}

bool valid(int row, int col, char v, set<string> &used)
{
   string val;
   val.append(1,v);
   if(used.find(val + " in row " + to_string(row)) == used.end() &&
    used.find(val + " in column " + to_string(col)) == used.end() &&
    used.find(val + " in block " + to_string(row/3) + "-" + to_string(col/3)) == used.end())
    {
        return true;
    } 
    return false;
}

int index(vector<char> &values, char v)
{
    auto itr = find(values.begin(),values.end(),v);
    int index = distance(values.begin(),itr);
    return index;
}

map<pair<int, int>, vector<char> > possible(array<array<char, 9>, 9> &puzzle, set<string> &used)
{
    map<pair<int, int>, vector<char> > possible;

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(puzzle[i][j] == '0')
            {
                vector<char> p;
                char test = '1';

                for(int k = 0; k < 9; k++)
                {
                    if(valid(i,j,test,used))
                    {
                        p.push_back(test);
                    }
                    test++;
                }

                if(p.size() == 1)
                {
                    puzzle[i][j] = p[0];

                    string val;
                    val.append(1,p[0]);
                    used.insert(val + " in row " + to_string(i));
                    used.insert(val + " in column " + to_string(j));
                    used.insert(val + " in block " + to_string(i/3) + "-" + to_string(j/3));
                }
                else
                {
                    possible.insert(make_pair(make_pair(i,j), p));
                }
            }
        }
    }
    return possible;
}

void hiddenSingles(array<array<char, 9>, 9> puzzle)
{

}

array<array<char, 9>, 9> solve(array<array<char, 9>, 9> sudoku)
{
    stack<pair<int,int> > cell;
    set<string> used = exists(sudoku);
    map<pair<int, int>, vector<char> > values = possible(sudoku,used);
    pair<int,int> pos = next_zero(sudoku, 0, 0);
    cell.push(pos);
    int row = pos.first;
    int col = pos.second;
    sudoku[row][col] = values[pos][0];
    int blocks_left = empty_blocks(sudoku);

    while (blocks_left >= 0)
    {
        if(invalid(row, col, sudoku[row][col], used))
        {
            int count = index(values[(make_pair(row,col))],sudoku[row][col]);

            while(invalid(row, col, sudoku[row][col], used))
            {
                if((values[make_pair(row,col)].size() - 1) == count)
                {
                   while(values[make_pair(row,col)].back() == sudoku[row][col])
                   {
                        sudoku[row][col] = '0';
                        cell.pop();

                        if(cell.empty()){
                            cout << "No Solution" << endl;
                            exit(0);
                        }

                        row = cell.top().first;
                        col = cell.top().second;
                        blocks_left++;

                        remove_from_set(row, col, sudoku[row][col], used);
                   }
                    count = index(values[(make_pair(row,col))],sudoku[row][col]);
                }
                count++;
                sudoku[row][col] = values[make_pair(row,col)][count];
            }

            if(blocks_left > 0)
            {
            pair<int,int> pos = next_zero(sudoku, row, col);
            row = pos.first;
            col = pos.second;
            sudoku[row][col] = values[pos][0];
            cell.push(make_pair(row,col));
            }

            blocks_left--;
        }
        else
        {
            if(blocks_left > 0)
            {
            pair<int,int> pos = next_zero(sudoku, row, col);
            row = pos.first;
            col = pos.second;
            sudoku[row][col] = values[pos][0];
            cell.push(make_pair(row,col));
            }

            blocks_left--;
        }
    }
       
    return sudoku;
}

void print(array<array<char, 9>, 9> sudoku)
{  
    for(int i = 0; i < 9; i++){
     for(int k = 0; k < 9; k++){
       cout << sudoku[i][k];
       if(k < 8){
           cout << " ";
       }
    } 
    cout << endl;
    }
}

int main()
{
    print(solve(grid()));
    return 0;
}