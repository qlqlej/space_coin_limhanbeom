#include "Iocp.h"

HANDLE						h_iocp;
array<SESSION, MAX_USER>	clients;
array<MONSETR, MAX_MONSTER>	monsters;

float Distance(SESSION& a, SESSION& b)
{
	float dx = (a.GetX() / 1000 - b.GetX() / 1000) * (a.GetX() / 1000 - b.GetX() / 1000);
	float dy = (a.GetY() / 1000 - b.GetY() / 1000) * (a.GetY() / 1000 - b.GetY() / 1000);
	float dz = (a.GetZ() / 1000 - b.GetZ() / 1000) * (a.GetZ() / 1000 - b.GetZ() / 1000);
	return (dx + dy + dz) * (dx + dy + dz);
}

float Distance(SESSION& a, MONSETR& b)
{
	float dx = (a.GetX() / 1000 - b.GetX() / 1000) * (a.GetX() / 1000 - b.GetX() / 1000);
	float dy = (a.GetY() / 1000 - b.GetY() / 1000) * (a.GetY() / 1000 - b.GetY() / 1000);
	float dz = (a.GetZ() / 1000 - b.GetZ() / 1000) * (a.GetZ() / 1000 - b.GetZ() / 1000);
	return (dx + dy) * (dx + dy);
}

float Distance2(SESSION& a, MONSETR& b)
{
	float dx = (a.GetX() / 1000 - b.GetX() / 1000) * (a.GetX() / 1000 - b.GetX() / 1000);
	float dy = (a.GetY() / 1000 - b.GetY() / 1000) * (a.GetY() / 1000 - b.GetY() / 1000);
	float dz = (a.GetZ() / 1000 - b.GetZ() / 1000) * (a.GetZ() / 1000 - b.GetZ() / 1000);
	return (dx + dy) * (dx + dy);
}

bool Seeable(SESSION& a, MONSETR& b) {
	bool ret = false;
	if (b.GetX() <= a.GetX() + SEE_RANGE && b.GetX() >= a.GetX() - SEE_RANGE) {
		if (b.GetY() <= a.GetY() + SEE_RANGE && b.GetY() >= a.GetY() - SEE_RANGE) {
			ret = true;
		}
	}
	return ret;
}

char* RemoveQuotes(const char* mess)
{
	char source[BUF_SIZE];
	ZeroMemory(source, BUF_SIZE);
	strcpy_s(source, BUF_SIZE, mess);
	char ret[BUF_SIZE];
	ZeroMemory(ret, BUF_SIZE);
	int len = strlen(source);
	for (int i = 0; i < len - 2; ++i) {
		ret[i] = source[i + 1];
	}

	ret[len - 2] = '\0';
	return ret;
}

COIN_DATA SaveCoinData(COIN_DATA destination, CoinAPI resource, int num)
{
	strcpy_s(destination.coin_name, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.coin_name.at(num).c_str())));
	strcpy_s(destination.opening_price, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.opening_price.at(num).c_str())));
	strcpy_s(destination.closing_price, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.closing_price.at(num).c_str())));
	strcpy_s(destination.min_price, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.min_price.at(num).c_str())));
	strcpy_s(destination.max_price, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.max_price.at(num).c_str())));
	strcpy_s(destination.units_traded, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.units_traded.at(num).c_str())));
	strcpy_s(destination.prev_closing_price, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.prev_closing_price.at(num).c_str())));
	strcpy_s(destination.acc_trade_value, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.acc_trade_value.at(num).c_str())));
	strcpy_s(destination.units_traded_24H, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.units_traded_24H.at(num).c_str())));
	strcpy_s(destination.acc_trade_value_24H, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.acc_trade_value_24H.at(num).c_str())));
	strcpy_s(destination.fluctate_24H, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.fluctate_24H.at(num).c_str())));
	strcpy_s(destination.fluctate_rate_24H, BUF_SIZE, RemoveQuotes(const_cast<char*>(resource.fluctate_rate_24H.at(num).c_str())));

	return destination;
}

char* AddCoinDataOneValue(char destination[], COIN_DATA resource)
{
	strcat_s(destination, strlen(destination) + strlen(resource.coin_name) + 1, resource.coin_name);
	strcat_s(destination, strlen(destination) + 2, ",");
	//strcat_s(destination, strlen(destination) + strlen(resource.opening_price) + 1, resource.opening_price);
	//strcat_s(destination, strlen(destination) + 2, ",");
	strcat_s(destination, strlen(destination) + strlen(resource.closing_price) + 1, resource.closing_price);
	strcat_s(destination, strlen(destination) + 2, ",");


	strcat_s(destination, strlen(destination) + strlen(resource.min_price) + 1, resource.min_price);
	strcat_s(destination, strlen(destination) + 2, ",");
	strcat_s(destination, strlen(destination) + strlen(resource.max_price) + 1, resource.max_price);
	strcat_s(destination, strlen(destination) + 2, ",");
	//strcat_s(destination, strlen(destination) + strlen(resource.units_traded) + 1, resource.units_traded);
	//strcat_s(destination, strlen(destination) + 2, ",");
	strcat_s(destination, strlen(destination) + strlen(resource.prev_closing_price) + 1, resource.prev_closing_price);
	strcat_s(destination, strlen(destination) + 2, ",");
	//strcat_s(destination, strlen(destination) + strlen(resource.acc_trade_value) + 1, resource.acc_trade_value);
	//strcat_s(destination, strlen(destination) + 2, ",");
	strcat_s(destination, strlen(destination) + strlen(resource.units_traded_24H) + 1, resource.units_traded_24H);
	strcat_s(destination, strlen(destination) + 2, ",");


	strcat_s(destination, strlen(destination) + strlen(resource.acc_trade_value_24H) + 1, resource.acc_trade_value_24H);
	strcat_s(destination, strlen(destination) + 2, ",");
	strcat_s(destination, strlen(destination) + strlen(resource.fluctate_24H) + 1, resource.fluctate_24H);
	strcat_s(destination, strlen(destination) + 2, ",");
	strcat_s(destination, strlen(destination) + strlen(resource.fluctate_rate_24H) + 1, resource.fluctate_rate_24H);
	strcat_s(destination, strlen(destination) + 2, ",");

	return destination;
}

SERVER::SERVER()
{	
	ret = 0;
	server = 0;
	server_addr = { 0 };
	client_addr = { 0 };
	client_socket = 0;
	err_no = 0;
	addr_size = 0;
	dataBase.Initialize();
}

SERVER::~SERVER()
{

}

int SERVER::GetNewClientID()
{
	for (int i = 0; i < MAX_USER; ++i) {
		clients[i].sl.lock();		
		if (SESSION_STATE::FREE == clients[i].GetSessionState()) {
			clients[i].SetSessionState(SESSION_STATE::WAIT);
			clients[i].sl.unlock();
			return i;
		}
		clients[i].sl.unlock();
	}
	return -1;
}

void SERVER::error_display(const char* msg, int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	cout << msg;
	wcout << L"ERROR " << lpMsgBuf << endl;
	while (true);
	LocalFree(lpMsgBuf);
}

void SERVER::disconnect(int client_id)
{
	clients[client_id].sl.lock();
	if (SESSION_STATE::FREE == clients[client_id].GetSessionState()) {
		clients[client_id].sl.unlock();
		return;
	}

	dataBase.SavePlayerInfo(clients[client_id].GetID(), clients[client_id].hp, clients[client_id].a_coin, clients[client_id].b_coin, clients[client_id].c_coin,
		clients[client_id].gold, clients[client_id].GetX(), clients[client_id].GetY(), clients[client_id].GetZ(), clients[client_id].GetPitch(), clients[client_id].GetYaw(),
		clients[client_id].GetRoll(), (int)clients[client_id].GetCharacterType(), (int)clients[client_id].GetMap(), clients[client_id].critical_up, clients[client_id].speed_up,
		clients[client_id].attack_up, clients[client_id].hp_up, clients[client_id].recover_hp_up, clients[client_id].sniper_up, clients[client_id].attack_hp_up,
		clients[client_id].launcher_up, clients[client_id].launcher_range_up, clients[client_id].jump_up);

	for (auto& monster : monsters) {
		if (monster.targetID == client_id) {
			monster.targetID = -1;
		}
	}

	closesocket(clients[client_id].GetSocket());
	clients[client_id].SetSessionState(SESSION_STATE::FREE);
	clients[client_id].SetSessionID(-1);
	clients[client_id].SetSocket(0);
	clients[client_id].view_list.clear();
	clients[client_id].SetUseCheck(false);
	clients[client_id].sl.unlock();

	for (auto& client : clients) {
		if (client_id == client.GetSessionID()) continue;
		if (false == client.GetUseCheck()) continue;
		client.sl.lock();
		if (SESSION_STATE::ACTIVE != client.GetSessionState()) {
			client.sl.unlock();
			continue;
		}
		client.vl.lock();
		if (0 != client.view_list.count(client_id)) {
			client.view_list.erase(client_id);
			client.vl.unlock();
			client.SendToRemovePlayer(client_id);
		}
		else {
			client.vl.unlock();
		}
		client.sl.unlock();
	}

	cout << "[INFO - LOGOUT] client session ID [" << client_id << "] logout.\n";
}

