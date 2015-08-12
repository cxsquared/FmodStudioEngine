#include "main.h"

CAudioEngine aEngine;

void GetMonsterInfo(ifstream &fin, tRoom &room, string strMonsterName) {
	string strLine = "";

	fin.seekg(NULL,ios::beg);
	fin.clear();

	room.monster.SetName(strMonsterName);

	while(getline(fin, strLine, '\n')) {
		if (strLine == "<" + room.strCurrentRoom + "|" + strMonsterName + ">") {
			int data = 0;
			fin >> strLine >> data;
			room.monster.SetHealth(data);
			
			fin >> strLine >> data;
			room.monster.SetDamage(data);

			fin >> strLine;
			getline(fin,strLine, '*');
			room.monster.SetAttackMessage(strLine);

			return;
		}
	}
}

void GetPlayerInfo(ifstream &fin, CPlayer &player) {
	string strWord;
	int data = 0;

	fin.seekg(NULL, ios::beg);
	fin.clear();

	fin >> strWord >> strWord;
	player.SetName(strWord);

	fin >> strWord >> data;
	player.SetHealth(data);

	fin >> strWord >> strWord;
	player.SetWeapon(strWord);

	fin >> strWord >> data;
	player.SetDamage(data);
}

void DisplayPlayer(const CPlayer &player) {
	cout << "Name: " << player.GetName() << endl;
	cout << "Health: " << player.GetHealth() << endl;
	cout << "Weapon: " << player.GetWeapon() << endl;
	cout << "Damage: " << player.GetDamage() << endl;
}

bool CheckLook(tRoom &room, string strLook) {
	for (int i = 0; i < MAX_LOOKS; i++){
		if (strLook == room.strLookArray[i])
			return true;
	}

	return false;
}

void DisplayLook(string strLookDescription) {
	cout << endl << strLookDescription << endl;
}

void GetLookInfo(ifstream &fin, tRoom &room, string strLook) {
	string strLine = "";

	fin.seekg(NULL, ios::beg);
	fin.clear();

	while(getline(fin, strLine, '\n')) {
		if (strLine == "<" + room.strCurrentRoom + "|" + strLook + ">") {
			getline(fin, room.strLookDescription, '*');

			return;
		}
	}
}

void DisplayRoom(tRoom &room) {
	cout << room.strRoomDescription << endl << endl;
}

void GetRoomInfo(ifstream &fin, tRoom &room) {
	string strLine = "";
	string strTemp = "";

	string strRoom = "<" + room.strCurrentRoom + ">";

	fin.seekg(NULL, ios::beg);
	fin.clear();

	while(getline(fin, strLine, '\n')) {
		if(strLine == strRoom) {
			getline(fin, room.strRoomDescription, '*');

			fin >> strTemp >> room.strRoomNorth;
			fin >> strTemp >> room.strRoomEast;
			fin >> strTemp >> room.strRoomSouth;
			fin >> strTemp >> room.strRoomWest;

			int count = 0, i = 0;

			fin >> strTemp >> count;

			for (i = 0; i < MAX_LOOKS; i++) {
				room.strLookArray[i] = "";
			}

			for (i = 0; i < count % MAX_LOOKS; i++) {
				fin >> room.strLookArray[i];
			}

			fin >> strTemp >> strTemp;

			if (strTemp == "None") {
				room.bMonsterInRoom = false;
				return;
			}

			room.bMonsterInRoom = true;

			GetMonsterInfo(fin, room, strTemp);

			return;
		}
	}
}

void Move(ifstream &fin, tRoom &room, string strRoom) {
	if (strRoom == "None"){
		cout << "You can't go that way!" << endl;
		return;
	}

	room.strCurrentRoom = strRoom;

	GetRoomInfo(fin, room);

	DisplayRoom(room);
}

