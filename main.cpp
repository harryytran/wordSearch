#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int numRows, numCols;
bool isWordFound;
vector<string> notFoundWords;

bool openFileForReading(ifstream &infile, string fileName) {
    infile.open(fileName);
    if (infile.good())
        return true;
    else {
        perror("Can't open the specified input file");
        cout << "Filename: " << fileName << endl;
        cout << "Enter filename (or type 'quit'): ";
        getline(cin, fileName);
        if (fileName != "quit") {
            cout << "The new file name is: " << fileName << endl;
            bool wasOpened = openFileForReading(infile, fileName);
            return wasOpened;
        } else {
            cout << "quitting" << endl;
            return false;
        }
    }
}

int xDirections[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int yDirections[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

bool search2D(char grid[100][100], int row, int col, string word, string& direction)
{
    // If first character of word doesn't match with
    // given starting point in grid.
    if (grid[row][col] != word[0])
      return false;

    int len = word.length();
    

    // Search word in all 8 directions starting from (row,col)
    for (int dir = 0; dir < 8; dir++)
    {
        // Initialize starting point for current direction
        int k, rd = row + x[dir], cd = col + y[dir];

        // First character is already checked, match remaining
        // characters
        for (k = 1; k < len; k++) {
            // If out of bound break
            if (rd >= rowNum || rd < 0 || cd >= colNum || cd < 0)
                break;

            // If not matched,  break
            if (grid[rd][cd] != word[k])
                break;

            //  Moving in particular direction
            rd += x[dir], cd += y[dir];
            
            
        }
        // If all character matched, then value of must
        // be equal to length of word
        if (k == len)
        {
            // Using if else to determine the direction of which the movie title is found
            if(x[dir] == -1 && y[dir] == -1)
            {
                direction = "NW"; //SW
            }
            else if(x[dir] == -1 && y[dir] == 0)
            {
                direction = "N"; //W
            }
            else if(x[dir] == -1 && y[dir] == 1)
            {
                direction = "NE"; //NW
            }
            else if(x[dir] == 0 && y[dir] == -1)
            {
                direction = "W"; //S
            }
            else if(x[dir] == 0 && y[dir] == 1)
            {
                direction = "E"; // N
            }
            else if(x[dir] == 1 && y[dir] == -1)
            {
                direction = "SW"; //SE
            }
            else if(x[dir] == 1 && y[dir] == 0)
            {
                direction = "S"; //E
            }
            else if(x[dir] == 1 && y[dir] == 1)
            {
                direction = "SE"; //NE
            }
             return true;
        }
    }
    return false;
}
string removeSpaces(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

void skipAnyCommentLines(ifstream &infile) {
    char ch;
    string textLine;
    ch = infile.peek();
    while (ch == '#' || ch == '\n') {
        getline(infile, textLine);
        ch = infile.peek();
    }
}

void patternSearch(char grid[][100], string word, string originalWord) {
    string foundDirection;
    isWordFound = false;
    for (int row = 0; row < numRows; row++)
        for (int col = 0; col < numCols; col++)
            if (search2D(grid, row, col, word, foundDirection)) {
                cout << originalWord << " found at " << row + 1 << ", " << col + 1 << ": (direction = " << foundDirection << ") " << endl;
                isWordFound = true;
            }
}

void convertToUpperCase(string& s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = toupper(s[i]);
    }
}

int main() {
    ifstream infile;
    string fileName, dummy, currentWord, rowStr, colStr, row, line;
    vector<string> movieTitles, originalMovieTitles;
    
    cout << "Please enter the name of the file: " << endl;
    cin >> fileName;
    
    if (openFileForReading(infile, fileName)) {
        skipAnyCommentLines(infile);
        getline(infile, rowStr);
        
        int spaceLocation = rowStr.find(" ");
        row = rowStr.substr(0, spaceLocation);
        colStr = rowStr.substr(spaceLocation);
        
        numRows = stoi(row);
        numCols = stoi(colStr);
        
        cout << "Rows: " << numRows << " ";
        cout << "Cols: " << numCols << endl;
        
        char matrix[100][100];
        getline(infile, dummy);
        getline(infile, dummy);
        
        for (int i = 0; i < numRows; i++) {
            getline(infile, line);
            for (int j = 0; j < numCols; j++) {
                matrix[i][j] = line[j];
            }
        }
        
        skipAnyCommentLines(infile);
        
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                cout << matrix[i][j];
            }
            cout << endl;
        }
        cout << endl;
        
        while (infile.good()) {
            if (infile.peek() != '#') {
                getline(infile, currentWord);
                movieTitles.push_back(currentWord);
            } else {
                getline(infile, dummy);
                skipAnyCommentLines(infile);
            }
        }
        originalMovieTitles = movieTitles;
        
        for (int i = 0; i < movieTitles.size(); i++) {
            movieTitles[i] = removeSpaces(movieTitles[i]);
            convertToUpperCase(movieTitles[i]);
            patternSearch(matrix, movieTitles[i], originalMovieTitles[i]);
            
            if (!isWordFound) {
                notFoundWords.push_back(originalMovieTitles[i]);
            }
        }
        
        cout << "Couldn't find these movies:" << endl;
        for (int i = 0; i < notFoundWords.size(); i++) {
            cout << notFoundWords[i] << endl;
        }
    }
    
    infile.close();
    return 0;
}