void SERVER::analyze_packet(int client_id, char* p)
{
	switch (p[1]) {
	case CS_LOGIN:
	{
		CS_LOGIN_PACKET* packet = reinterpret_cast<CS_LOGIN_PACKET*>(p);
		SC_LOGIN_INFO_PACKET info_packet;
		bool result = dataBase.SearchAccount(packet->id, packet->pw, info_packet);
		if (true == result) {			
			// login success
			clients[client_id].InitLogin(info_packet, packet);

			// send init data to login user
			clients[client_id].SendToLoginInfo(info_packet, client_id);

			// client : streaming level 구현시 미사용 예정
			// 현재 open level 함수로 레벨 로드해서 로드중 데이터 전송 방지 위한
			// 악수 코딩
			this_thread::sleep_for(seconds(DATA_LAG));
			MAP_TYPE current_map_type = info_packet.map_type;
			clients[client_id].SetMap(info_packet.map_type);			

			// 플레이어 정보 상호전송
			//for (auto& client : clients) {
			//	if (client_id == client.GetSessionID()) continue;
			//	if (false == client.GetUseCheck()) continue;
			//	client.sl.lock();
			//	if (SESSION_STATE::ACTIVE != client.GetSessionState()) {
			//		client.sl.unlock();
			//		continue;
			//	}
			//	client.ml.lock();
			//	if (clients[client_id].GetMap() != client.GetMap()) {
			//		client.ml.unlock();
			//		client.sl.unlock();
			//		continue;
			//	}
			//	if (RANGE * RANGE >= Distance(client, clients[client_id])) {
			//		client.vl.lock();
			//		if (0 == client.view_list.count(client_id)) {
			//			client.view_list.insert(client_id);
			//			client.vl.unlock();
			//			client.SendToAddPlayer(clients[client_id]);
			//		}
			//		else {
			//			client.vl.unlock();
			//		}
			//
			//		clients[client_id].vl.lock();
			//		if (0 == client.view_list.count(client.GetSessionID())) {
			//			clients[client_id].view_list.insert(client.GetSessionID());
			//			clients[client_id].vl.unlock();
			//			clients[client_id].SendToAddPlayer(client);
			//		}
			//		else {
			//			clients[client_id].vl.unlock();
			//		}
			//	}
			//	client.ml.unlock();
			//	client.sl.unlock();
			//}
			//
			// 몬스터 정보 전송		
			for (auto& monster : monsters) {
				if (current_map_type != monster.GetMapType()) continue;
				if (MONSTER_TYPE::F_FISH != monster.GetMonsterType()) {
					if (RANGE * RANGE >= Distance(clients[client_id], monster)) {
						clients[client_id].mvl.lock();
						if (0 == clients[client_id].monster_view_list.count(monster.GetSessionID())) {
							clients[client_id].monster_view_list.insert(monster.GetSessionID());
							clients[client_id].SendToAddMonster(monster);
							clients[client_id].mvl.unlock();
						}
						else {
							clients[client_id].mvl.unlock();
						}
					}
				}
				else {
					if (RANGE * RANGE >= Distance2(clients[client_id], monster)) {
						clients[client_id].mvl.lock();
						if (0 == clients[client_id].monster_view_list.count(monster.GetSessionID())) {
							clients[client_id].monster_view_list.insert(monster.GetSessionID());
							clients[client_id].SendToAddMonster(monster);
							clients[client_id].mvl.unlock();
						}
						else {
							clients[client_id].mvl.unlock();
						}
					}
				}
			}
			clients[client_id].sl.lock();
			clients[client_id].SetSessionState(SESSION_STATE::ACTIVE);
			clients[client_id].sl.unlock();
		}
		else {			
			ALL_STATE_PACKET state_Info_packet;
			state_Info_packet.size = sizeof(ALL_STATE_PACKET);
			state_Info_packet.type = ALL_STATE;
			state_Info_packet.state = PACKET_STATE::LOGINFAIL;
			clients[client_id].sending(&state_Info_packet);
		}
		break;
	}
	case CS_SIGNUP:
	{
		CS_SIGNUP_PACKET* packet = reinterpret_cast<CS_SIGNUP_PACKET*>(p);
		bool result = dataBase.SignupAccount(packet->name, packet->id, packet->pw);
		ALL_STATE_PACKET info_packet;
		info_packet.size = sizeof(ALL_STATE_PACKET);
		info_packet.type = ALL_STATE;
		if (true == result) {
			cout << "Signup Success!\n";
			info_packet.state = PACKET_STATE::SUCCESS;
		}
		else
		{
			cout << "Signup failed!\n";
			info_packet.state = PACKET_STATE::SIGNUPFAIL;
		}
		//this_thread::sleep_for(milliseconds(16));
		clients[client_id].sending(&info_packet);
		break;
	}
	case CS_MOVE:
	{
		CS_MOVE_PACKET* packet = reinterpret_cast<CS_MOVE_PACKET*>(p);
		if (packet->sessionID < 0 || packet->sessionID > 9999) {
			cout << "[CS_MOVE] Unknown session ID\n";
			break;
		}
		//cout << client_id << endl;
		clients[client_id].SetPosition(packet->x, packet->y, packet->z);
		clients[client_id].SetRotation(packet->pitch, packet->yaw, packet->roll);
		clients[client_id].SetVelocity(packet->vx, packet->vy, packet->vz);
		clients[client_id].SetCRotation(packet->cpitch, packet->cyaw, packet->croll);
		clients[client_id].SetShipSpeed(packet->ship_speed);
		clients[client_id].hp = packet->hp;
		//clients[client_id].vl.lock();
		auto old_vl = clients[client_id].view_list;
		//clients[client_id].vl.unlock();
		clients[client_id].mvl.lock();
		auto old_mvl = clients[client_id].monster_view_list;
		clients[client_id].mvl.unlock();

		unordered_set <int> new_vl;
		for (auto& client : clients) {
			if (client_id == client.GetSessionID()) continue;
			if (false == client.GetUseCheck()) continue;
			if (SESSION_STATE::ACTIVE != client.GetSessionState()) continue;
			if (clients[client_id].GetMap() != client.GetMap()) continue;
			if (RANGE * RANGE < Distance(client, clients[client_id])) continue;
			new_vl.insert(client.GetSessionID());
		}

		unordered_set <int> new_mvl;
		for (auto& monster : monsters) {			
			if (clients[client_id].GetMap() != monster.GetMapType()) continue;
			if (false != monster.death) continue;
			if (MONSTER_TYPE::F_FISH != monster.GetMonsterType()) {
				if (RANGE * RANGE < Distance(clients[client_id], monster)) continue;
				new_mvl.insert(monster.GetSessionID());
			}
			else {
				if (RANGE * RANGE < Distance2(clients[client_id], monster)) continue;
				new_mvl.insert(monster.GetSessionID());
			}
		}

		for (auto clientID : new_vl) {
			if (0 == old_vl.count(clientID)) {
				clients[client_id].vl.lock();
				clients[client_id].view_list.insert(clientID);
				clients[client_id].SendToAddPlayer(clients[clientID]);
				clients[client_id].vl.unlock();
			}

			clients[clientID].vl.lock();
			if (0 == clients[clientID].view_list.count(client_id)) {
				clients[clientID].view_list.insert(client_id);
				clients[clientID].SendToAddPlayer(clients[client_id]);
				clients[clientID].vl.unlock();
			}
			else {
				clients[clientID].SendToMovePlayer(clients[client_id]);
				clients[clientID].vl.unlock();
			}
		}

		for (auto clientID : old_vl) {
			if (0 == new_vl.count(clientID)) {
				clients[client_id].vl.lock();
				clients[client_id].view_list.erase(clientID);
				clients[client_id].SendToRemovePlayer(clientID);
				clients[client_id].vl.unlock();

				clients[clientID].vl.lock();
				if (0 != clients[clientID].view_list.count(client_id)) {
					clients[clientID].view_list.erase(client_id);
					clients[clientID].SendToRemovePlayer(client_id);
					clients[clientID].vl.unlock();
				}
			}
		}

		for (auto monsterID : new_mvl) {
			if (0 == old_mvl.count(monsterID)) {
				clients[client_id].mvl.lock();
				clients[client_id].monster_view_list.insert(monsterID);
				clients[client_id].SendToAddMonster(monsters[monsterID]);
				clients[client_id].mvl.unlock();
			}
		}

		for (auto monsterID : old_mvl) {
			if (0 == new_mvl.count(monsterID)) {
				clients[client_id].mvl.lock();
				clients[client_id].monster_view_list.erase(monsterID);
				clients[client_id].SendToRemoveMonster(monsterID);
				clients[client_id].mvl.unlock();
			}
		}

		break;
	}
	case CS_CHARACTER_CHANGE:
	{
		CS_CHANGE_CHARACTER_PACKET* packet = reinterpret_cast<CS_CHANGE_CHARACTER_PACKET*>(p);
		if (packet->sessionID < 0 || packet->sessionID > 9999) {
			cout << "[CS_CHARACTER_CHANGE] Unknown session ID\n";
			break;
		}
		clients[client_id].SetCharacterType(packet->character_type);

		clients[client_id].vl.lock();
		auto cur_vl = clients[client_id].view_list;
		clients[client_id].vl.unlock();

		for (auto clientID : cur_vl) {
			clients[clientID].SendToChangeCharacter(clients[client_id]);
		}
		break;
	}
	case CS_WEAPON_CHANGE:
	{
		CS_CHANGE_CHARACTER_PACKET* packet = reinterpret_cast<CS_CHANGE_CHARACTER_PACKET*>(p);
		if (packet->sessionID < 0 || packet->sessionID > 9999) {
			cout << "[CS_WEAPON_CHANGE] Unknown session ID\n";
			break;
		}
		clients[client_id].SetWeaponType(packet->weapon_type);

		clients[client_id].vl.lock();
		auto cur_vl = clients[client_id].view_list;
		clients[client_id].vl.unlock();

		for (auto clientID : cur_vl) {
			clients[clientID].SendToChangeWeapon(clients[client_id]);
		}
		break;
	}
	case CS_ATTACK_SKILL:
	{
		CS_ATTACK_SKILL_PACKET* packet = reinterpret_cast<CS_ATTACK_SKILL_PACKET*>(p);		
		if (packet->sessionID < 0 || packet->sessionID > 9999) {
			cout << "[CS_ATTACK_SKILL] Unknown session ID\n";
			break;
		}

		if (ATTACK_SKILL_TYPE::DEFENSE == packet->attack_skill_type) {
			clients[packet->sessionID].shield_time = system_clock::now();
		}

		clients[client_id].vl.lock();
		auto cur_vl = clients[client_id].view_list;
		clients[client_id].vl.unlock();

		for (auto clientID : cur_vl) {
			clients[clientID].vl.lock();
			if (0 != clients[clientID].view_list.count(client_id)) {
				clients[clientID].SendToAttackSkill(packet);
			}
			clients[clientID].vl.unlock();
		}
		break;
	}
	case CS_CHANGE_MAP:
	{
		CS_CHANGE_MAP_PACKET* packet = reinterpret_cast<CS_CHANGE_MAP_PACKET*>(p);
		if (packet->sessionID < 0 || packet->sessionID > 9999) {
			cout << "[CS_CHANGE_MAP] Unknown session ID\n";
			break;
		}
		
		clients[client_id].sl.lock();
		clients[client_id].SetSessionState(SESSION_STATE::WAIT);
		clients[client_id].sl.unlock();

		clients[client_id].vl.lock();
		auto prev_vl = clients[client_id].view_list;
		clients[client_id].view_list.clear();
		clients[client_id].vl.unlock();
		
		clients[client_id].mvl.lock();		
		clients[client_id].monster_view_list.clear();
		clients[client_id].mvl.unlock();

		for (auto& monster : monsters) {
			if (client_id == monster.targetID) {
				monster.targetID = -1;
			}
		}

		clients[client_id].ml.lock();
		MAP_TYPE prev_map = clients[client_id].GetMap();
		clients[client_id].SetMap(packet->map_type);
		clients[client_id].ml.unlock();

		SC_CHANGE_MAP_PACKET change_map;
		change_map.size = sizeof(SC_CHANGE_MAP_PACKET);
		change_map.type = SC_CHANGE_MAP;		
		if (MAP_TYPE::SPACE == prev_map) {
			if (MAP_TYPE::ICE == clients[client_id].GetMap()) {
				change_map.x = 0;
				change_map.y = 0;
				change_map.z = 50000;
			}
			else if (MAP_TYPE::SAND == clients[client_id].GetMap()) {
				change_map.x = 0;
				change_map.y = 0;
				change_map.z = 50000;
			}
			else if (MAP_TYPE::WATER == clients[client_id].GetMap()) {
				change_map.x = 0;
				change_map.y = 0;
				change_map.z = 50000;
			}
		}
		else if (MAP_TYPE::ICE == prev_map) {
			change_map.x = 16866;
			change_map.y = -13774;
			change_map.z = 4848;
		}
		else if (MAP_TYPE::SAND == prev_map) {
			change_map.x = -17534;
			change_map.y = -3330;
			change_map.z = 10316;
		}
		else if (MAP_TYPE::WATER == prev_map) {
			change_map.x = 12712;
			change_map.y = 13870;
			change_map.z = -9203;
		}
		clients[client_id].sending(&change_map);

		for (auto clientID : prev_vl) {
			clients[clientID].vl.lock();
			if (0 != clients[clientID].view_list.count(client_id)) {
				clients[clientID].view_list.erase(client_id);
				clients[clientID].vl.unlock();
				clients[clientID].SendToRemovePlayer(client_id);
			}
			else {
				clients[clientID].vl.unlock();
			}
		}

		// client : streaming level 구현시 미사용 예정
		// 현재 open level 함수로 레벨 로드해서 로드중 데이터 전송 방지 위한
		// 악수 코딩
		this_thread::sleep_for(seconds(8));
		clients[client_id].sl.lock();
		clients[client_id].SetSessionState(SESSION_STATE::ACTIVE);
		clients[client_id].sl.unlock();
		break;
	}
	case CS_SPAWN_SPACESHIP:
	{
		CS_SPAWN_SPACESHIP_PACKET* packet = reinterpret_cast<CS_SPAWN_SPACESHIP_PACKET*>(p);
		if (packet->owner_sessionID < 0 || packet->owner_sessionID > 9999) {
			cout << "[CS_SPAWN_SPACESHIP] Unknown session ID\n";
			break;
		}

		clients[client_id].vl.lock();
		auto cur_vl = clients[client_id].view_list;
		clients[client_id].vl.unlock();

		for (auto clientID : cur_vl) {
			clients[clientID].SendToSpawnShip(packet);
		}
		break;
	}
	case CS_DAMAGE_MONSTER:
	{
		CS_DAMAGE_MONSTER_PACKET* packet = reinterpret_cast<CS_DAMAGE_MONSTER_PACKET*>(p);
		if (packet->sessionID < 0 || packet->sessionID > 9999) {
			cout << "[CS_DAMAGE_MONSTER] Unknown session ID\n";
			break;
		}

		bool bBoss = MONSTER_TYPE::T_ICE == monsters[packet->sessionID].GetMonsterType() ||
			MONSTER_TYPE::T_SAND == monsters[packet->sessionID].GetMonsterType() ||
			MONSTER_TYPE::T_WATER == monsters[packet->sessionID].GetMonsterType();

		monsters[packet->sessionID].hp_l.lock();
		float _hp = monsters[packet->sessionID].GetHP();
		if (0 >= _hp) {
			monsters[packet->sessionID].hp_l.unlock();
			break;
		}
		if (bBoss) {
			if (false == monsters[packet->sessionID].bdamaged) {
				_hp -= packet->damage;
			}
		}
		else {
			_hp -= packet->damage;
		}
		DROP_ITEM _item;
		if (0 >= _hp) {
			_hp = 0.f;			
			monsters[packet->sessionID].respawn_time = system_clock::now();	
			random_device rd;
			mt19937_64 mt(rd());
			uniform_int_distribution<int> uid{ 0,100000 };
			int per = uid(rd);
			if (per <= 33333) {
				_item = DROP_ITEM::A_COIN;
			}
			else if (per <= 66666) {
				_item = DROP_ITEM::B_COIN;
			}
			else {
				_item = DROP_ITEM::C_COIN;
			}
		}
		monsters[packet->sessionID].SetHP(_hp);
		if (bBoss) {
			monsters[packet->sessionID].ex_l.lock();
			float _ex = monsters[packet->sessionID].GetEX();
			_ex += packet->damage;
			monsters[packet->sessionID].SetEX(_ex);
			monsters[packet->sessionID].ex_l.unlock();
		}
		if (-1 == monsters[packet->sessionID].targetID) {
			monsters[packet->sessionID].targetID = packet->attack_sessionID;
			if (bBoss) {
				for (int i = 1; i < 7; ++i) {
					monsters[packet->sessionID + i].targetID = packet->attack_sessionID;
				}
			}
		}
		else {
			if (bBoss) {
				for (int i = 1; i < 7; ++i) {
					monsters[packet->sessionID + i].targetID = packet->attack_sessionID;
				}
			}
		}

		SC_DAMAGE_MONSTER_PACKET damage_packet;
		damage_packet.size = sizeof(SC_DAMAGE_MONSTER_PACKET);
		damage_packet.type = SC_DAMAGE_MONSTER;
		damage_packet.sessionID = packet->sessionID;
		damage_packet.hp = monsters[packet->sessionID].GetHP();
		if (_hp == 0.f) {
			damage_packet.attack_sessionID = packet->attack_sessionID;
			damage_packet.item = _item;
			//cout << packet->sessionID << " hp : " << damage_packet.hp << endl;
		}
		damage_packet.monsterType = packet->monsterType;

		for (auto& client : clients) {
			//client.sl.lock();
			if (SESSION_STATE::ACTIVE != client.GetSessionState()) {
				//client.sl.unlock();
				continue;
			}
			client.mvl.lock();
			if (0 != client.monster_view_list.count(packet->sessionID)) {
				if (0 >= monsters[packet->sessionID].GetHP()) {
					client.monster_view_list.erase(packet->sessionID);
				}
				this_thread::sleep_for(milliseconds(DELAY_TIME));
				client.sending(&damage_packet);
				client.mvl.unlock();
			}
			else {
				client.mvl.unlock();
			}
			//client.sl.unlock();
		}
	
		if (0 >= monsters[packet->sessionID].GetHP()) {
			monsters[packet->sessionID].death = true;
		}
		monsters[packet->sessionID].hp_l.unlock();
		//this_thread::sleep_for(milliseconds(16));
		break;
	}
	case CS_GET_ITEM:
	{
		CS_GET_ITEM_PACKET* packet = reinterpret_cast<CS_GET_ITEM_PACKET*>(p);
		if (packet->sessionID < 0 || packet->sessionID > 9999) {
			cout << "[CS_GET_ITEM] Unknown session ID\n";
			break;
		}
		clients[client_id].il.lock();
		switch (packet->item)
		{
		case DROP_ITEM::A_COIN:
			clients[client_id].a_coin += 1;
			break;
		case DROP_ITEM::B_COIN:
			clients[client_id].b_coin += 1;
			break;
		case DROP_ITEM::C_COIN:
			clients[client_id].c_coin += 1;
			break;		
		}		
		//this_thread::sleep_for(milliseconds(16));
		clients[client_id].SendToGetItem();		
		clients[client_id].il.unlock();
		break;
	}
	case CS_BUY_SELL:
	{
		CS_BUY_SELL_PACKET* packet = reinterpret_cast<CS_BUY_SELL_PACKET*>(p);
		if (packet->sessionID < 0 || packet->sessionID > 9999) {
			cout << "[CS_BUY_SELL] Unknown session ID\n";
			break;
		}
		bool bsend = false;
		clients[client_id].il.lock();
		int current_gold = clients[client_id].gold;
		int current_a_coin = clients[client_id].a_coin;
		int current_b_coin = clients[client_id].b_coin;
		int current_c_coin = clients[client_id].c_coin;
		switch (packet->buy_sell_type)
		{
		case BUY_SELL::BUY:
		{
			int buy_price = (packet->count * packet->price);			
			if (0 <= current_gold - buy_price) {
				bsend = true;
				clients[client_id].gold = current_gold - buy_price;
				switch (packet->coin_type)
				{
				case DROP_ITEM::A_COIN:
					clients[client_id].a_coin = current_a_coin + packet->count;
					break;
				case DROP_ITEM::B_COIN:
					clients[client_id].b_coin = current_b_coin + packet->count;
					break;
				case DROP_ITEM::C_COIN:
					clients[client_id].c_coin = current_c_coin + packet->count;
					break;
				}				
			}
			break;
		}
		case BUY_SELL::SELL:
		{
			int sell_price = (packet->count * packet->price);			
			switch (packet->coin_type)
			{
			case DROP_ITEM::A_COIN:
				if (0 <= current_a_coin - packet->count) {
					bsend = true;
					clients[client_id].a_coin = current_a_coin - packet->count;
				}				
				break;
			case DROP_ITEM::B_COIN:
				if (0 <= current_b_coin - packet->count) {
					bsend = true;
					clients[client_id].b_coin = current_b_coin - packet->count;
				}
				break;
			case DROP_ITEM::C_COIN:
				if (0 <= current_c_coin - packet->count) {
					bsend = true;
					clients[client_id].c_coin = current_c_coin - packet->count;
				}
				break;
			}
			if (true == bsend) {
				clients[client_id].gold = current_gold + sell_price;
			}
			break;
		}
		}

		if (true == bsend) {			
			//this_thread::sleep_for(milliseconds(16));			
			clients[client_id].SendToBuySell();
		}
		clients[client_id].il.unlock();
		break;
	}
	case CS_ITEM_BUY:
	{
		CS_ITEM_BUY_PACKET* packet = reinterpret_cast<CS_ITEM_BUY_PACKET*>(p);
		if (packet->sessionID < 0 || packet->sessionID > 9999) {
			cout << "[CS_ITEM_BUY] Unknown session ID\n";
			break;
		}
		clients[client_id].il.lock();
		switch (packet->item_type) {
		case ITEM_TYPE::CRITICAL_UP:
		{
			int current_gold = clients[client_id].gold;
			clients[client_id].gold = current_gold - packet->price;
			clients[client_id].critical_up += 1;
			break;
		}
		case ITEM_TYPE::SPEED_UP:
		{
			int current_gold = clients[client_id].gold;
			clients[client_id].gold = current_gold - packet->price;
			clients[client_id].speed_up += 1;
			break;
		}
		case ITEM_TYPE::ATTACK_UP:
		{
			int current_gold = clients[client_id].gold;
			clients[client_id].gold = current_gold - packet->price;
			clients[client_id].attack_up += 1;
			break;
		}
		case ITEM_TYPE::HP_UP:
		{
			int current_gold = clients[client_id].gold;
			clients[client_id].gold = current_gold - packet->price;
			clients[client_id].hp_up += 1;
			break;
		}
		case ITEM_TYPE::RECOVER_UP:
		{
			int current_gold = clients[client_id].gold;
			clients[client_id].gold = current_gold - packet->price;
			clients[client_id].recover_hp_up += 1;
			break;
		}
		case ITEM_TYPE::SNIPER_UP:
		{
			int current_gold = clients[client_id].gold;
			clients[client_id].gold = current_gold - packet->price;
			clients[client_id].sniper_up += 1;
			break;
		}
		case ITEM_TYPE::ATTACK_HP_UP:
		{
			int current_gold = clients[client_id].gold;
			clients[client_id].gold = current_gold - packet->price;
			clients[client_id].attack_hp_up += 1;
			break;
		}
		case ITEM_TYPE::LAUNCHER_UP:
		{
			int current_gold = clients[client_id].gold;
			clients[client_id].gold = current_gold - packet->price;
			clients[client_id].launcher_up += 1;
			break;
		}
		case ITEM_TYPE::LAUNCHER_RANGE_UP:
		{
			int current_gold = clients[client_id].gold;
			clients[client_id].gold = current_gold - packet->price;
			clients[client_id].launcher_range_up += 1;
			break;
		}
		case ITEM_TYPE::JUMP_UP:
		{
			int current_gold = clients[client_id].gold;
			clients[client_id].gold = current_gold - packet->price;
			clients[client_id].jump_up += 1;
			break;
		}
		}

		if (0 <= clients[client_id].gold) {
			clients[client_id].SendToItemBuy(packet);
		}
		else {
			clients[client_id].gold += packet->price;
		}
		clients[client_id].il.unlock();
		break;
	}
	case CS_COIN_PRICE:
	{
		CS_COIN_PRICE_PACKET* packet = reinterpret_cast<CS_COIN_PRICE_PACKET*>(p);
		if (packet->sessionID < 0 || packet->sessionID > 9999) {
			cout << "[CS_COIN_PRICE] Unknown session ID\n";
			break;
		}
		coin_l.lock();
		coin_api.GetCoinAPI();

		COIN_DATA a_coin{ 0 }, b_coin{ 0 }, c_coin{ 0 };
		a_coin = SaveCoinData(a_coin, coin_api, 0);
		b_coin = SaveCoinData(b_coin, coin_api, 1);
		c_coin = SaveCoinData(c_coin, coin_api, 2);
		coin_l.unlock();

		SC_COIN_PRICE_PACKET coin_packet;
		coin_packet.type = SC_COIN_PRICE;
		ZeroMemory(coin_packet.coin_data, sizeof(coin_packet.coin_data));
		strcpy_s(coin_packet.coin_data, sizeof(coin_packet.coin_data), AddCoinDataOneValue(coin_packet.coin_data, a_coin));
		strcpy_s(coin_packet.coin_data, sizeof(coin_packet.coin_data), AddCoinDataOneValue(coin_packet.coin_data, b_coin));
		strcpy_s(coin_packet.coin_data, sizeof(coin_packet.coin_data), AddCoinDataOneValue(coin_packet.coin_data, c_coin));

		coin_packet.size = sizeof(SC_COIN_PRICE_PACKET) - sizeof(coin_packet.coin_data) + strlen(coin_packet.coin_data) + 1;

		clients[client_id].sending(&coin_packet);
		break;
	}
	default:
		cout << "not exists type\n";
		break;
	}
}

