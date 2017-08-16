#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;
using namespace sf;

int main(int argc, char **argv)
{
	string fileToPlay(argv[1]);

	Music music;

	cout << "Simple player!\n";

	music.openFromFile(fileToPlay);
	music.play();

	int x;
	cin >> x;
	return 0;
}