#include "player.h"
#define STAT 25
#define GCD_COUNT 60
#define VERS statPriority[1]
using namespace std;

Player::Player( bool vigour ){

	if( vigour == true ){
		energy = 170;
		this -> vigour = true;
	}else{
		energy = 100;
		this -> vigour = false;
	}

	//randomise some values for the stats
	float haste = rand() % 100 + 0;
	float vers = rand() % 100 + 0;
	float crit = rand() % 100 + 0;
	float mast = rand() % 100 + 0;

	float temp_haste = STAT;//((double) haste/(haste + vers + crit + mast)) * 100;
	float temp_vers = STAT;//((double) vers/(haste + vers + crit + mast)) * 100;
	float temp_crit = STAT;//((double) crit/(haste + vers + crit + mast)) * 100;
	float temp_mast = STAT;//((double) mast/(haste + vers + crit + mast)) * 100;
	
    temp_vers = 1.0 + (temp_vers / 200);
    
	statPriority.push_back(temp_haste);
	statPriority.push_back(temp_vers);
	statPriority.push_back(temp_crit);
	statPriority.push_back(temp_mast);

	//calculate energy regen
	energy_regen = 10 * (1 + (haste/100));
	
	//for vigour
	if( vigour == true ){
		energy_regen = energy_regen + (energy_regen/10);
	}

	//pick ability type
	for( int i = 0; i < GCD_COUNT; i++ ){
		int picker = rand() % 100 + 0;
		if( picker < 50 ){
			abilityPriority.push_back(true);	
		}else{
			abilityPriority.push_back(false);
		}
	}

	//pick 1 of 2 abilities
	for( int i = 0; i < GCD_COUNT; i++ ){
		int picker = rand() % 100 + 0;
		if( picker < 50 ){
			abilityPicker.push_back(true);
		}else{
			abilityPicker.push_back(false);
		}
	}

	bleed = false;
	combo_points = 0;
	rupture_duration = 0;
	rupture_combo = 0;
	damage = 0;
}

void Player::regenEnergy(){

    if( bleed ){
        cast_log.push_back("regening 7 extra energy from venomous wounds!\n");
        energy += 7;
    }
    
	energy += energy_regen;
	
	if( combo_points > 5 ){
		combo_points = 5;
	}
}

bool Player::rollCrit(){
	int crit_roll = rand() % 100 + 0;
	if( crit_roll <= statPriority[2] ){
		return true;
	}else{
		return false;
	}
}

bool Player::mutilate(){

	string msg;

	if( energy >= 50 ){
        //CAST SUCCESS
		msg = "energy = " + to_string(energy) + " , cast: Mutilate dealing ";
		energy -= 50;
        
		if( rollCrit() ){
			combo_points += 4;
            int dmg = 100 * VERS;
			damage += dmg;
			msg += to_string(dmg) + " damage!(CRIT), combo points = "+ to_string(combo_points) + "\n";
		}else{
			combo_points += 2;
            int dmg = 50 * VERS;
			damage += dmg;
			msg += to_string(dmg) + " damage!, combo points = " + to_string(combo_points) + "\n";
		}
        
		cast_log.push_back(msg);
		return true;
	}else{
        //CAST FAILED
		msg = "energy = " + to_string(energy) + ", cast: Mutilate failed! - Insufficient energy\n";
		cast_log.push_back(msg);
		return false;
	}
}

bool Player::fanOfKnives(){

	string msg;

	if( energy >= 35 ){
        //CAST SUCCESS
		msg = "energy = " + to_string(energy) + ", cast: Fan of knives dealing ";
		energy -= 35;
        
		if( rollCrit() ){
			combo_points += 2;
            int dmg = 70 * VERS;
			damage += dmg;
			msg += to_string(dmg) + " damage!(CRIT), combo points = " + to_string(combo_points) + "\n";
		}else{
			combo_points += 1;
            int dmg = 35 * VERS;
            damage += dmg;
			msg += to_string(dmg) + " damage!, combo points = " + to_string(combo_points) + "\n";
		}
        
        cast_log.push_back(msg);
		return true;
	}else{
        //CAST FAILED
		msg = "energy = " + to_string(energy) + ", cast: Fan of knives failed! - Insufficient energy\n";
		cast_log.push_back(msg);
		return false;
	}
}