int GetInput(ifstream &fin, tRoom &room, CPlayer &player) {
	string strInput = "";

	cout << endl << ": ";

	cin >> strInput;

	if (strInput == "look" || strInput == "l") {
		DisplayRoom(room);
	} else if (strInput == "view" || strInput == "v") {
		cout << "What do you want to look at? ";
		cin >> strInput;

		if (CheckLook(room, strInput)) {
			GetLookInfo(fin, room, strInput);
			DisplayLook(room.strLookDescription);
		} else {
			cout << "I don't see anything like that..." << endl;
		}
	} else if (strInput == "status") {
		DisplayPlayer(player);
	} else if (strInput == "north" || strInput == "n") {
		aEngine.PlaySounds("res/footsteps.wav", Vector3{ 0, 0, 0 }, aEngine.VolumeTodB(1.0f));
		Move(fin, room, room.strRoomNorth);
	} else if (strInput == "east" || strInput == "e") {
		Move(fin, room, room.strRoomEast);
	} else if (strInput == "south" || strInput == "s") {
		Move(fin, room, room.strRoomSouth);
	} else if (strInput == "west" || strInput == "w") {
		Move(fin, room, room.strRoomWest);
	} else if (strInput == "quit") {
		cout << "Did you give up already?" << endl;
		return QUIT;
	} else if (strInput == "help" || strInput == "?") {
		cout << endl << "Commands: look north south east weat help quit" << endl;
	} else {
		cout << endl << "Huh??" << endl;
	}

	return STILL_PLAYING;
}

int AttackPlayer(tRoom &room, CPlayer &player, const string &strEvent) {
	if (room.bMonsterInRoom) {
		if (!aEngine.IsEventPlaying(strEvent)){
			aEngine.PlayEvent(strEvent);
		}
	}
	while (room.bMonsterInRoom) {
		cout << room.monster.GetAttackMessage() << endl;

		player.SetHealth(player.GetHealth() - room.monster.GetDamage());

		cout << "You now have " << player.GetHealth() << " health." << endl;

		cout << "You attack the " << room.monster.GetName()
			 << " with your " << player.GetWeapon() << " for "
			 << player.GetDamage() << " hit points!" << endl;

		room.monster.SetHealth(room.monster.GetHealth() - player.GetDamage());

		cout << "It now has " << room.monster.GetHealth() << " health." << endl;

		if (player.GetHealth() <= 0) {
			cout << "You are dead!" << endl;
			aEngine.StopEvent(strEvent, true);
			return PLAYER_IS_DEAD;
		} else if (room.monster.GetHealth() <= 0) {
			cout << "You Killed the " << room.monster.GetName() << "!" << endl;
			room.bMonsterInRoom = false;
		}

		aEngine.SetEventParameter(strEvent, "Intensity", float(player.GetHealth()) / MAX_HEALTH);

		Sleep(1500);
	}

	aEngine.StopEvent(strEvent, true);
	return PLAYER_STILL_ALIVE;
}

void AudioUpdate(CAudioEngine* engine) {
	while (1){
		engine->Update();
	}
}

int main() {
	aEngine.Init();
	aEngine.LoadBank("res/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
	aEngine.LoadBank("res/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
	aEngine.LoadEvent("event:/SwordBattle");

	aEngine.LoadSound("res/footsteps.wav", false);

	future<void> fut = async(AudioUpdate, &aEngine);

	ifstream fin;

	tRoom room;

	fin.open(GAME_FILE);

	if (fin.fail()) {
		cout << "Unable to find world file" << endl;
		Sleep(3000);
		return -1;
	}

	CPlayer player;

	GetPlayerInfo(fin, player);

	room.bMonsterInRoom = false;

	fin >> room.strCurrentRoom >> room.strCurrentRoom;

	GetRoomInfo(fin, room);

	DisplayRoom(room);

	while(STILL_PLAYING) {
		if (GetInput(fin, room, player) == QUIT)
			break;

		if (AttackPlayer(room, player, "event:/SwordBattle") == PLAYER_IS_DEAD)
			break;
	}

	fin.close();

	aEngine.Shutdown();

	Sleep(3000);
	
	return 0;
}