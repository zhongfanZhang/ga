#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

class Player{

	private:

        //gene containers
		std::vector<float> statPriority;
		std::vector<bool> abilityPriority;
		std::vector<bool> abilityPicker;
    
        //debugging variables
		std::vector<std::string> cast_log;
    
        //game mechanics variables
		int energy;
		int damage;
		int energy_regen;
		bool vigour;
		bool bleed;
		int rupture_duration;
		int rupture_combo;
		int combo_points;

        //player abilities
		void regenEnergy();
		bool mutilate();
		bool envenom();
		bool fanOfKnives();
		bool rupture();
		bool rollCrit();

	public:

		Player(bool);
    
        //main function
		void playGCD(int);

        //interaction functions
        int getDamage();
		void setAbilityPriority( std::vector<bool> );
		void setAbilityPicker( std::vector<bool> );
		void resetPlayer();
        
        //debugging functions
        std::vector<bool> getAbilityPriority();
        std::vector<bool> getAbilityPicker();
        std::vector<float> getStats();
		void dumpPlayerRotation(int, int);
};

#endif