void SERVER::main_loop()
{
	while (true) {
		DWORD num_bytes;
		ULONG_PTR key;
		WSAOVERLAPPED* over = nullptr;
		BOOL ret = GetQueuedCompletionStatus(h_iocp, &num_bytes, &key, &over, INFINITE);		
		EXT_OVER* ext_over = reinterpret_cast<EXT_OVER*>(over);
		if (FALSE == ret) {
			if (OVER_TYPE::ACCEPT == ext_over->GetOverType())
				cout << "Accept Error\n";
			else {
				cout << "GQCS Error on client[" << key << "]\n";
				disconnect(static_cast<int>(key));				
				if (OVER_TYPE::SEND == ext_over->GetOverType()) delete ext_over;
				continue;
			}
		}

		switch (ext_over->GetOverType()) {
		case OVER_TYPE::ACCEPT:
		{
			int client_id = GetNewClientID();
			if (-1 != client_id) {
				cout << "Client [" << client_id << "] accepted\n";
				clients[client_id].SetSessionState(SESSION_STATE::WAIT);
				clients[client_id].SetAcceptInit(OVER_TYPE::RECV, client_socket, true, client_id, 0);
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_socket), h_iocp, client_id, 0);
				clients[client_id].recving();
				client_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			}
			else {
				cout << "Max user exceeded.\n";
			}
			ZeroMemory(&accept_over.GetWSAOverlapped(), sizeof(accept_over.GetWSAOverlapped()));
			AcceptEx(server, client_socket, accept_over.GetSendBuf(), 0, addr_size + 16, addr_size + 16, 0, &accept_over.GetWSAOverlapped());		
			break;
		}
		case OVER_TYPE::RECV:
		{
			int data_stack = num_bytes + clients[key].GetPrevRemainData();
			char* packet = ext_over->GetSendBuf();
			int packet_size = packet[0];
			while (data_stack >= packet_size && 0 != data_stack) {
				analyze_packet(static_cast<int>(key), packet);
				packet = packet + packet_size;
				data_stack = data_stack - packet_size;
				packet_size = packet[0];
			}
			clients[key].SetPrevRemainData(data_stack);
			if (0 < data_stack) {
				memcpy(ext_over->GetSendBuf(), packet, data_stack);
			}
			clients[key].recving();
			break;
		}
		case OVER_TYPE::SEND:
		{
			delete ext_over;
			break;
		}
		case OVER_TYPE::PLAYER_MOVE:
		{
			auto L = monsters[key].L;
			monsters[key].vm_l.lock();
			lua_getglobal(L, "event_player_move");
			lua_pushnumber(L, ext_over->target_id);
			lua_pcall(L, 1, 0, 0);
			monsters[key].vm_l.unlock();
			delete ext_over;
			break;
		}
		}
	}
	closesocket(server);
	WSACleanup();
}

