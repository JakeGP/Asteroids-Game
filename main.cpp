#include <iostream>		//used for console functions in my game, for displaying errors
#include <SDL.h>		//the SDL library which i have used as my main GL
#include <SDL_mixer.h>	//the SDL library used for audio
#include <SDL_ttf.h>	//the SDL library used for displaying text, used in my game to display the score 
#include <cstdlib>		//included for the 'rand' function
#include <ctime>		//included for time functions for selected random numbers
#include <math.h>		//maths library, needed mathematic functions within the code 
#include <string>		//string and stream librarys used for when I 
#include <sstream>		//include text into the game mechanics

//Additional header files
#include "bullet.h"
#include "collision.h"
#include "bulletcollision.h"

//main program
int main(int argc, char *argv[])
{
	//initialise bullets and test for error
	Bullet *bullets[NUM_Bullets];

	for (int i = 0; i < NUM_Bullets; i++)
	{
		bullets[i] = 0;
	}

	//INITIALISING SDL and testing for error
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) //SDL_INIT_VIDEO initialises all the subsystems for this game
	{
		std::cout << "Error initialising SDL, application will close!" << std::endl; //an error tester incase SDL cannot intialise
		return -1;																	 //returns -1
	}

	//Initialising SDL_Mix and test for error
	if (MIX_INIT_MP3 < 0)
	{
		std::cout << "Error initialising SDL Mixer, application will now close!" << std::endl;
		return -1;
	}

	//Initialising SDL_ttf and test for error
	if (TTF_Init() == 1)
	{
		std::cout << "Error initialising SDL ttf, application will now close!" << std::endl;
		return -1;
	}

	//Setting up the music and sound effects for the game using SDL_Mix functions
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);	//Opens the audio device for the game, and sets the frequency, format, channels and chunksize for audio playback
	Mix_Music *music = NULL;			//opens a music function
	Mix_Chunk *explosionsound = NULL;	//opens a chunk for playback,
	Mix_Chunk *lasersound = NULL;		//of WAVE sound files
	Mix_Chunk *rocksound = NULL;

	music = Mix_LoadMUS("Game.mp3");				//uses the new music function to load an MP3
	//the music i used was 100% royalty free and fully available for use in my project.
	//Music was found on a youtube channel dedicated to creating royalty free 8-bit music
	//song used can be found here: https://www.youtube.com/watch?v=2tfi5EIj04A&list=PLBE459782E55DE0D8&index=3

	explosionsound = Mix_LoadWAV("Explosion.wav");	//uses the chunk functions,
	lasersound = Mix_LoadWAV("Laser.wav");			//to open the three WAVE files
	rocksound = Mix_LoadWAV("Rock.wav");			//for the sound effects
	//all sound effects used were 100% royalty free and from a free sound effects download website.
	//all sound effects were downloaded from this site: http://www.downloadfreesound.com/8-bit-sound-effects/

	Mix_FadeInMusic(music, -1, 2000);				//this MIX function fades in the music when the game begins

	//setting up fonts for SDL_ttf
	TTF_Font* scorefont = TTF_OpenFont("Paragon.ttf", 16);

	//Integers to define the size and location of the window
	int win_posX = 600;
	int win_posY = 200;	
	int win_width = 640;
	int win_height = 480;

	//float figures to define the start point of the ship when the game begins
	//also initialises the velocity variable
	float shipPositionX = 300;
	float shipPositionY = 220;
	float velocityX = 0;
	float velocityY = 0;

	int fire_delay = 325;
	int next_shot = 0;

	//this sets the initial position of the bullet when it will be rendered
	//which will at the front of the ships texture
	float posx = shipPositionX + 13;
	float posy = shipPositionY;
	const int bulletSpeed = 400;

	//float figures to define the start point of the scrolling text in the menu
	//also initialises the velocity variable of this text so it can be made to scroll on the x-axis
	float scrollingPositionX = 640;
	float scrollingPositionY = 415;
	float scrollingVelocityX = -75;
	float scrollingVelocityY = 0;

	//float of PI
	float PI = 3.14159265;	//float figure of PI, used when working out the ship and bullets angles of travel

	//the double number that is used for the ships angle
	double shipAngle = 0;	//double figure used in equations when working out the ships angle
							//and then used to determine the direction of travel

	//booleans used for various true or false statements
	bool gamestate = true;			//used when the game is running for the 'while' loop it is confined within
	bool menustate = true;			//used for the menu window 'while' loop, when false the menu closes and the game window opens
	bool instructions = false;		//used to determine if the player has pressed the option to view the instructions
	bool movement = false;			//used for the key presses to determine whether a movement key has been pressed
	bool shotsfired = false;		//used for the game loop to determine whether the shoot key has been pressed
	bool collision = false;			//used for the game loop to determine if a collision has occured between the ship and an asteroid
	bool loading = false;			//used for the menu loop to display a texture whilst the game loop is loading the game textures

	bool move_up, move_down, rotate_clockwise, rotate_anticlockwise;	
	move_up = move_down = rotate_clockwise = rotate_anticlockwise = false;	//these booleans are used to determine which movement is to happen

	unsigned int lastTime = SDL_GetTicks();	//SDL function to get the current ticks at this point in the program

	//THE MENU WINDOW STARTS HERE AND WILL CLOSE WHEN THE USER SELECTS AN OPTION, GAME WINDOW WILL OPEN IF USER SELECTS START

	SDL_Window *menu_window = SDL_CreateWindow("ASTEROIDS",				//this line is the title of the game window
												win_posX, win_posY,		//this line defines its position on the screen
												win_width, win_height,	//this line defines the windows width and height
												SDL_WINDOW_SHOWN);		//this line is for window characteristics, is this instance ive just requested it to be shown

	//this SDL functions creates a renderer for the game, you define the window it will be rendering within.
	SDL_Renderer *menu_renderer = SDL_CreateRenderer(menu_window, -1, 0); //this renderer is for the menu window

	//string code to represent the variable names 
	//and filenames of each bitmap we are loading
	std::string title("title.bmp");
	std::string spacemenu("spacemenu.bmp");
	std::string options("options.bmp");
	std::string shipf("shipflames.bmp");
	std::string banner("banner.bmp");
	std::string scrolling("scrolling.bmp");
	std::string controls("controls.bmp");
	std::string loadingmenu("loadingmenu.bmp");
	std::string instructionsmenu("instructions.bmp");

	//this code loads each bitmap into a temporary SDL Surface
	//which we can use to make each image a texture
	SDL_Surface *title_temp = SDL_LoadBMP(title.c_str()),
				*spacemenu_temp = SDL_LoadBMP(spacemenu.c_str()),
				*loadingmenu_temp = SDL_LoadBMP(loadingmenu.c_str()),
				*instructions_temp = SDL_LoadBMP(instructionsmenu.c_str()),
				*options_temp = SDL_LoadBMP(options.c_str()),
				*shiptitle_temp = SDL_LoadBMP(shipf.c_str()),
				*banner_temp = SDL_LoadBMP(banner.c_str()),
				*scrolling_temp = SDL_LoadBMP(scrolling.c_str()),
				*controls_temp = SDL_LoadBMP(controls.c_str());

	//this SDL code turns the SDL Surfaces into SDL Textures
	SDL_Texture * title_texture = SDL_CreateTextureFromSurface(menu_renderer, title_temp),
				*spacemenu_texture = SDL_CreateTextureFromSurface(menu_renderer, spacemenu_temp),
				*loadingmenu_texture = SDL_CreateTextureFromSurface(menu_renderer, loadingmenu_temp),
				*instructionsmenu_texture = SDL_CreateTextureFromSurface(menu_renderer, instructions_temp),
				*options_texture = SDL_CreateTextureFromSurface(menu_renderer, options_temp),
				*shiptitle_texture = SDL_CreateTextureFromSurface(menu_renderer, shiptitle_temp),
				*banner_texture = SDL_CreateTextureFromSurface(menu_renderer, banner_temp),
				*scrolling_texture = SDL_CreateTextureFromSurface(menu_renderer, scrolling_temp),
				*controls_texture = SDL_CreateTextureFromSurface(menu_renderer, controls_temp);

	//SDL command to free the temporary SDL surfaces we created
	SDL_FreeSurface(title_temp), (spacemenu_temp), (options_temp), (shiptitle_temp), (banner_temp), (scrolling_temp), (controls_temp), (loadingmenu_temp), (instructions_temp);

	//an SDL event function for any menu events that may happen in the menu loop
	SDL_Event menuevent, controlsevent;

	//this collects the current tick rate at this point, used further down for timekeeping
	lastTime = SDL_GetTicks();

	//main menu loop starts here
	while (menustate != false)
	{
		while (SDL_PollEvent(&menuevent))
		{
			switch (menuevent.type)
			{
			case SDL_QUIT:				//if a quit action is entered (pressing the 'x' in the corner of the window
				menustate == false;		//both the menustate and gamestate will be false
				gamestate == false;		//so SDL will exit
				SDL_Quit();
				return-1;
			case SDL_KEYDOWN:			//used for key events where the key is pressed in
				switch (menuevent.key.keysym.sym)
				{
				case SDLK_s:			//if the 'S' key is pressed down
					menustate = false;	//the menustate will be false and the game window will run
					loading = true;
					break;
				case SDLK_i:
					if (instructions == true) //if statement to toggle my instructions texture pooping up as an overlay
					{
						instructions = false;
					}
					else 
					{
						instructions = true;
					}
					break;
				case SDLK_ESCAPE:		//if the 'ESCAPE' key is pressed down
					SDL_Quit();			//SDL will quit
					return -1;			//and return -1
				}
			}
		}

		/*equation to work out the time ticks in the menu
		from ticks at the beginning to the ticks in each loop*/
		unsigned int current = SDL_GetTicks();
		float deltaTs = (float)(current - lastTime) / 1000.0f;
		lastTime = current;

		//this is used to make an image i created with some text in, scroll across the bottom of the screen
		scrollingPositionX = scrollingPositionX + (scrollingVelocityX * deltaTs);
		scrollingPositionY = scrollingPositionY + (scrollingVelocityY * deltaTs);

		//this means if the text has gone off the screen edge it will reappear on the opposite side
		if (scrollingPositionX < -680)
			scrollingPositionX = 645;	

		//this sets the renderers draw colour
		//and clears previous renders
		SDL_SetRenderDrawColor(menu_renderer, 0, 0, 0, 1);
		SDL_RenderClear(menu_renderer);

		//this SDL rect describes the location and size of the menu background
		SDL_Rect menuRect;
		menuRect.x = 0;
		menuRect.y = 0;
		menuRect.w = 640;
		menuRect.h = 480;
		SDL_RenderCopy(menu_renderer, spacemenu_texture, NULL, &menuRect);

		//this SDL rect describes the location of the title image
		SDL_Rect titleRect;
		titleRect.x = 0;
		titleRect.y = 0;
		titleRect.w = 640;
		titleRect.h = 480;
		SDL_RenderCopy(menu_renderer, title_texture, NULL, &titleRect);

		//this one, is the options that appear below the title
		SDL_Rect optionsRect;
		optionsRect.x = 0;
		optionsRect.y = 0;
		optionsRect.w = 640;
		optionsRect.h = 480;
		SDL_RenderCopy(menu_renderer, options_texture, NULL, &optionsRect);

		//The next two rectangles are used to have a scrolling text at the bottom of my menu
		//mainly a visual thing
		SDL_Rect bannerRect;
		bannerRect.x = 0;
		bannerRect.y = 415;
		bannerRect.w = 640;
		bannerRect.h = 40;
		SDL_RenderCopy(menu_renderer, banner_texture, NULL, &bannerRect);

		//This is the scrolling text at the bottom of the menu
		SDL_Rect scrollingRect;
		scrollingRect.x = scrollingPositionX;
		scrollingRect.y = scrollingPositionY;
		scrollingRect.w = 720;
		scrollingRect.h = 40;
		SDL_RenderCopy(menu_renderer, scrolling_texture, NULL, &scrollingRect);

		//if statements controlled by my toggle to display the instructions
		if (instructions != false)
		{
			SDL_Rect instructionsmenuRect;
			instructionsmenuRect.x = 0;
			instructionsmenuRect.y = 0;
			instructionsmenuRect.w = 640;
			instructionsmenuRect.h = 480;
			SDL_RenderCopy(menu_renderer, instructionsmenu_texture, NULL, &instructionsmenuRect);
		}
		
		//if statement to display a texture with a loading screen when the game is loading the textures required
		//more aesphetically pleasing than if the screen was just to freeze whilst textures were loaded
		if (loading != false)
		{
			SDL_Rect loadingRect;
			loadingRect.x = 0;
			loadingRect.y = 0;
			loadingRect.w = 640;
			loadingRect.h = 480;
			SDL_RenderCopy(menu_renderer, loadingmenu_texture, NULL, &loadingRect);
		}

		//this tells the 'menu_renderer' to display what it has previously just rendered in this loop cycle
		SDL_RenderPresent(menu_renderer);

		//section of code to keep the animations in time
		if (deltaTs < (1.0f / 50.0f))
		{
			SDL_Delay((unsigned int)(((1.0f / 50.0f) - deltaTs)*1000.0f));
		}
	}//menu loop ends here



	//			//			//			//			//			//			//			//			//
	//			//			//			//			//			//			//			//			//
	//			//			//			//			//			//			//			//			//



	//THE GAME WINDOW IS HERE, AND WILL OPEN WHEN THE MENU WINDOW IS CLOSED

	SDL_Window *game_window = SDL_CreateWindow("ASTEROIDS",				//this line is the title of the game window
												win_posX, win_posY,		//this line defines its position on the screen
												win_width, win_height,	//this line defines the windows width and height
												SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);	//this line is for window characteristics, is this instance ive just requested it to be shown and borderless so it fits neatly over the menu window

	//this SDL functions creates a renderer for the game, you define the window it will be rendering within. 
	SDL_Renderer *renderer = SDL_CreateRenderer(game_window, -1, 0);	//this renderer is for the game window

	//string code to represent the variable names and filenames
	//of each bitmap we are loading
	std::string space("space.bmp");
	std::string ship("ship.bmp");
	std::string largerock("asteroidlarge.bmp");
	std::string mediumrock("asteroidmedium.bmp");
	std::string smallrock("asteroidsmall.bmp");
	std::string shipflames("shipflames.bmp");
	std::string threelives("threelives.bmp");
	std::string twolives("twolives.bmp");
	std::string onelives("onelives.bmp");
	std::string explosion("explosion.bmp");
	std::string gameover("gameover.bmp");
	std::string bulletsurf("bullet.bmp");

	//this code loads each bitmap into a temporary SDL Surface
	//which we can use to make each image a texture
	SDL_Surface *space_temp = SDL_LoadBMP(space.c_str()),
				*ship_temp = SDL_LoadBMP(ship.c_str()),
				*large_temp = SDL_LoadBMP(largerock.c_str()),
				*medium_temp = SDL_LoadBMP(mediumrock.c_str()),
				*small_temp = SDL_LoadBMP(smallrock.c_str()),
				*shipf_temp = SDL_LoadBMP(shipflames.c_str()),
				*threelives_temp = SDL_LoadBMP(threelives.c_str()),
				*twolives_temp = SDL_LoadBMP(twolives.c_str()),
				*onelives_temp = SDL_LoadBMP(onelives.c_str()),
				*gameover_temp = SDL_LoadBMP(gameover.c_str()),
				*bullet_temp = SDL_LoadBMP(bulletsurf.c_str()),
				*explosion_temp = SDL_LoadBMP(explosion.c_str());

	//this SDL code turns the previous 5 SDL Surfaces into SDL Textures
	SDL_Texture *space_texture = SDL_CreateTextureFromSurface(renderer, space_temp),
				*ship_texture = SDL_CreateTextureFromSurface(renderer, ship_temp),
				*largerock_texture = SDL_CreateTextureFromSurface(renderer, large_temp),
				*mediumrock_texture = SDL_CreateTextureFromSurface(renderer, medium_temp),
				*smallrock_texture = SDL_CreateTextureFromSurface(renderer, small_temp),
				*shipf_texture = SDL_CreateTextureFromSurface(renderer, shipf_temp),
				*threelives_texture = SDL_CreateTextureFromSurface(renderer, threelives_temp),
				*twolives_texture = SDL_CreateTextureFromSurface(renderer, twolives_temp),
				*onelives_texture = SDL_CreateTextureFromSurface(renderer, onelives_temp),
				*explosion_texture = SDL_CreateTextureFromSurface(renderer, explosion_temp),
				*gameover_texture = SDL_CreateTextureFromSurface(renderer, gameover_temp),
				*ship_moving_texture = NULL,
				*numberoflives = NULL;

	//SDL code to free the temporary surfaces of each bitmap
	//that is now transformed into a texture by the renderer
	SDL_FreeSurface(space_temp),
				   (ship_temp),
				   (shipf_temp),
				   (threelives_temp),
				   (twolives_temp),
				   (onelives_temp),
				   (explosion_temp),
				   (gameover_temp);

	//SDL code used to name an event variable
	SDL_Event incomingevent;
	SDL_Event gameoverevent;

	//this is used alongside the rand function
	srand(time(0));

	//the following code is used to randomize the size, start position
	//and velocity of each asteroid, so that they will all have different properties.
	
	//sets the start position of the rock somewhere in the game scene
	//and the velocity of the rock between the range -150 and 300
	float rockPositionX = rand() % 640 + 1;
	float rockPositionY = 0;
	float rockVelocityX = rand() % 150 + 50;
	float rockVelocityY = rand() % 150 + 50;

	//sets the start position of the rock somewhere in the game scene
	//and the velocity of the rock between the range -150 and 300
	float rock2PositionX = rand() % 640 + 1;
	float rock2PositionY = 480;
	float rock2VelocityX = rand() % -150 + -50;
	float rock2VelocityY = rand() % -150 + -50;

	//sets the start position of the rock somewhere in the game scene
	//and the velocity of the rock between the range -150 and 300
	float rock3PositionX = rand() % 640 + 350;
	float rock3PositionY = 0;
	float rock3VelocityX = rand() % -259 + -50;
	float rock3VelocityY = rand() % -250 + -50;

	//sets the start position of the rock somewhere in the game scene
	//and the velocity of the rock between the range -150 and 300
	float rock4PositionX = rand() % 640 + 350;
	float rock4PositionY = 0;
	float rock4VelocityX = rand() % -259 + -50;
	float rock4VelocityY = rand() % -250 + -50;

	//sets the start position of the rock somewhere in the game scene
	//and the velocity of the rock between the range -150 and 300
	float rock5PositionX = rand() % 200 + 1;
	float rock5PositionY = rand() % 480 + 280;
	float rock5VelocityX = rand() % 200 + 50;
	float rock5VelocityY = rand() % 200 + 50;

	//sets the start position of the rock somewhere in the game scene
	//and the velocity of the rock between the range -150 and 300
	float rock6PositionX = rand() % 200 + 1;
	float rock6PositionY = rand() % 480 + 280;
	float rock6VelocityX = rand() % 200 + 50;
	float rock6VelocityY = rand() % 200 + 50;

	//this chooses a random angle for each rock between 0 and 10
	//this will determine how fast each rock will turn
	//each rock will turn differently
	float rockAngle = rand() % 30 + 1;
	float rock2Angle = rand() % 30 + 1;
	float rock3Angle = rand() % 30 + 1;
	float rock4Angle = rand() % 30 + 1;
	float rock5Angle = rand() % 30 + 1;
	float rock6Angle = rand() % 30 + 1;

	//this code sets the rock sizes for large, medium and small rocks
	float smallrocksize = 40;
	float mediumrocksize = 80;
	float largerocksize = 100;
	
	//this tells the ship what texture to display upon starting
	//is then changed to a movement texture if a movement button is pressed
	ship_moving_texture = ship_texture;

	//a float figure used for the amount of game lives
	int gameLives = 3;
	float score = 0.0f;

		//while the gamestate is true (i.e - game is running)
		//the main game loop starts here
	while (gamestate = true)
	{
		while (SDL_PollEvent(&incomingevent))
		{
			switch (incomingevent.type)
			{
			case SDL_QUIT:			//if a quit action is entered (pressing the 'x' in the corner of the window)
				gamestate = false;
				SDL_Quit;
				break;
				//this section determines what actions to take when the following keys are pressed down
			case SDL_KEYDOWN:
				switch (incomingevent.key.keysym.sym)
				{
				case SDLK_UP:		//up arrow
					move_up = true;					//tells 'move_up' to be true as UP has been pressed
					ship_moving_texture = shipf_texture;
					break;
				case SDLK_DOWN:		//down arrow
					move_down = true;				//tells 'move_down' to be true as DOWN has been pressed
					ship_moving_texture = shipf_texture;
					break;

				case SDLK_LEFT:		//left arrow
					rotate_anticlockwise = true;	//tells 'rotate_anticlockwise' to be true as A has been pressed
					break;
				case SDLK_RIGHT:	//right arrow
					rotate_clockwise = true;		//tells 'rotate_clockwise' to be true as D has been pressed		
					break;
				case SDLK_SPACE:	//spacebar
					shotsfired = true;				//tells the function 'shotsfired' to become true as the space key has been pressed
					break;

				case SDLK_ESCAPE:	//escape key
					SDL_DestroyWindow(game_window); //destroys window
					SDL_Quit();						//quits SDL
					return -1;						//returns -1
				}
				break;

				//this section determines what actions to take when the following keys are depressed
			case SDL_KEYUP:
				switch (incomingevent.key.keysym.sym)
				{
				case SDLK_UP:		//up arrow
					move_up = false;				//tells 'move_up' to no longer be true as UP has been depressed
					ship_moving_texture = ship_texture;
					break;
				case SDLK_DOWN:		//down arrow
					move_down = false;				//tells 'move_down' to no longer be true as DOWN has been depressed						
					ship_moving_texture = ship_texture;
					break;

				case SDLK_LEFT:		//left arrow
					rotate_anticlockwise = false;	//tells 'rotate_anticlockwise' to no longer be true as A has been depressed
					break;
				case SDLK_RIGHT:	//right arrow
					rotate_clockwise = false;		//tells 'rotate_clockwise' to no longer be true as D has been depressed
					break;

				case SDLK_SPACE:	//spacebar
					shotsfired = false;				//tells the function 'shotsfired' to become false as the space key has been depressed
					break;
				}
				break;
			}
			break;
		}

		/*equation to work out the time ticks in game
		from ticks at the beginning to the ticks in each loop*/
		unsigned int current = SDL_GetTicks();
		float deltaTs = (float)(current - lastTime) / 1000.0f;
		lastTime = current;

		//if statements to control the actions of the key presses
		float shipthrust = 0;

		if (move_up &!move_down)	//if the up arrow was pressed and not the down
		{
			shipthrust = -900.0f;		//changes the thrust by negative 3
		}
		if (move_down &!move_up)	//if the down arrow was pressed and not the up
		{
			shipthrust = 900.0f;		//changes the thrust by positive 300
		}

		if (rotate_clockwise &!rotate_anticlockwise)	//if the key to rotate clockwise was pressed and anticlockwise was not
		{
			shipAngle += 4;								//the ships andgle is increased by 4
		}
		if (rotate_anticlockwise &!rotate_clockwise)	//if the key to rotate anti clockwise was pressed and clockwise was not
		{
			shipAngle -= 4;								//the ships angle is decreased by 4
		}

		/*equation to calculate the ships direction,
		which allows for the direction of thrust*/
		float radians = shipAngle * PI / 180;

		float Forcex = -sinf(radians) * shipthrust;
		float Forcey = cosf(radians) * shipthrust;

		/*equation to calculate in the drag on the ship,
		allowing its velocity to decrease when thrust key is depressed*/
		float shipdrag = 0.35f;
		Forcex -= velocityX * shipdrag;
		Forcey -= velocityY * shipdrag;

		velocityX += Forcex * deltaTs;
		velocityY += Forcey * deltaTs;

		//position equation for the ship
		shipPositionX = shipPositionX + (velocityX * deltaTs);
		shipPositionY = shipPositionY + (velocityY * deltaTs);

		//position equations of each asteroid
		rockPositionX = rockPositionX + (rockVelocityX * deltaTs);
		rockPositionY = rockPositionY + (rockVelocityY * deltaTs);
		rock2PositionX = rock2PositionX + (rock2VelocityX * deltaTs);
		rock2PositionY = rock2PositionY + (rock2VelocityY * deltaTs);
		rock3PositionX = rock3PositionX + (rock3VelocityX * deltaTs);
		rock3PositionY = rock3PositionY + (rock3VelocityY * deltaTs);
		rock4PositionX = rock4PositionX + (rock4VelocityX * deltaTs);
		rock4PositionY = rock4PositionY + (rock4VelocityY * deltaTs);
		rock5PositionX = rock5PositionX + (rock5VelocityX * deltaTs);
		rock5PositionY = rock5PositionY + (rock5VelocityY * deltaTs);
		rock6PositionX = rock6PositionX + (rock6VelocityX * deltaTs);
		rock6PositionY = rock6PositionY + (rock6VelocityY * deltaTs);

		//angle equations for each asteroid
		rockAngle = rockAngle + (rockVelocityX * deltaTs);
		rock2Angle = rock2Angle + (rock2VelocityX * deltaTs);
		rock3Angle = rock3Angle + (rock3VelocityY * deltaTs);
		rock4Angle = rock4Angle + (rock4VelocityY * deltaTs);
		rock5Angle = rock5Angle + (rock5VelocityY * deltaTs);
		rock6Angle = rock6Angle + (rock6VelocityX * deltaTs);


		if (shipPositionX > 650)	//section of code 
			shipPositionX = -10;		//to determine
		if (shipPositionX < -10)		//whether or not
			shipPositionX = 650;	//the ship has 
		if (shipPositionY > 490)	//gone off the 
			shipPositionY = -10;		//screen edge
		if (shipPositionY < -10)
			shipPositionY = 490;

		if (rockPositionX > 660)	//section of code 
			rockPositionX = -120;	//to determine
		if (rockPositionX < -120)	//whether or not
			rockPositionX = 660;	//the asteroids
		if (rockPositionY > 500)	//has gone off the 
			rockPositionY = -120;	//screen edge
		if (rockPositionY < -120)
			rockPositionY = 500;

		if (rock2PositionX > 660)	//if the asteroids
			rock2PositionX = -120;	//or ship have passed
		if (rock2PositionX < -120)	//the boundries of the 
			rock2PositionX = 660;	//game area, they are
		if (rock2PositionY > 500)	//repositioned on the opposite
			rock2PositionY = -120;	//side on the same x or y
		if (rock2PositionY < -120)	//co-ordinate, dependent on which
			rock2PositionY = 500;	//side they exit

		if (rock3PositionX > 660)
			rock3PositionX = -120;
		if (rock3PositionX < -120)
			rock3PositionX = 660;
		if (rock3PositionY > 500)
			rock3PositionY = -120;
		if (rock3PositionY < -120)
			rock3PositionY = 500;

		if (rock4PositionX > 660)
			rock4PositionX = -120;
		if (rock4PositionX < -120)
			rock4PositionX = 660;
		if (rock4PositionY > 500)
			rock4PositionY = -120;
		if (rock4PositionY < -120)
			rock4PositionY = 500;

		if (rock5PositionX > 660)
			rock5PositionX = -120;
		if (rock5PositionX < -120)
			rock5PositionX = 660;
		if (rock5PositionY > 500)
			rock5PositionY = -120;
		if (rock5PositionY < -120)
			rock5PositionY = 500;

		if (rock6PositionX > 660)
			rock6PositionX = -120;
		if (rock6PositionX < -120)
			rock6PositionX = 660;
		if (rock6PositionY > 500)
			rock6PositionY = -120;
		if (rock6PositionY < -120)
			rock6PositionY = 500;

		//set the renderers draw colour and clears its history
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
		SDL_RenderClear(renderer);

		//if statement for when a shot has been fired, 'shots 
		if (shotsfired == true)
		{
			if (next_shot <= current) //this creates a delay between shots being fired, so you cannot continuously fire a stream of bullets
			{
				for (int i = 0; i < NUM_Bullets; i++)
				{
					if (bullets[i] == 0)
					{
						bullets[i] = createBullet(shipPositionX + (3 * sin(radians)), shipPositionY - (3 * cos(radians)), shipAngle, renderer);
						break;
					}
				}
				next_shot = current + fire_delay;
				Mix_PlayChannel(-1, lasersound, 0); //plays a laser sound when the shot is fired
			}
		}

			//renders a rectangle for the background texture to be rendered upon
			SDL_Rect spaceRect;
			spaceRect.x = 0;
			spaceRect.y = 0;
			spaceRect.w = 640;
			spaceRect.h = 480;
			SDL_RenderCopy(renderer, space_texture, NULL, &spaceRect);

			//renders a rectangle for asteroid 1
			SDL_Rect rockRect;
			rockRect.x = rockPositionX;
			rockRect.y = rockPositionY;
			rockRect.w = largerocksize;
			rockRect.h = largerocksize;
			SDL_RenderCopyEx(renderer, largerock_texture, NULL, &rockRect, rockAngle, NULL, SDL_FLIP_NONE);

			//renders a rectangle for asteroid 2
			SDL_Rect rockRect2;
			rockRect2.x = rock2PositionX;
			rockRect2.y = rock2PositionY;
			rockRect2.w = largerocksize;
			rockRect2.h = largerocksize;
			SDL_RenderCopyEx(renderer, largerock_texture, NULL, &rockRect2, rock2Angle, NULL, SDL_FLIP_NONE);

			//renders a rectangle for asteroid 3
			SDL_Rect rockRect3;
			rockRect3.x = rock3PositionX;
			rockRect3.y = rock3PositionY;
			rockRect3.w = mediumrocksize;
			rockRect3.h = mediumrocksize;
			SDL_RenderCopyEx(renderer, mediumrock_texture, NULL, &rockRect3, rock3Angle, NULL, SDL_FLIP_NONE);

			//renders a rectangle for asteroid 3
			SDL_Rect rockRect4;
			rockRect4.x = rock4PositionX;
			rockRect4.y = rock4PositionY;
			rockRect4.w = mediumrocksize;
			rockRect4.h = mediumrocksize;
			SDL_RenderCopyEx(renderer, mediumrock_texture, NULL, &rockRect4, rock3Angle, NULL, SDL_FLIP_NONE);

			//renders a rectangle for asteroid 4
			SDL_Rect rockRect5;
			rockRect5.x = rock5PositionX;
			rockRect5.y = rock5PositionY;
			rockRect5.w = smallrocksize;
			rockRect5.h = smallrocksize;
			SDL_RenderCopyEx(renderer, smallrock_texture, NULL, &rockRect5, rock5Angle, NULL, SDL_FLIP_NONE);

			//renders a rectangle for asteroid 5
			SDL_Rect rockRect6;
			rockRect6.x = rock6PositionX;
			rockRect6.y = rock6PositionY;
			rockRect6.w = smallrocksize;
			rockRect6.h = smallrocksize;
			SDL_RenderCopyEx(renderer, smallrock_texture, NULL, &rockRect6, rock6Angle, NULL, SDL_FLIP_NONE);

			//renders a rectangle for the ship texture to be rendered upon when
			//you move the ship in game, the rectangled is what is actually moved, and
			//the ship texture rendered upon it each time it is translated to a new position	
			SDL_Rect shipRect;
			shipRect.x = shipPositionX;
			shipRect.y = shipPositionY;
			shipRect.w = 40;
			shipRect.h = 40;
			SDL_RenderCopyEx(renderer, ship_moving_texture, NULL, &shipRect, shipAngle, NULL, SDL_FLIP_NONE);

			//Draws the bullet
			for (int i = 0; i < NUM_Bullets; i++)
			{
				if (bullets[i] != 0)
				{
					bullets[i]->xPos += (bulletSpeed * sinf(bullets[i]->angle*PI / 180)) * deltaTs;
					bullets[i]->yPos += (bulletSpeed * -cosf(bullets[i]->angle*PI / 180)) * deltaTs;

					bullets[i]->killTime += deltaTs;

					//if statements using the SDL_Collide function created by  Amir 'Genjix' Taaki to determine a bullet collision
					if (SDL_Collide(bullet_temp, bullets[i]->xPos, bullets[i]->yPos, large_temp, rockRect.x, rockRect.y))
					{
						Mix_PlayChannel(-1, rocksound, 0);
						rockPositionX = rand()% 640 + 1;
						rockPositionY = -120;
						rockVelocityX = rand() % 150 + -150;
						rockVelocityY = rand() % 150 + -150;
						score += 25;
						delete bullets[i];
						bullets[i] = 0;
						continue;
					}
					//if statements using the SDL_Collide function created by  Amir 'Genjix' Taaki to determine a bullet collision
					if (SDL_Collide(bullet_temp, bullets[i]->xPos, bullets[i]->yPos, large_temp, rockRect2.x, rockRect2.y))
					{
						Mix_PlayChannel(-1, rocksound, 0);
						rock2PositionX = rand() % 640 + 1;
						rock2PositionY = -120;
						rock2VelocityX = rand() % 150 + -150;
						rock2VelocityY = rand() % 150 + -150;
						score += 25;
						delete bullets[i];
						bullets[i] = 0;
						continue;
					}
					//if statements using the SDL_Collide function created by  Amir 'Genjix' Taaki to determine a bullet collision
					if (SDL_Collide(bullet_temp, bullets[i]->xPos, bullets[i]->yPos, medium_temp, rockRect3.x, rockRect3.y))
					{
						Mix_PlayChannel(-1, rocksound, 0);
						rock3PositionX = -120;
						rock3PositionY = rand() % 480 + 1;
						rock3VelocityX = rand() % 250 + -250;
						rock3VelocityY = rand() % 250 + -250;
						score += 50;
						delete bullets[i];
						bullets[i] = 0;
						continue;
					}
					//if statements using the SDL_Collide function created by  Amir 'Genjix' Taaki to determine a bullet collision
					if (SDL_Collide(bullet_temp, bullets[i]->xPos, bullets[i]->yPos, small_temp, rockRect4.x, rockRect4.y))
					{
						Mix_PlayChannel(-1, rocksound, 0);
						rock4PositionX = -120;
						rock4PositionY = rand() % 480 + 1;
						rock4VelocityX = rand() % 180 + -180;
						rock4VelocityY = rand() % 180 + -180;
						score += 100;
						delete bullets[i];
						bullets[i] = 0;
						continue;
					}
					//if statements using the SDL_Collide function created by  Amir 'Genjix' Taaki to determine a bullet collision
					if (SDL_Collide(bullet_temp, bullets[i]->xPos, bullets[i]->yPos, small_temp, rockRect5.x, rockRect5.y))
					{
						Mix_PlayChannel(-1, rocksound, 0);
						rock5PositionX = -120;
						rock5PositionY = rand() % 480 + 1;
						rock5VelocityX = rand() % 180 + -180;
						rock5VelocityY = rand() % 180 + -180;
						score += 100;
						delete bullets[i];
						bullets[i] = 0;
						continue;
					}
					//if statements using the SDL_Collide function created by  Amir 'Genjix' Taaki to determine a bullet collision
					if (SDL_Collide(bullet_temp, bullets[i]->xPos, bullets[i]->yPos, small_temp, rockRect6.x, rockRect6.y))
					{
						Mix_PlayChannel(-1, rocksound, 0);
						rock6PositionX = -120;
						rock6PositionY = rand() % 480 + 1;
						rock6VelocityX = rand() % 180 + -180;
						rock6VelocityY = rand() % 180 + -180;
						score += 100;
						delete bullets[i];
						bullets[i] = 0;
						continue;
					}
				}

				if (bullets[i] != 0)
				{
					if (bullets[i]->killTime > 0.5f) //this kills the bullet if it has been on the screen for longer than half a second
					{
						delete bullets[i];	//deletes the bullet from the array
						bullets[i] = 0;
					}
					else
					{
						bullets[i]->drawBullet(bullets[i]->xPos, bullets[i]->yPos, renderer, bullets[i]->angle, NULL);
					}
				}
			}

			//The code here tells the game what to do if the ship collides with an asteroid
			//In this instance the ship is reset back to the centre and a life is lost
			if ((shipcollision(&shipRect, &rockRect)) || (shipcollision2(&shipRect, &rockRect2)) || (shipcollision3(&shipRect, &rockRect3)) || (shipcollision4(&shipRect, &rockRect4)) || (shipcollision5(&shipRect, &rockRect5)) || (shipcollision6(&shipRect, &rockRect6)))
			{
				Mix_PlayChannel(-1, explosionsound, 0);
				SDL_Rect shipRect;
				shipRect.x = shipPositionX;
				shipRect.y = shipPositionY;
				shipRect.w = 40;
				shipRect.h = 40;
				SDL_RenderCopyEx(renderer, explosion_texture, NULL, &shipRect, shipAngle, NULL, SDL_FLIP_NONE);
				SDL_RenderPresent(renderer);
				shipPositionX = 300;
				shipPositionY = 220;
				velocityX = 0;
				velocityY = 0;
				shipthrust = 0;
				Forcex = 0;
				Forcey = 0;
				shipAngle = 0;
				gameLives -= 1;
				rockPositionX = rand() % 20 + 1;
				rock2PositionX = rand() % 640 + 620;
				rock3PositionX = rand() % 20 + 1;
				rock4PositionX = rand() % 640 + 620;
				rock5PositionX = rand() % 640 + 620;
				rockPositionY = rand() % 20 + 1;
				rock2PositionY = rand() % 20 + 1;
				rock3PositionY = rand() % 480 + 460;
				rock4PositionY = rand() % 480 + 460;
				rock5PositionY = rand() % 480 + 460;
				SDL_Delay(100);
			}

			std::string scoreString = std::to_string(static_cast<long long>(score));
			SDL_Color scorecolor = { 255, 255, 255 };
			SDL_Surface* scoresurface = TTF_RenderText_Solid(scorefont,scoreString.c_str(), scorecolor);
			SDL_Texture* scoretexture = SDL_CreateTextureFromSurface(renderer, scoresurface);

			SDL_Rect scoreRect;
			scoreRect.x = 580;
			scoreRect.y = 10;
			scoreRect.w = 40;
			scoreRect.h = 30;
			SDL_RenderCopy(renderer, scoretexture, NULL, &scoreRect);

			SDL_FreeSurface(scoresurface);

			//displays the amount of lives in the corner of the screen, the texture is modified by a switch statement
			SDL_Rect livesRect;
			livesRect.x = 30;
			livesRect.y = -15;
			livesRect.w = 85;
			livesRect.h = 64;
			SDL_RenderCopy(renderer, numberoflives, NULL, &livesRect);

			//the switch statement here, uses a null SDL_Texture to modify the 
			//the texture of the livesRect above.
			switch (gameLives)
			{
			case 3:
				numberoflives = threelives_texture; // if the gameLives integer is three, the null texture is replaced by a threelives texture
				break;
			case 2:
				numberoflives = twolives_texture;	// if the gameLives integer is two, the null texture is replaced by a twolives texture
				break;
			case 1:
				numberoflives = onelives_texture;	// if the gameLives integer is one, the null texture is replaced by a onelives texture
				break;
			case 0:
				numberoflives = NULL;				//if the lives are zero, the texture is reverted back to null, and the game over screen displays
				Mix_FadeOutMusic(200);				//music is faded out
				rockVelocityX = 0;		//all the asteroid
				rockVelocityY = 0;		//velocities are
				rock2VelocityX = 0;		//reverted back
				rock2VelocityY = 0;		//to zero so that
				rock3VelocityX = 0;		//they no longer
				rock3VelocityY = 0;		//move around possibly 
				rock4VelocityX = 0;		//causing collision
				rock4VelocityY = 0;		//with the ship
				rock5VelocityX = 0;
				rock5VelocityY = 0;
				
				SDL_Rect gameover;		//and a gameover screen is overlayed
				gameover.x = 0;
				gameover.y = 0;
				gameover.w = 640;
				gameover.h = 480;
				SDL_RenderCopy(renderer, gameover_texture, NULL, &gameover);

				SDL_Rect score2Rect;
				score2Rect.x = 360;
				score2Rect.y = 365;
				score2Rect.w = 40;
				score2Rect.h = 37;
				SDL_RenderCopy(renderer, scoretexture, NULL, &score2Rect);
			}

			//this SDL command tells the renderer to display everything to the game window
			SDL_RenderPresent(renderer);

			//small piece of code to keep everything in a constant time
			if (deltaTs < (1.0f / 50.0f))
			{
				SDL_Delay((unsigned int)(((1.0f / 50.0f) - deltaTs)*1000.0f));
			}
			//referenced from the worksheets and example code on myBU

		}
	

	
	//code here destroys the game window cleanly
	SDL_DestroyWindow(game_window);
	//exits SDL
	SDL_QUIT;
	//and returns 0
	return 0;
}