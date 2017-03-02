#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <array>
#include <chrono>
#include <map>

using namespace std;

const string CMove = "MOVE ";
const string CWait = "WAIT";
const string CMsg = "MSG ";
const string CBomb = "BOMB ";
const string CInc = "INC ";

const string Factory = "FACTORY";
const string Troop = "TROOP";
const string Bomb = "BOMB";

const int My = 1;
const int Enemy = -1;
const int Neutral = 0;

const int IncCost = 10;
const int IncLimit = 3;
const int Inf = 999999;

class Stopwatch {
public:

	inline void start() {
		s = std::chrono::high_resolution_clock::now();
		e = s;
	}
	inline void stop() {
		e = std::chrono::high_resolution_clock::now();
	}

	inline const long long nanoseconds() const { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
	inline const long long microseconds() const { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
	inline const long long millisecond() const { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }
	inline const long long second() const { return std::chrono::duration_cast<std::chrono::seconds>(e - s).count(); }
	inline const long long minutes() const { return std::chrono::duration_cast<std::chrono::minutes>(e - s).count(); }
	inline const long long hours() const { return std::chrono::duration_cast<std::chrono::hours>(e - s).count(); }

private:

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	std::chrono::time_point<std::chrono::high_resolution_clock> e;

};

class Timer {
public:

	Timer() = default;
	Timer(const std::chrono::nanoseconds& _time) { type = Type::nanoseconds; time = _time.count(); }
	Timer(const std::chrono::microseconds& _time) { type = Type::microseconds; time = _time.count(); }
	Timer(const std::chrono::milliseconds& _time) { type = Type::milliseconds; time = _time.count(); }
	Timer(const std::chrono::seconds& _time) { type = Type::seconds; time = _time.count(); }
	Timer(const std::chrono::minutes& _time) { type = Type::minutes; time = _time.count(); }
	Timer(const std::chrono::hours& _time) { type = Type::hours; time = _time.count(); }

	void set(const std::chrono::nanoseconds& _time) { type = Type::nanoseconds; time = _time.count(); }
	void set(const std::chrono::microseconds& _time) { type = Type::microseconds; time = _time.count(); }
	void set(const std::chrono::milliseconds& _time) { type = Type::milliseconds; time = _time.count(); }
	void set(const std::chrono::seconds& _time) { type = Type::seconds; time = _time.count(); }
	void set(const std::chrono::minutes& _time) { type = Type::minutes; time = _time.count(); }
	void set(const std::chrono::hours& _time) { type = Type::hours; time = _time.count(); }

	void start() { s = std::chrono::high_resolution_clock::now(); }

	inline const bool check() const {
		const auto e = std::chrono::high_resolution_clock::now();
		switch (type)
		{
		case Type::nanoseconds: return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() >= time;
		case Type::microseconds: return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count() >= time;
		case Type::milliseconds: return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() >= time;
		case Type::seconds: return std::chrono::duration_cast<std::chrono::seconds>(e - s).count() >= time;
		case Type::minutes: return std::chrono::duration_cast<std::chrono::minutes>(e - s).count() >= time;
		case Type::hours: return std::chrono::duration_cast<std::chrono::hours>(e - s).count() >= time;
		default: return true;
		}
	}

	operator bool() const { return check(); }

private:

	enum class Type {
		nanoseconds,
		microseconds,
		milliseconds,
		seconds,
		minutes,
		hours
	};

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	long long time;
	Type type;

};

struct Entity {
	int id;
	string type;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
	int arg5;
};

struct Input;
class Share {
public:

	inline static const int& GetFactoryNumber() { return factoryNumber; }
	inline static const vector<vector<int>>& GetDistance() { return distance; }

	inline static const map<int, Entity>& GetFactory() { return factory; }
	inline static const map<int, Entity>& GetMyFactory() { return myFactory; }
	inline static const map<int, Entity>& GetEnFactory() { return enFactory; }
	inline static const map<int, Entity>& GetNeFactory() { return neFactory; }

	inline static const map<int, Entity>& GetMyTroop() { return myTroop; }
	inline static const map<int, Entity>& GetEnTroop() { return enTroop; }

	inline static const map<int, Entity>& GetMyBomb() { return myBomb; }
	inline static const map<int, Entity>& GetEnBomb() { return enBomb; }

private:

	friend Input;

	/// <summary>工場の総数</summary>
	static int factoryNumber;
	/// <summary>工場の間の距離</summary>
	static vector<vector<int>> distance;

	/// <summary>工場データ</summary>
	static map<int, Entity> factory;
	/// <summary>自分工場データ</summary>
	static map<int, Entity> myFactory;
	/// <summary>相手工場データ</summary>
	static map<int, Entity> enFactory;
	/// <summary>放置工場データ</summary>
	static map<int, Entity> neFactory;
	/// <summary>自分軍隊データ</summary>
	static map<int, Entity> myTroop;
	/// <summary>相手軍隊データ</summary>
	static map<int, Entity> enTroop;
	/// <summary>自分爆弾データ</summary>
	static map<int, Entity> myBomb;
	/// <summary>相手爆弾データ</summary>
	static map<int, Entity> enBomb;

};

inline const string MoveCommand(int begin, int end, int num) { return CMove + to_string(begin) + " " + to_string(end) + " " + to_string(num) + ";"; }
inline const string WaitCommand() { return CWait + ";"; }
inline const string MessageCommand(const string& msg) { return CMsg + msg + ";"; }
inline const string BombCommand(int begin, int end) { return CBomb + to_string(begin) + " " + to_string(end) + ";"; }
inline const string IncCommand(int id) { return CInc + to_string(id) + ";"; }

int Share::factoryNumber;
vector<vector<int>> Share::distance;
map<int, Entity> Share::factory;
map<int, Entity> Share::myFactory;
map<int, Entity> Share::enFactory;
map<int, Entity> Share::neFactory;
map<int, Entity> Share::myTroop;
map<int, Entity> Share::enTroop;
map<int, Entity> Share::myBomb;
map<int, Entity> Share::enBomb;

struct Input {

	static void Setup() {

		int number;
		cin >> number;
		cin.ignore();

		Share::factoryNumber = number;
		Share::distance.resize(number);
		for (auto& d : Share::distance) d.resize(number);

		int link;
		cin >> link;
		cin.ignore();

		cerr << "Unit:" << number << ", Link:" << link << endl;

		for (int i = 0; i < link; i++)
		{
			int factory1, factory2;
			int distance;
			cin >> factory1 >> factory2 >> distance;
			cin.ignore();
			Share::distance[factory1][factory2] = distance;
			Share::distance[factory2][factory1] = distance;
		}

	}

	static void Loop() {

		int number;
		cin >> number;
		cin.ignore();

		Share::factory.clear();
		Share::myFactory.clear();
		Share::enFactory.clear();
		Share::neFactory.clear();
		Share::myTroop.clear();
		Share::enTroop.clear();
		Share::myBomb.clear();
		Share::enBomb.clear();

		for (int i = 0; i < number; i++)
		{
			Entity entity;
			cin >> entity.id >> entity.type >> entity.arg1 >> entity.arg2 >> entity.arg3 >> entity.arg4 >> entity.arg5;
			cin.ignore();

			if (entity.type == Factory)
			{
				Share::factory[entity.id] = entity;
				if (entity.arg1 == My)
					Share::myFactory[entity.id] = move(entity);
				else if (entity.arg1 == Enemy)
					Share::enFactory[entity.id] = move(entity);
				else
					Share::neFactory[entity.id] = move(entity);
			}
			else if (entity.type == Troop)
			{
				if (entity.arg1 == My)
					Share::myTroop[entity.id] = move(entity);
				else if (entity.arg1 == Enemy)
					Share::enTroop[entity.id] = move(entity);
			}
			else if (entity.type == Bomb)
			{
				if (entity.arg1 == My)
					Share::myBomb[entity.id] = move(entity);
				else if (entity.arg1 == Enemy)
					Share::enBomb[entity.id] = move(entity);
			}


		}

	}

};

class AI {
public:

	const string think() {

		const auto& factories = Share::GetFactory();
		const auto& myFactories = Share::GetMyFactory();
		const auto& enFactories = Share::GetEnFactory();
		const auto& neFactories = Share::GetNeFactory();

		const auto& risk = checkRisk();

		string com = WaitCommand();
		for (const auto& my : myFactories)
		{
			com += factoryThink(my.second, risk);
		}

		return com;
	}

private:
	const static int Turn = 20;

	array<vector<int>, Turn + 1> troopTable;

	//全ての工場の危険度を調べる・数値は全滅するまでのターン数
	const map<int, int> checkRisk() {

		const auto& myTroop = Share::GetMyTroop();
		const auto& enTroop = Share::GetEnTroop();

		auto factories = Share::GetFactory();

		const auto checkTeam = [](const int e1, const int e2) {
			return e1 == e2 ? 1 : -1;
		};

		map<int, int> risk;


		for (auto& vec : troopTable)
		{
			vec.clear();
			vec.resize(factories.size(), 0);
		}

		for (const auto& troop : myTroop)
			troopTable[troop.second.arg5][troop.second.arg3] += troop.second.arg4 * checkTeam(factories[troop.second.arg3].arg1, troop.second.arg1);
		for (const auto& troop : enTroop)
			troopTable[troop.second.arg5][troop.second.arg3] += troop.second.arg4 * checkTeam(factories[troop.second.arg3].arg1, troop.second.arg1);

		/*
		for (const auto& vec : troopTable)
		{
			for (const auto& v : vec)
				cerr << v << ",";
			cerr << endl;
		}
		*/

		for (const auto& fac : factories)
		{
			risk[fac.second.id] = checkRisk(fac.second.id, fac.second.arg2, fac.second.arg3);
		}

		return risk;
	}
	const int checkRisk(const int id, const int number, const int inc) {

		int risk = Inf;
		int cyborg = number;

		for (int t = 0; t <= Turn; t++)
		{
			cyborg += troopTable[t][id];
			cyborg += inc;

			if (cyborg <= 0)
				return t;
		}

		return risk;
	}

	//工場に攻撃した時の攻撃者側のサイボーグの総数
	const int checkAttack(const int begin, const int end, const int number) {

		const auto& factories = Share::GetFactory();
		const auto& distance = Share::GetDistance();

		const int attack = number;
		const int defense = factories.at(end).arg2;
		const int inc = factories.at(end).arg3;
		const int range = distance[begin][end];

		int damage = 0;

		for (int i = 0; i < range; i++)
			damage += troopTable[i][end];

		return attack + damage - (defense + inc*range);
	}

	const string factoryThink(const Entity& factory, map<int, int> risk) {

		const auto& factories = Share::GetFactory();
		const auto& myFactories = Share::GetMyFactory();
		const auto& enFactories = Share::GetEnFactory();
		const auto& distance = Share::GetDistance();

		int cyborg = factory.arg2;

		string command = "";

		if (risk[factory.id] == Inf)
		{
			if (factory.arg3 < IncLimit)
			{
				if (cyborg >= IncCost)
				{
					const int turn = checkRisk(factory.id, cyborg - IncCost, factory.arg3 + 1);
					//cerr << "INC:" << factory.id << "-" << turn << endl;
					if (turn == Inf)
					{
						cyborg -= IncCost;
						command += IncCommand(factory.id);
					}
				}
			}
		}

		int moveCount = 0;

		vector<Entity> nearFactories(factories.size());
		for (const auto& f : factories)
			nearFactories[f.second.id] = f.second;
		sort(nearFactories.begin(), nearFactories.end(),
			[&](const Entity& e1, const Entity& e2) {
			if (e1.arg3 != e1.arg3)
				return e1.arg3 < e1.arg3;
			else
				return distance[factory.id][e1.id] < distance[factory.id][e2.id];
		});

		for (const auto& fac : nearFactories)
		{
			if (fac.arg1 == Neutral && fac.arg3 > 0)
			{
				if (cyborg > fac.arg2)
				{
					command += MoveCommand(factory.id, fac.id, fac.arg2 + 1);
					cyborg -= fac.arg2 + 1;
					moveCount++;
				}
			}
		}

		for (const auto& fac : nearFactories)
		{
			if (cyborg > 10 || (factory.arg3 == IncLimit && cyborg > 0))
			{
				if (fac.arg1 != My)
				{
					const int damage = checkAttack(factory.id, fac.id, cyborg);
					const int move = cyborg - damage + 1;
					if (damage >= 10 && move >= 10)
					{
						command += MoveCommand(factory.id, fac.id, move);
						cyborg -= move;
						moveCount++;
					}
				}
			}
		}

		const auto findMaxFactory = [](int id, const map<int, Entity>& facs) {

			int maxId = 0;
			int maxCyborg = 0;
			for (const auto& fac : facs)
			{
				if (id != fac.second.id)
				{
					if (maxCyborg < fac.second.arg2)
					{
						maxCyborg = fac.second.arg2;
						maxId = fac.second.id;
					}
				}
			}
			return maxId;
		};

		if (risk[factory.id] == 1)
		{
			int id = findMaxFactory(factory.id, myFactories);
			command += MoveCommand(factory.id, id, cyborg);
			moveCount++;
		}

		if (moveCount == 0)
		{
			int id = findMaxFactory(factory.id, enFactories);
			command += BombCommand(factory.id, id);
		}

		return command;
	}

};

int main() {

	Input::Setup();

	AI ai;
	Stopwatch sw;

	while (true)
	{
		Input::Loop();

		sw.start();
		const string com = ai.think();
		sw.stop();

		cout << com << MessageCommand(to_string(sw.millisecond()) + "ms") << endl;
	}
}