int API_get_player_x(lua_State* L)
{
	int player_id = static_cast<int>(lua_tonumber(L, -1));
	lua_pop(L, 2);

	float x = clients[player_id].GetX();

	lua_pushnumber(L, x);
	return 1;
}

int API_get_player_y(lua_State* L)
{
	int player_id = static_cast<int>(lua_tonumber(L, -1));
	lua_pop(L, 2);

	float y = clients[player_id].GetY();

	lua_pushnumber(L, y);
	return 1;
}

int API_get_player_z(lua_State* L)
{
	int player_id = static_cast<int>(lua_tonumber(L, -1));
	lua_pop(L, 2);

	float z = clients[player_id].GetZ();

	lua_pushnumber(L, z);
	return 1;
}

int API_get_monster_x(lua_State* L)
{
	int monster_id = static_cast<int>(lua_tonumber(L, -1));
	lua_pop(L, 2);

	float x = monsters[monster_id].GetX();

	lua_pushnumber(L, x);
	return 1;
}

int API_get_monster_y(lua_State* L)
{
	int monster_id = static_cast<int>(lua_tonumber(L, -1));
	lua_pop(L, 2);

	float y = monsters[monster_id].GetY();

	lua_pushnumber(L, y);
	return 1;
}

int API_get_monster_z(lua_State* L)
{
	int monster_id = static_cast<int>(lua_tonumber(L, -1));
	lua_pop(L, 2);

	float z = monsters[monster_id].GetZ();

	lua_pushnumber(L, z);
	return 1;
}

