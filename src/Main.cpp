/// <summary>
/// Startup project, will have all the necessary project settings done and ready to go
/// for Box2D and Thor. I have set up th eproject in such a way that does not require
/// any external installation.
/// Author: Daryl keogh
/// Date: 17/10/2017
/// </summary>

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
//#pragma comment(lib,"thor-d.lib") currently i have Thor 2.0 x32 library files, we will either have to get the 64 bit files or try to get the 32 bit library files for Box2D
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
//#pragma comment(lib,"thor.lib") 
#endif 

//include any header files needed
#include <Game.h>

//main entry point of our game, create our game and call run
void main()
{
	Game game;
	game.run();
}