#include "hex.h"
#include <ctime>

int main(){
  
  int x, y;
  int size, difficulty;
  cout << endl;
  cout << "================================= HEX Game ======================================" << endl;
  cout << endl;
  cout << "for board size smaller than or equal to 7*7, 3000 difficulty will make it very hard to win the computer" << endl;
  cout << "for board size 9*9, you may need 6000+ difficulty to make your opponent smart enough" << endl;
  cout << "for board size 11*11, you'll need at least 12000 difficulty" << endl;
  cout << endl;
  cout << "please input the board size" << endl;
  cin >> size;
  cout << "please input difficulty" << endl;
  cin >> difficulty;
  Hex game(size);
  game.setBoard();

  cout << game << endl;
  
  srand(time(NULL));
  while (true){
    cout << "White side plays: please input the location" << endl;
    while (true){
      cin >> x >> y;
      if(game.move(x, y, Color::White))   // white moves first
	break;
      else cout << "illegal movement, please try agagin" << endl;
    }
    cout << game << endl;
    if (game.wins()) {
      cout << "White wins!" << endl;
      break;
    }

    cout << "computer plays..." << endl;
	game.AI(x, y, difficulty);
	game.move(x, y, Color::Black);
    cout << game << endl;
    if (game.wins()) {
      cout << "computer wins!" << endl;
      break;
    }
  }
  cout << "game is over, press any key to exit" << endl;
  cin >> x;
  exit(0);
  return 0;
}   