int API_get_monster_ex(lua_State* L)
{
	int monster_id = static_cast<int>(lua_tonumber(L, -1));
	lua_pop(L, 2);

	monsters[monster_id].ex_l.lock();
	float ex = monsters[monster_id].GetEX();
	monsters[monster_id].ex_l.unlock();

	lua_pushnumber(L, ex);
	return 1;
}

int API_normal_attack(lua_State* L)
{
	int monster_id = static_cast<int>(lua_tonumber(L, -2));
	int player_id = static_cast<int>(lua_tonumber(L, -1));
	lua_pop(L, 3);

	auto now_t = system_clock::now();
	auto ret_t = now_t - monsters[monster_id].act_time;
	if (true == monsters[monster_id].death) return 0;
	if (milliseconds(1000).count() <= duration_cast<milliseconds>(ret_t).count()) {
		monsters[monster_id].act_time = now_t;
		for (auto& client : clients) {
			client.mvl.lock();
			if (0 != client.monster_view_list.count(monster_id)) {
				client.SendToAttackMonster(monsters[monster_id], clients[player_id], MONSTER_ATTACK_TYPE::NORMAL);
				client.mvl.unlock();
			}
			else {
				client.mvl.unlock();
			}
		}		
		cout << "[INFO - ATTACK] session [" << player_id << "] attacked by monster [" << monster_id << "] normal attack\n";
		if (MONSTER_TYPE::F_FISH != monsters[monster_id].GetMonsterType()) {
			if ((RANGE / MONSTER_NORMAL_RANGE) * (RANGE / MONSTER_NORMAL_RANGE) >= Distance(clients[player_id], monsters[monster_id])) {
				auto t_now = system_clock::now();
				auto t_ret = t_now - clients[player_id].shield_time;
				if (milliseconds(5500).count() <= duration_cast<milliseconds>(t_ret).count()) {
					SC_MONSTER_ATTACK_PACKET damage_packet;
					damage_packet.size = sizeof(SC_MONSTER_ATTACK_PACKET);
					damage_packet.type = SC_MONSTER_ATTACK;
					switch (monsters[monster_id].GetMonsterType())
					{
					case MONSTER_TYPE::F_ICE:
						damage_packet.damage = F_DAMAGE;
						break;
					case MONSTER_TYPE::S_ICE:
						damage_packet.damage = S_DAMAGE;
						break;
					case MONSTER_TYPE::T_ICE:
						damage_packet.damage = T_DAMAGE;
						break;
					case MONSTER_TYPE::F_SAND:
						damage_packet.damage = F_DAMAGE;
						break;
					case MONSTER_TYPE::S_SAND:
						damage_packet.damage = S_DAMAGE;
						break;
					case MONSTER_TYPE::T_SAND:
						damage_packet.damage = T_DAMAGE;
						break;
					case MONSTER_TYPE::F_WATER:
						damage_packet.damage = F_DAMAGE;
						break;
					case MONSTER_TYPE::S_WATER:
						damage_packet.damage = S_DAMAGE;
						break;
					case MONSTER_TYPE::T_WATER:
						damage_packet.damage = T_DAMAGE;
						break;
					}
					//this_thread::sleep_for(milliseconds(16));
					cout << "[INFO - DAMAGED] session [" << clients[player_id].GetSessionID() << "] damaged by monster amount " << damage_packet.damage << "\n";
					clients[player_id].sending(&damage_packet);
				}
				else {
					cout << "[INFO - SHIELD] session [" << clients[player_id].GetSessionID() << "] shield\n";
				}
			}
		}
		else {
			if ((RANGE / MONSTER_NORMAL_RANGE) * (RANGE / MONSTER_NORMAL_RANGE) >= Distance2(clients[player_id], monsters[monster_id])) {
				auto t_now = system_clock::now();
				auto t_ret = t_now - clients[player_id].shield_time;
				if (milliseconds(5500).count() <= duration_cast<milliseconds>(t_ret).count()) {
					SC_MONSTER_ATTACK_PACKET damage_packet;
					damage_packet.size = sizeof(SC_MONSTER_ATTACK_PACKET);
					damage_packet.type = SC_MONSTER_ATTACK;
					damage_packet.damage = F_DAMAGE;
					//this_thread::sleep_for(milliseconds(16));
					cout << "[INFO - DAMAGED] session [" << clients[player_id].GetSessionID() << "] damaged by monster amount " << damage_packet.damage << "\n";
					clients[player_id].sending(&damage_packet);
				}
				else {
					cout << "[INFO - SHIELD] session [" << clients[player_id].GetSessionID() << "] shield\n";
				}
			}
		}
	}
	return 0;
}

