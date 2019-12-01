#include "playerManager.h"
#define GCD_COUNT 60
using namespace std;

PlayerManager::PlayerManager( int count ){
    
	srand(time(NULL));
    goat_dmg = 0;
    
	player_count = count;
	for( int i = 0; i < count; i++ ){
		Player* player = new Player(true);
		players.push_back(player);
	}
    
    lastFiftyAverage.push_back(0);
}

void PlayerManager::run( int gcds ){

	//play gcds and collect stats for all players
	for( int i = 0; i < player_count; i++ ){
		for( int j = 0; j < gcds; j++ ){
			players[i] -> playGCD(j);
		}
		player_damage.push_back(players[i] -> getDamage());
	}
}

int PlayerManager::findBestPlayer(){
/*  returns the index of the player with the highest
	fitness value */
	
	int best_player = 0;

	for( int i = 0; i < player_count; i++ ){
		if( player_damage[i] >= player_damage[best_player] ){
			best_player = i;
		}
	}

	return best_player;
}

void PlayerManager::mate(){
/*  find the top player, then get their abilityPriority
	vector. Change 20% of every other players aP vector
	to be the same. */

	//grabbing info from the best player
	int best_player = findBestPlayer();
	bestPlayerDamage.push_back(player_damage[best_player]);
    
    //greatest of all time check
    if( player_damage[best_player] >= goat_dmg ){
        goat_dmg = player_damage[best_player];
    }

	vector<bool> bestAbilityPriority = players[best_player] -> getAbilityPriority();
	vector<bool> bestAbilityPicker = players[best_player] -> getAbilityPicker();

	//changing the players
	for( int i = 0; i < player_count; i++ ){
		if( i != best_player ){
			vector<bool> tempPlayerAP = players[i] -> getAbilityPriority();
			vector<bool> tempPlayerPicker = players[i] -> getAbilityPicker();

            for( int j = 0; j < GCD_COUNT; j++ ){
				int chance = rand() % 100 + 0;
				if( chance < 5 ){
					tempPlayerAP[j] = bestAbilityPriority[j];
					tempPlayerPicker[j] = bestAbilityPicker[j];
				}
			}
            
			players[i] -> setAbilityPriority( tempPlayerAP );
			players[i] -> setAbilityPicker( tempPlayerPicker );
		}
        players[i] -> resetPlayer();
	}
	//reset important info
	player_damage.erase(player_damage.begin(), player_damage.end());
}

void PlayerManager::dumpPlayerDamage(){
	for( int i = 0; i < player_count; i++ ){
		cout << player_damage[i] << " ";
	}
	cout << endl;
}

double PlayerManager::displayAverageDamage( int generation ){
	
	int total_damage = 0;
	for( int i = 0; i < player_count; i++ ){
		total_damage += player_damage[i];
	}

	double average = (double) total_damage/player_count;
	cout << "Generation " << generation << " average damage = " << average << endl;
	
	//dump best player rotation
	int ind = findBestPlayer();
    if( player_damage[ind] >= goat_dmg ){
        goat_dmg = player_damage[ind];
        players[ind] -> dumpPlayerRotation(generation, goat_dmg );
    }
    averageDamage.push_back(average);
	return average;
}

vector<int> PlayerManager::getBestDamage(){
	return bestPlayerDamage;
}

PlayerManager::~PlayerManager(){
    for( int i = 0; i < player_count; i++ ){
        delete players[i];
    }
    players.erase(players.begin(),players.end());
}

bool PlayerManager::breakPoint( int breakDmg, int generation ){
    float total_average = 0;
    for( int i = generation; i > generation - 50; i-- ){
        total_average += averageDamage[i];
    }
    
    total_average /= 50;
    int test = total_average - lastFiftyAverage.back();
    if( test <= breakDmg ){
        return true;
    }
    lastFiftyAverage.push_back(total_average);
    return false;
}
