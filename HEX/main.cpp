#include "hex.h"

int main(){
  
  int x, y;
  int size, difficulty;

  cout << "please input the board size" << endl;
  cin >> size;
  cout << "please input difficulty" << endl;
  cin >> difficulty;
  Hex game(size);
  game.setBoard();

  cout << game << endl;

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