int API_skill_attack(lua_State* L)
{
	int monster_id = static_cast<int>(lua_tonumber(L, -2));
	int player_id = static_cast<int>(lua_tonumber(L, -1));
	lua_pop(L, 3);
	
	auto now_t = system_clock::now();
	auto ret_t = now_t - monsters[monster_id].act_time;
	if (true == monsters[monster_id].death) return 0;
	if (milliseconds(1000).count() <= duration_cast<milliseconds>(ret_t).count()) {
		monsters[monster_id].act_time = system_clock::now();
		monsters[monster_id].ex_l.lock();
		float _ex = monsters[monster_id].GetEX();
		_ex -= 100.f;
		monsters[monster_id].SetEX(_ex);
		monsters[monster_id].ex_l.unlock();
		monsters[monster_id].act_time = now_t;
		for (auto& client : clients) {
			client.mvl.lock();
			if (0 != client.monster_view_list.count(monster_id)) {
				client.SendToAttackMonster(monsters[monster_id], clients[player_id], MONSTER_ATTACK_TYPE::SKILL);
				client.mvl.unlock();
			}
			else {
				client.mvl.unlock();
			}
		}		
		cout << "[INFO - ATTACK] session [" << player_id << "] attacked by monster [" << monster_id << "] skill attack\n";
		if ((RANGE / MONSTER_SKILL_RANGE) * (RANGE / MONSTER_SKILL_RANGE) >= Distance(clients[player_id], monsters[monster_id])) {
			auto t_now = system_clock::now();
			auto t_ret = t_now - clients[player_id].shield_time;
			if (milliseconds(5500).count() <= duration_cast<milliseconds>(t_ret).count()) {
				SC_MONSTER_ATTACK_PACKET damage_packet;
				damage_packet.size = sizeof(SC_MONSTER_ATTACK_PACKET);
				damage_packet.type = SC_MONSTER_ATTACK;
				damage_packet.damage = SKILL_DAMAGE;
				//this_thread::sleep_for(milliseconds(16));
				clients[player_id].sending(&damage_packet);
				cout << "[INFO - DAMAGED] session [" << clients[player_id].GetSessionID() << "] damaged by amount " << damage_packet.damage << "\n";
			}
			else {
				cout << "[INFO - SHIELD] session [" << clients[player_id].GetSessionID() << "] shield\n";
			}
		}
	}
	return 0;
}

void monster_move(int id)
{
	float x = monsters[id].GetX();
	float y = monsters[id].GetY();
	float z = monsters[id].GetZ();
	float dx = (static_cast<float>((rand() % 601)) - 300.f);
	float dy = (static_cast<float>((rand() % 601)) - 300.f);
	float yaw = atan(dx / dy);
	x += dx;
	y += dy;
	//z += (static_cast<float>((rand() % 11)) - 5.f);
	monsters[id].SetPosition(x, y, monsters[id].GetZ());
	monsters[id].SetYaw(yaw);
	float min = 99999.f;
	bool bBoss = MONSTER_TYPE::T_ICE == monsters[id].GetMonsterType() ||
		MONSTER_TYPE::T_SAND == monsters[id].GetMonsterType() ||
		MONSTER_TYPE::T_WATER == monsters[id].GetMonsterType();

	for (auto& client : clients) {
		//client.sl.lock();
		if (SESSION_STATE::ACTIVE != client.GetSessionState()) {
			//client.sl.unlock();
			continue;
		}
		client.ml.lock();
		if (monsters[id].GetMapType() != client.GetMap()) {
			client.ml.unlock();
			//client.sl.unlock();
			continue;
		}
		float dist;
		if (MONSTER_TYPE::F_FISH != monsters[id].GetMonsterType()) {
			dist = Distance(client, monsters[id]);
		}
		else {
			dist = Distance2(client, monsters[id]);
		}
		client.mvl.lock();
		if (0 != client.monster_view_list.count(id)) {			
			if (RANGE * RANGE >= dist) {
				if (true == bBoss) {
					if ((RANGE / 10) * (RANGE / 10) >= dist) {
						if (min > dist) {
							min = dist;
							monsters[id].targetID = client.GetSessionID();
						}
					}
				}
				client.SendToMoveMonster(monsters[id]);
				client.mvl.unlock();
			}
			else {
				client.monster_view_list.erase(id);
				client.SendToRemoveMonster(id);
				client.mvl.unlock();
			}
		}
		else {
			if (RANGE * RANGE >= dist) {
				client.monster_view_list.insert(id);
				client.SendToAddMonster(monsters[id]);
				client.mvl.unlock();
			}
			else {
				client.mvl.unlock();
			}
		}
		client.ml.unlock();
		//client.sl.unlock();
	}
}

