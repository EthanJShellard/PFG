#include "Input.h"

/*! \brief Brief description.
*  Input class captures and processs keyboard and mouse events
*
*/
Input::Input()
{
	Quit = cmd_s = cmd_a = cmd_w = cmd_d = keyDown = cmd_mouseleft = cmd_mouseleftUp = cmd_mouseright = cmd_mouserightUp = once = cmd_z = cmd_x = cmd_c = false;
}

std::vector<int> Input::keys;
std::vector<int> Input::downKeys;
std::vector<int> Input::upKeys;

Input::~Input()
{
}

glm::vec2 Input::getMouseDelta()
{
	return mouseDelta;
}

void Input::update()
{
	mouseDelta.x = 0;
	mouseDelta.y = 0;
	cmd_mouserightUp = false;
	cmd_mouseleftUp = false;

	while (SDL_PollEvent(&eventQueue) != 0)
	{
		if (eventQueue.type == SDL_QUIT)
		{
			Quit = 1;
		}
		else if (eventQueue.type == SDL_MOUSEMOTION)
		{

			mouse_x = eventQueue.motion.x;
			mouse_y = eventQueue.motion.y;

			mouseDelta.x += mouse_x - 400;
			mouseDelta.y += mouse_y - 300;

		}
		else if (eventQueue.type == SDL_MOUSEBUTTONDOWN && eventQueue.button.button == SDL_BUTTON_LEFT)
		{
			cmd_mouseleft = true;
		}
		else if (eventQueue.type == SDL_MOUSEBUTTONDOWN && eventQueue.button.button == SDL_BUTTON_RIGHT)
		{
			cmd_mouseright = true;
		}
		else if (eventQueue.type == SDL_MOUSEBUTTONUP && eventQueue.button.button == SDL_BUTTON_LEFT)
		{
			cmd_mouseleft = false;
			cmd_mouseleftUp = true;
		}
		else if (eventQueue.type == SDL_MOUSEBUTTONUP && eventQueue.button.button == SDL_BUTTON_RIGHT)
		{
			cmd_mouseright = false;
			cmd_mouserightUp = true;
		}
		else if (eventQueue.type == SDL_KEYDOWN)
		{
			if (eventQueue.key.keysym.sym == SDLK_UP || eventQueue.key.keysym.sym == SDLK_w)
			{
				//mouseDelta.y = 10;
				cmd_w = true;
				std::cout << "w pressed! \n";
			}
			else if (eventQueue.key.keysym.sym == SDLK_DOWN || eventQueue.key.keysym.sym == SDLK_s)
			{
				//mouseDelta.y = -10;
				cmd_s = true;
				std::cout << "s pressed! \n";
			}
			else if (eventQueue.key.keysym.sym == SDLK_RIGHT || eventQueue.key.keysym.sym == SDLK_d)
			{
				//mouseDelta.x = 10;
				cmd_d = true;
				std::cout << "d pressed! \n";
			}
			else if (eventQueue.key.keysym.sym == SDLK_LEFT || eventQueue.key.keysym.sym == SDLK_a)
			{
				//mouseDelta.x = -10;
				cmd_a = true;
				std::cout << "a pressed! \n";
			}
			else if (eventQueue.key.keysym.sym == SDLK_x)
			{
				cmd_x = true;
				std::cout << "x pressed! \n";
			}
			else if (eventQueue.key.keysym.sym == SDLK_ESCAPE)
			{
				Quit = true;
			}


		}
		else if (eventQueue.type == SDL_KEYUP)
		{
			if (eventQueue.key.keysym.sym == SDLK_UP || eventQueue.key.keysym.sym == SDLK_w )
			{
				//mouseDelta.y = 0;
				cmd_w = false;
				

			}
			else if (eventQueue.key.keysym.sym == SDLK_DOWN || eventQueue.key.keysym.sym == SDLK_s)
			{
				cmd_s = false;
			}
			else if (eventQueue.key.keysym.sym == SDLK_RIGHT || eventQueue.key.keysym.sym == SDLK_d)
			{
				//mouseDelta.x = 0;
				cmd_d = false;
			}
			else if (eventQueue.key.keysym.sym == SDLK_LEFT || eventQueue.key.keysym.sym == SDLK_a)
			{
				cmd_a = false;
			}
		}

		/// move the mouse back to the middle of the screen each frame
		SDL_WarpMouseInWindow(NULL, 400, 300);

	}

}





