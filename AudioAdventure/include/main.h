#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <future>
#include "AudioEngine.h"

using namespace std;

#define GAME_FILE "res/data.lvl"

#define STILL_PLAYING 1
#define QUIT 0

#define MAX_LOOKS 3

#define PLAYER_STILL_ALIVE 1
#define PLAYER_IS_DEAD 0

#define MAX_HEALTH 275.0f

class CPlayer {
public:
	void SetName(string strPlayerName) {m_strName = strPlayerName;}
	void SetHealth(int playerHealth) {m_health = playerHealth;}
	void SetWeapon(string strPlayerWeapon) {m_strWeapon = strPlayerWeapon;}
	void SetDamage(int playerDamage) {m_damage = playerDamage;}

	string GetName() const {return m_strName;}
	string GetWeapon() const {return m_strWeapon;}

	int GetDamage() const {return m_damage;}
	int GetHealth() const {return m_health;}

private:
	string m_strName;
	int m_health;
	string m_strWeapon;
	int m_damage;
};

class CMonster {
public:
	void SetName(string strMonsterName) {m_strName = strMonsterName;}
	void SetAttackMessage(string strMessage) {m_strAttackMessage = strMessage;}
	void SetHealth(int monsterHealth) {m_health = monsterHealth;}
	void SetDamage(int monsterDamage) {m_damage = monsterDamage;}

	string GetName() const {return m_strName;}
	string GetAttackMessage() const {return m_strAttackMessage;}

	int GetHealth() const {return m_health;}
	int GetDamage() const {return m_damage;}

private:
	string m_strName;
	string m_strAttackMessage;
	int m_health;
	int m_damage;
};

struct tRoom {
	string strCurrentRoom;
	string strRoomDescription;
	string strRoomNorth;
	string strRoomEast;
	string strRoomSouth;
	string strRoomWest;
	
	string strLookArray[MAX_LOOKS];
	string strLookDescription;

	CMonster monster;
	bool bMonsterInRoom;
};

#endif