bool Player::envenom(){

	string msg;

	if( energy >= 30 && combo_points >= 1 ){
        //CAST SUCCESS
		msg = "energy = " + to_string(energy) + " , cast: Envenom with combo points = " + to_string(combo_points) + ", dealing ";
		energy -= 30;
        
		if( rollCrit() ){
			int dmg = ((50 + (50 * combo_points)) * 2) * VERS;
			damage += dmg;
			msg += to_string(dmg) + " damage!(CRIT)\n";
		}else{
			int dmg = (50 + (50 * combo_points)) * VERS;
			damage += dmg;
			msg += to_string(dmg) + " damage!\n";
		}
        
		cast_log.push_back(msg);
		combo_points = 0;
		return true;
	}else{
        //CAST FAILED
		msg = "energy = " + to_string(energy) + ", combo points = " + to_string(combo_points) + " cast: Envenom failed!\n";
		cast_log.push_back(msg);
		return false;
	}
}

bool Player::rupture(){

	string msg;

	if( energy >= 25 && combo_points >= 1 ){
        //CAST SUCCESS
		msg = "energy = " + to_string(energy) + ", combo points = " + to_string(combo_points) + ", cast: Rupture successful!\n";
		energy -= 25;
        bleed = true;
        
		if( rupture_duration < 5 ){
			rupture_duration += 4 + (combo_points * 4);
			rupture_combo = combo_points;
		}else{
			rupture_duration = 4 + (combo_points * 4);
			rupture_combo = combo_points;
		}
        
		cast_log.push_back(msg);
		combo_points = 0;
		return true;
	}else{
        //CAST FAILED
		msg = "energy = " + to_string(energy) + ", combo points = " + to_string(combo_points) +  ", cast:rupture failed!\n";
		cast_log.push_back(msg);
		return false;
	}
}

void Player::playGCD( int timer ){

	//energy regen calculation
    this -> regenEnergy();
    if( this -> energy > 170 ){
        energy = 170;
    }
    
    cast_log.push_back("GCD " + to_string(timer) + "\n");
    
	//check ability priority
	if( this -> abilityPriority[timer] == true ){
		//generator case
        bool temp = abilityPicker[timer]? mutilate() : fanOfKnives();
	}else{
		//finisher case
        bool temp = abilityPicker[timer]? envenom() : rupture();
	}

	//rupture damage calculation
	if( rupture_duration >= 1 ){
        int dmg = (5 + rupture_combo) * VERS;
		damage += dmg;
        string msg =  "rupture has " + to_string(rupture_duration) + " GCDS left, doing " + to_string(dmg) +
        " damage per GCD\n";
        cast_log.push_back(msg);
		rupture_duration--;
    }else{
        bleed = false;
        rupture_combo = 0;
    }
}

int Player::getDamage(){
	return this -> damage;
}

vector<float> Player::getStats(){
	return statPriority;
}

vector<bool> Player::getAbilityPriority(){
	return abilityPriority;
}

vector<bool> Player::getAbilityPicker(){
	return abilityPicker;
}

void Player::setAbilityPriority( vector<bool> newPriority ){
	abilityPriority = newPriority;
}

void Player::setAbilityPicker( vector<bool> newPicker ){
	this -> abilityPicker = newPicker;
}

void Player::resetPlayer(){
	damage = 0;
	energy = 170;
	bleed = false;
	rupture_duration = 0;
	combo_points = 0;
	rupture_combo = 0;
    cast_log.erase(cast_log.begin(),cast_log.end());
}


void Player::dumpPlayerRotation( int generation, int d ){

	ofstream file;
    string filename = "player_rotations/" + to_string(generation) + "_rotation.txt";
	file.open(filename);
    
    file << "Player is from generation " << generation << " and did " << d << " damage\n\n";
    
	for( int i = 0; i < cast_log.size(); i++ ){
        file << cast_log[i] << endl;
	}
    
    file.close();

}
