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

		for (const auto& r : risk)
			cerr << r.first << ":" << r.second << endl;

		string com;
		for (const auto& my : myFactories)
		{
			com += factoryThink(my.second, risk);
		}

		return WaitCommand();
	}

private:

	//全ての工場の危険度を調べる・数値は全滅するまでのターン数
	const map<int, int> checkRisk() {

		const auto& myTroop = Share::GetMyTroop();
		const auto& enTroop = Share::GetEnTroop();

		auto factories = Share::GetFactory();

		const auto checkTeam = [](const Entity& e1, const Entity& e2) {
			return e1.arg1 == e2.arg1 ? 1 : -1;
		};

		map<int, int> risk;
		for (const auto& f : factories)
		{
			risk[f.second.id] = Inf;
		}

		for (auto& r : risk) r.second = Inf;

		const int Turn = 20;
		for (int t = 0; t <= Turn; t++)
		{
			for (const auto& troop : enTroop)
			{
				if (troop.second.arg5 == t)
					factories[troop.second.arg3].arg2 += troop.second.arg4 * checkTeam(factories[troop.second.arg3], troop.second);
			}
			for (const auto& troop : myTroop)
			{
				if (troop.second.arg5 == t)
					factories[troop.second.arg3].arg2 += troop.second.arg4 * checkTeam(factories[troop.second.arg3], troop.second);
			}

			for (auto& fac : factories)
			{
				if (fac.second.arg1 != 0)
					fac.second.arg2 += fac.second.arg3;
				if (fac.second.arg2 <= 0)
					risk[fac.second.id] = min(risk[fac.second.id], t);
			}
		}

		return risk;
	}
	const int checkRisk(const Entity& factory) {

		int risk = Inf;
		for (int t = 0; t <= 20; t++)
		{

		}

		return risk;
	}

	const string factoryThink(const Entity& factory, const map<int, int>& risk) {

		const auto& factories = Share::GetFactory();
		const auto& distance = Share::GetDistance();

		if (risk[factory.id] == Inf)
		{
			if (factory.arg2 >= IncCost)
			{

			}
		}

		return WaitCommand();
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