void monster_move(int id, int target_id)
{
	float x = monsters[id].GetX();
	float y = monsters[id].GetY();
	float z = monsters[id].GetZ();
	float dist;
	if (MONSTER_TYPE::F_FISH != monsters[id].GetMonsterType()) {
		dist = Distance(clients[target_id], monsters[id]);
	}
	else {
		dist = Distance2(clients[target_id], monsters[id]);
	}
	if ((RANGE / 10) * (RANGE / 10) >= dist) {
		if ((RANGE / MONSTER_NORMAL_RANGE) * (RANGE / MONSTER_NORMAL_RANGE) >= dist) {
			auto ex_over = new EXT_OVER;
			ex_over->SetOverType(OVER_TYPE::PLAYER_MOVE);
			ex_over->target_id = target_id;
			PostQueuedCompletionStatus(h_iocp, 1, id, &ex_over->GetWSAOverlapped());
		}
		else {
			float _x = clients[target_id].GetX();
			float _y = clients[target_id].GetY();
			float _z = clients[target_id].GetZ();
			float dx = _x - x;
			float dy = _y - y;
			float yaw = atan(dx / dy);
			if (abs(_x - x) <= 300.f) x = _x - 20.f;
			else {
				if (_x > x) x += 300.f;
				if (_x < x) x -= 300.f;
			}
			if (abs(_y - y) <= 300.f) y = _y - 20.f;
			else {
				if (_y > y) y += 300.f;
				if (_y < y) y -= 300.f;
			}
			if (abs(_z - z) <= 0.5f) z = _z + 0.3f;
			else {
				if (_z > z) z += 0.5f;
				if (_z < z) z -= 0.5f;
			}
			monsters[id].SetPosition(x, y, z);
			monsters[id].SetYaw(yaw);
		}
	}
	else {
		bool bBoss = MONSTER_TYPE::T_ICE == monsters[id].GetMonsterType() ||
			MONSTER_TYPE::T_SAND == monsters[id].GetMonsterType() ||
			MONSTER_TYPE::T_WATER == monsters[id].GetMonsterType();
		if (!bBoss) {
			if (false == monsters[monsters[id].bossID].death) {
				if (RANGE * RANGE < dist) {
					float dx = (static_cast<float>((rand() % 601)) - 300.f);
					float dy = (static_cast<float>((rand() % 601)) - 300.f);
					float yaw = atan(dx / dy);
					x += dx;
					y += dy;
					//z += (static_cast<float>((rand() % 11)) - 5.f);
					monsters[id].SetPosition(x, y, monsters[id].GetZ());
					monsters[id].SetYaw(yaw);
					monsters[id].targetID = -1;
				}
				else {
					float _x = clients[target_id].GetX();
					float _y = clients[target_id].GetY();
					float _z = clients[target_id].GetZ();
					float dx = _x - x;
					float dy = _y - y;
					float yaw = atan(dx / dy);
					if (abs(_x - x) <= 300.f) x = _x - 20.f;
					else {
						if (_x > x) x += 300.f;
						if (_x < x) x -= 300.f;
					}
					if (abs(_y - y) <= 300.f) y = _y - 20.f;
					else {
						if (_y > y) y += 300.f;
						if (_y < y) y -= 300.f;
					}
					if (abs(_z - z) <= 0.5f) z = _z + 0.3f;
					else {
						if (_z > z) z += 0.5f;
						if (_z < z) z -= 0.5f;
					}
					monsters[id].SetPosition(x, y, z);
					monsters[id].SetYaw(yaw);
				}
			}
			else {
				float dx = (static_cast<float>((rand() % 601)) - 300.f);
				float dy = (static_cast<float>((rand() % 601)) - 300.f);
				float yaw = atan(dx / dy);
				x += dx;
				y += dy;
				//z += (static_cast<float>((rand() % 11)) - 5.f);
				monsters[id].SetPosition(x, y, monsters[id].GetZ());
				monsters[id].SetYaw(yaw);
				monsters[id].targetID = -1;
			}
		}
		else {
			float dx = (static_cast<float>((rand() % 601)) - 300.f);
			float dy = (static_cast<float>((rand() % 601)) - 300.f);
			float yaw = atan(dx / dy);
			x += dx;
			y += dy;
			//z += (static_cast<float>((rand() % 11)) - 5.f);
			monsters[id].SetPosition(x, y, monsters[id].GetZ());
			monsters[id].SetYaw(yaw);
			monsters[id].targetID = -1;
		}
	}

	for (auto& client : clients) {
		//client.sl.lock();
		if (SESSION_STATE::ACTIVE != client.GetSessionState()) {
			//client.sl.unlock();
			continue;
		}
		client.ml.lock();
		if (monsters[id].GetMapType() != client.GetMap()) {
			client.ml.unlock();
			//client.sl.unlock();
			continue;
		}
		float dist;
		if (MONSTER_TYPE::F_FISH != monsters[id].GetMonsterType()) {
			dist = Distance(client, monsters[id]);
		}
		else {
			dist = Distance2(client, monsters[id]);
		}
		client.mvl.lock();
		if (0 != client.monster_view_list.count(id)) {			
			if (RANGE * RANGE >= dist) {
				client.SendToMoveMonster(monsters[id]);
				client.mvl.unlock();
			}
			else {
				client.monster_view_list.erase(id);
				client.SendToRemoveMonster(id);
				client.mvl.unlock();
			}
		}
		else {
			if (RANGE * RANGE >= dist) {
				client.monster_view_list.insert(id);
				client.SendToAddMonster(monsters[id]);
				client.mvl.unlock();
			}
			else {
				client.mvl.unlock();
			}
		}
		client.ml.unlock();
		//client.sl.unlock();
	}
}

