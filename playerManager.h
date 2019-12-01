#include <iostream>
#include "player.h"
#include <vector>

class PlayerManager{

	private:
		
        //internal variables
		std::vector<Player*> players;
		std::vector<int> player_damage;
        std::vector<double> averageDamage;
        std::vector<double> lastFiftyAverage;
		int player_count;
    
        //output variables
		std::vector<int> bestPlayerDamage;
        int goat_dmg;

	public:

		PlayerManager( int );
        ~PlayerManager();
    
        //main functions
		void run( int );
		void mate();
        bool breakPoint(int, int);
    
        //display/get functions
		std::vector<int> getFitness();
		int findBestPlayer();
		double displayAverageDamage(int);
		std::vector<int> getBestDamage();
    
        //debug functions
        void dumpPlayerPriorities();
        void dumpPlayerStats();
        void dumpPlayerDamage();
};
