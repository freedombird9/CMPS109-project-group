#ifndef HEX_H
#define HEX_H

#include <iostream>
#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

enum class Color{Black, White, Empty};

class Hex {

public:
  friend ostream& operator<<(ostream &out, const Hex &input);
  Hex(int s);
  void setBoard ();
  bool wins ();  
  bool move (int x, int y, Color c);
  void AI(int &x, int &y, int difficulty = 1000);  // computer AI, will generate a move
private:
  bool upjudge (int node);
  bool leftjudge (int node);
  void indexToCoordin (int index, int &row, int &col);
  int coordinToIndex (int x, int y);
  int size;
  vector<bool> visitorInfo;
  vector<Color> color;
  vector<list<int> > adjList;
};

#endif