void Monster_action(int id)
{
	if (true == monsters[id].death) {
		bool bBoss = MONSTER_TYPE::T_ICE == monsters[id].GetMonsterType() ||
			MONSTER_TYPE::T_SAND == monsters[id].GetMonsterType() ||
			MONSTER_TYPE::T_WATER == monsters[id].GetMonsterType();
		if (!bBoss) {
			auto now_t = system_clock::now();
			auto ret_t = now_t - monsters[id].respawn_time;
			if (milliseconds(10000).count() <= duration_cast<milliseconds>(ret_t).count()) {
				cout << "[INFO - RESPAWN] monster [" << id << "] respawn\n";
				monsters[id].hp_l.lock();
				monsters[id].SetHP(monsters[id].GetInitHP());
				monsters[id].hp_l.unlock();
				monsters[id].SetRotation(0, 0, 0);
				monsters[id].death = false;
				if (false == monsters[(id / 7) * 7].death) {
					monsters[id].targetID = monsters[(id / 7) * 7].targetID;
					monsters[id].SetPosition(monsters[(id / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (id % 7) : -50.f * (id % 7)),
						monsters[(id / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (id % 7) : -50.f * (id % 7)),
						monsters[(id / 7) * 7].GetZ());
				}
				else {
					monsters[id].targetID = -1;
					monsters[id].SetPosition(monsters[id].GetInitX(), monsters[id].GetInitY(), monsters[id].GetInitZ());
				}
			}
		}
		else {
			auto now_t = system_clock::now();
			auto ret_t = now_t - monsters[id].respawn_time;
			if (milliseconds(20000).count() <= duration_cast<milliseconds>(ret_t).count()) {
				cout << "[INFO - RESPAWN] monster [" << id << "] respawn\n";
				monsters[id].hp_l.lock();
				monsters[id].SetHP(monsters[id].GetInitHP());
				monsters[id].hp_l.unlock();
				monsters[id].SetPosition(monsters[id].GetInitX(), monsters[id].GetInitY(), monsters[id].GetInitZ());
				monsters[id].SetRotation(0, 0, 0);
				monsters[id].targetID = -1;
				monsters[id].mob_num = 0;
				monsters[id].bdamaged = false;
				monsters[id].death = false;
			}
		}
	}
	else {
		if (-1 == monsters[id].targetID) {
			monster_move(id);
		}
		else {
			monster_move(id, monsters[id].targetID);
		}
	}
}

void Monster_move_timer()
{
	while (true)
	{
		auto start_t = chrono::system_clock::now();
		for (int i = 0; i < MAX_MONSTER; ++i) {
			Monster_action(i);
		}
		this_thread::sleep_until(start_t + chrono::milliseconds(700));
	}
}

// 몬스터 생성 위치 추가
void SERVER::MonsterCreate()
{
	cout << "Monster initialization start!\n";
	for (int i = 0; i < MAX_MONSTER; ++i) {
		monsters[i].SetSessionID(i);
		monsters[i].act_time = system_clock::now();
		lua_State* L = luaL_newstate();
		monsters[i].L = L;
		int lv = 0;
		if (i < 49) {
			monsters[i].SetMapType(MAP_TYPE::ICE);
			monsters[i].SetRotation(0, static_cast<float>(rand() % 361), 0);
			monsters[i].SetVelocity(0, 0, 0);
			switch (i % 7) {
			case 0:
				lv = 3;
				monsters[i].SetMonseterType(MONSTER_TYPE::T_ICE);
				monsters[i].SetHP(400);
				break;
			case 1:
			case 2:
				lv = 2;
				monsters[i].SetMonseterType(MONSTER_TYPE::S_ICE);
				monsters[i].SetHP(200);
				monsters[i].bossID = i / 7;
				break;
			case 3:
			case 4:
			case 5:
			case 6:
				lv = 1;
				monsters[i].SetMonseterType(MONSTER_TYPE::F_ICE);
				monsters[i].SetHP(100);
				monsters[i].bossID = i / 7;
				break;
			}
			monsters[i].SetInitHP(monsters[i].GetHP());
			switch (i / 7) {
			case 0:
				if (3 == lv) {
					monsters[i].SetPosition(9950.0f, -3770.0f, 540.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 1:
				if (3 == lv) {
					monsters[i].SetPosition(-25490.0f, 18472.0f, 1018.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 2:
				if (3 == lv) {
					monsters[i].SetPosition(11210.0f, 47994.0f, 1211.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 3:
				if (3 == lv) {
					monsters[i].SetPosition(43358.0f, 47438.0f, 270.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 4:
				if (3 == lv) {
					monsters[i].SetPosition(43088.0f, -17082.0f, 1084.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 5:
				if (3 == lv) {
					monsters[i].SetPosition(31638.0f, -69272.0f, 1504.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7 ].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 6:
				if (3 == lv) {
					monsters[i].SetPosition(-35552.0f, -57532.0f, 4876.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			}
			monsters[i].SetInitPosition(monsters[i].GetX(), monsters[i].GetY(), monsters[i].GetZ());
		}
		else if (i < 98) {
			monsters[i].SetMapType(MAP_TYPE::SAND);
			monsters[i].SetRotation(0, static_cast<float>(rand() % 361), 0);
			monsters[i].SetVelocity(0, 0, 0);
			switch (i % 7) {
			case 0:
				lv = 3;
				monsters[i].SetMonseterType(MONSTER_TYPE::T_SAND);
				monsters[i].SetHP(400);
				break;
			case 1:
			case 2:
				lv = 2;
				monsters[i].SetMonseterType(MONSTER_TYPE::S_SAND);
				monsters[i].SetHP(200);
				monsters[i].bossID = i / 7;
				break;
			case 3:
			case 4:
			case 5:
			case 6:
				lv = 1;
				monsters[i].SetMonseterType(MONSTER_TYPE::F_SAND);
				monsters[i].SetHP(100);
				monsters[i].bossID = i / 7;
				break;
			}
			monsters[i].SetInitHP(monsters[i].GetHP());
			switch (i / 7 - 7) {
			case 0:
				if (3 == lv) {
					monsters[i].SetPosition(63050.0f, -14604.0f, 13502.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 1:
				if (3 == lv) {
					monsters[i].SetPosition(24248.0f, 34028.0f, 2433.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 2:
				if (3 == lv) {
					monsters[i].SetPosition(8688.0f, -11302.0f, 260.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 3:
				if (3 == lv) {
					monsters[i].SetPosition(-42342.0f, 32398.0f, 1209.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 4:
				if (3 == lv) {
					monsters[i].SetPosition(-62432.0f, 69198.0f, 293.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 5:
				if (3 == lv) {
					monsters[i].SetPosition(-54032.0f, -54012.0f, 260.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 6:
				if (3 == lv) {
					monsters[i].SetPosition(-15582.0f, -55612.0f, 1686.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			}
			monsters[i].SetInitPosition(monsters[i].GetX(), monsters[i].GetY(), monsters[i].GetZ());
		}
		else if(i < 147){
			monsters[i].SetMapType(MAP_TYPE::WATER);
			monsters[i].SetRotation(0, static_cast<float>(rand() % 361), 0);
			monsters[i].SetVelocity(0, 0, 0);
			switch (i % 7) {
			case 0:
				lv = 3;
				monsters[i].SetMonseterType(MONSTER_TYPE::T_WATER);
				monsters[i].SetHP(400);
				break;
			case 1:
			case 2:
				lv = 2;
				monsters[i].SetMonseterType(MONSTER_TYPE::S_WATER);
				monsters[i].SetHP(200);
				monsters[i].bossID = i / 7;
				break;
			case 3:
			case 4:
			case 5:
			case 6:
				lv = 1;
				monsters[i].SetMonseterType(MONSTER_TYPE::F_WATER);
				monsters[i].SetHP(100);
				monsters[i].bossID = i / 7;
				break;
			}
			monsters[i].SetInitHP(monsters[i].GetHP());
			switch (i / 7 - 14) {
			case 0:
				if (3 == lv) {
					monsters[i].SetPosition(-4130.0f, 15984.0f, 3403.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 1:
				if (3 == lv) {
					monsters[i].SetPosition(-21992.0f, 26978.0f, 3429.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 2:
				if (3 == lv) {
					monsters[i].SetPosition(26578.0f, 10108.0f, 3438.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 3:
				if (3 == lv) {
					monsters[i].SetPosition(-11612.0f, -12752.0f, 3389.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 4:
				if (3 == lv) {
					monsters[i].SetPosition(30908.0f, -11672.0f, 3437.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 5:
				if (3 == lv) {
					monsters[i].SetPosition(-1252.0f, -34392.0f, 3409.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 6:
				if (3 == lv) {
					monsters[i].SetPosition(-34262.0f, -7652.0f, 3436.0f + POSZ);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			}
			monsters[i].SetInitPosition(monsters[i].GetX(), monsters[i].GetY(), monsters[i].GetZ());
		}
		else {
			monsters[i].SetMapType(MAP_TYPE::WATER);
			monsters[i].SetRotation(0, static_cast<float>(rand() % 361), 0);
			monsters[i].SetVelocity(0, 0, 0);
			switch (i % 7) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				lv = 1;
				monsters[i].SetMonseterType(MONSTER_TYPE::F_FISH);
				monsters[i].SetHP(100);
				monsters[i].bossID = i / 7;
				break;
			}
			monsters[i].SetInitHP(monsters[i].GetHP());
			switch (i / 7 - 21) {
			case 0:
				if (i == (i / 7) * 7) {
					monsters[i].SetPosition(21370.0f, -37750.0f, 930.f);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 1:
				if (i == (i / 7) * 7) {
					monsters[i].SetPosition(-24010.0f, -50720.0f, 660.0f);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 2:
				if (i == (i / 7) * 7) {
					monsters[i].SetPosition(-55100.0f, -39840.0f, 1050.0f);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 3:
				if (i == (i / 7) * 7) {
					monsters[i].SetPosition(-60050.0f, -100.0f, 950.0f);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 4:
				if (i == (i / 7) * 7) {
					monsters[i].SetPosition(-52070.0f, 21540.0f, 1030.0f);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			case 5:
				if (i == (i / 7) * 7) {
					monsters[i].SetPosition(470.0f, 47550.0f, 1140.0f);
				}
				else {
					monsters[i].SetPosition(monsters[(i / 7) * 7].GetX() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetY() + ((rand() % 2) == 1 ? 50.f * (i % 7) : -50.f * (i % 7)),
						monsters[(i / 7) * 7].GetZ());
				}
				break;
			}
			monsters[i].SetInitPosition(monsters[i].GetX(), monsters[i].GetY(), monsters[i].GetZ());
		}
		luaL_openlibs(L);
		luaL_loadfile(L, "monsterAI.lua");
		lua_pcall(L, 0, 0, 0);

		lua_getglobal(L, "set_monster_id");
		lua_pushnumber(L, i);
		lua_pushnumber(L, lv);
		lua_pcall(L, 2, 0, 0);

		lua_register(L, "API_get_player_x", API_get_player_x);
		lua_register(L, "API_get_player_y", API_get_player_y);
		lua_register(L, "API_get_player_z", API_get_player_z);
		lua_register(L, "API_get_monster_x", API_get_monster_x);
		lua_register(L, "API_get_monster_y", API_get_monster_y);
		lua_register(L, "API_get_monster_z", API_get_monster_z);
		lua_register(L, "API_normal_attack", API_normal_attack);
		lua_register(L, "API_skill_attack", API_skill_attack);
		lua_register(L, "API_get_monster_ex", API_get_monster_ex);
	}
	cout << "Monster initialization complete!\n";
}

void SERVER::initialize()
{
	MonsterCreate();
	WSADATA WSAData;

	ret = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (0 != ret) {
		err_no = WSAGetLastError();
		error_display("WSAStartup  : ", err_no);
	}

	server = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;

	ret = bind(server, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	if (0 != ret) {
		err_no = WSAGetLastError();
		error_display("bind  : ", err_no);
	}

	ret = listen(server, SOMAXCONN);
	if (SOCKET_ERROR == ret) {
		err_no = WSAGetLastError();
		error_display("listen  : ", err_no);
	}

	addr_size = sizeof(client_addr);

	h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(server), h_iocp, 9999, 0);
	client_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);	
	accept_over.SetOverType(OVER_TYPE::ACCEPT);
	AcceptEx(server, client_socket, accept_over.GetSendBuf(), 0, addr_size + 16, addr_size + 16, 0, &accept_over.GetWSAOverlapped());

	vector<thread> worker_threads;
	for (int i = 0; i < THREADS_NUM; ++i)
		worker_threads.emplace_back(&SERVER::main_loop, this);
	thread ai_thread{ Monster_move_timer };
	ai_thread.join();
	for (auto& th : worker_threads)
		th.join();

	main_loop();

	closesocket(server);
	WSACleanup();
}
