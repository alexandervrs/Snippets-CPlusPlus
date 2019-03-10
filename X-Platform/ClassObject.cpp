
/* -----------------------------------------
   Libraries
----------------------------------------- */

// None


/* -----------------------------------------
   Unicode
----------------------------------------- */

#ifndef UNICODE
	#undef _UNICODE
#else
	#ifndef _UNICODE
		#define _UNICODE
	#endif
#endif

/* -----------------------------------------
   Includes
----------------------------------------- */

#include <string>
#include <iostream>


class player
{
	// public scope
	public:
		
		int x, y, health, lives;
		bool visible;
	
		//constructor
		player()
		{
			// object properties
			x      = 32;
			y      = 64;
			health = 100;
			lives  = 4;
			
			std::cout << "!! Player Created !!" << std::endl;
		};
		
		//destructor
		~player(){
			std::cout << "!! Player Destroyed !!" << std::endl;
		}; 

		int setHealth(int amount)
		{
			health = amount;
			return health;
		}

		int addHealth(int amount)
		{
			health = health + amount;
			return health;
		}

		int subHealth(int amount)
		{
			health = health - amount;
			return health;
		}

		int getHealth()
		{
			return health;
		}
	
	

	// private scope
	private: 
		
	
	// protected scope
	protected: 
	
};

int main()
{
	
	// application start
	std::cout << "--------------/ Start \\-----------------" << std::endl << std::endl;
	
	// application run
	player* player1 = new player(); // create new object
	std::cout << "Created player object with ID " << &player1 << std::endl;
	std::cout << "Default player health is: " << player1->getHealth() << std::endl;
	
	std::cout << "Setting player health to 20" << std::endl;
	player1->setHealth(20); // setter
	
	int phealth = player1->getHealth(); // getter
	std::cout << "Getting player health: " << phealth << std::endl;
	
	delete player1; // destroy
	
	// application end
	std::cout << std::endl << "--------------/ Finish \\-----------------" << std::endl;
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
	
	return 0;
	
}