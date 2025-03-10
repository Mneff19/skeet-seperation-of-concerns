/***********************************************************************
 * Source File:
 *    GAME
 * Author:
 *    Br. Helfrich
 * Summary:
 *    The file that contains main()
 ************************************************************************/

#include "uiInteract.h"
#include "skeet.h"
#include "position.h"

#define WIDTH  800.0
#define HEIGHT 800.0

PositionStorage Bullet::dimensions(WIDTH, HEIGHT);
PositionStorage Bird::dimensions(WIDTH, HEIGHT);

 /*************************************
  * All the interesting work happens here, when
  * I get called back from OpenGL to output a frame.
  * When I am finished drawing, then the graphics
  * engine will wait until the proper amount of
  * time has passed and put the drawing on the screen.
  **************************************/
void callBack(const UserInput* pUI, void* p)
{
   // the first step is to cast the void pointer into a game object. This
   // is the first step of every single callback function in OpenGL. 
   SkeetInterface* pSkeet = (SkeetInterface*)p;

   // handle user input
   pSkeet->interact(*pUI);
}

/*********************************
 * Main is pretty sparse.  Just initialize
 * my Skkeep type and call the display engine.
 * That is all!
 *********************************/
#ifdef _WIN32_X
#include <windows.h>
int WINAPI wWinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ PWSTR pCmdLine,
   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   // initialize OpenGL
   PositionStorage dimensions(WIDTH, HEIGHT);
   UserInput ui(0, NULL,
      "Skeet",
      dimensions);

   // initialize the game class
   SkeetInterface skeet(dimensions);

   // set everything into action
   ui.run(callBack, &skeet);

   return 0;
}
