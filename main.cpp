#include <iostream>
#include <vector>
#include "player.h"
#include "playerManager.h"
#define GCD_COUNT 60
using namespace std;

int main(){

	vector<double> damage;

	//100 players for 1000 generations
	PlayerManager glados(100);
	for( int i = 0; i < 1000; i++ ){
        if( (i+1)% 50 == 0 ){
            if( glados.breakPoint(10,i) ){
                cout << "convergence reached, terminating loop\n";
                break;
            };
        }
		glados.run(GCD_COUNT);
		damage.push_back( glados.displayAverageDamage(i));
		glados.mate();
	}

	double top_dmg = 0;
	int top_dmg_gen = 0;
	for( int i = 0; i < damage.size(); i++ ){
		if( damage[i] > top_dmg ){
			top_dmg = damage[i];
			top_dmg_gen = i;
		}
	}

	cout << "The top average damage is generation: " << top_dmg_gen << " with average damage = " << top_dmg << "!\n";
	
	vector<int> top_damage = glados.getBestDamage();

	int top_player_gen = 0;
	int top_player_damage = 0;
	for( int i = 0; i < top_damage.size(); i++ ){
		if( top_damage[i] >= top_player_damage ){
			top_player_damage = top_damage[i];
			top_player_gen = i;
		}
	}

	cout << "The best player of all time is from generation: " << top_player_gen << " and dealt: " << top_player_damage << " damage!\n";
    
}
