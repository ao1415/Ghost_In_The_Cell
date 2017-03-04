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
#include <iomanip>

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

struct FactoryEntity {
	/// <summary>id</summary>
	int id;
	/// <summary>所有者</summary>
	int owns;
	/// <summary>サイボーグ数</summary>
	int number;
	/// <summary>生産量</summary>
	int production;
	/// <summary>復帰ターン数</summary>
	int restart;
};
struct TroopEntity {
	/// <summary>id</summary>
	int id;
	/// <summary>所有者</summary>
	int owns;
	/// <summary>出発地点</summary>
	int leave;
	/// <summary>到着地点</summary>
	int target;
	/// <summary>サイボーグ数</summary>
	int number;
	/// <summary>到着ターン数</summary>
	int arrive;
};
struct BombEntity {
	/// <summary>id</summary>
	int id;
	/// <summary>所有者</summary>
	int owns;
	/// <summary>出発地点</summary>
	int leave;
	/// <summary>到着地点</summary>
	int target;
	/// <summary>到着ターン数</summary>
	int arrive;
};

enum class CommandType {
	Move,
	Wait,
	Msg,
	Bomb,
	Inc
};

const int FactoryLimit = 15;
const int DistanceLimit = 20;
using FactoryDistance = array<array<int, FactoryLimit>, FactoryLimit>;

inline const string BombCommand(int begin, int end);

struct Input;
class Share {
public:

	inline static const int& GetFactoryNumber() { return factoryNumber; }
	inline static const FactoryDistance& GetDistance() { return distance; }

	inline static const vector<FactoryEntity>& GetFactory() { return factory; }
	inline static const vector<FactoryEntity>& GetMyFactory() { return myFactory; }
	inline static const vector<FactoryEntity>& GetEnFactory() { return enFactory; }
	inline static const vector<FactoryEntity>& GetNeFactory() { return neFactory; }

	inline static const vector<TroopEntity>& GetTroop() { return troop; }
	inline static const vector<TroopEntity>& GetMyTroop() { return myTroop; }
	inline static const vector<TroopEntity>& GetEnTroop() { return enTroop; }

	inline static const vector<BombEntity>& GetMyBomb() { return myBomb; }
	inline static const vector<BombEntity>& GetEnBomb() { return enBomb; }

	inline static const int GetBomb() { return bombNumber; }

private:

	friend Input;
	friend const string BombCommand(int, int);

	/// <summary>工場の総数</summary>
	static int factoryNumber;
	/// <summary>工場の間の距離</summary>
	static FactoryDistance distance;

	/// <summary>工場データ</summary>
	static vector<FactoryEntity> factory;
	/// <summary>自分工場データ</summary>
	static vector<FactoryEntity> myFactory;
	/// <summary>相手工場データ</summary>
	static vector<FactoryEntity> enFactory;
	/// <summary>放置工場データ</summary>
	static vector<FactoryEntity> neFactory;

	/// <summary>軍隊データ</summary>
	static vector<TroopEntity> troop;
	/// <summary>自分軍隊データ</summary>
	static vector<TroopEntity> myTroop;
	/// <summary>相手軍隊データ</summary>
	static vector<TroopEntity> enTroop;

	/// <summary>自分爆弾データ</summary>
	static vector<BombEntity> myBomb;
	/// <summary>相手爆弾データ</summary>
	static vector<BombEntity> enBomb;

	/// <summary>ボムの総数</summary>
	static int bombNumber;

};

int Share::factoryNumber;
FactoryDistance Share::distance;
vector<FactoryEntity> Share::factory;
vector<FactoryEntity> Share::myFactory;
vector<FactoryEntity> Share::enFactory;
vector<FactoryEntity> Share::neFactory;
vector<TroopEntity> Share::troop;
vector<TroopEntity> Share::myTroop;
vector<TroopEntity> Share::enTroop;
vector<BombEntity> Share::myBomb;
vector<BombEntity> Share::enBomb;
int Share::bombNumber = 2;

inline const string MoveCommand(int begin, int end, int num) { return CMove + to_string(begin) + " " + to_string(end) + " " + to_string(num) + ";"; }
inline const string WaitCommand() { return CWait + ";"; }
inline const string MessageCommand(const string& msg) { return CMsg + msg + ";"; }
inline const string IncCommand(int id) { return CInc + to_string(id) + ";"; }
inline const string BombCommand(int begin, int end) {
	if (Share::bombNumber <= 0)
		return "";
	Share::bombNumber--;
	return CBomb + to_string(begin) + " " + to_string(end) + ";";
}

struct Command {
	CommandType type;
	int arg1;
	int arg2;
	int arg3;

	const string toString() const {
		switch (type)
		{
		case CommandType::Move: return MoveCommand(arg1, arg2, arg3);
		case CommandType::Wait: return WaitCommand();
		case CommandType::Msg: return MessageCommand("No Massage");
		case CommandType::Bomb: return BombCommand(arg1, arg2);
		case CommandType::Inc: return IncCommand(arg1);
		}
		return "";
	}

};

struct Input {

	static void Setup() {

		int number;
		cin >> number;
		cin.ignore();

		//cerr << number << endl;

		Share::factoryNumber = number;

		for (auto& d : Share::distance) d.fill(0);

		int link;
		cin >> link;
		cin.ignore();

		//cerr << link << endl;

		//cerr << "Unit:" << number << ", Link:" << link << endl;

		for (int i = 0; i < link; i++)
		{
			int factory1, factory2;
			int distance;
			cin >> factory1 >> factory2 >> distance;
			cin.ignore();
			Share::distance[factory1][factory2] = distance;
			Share::distance[factory2][factory1] = distance;

			//cerr << factory1 << " " << factory2 << " " << distance << endl;

		}

		/*
		for (const auto& vec : Share::distance)
		{
			for (const auto& v : vec)
			{
				cerr << setw(2) << setfill(' ') << right << v << ",";
			}
			cerr << endl;
		}
		cerr << resetiosflags(ios_base::floatfield);
		//*/
	}

	static void Loop() {

		int number;
		cin >> number;
		cin.ignore();

		Share::factory.clear();
		Share::myFactory.clear();
		Share::enFactory.clear();
		Share::neFactory.clear();

		Share::troop.clear();
		Share::myTroop.clear();
		Share::enTroop.clear();

		Share::myBomb.clear();
		Share::enBomb.clear();

		Share::factory.resize(Share::factoryNumber);

		for (int i = 0; i < number; i++)
		{
			int id;
			string type;
			int arg1;
			int arg2;
			int arg3;
			int arg4;
			int arg5;
			cin >> id >> type >> arg1 >> arg2 >> arg3 >> arg4 >> arg5;
			cin.ignore();

			if (type == Factory)
			{
				FactoryEntity entity;
				entity.id = id;
				entity.owns = arg1;
				entity.number = arg2;
				entity.production = arg3;
				entity.restart = arg4;

				Share::factory[entity.id] = entity;
				if (entity.owns == My)
					Share::myFactory.push_back(move(entity));
				else if (entity.owns == Enemy)
					Share::enFactory.push_back(move(entity));
				else
					Share::neFactory.push_back(move(entity));
			}
			else if (type == Troop)
			{
				TroopEntity entity;
				entity.id = id;
				entity.owns = arg1;
				entity.leave = arg2;
				entity.target = arg3;
				entity.number = arg4;
				entity.arrive = arg5;

				Share::troop.push_back(entity);
				if (entity.owns == My)
					Share::myTroop.push_back(move(entity));
				else if (entity.owns == Enemy)
					Share::enTroop.push_back(move(entity));
			}
			else if (type == Bomb)
			{
				BombEntity entity;
				entity.id = id;
				entity.owns = arg1;
				entity.leave = arg2;
				entity.target = arg3;
				entity.arrive = arg4;

				if (entity.owns == My)
					Share::myBomb.push_back(move(entity));
				else if (entity.owns == Enemy)
					Share::enBomb.push_back(move(entity));
			}


		}

	}

};

class AI {
public:

	using RiskType = array<array<int, FactoryLimit>, DistanceLimit + 1>;
	using TreeArray = array<array<int, FactoryLimit>, FactoryLimit>;

	const string think() {

		const auto& distance = Share::GetDistance();

		const auto& factories = Share::GetFactory();
		const auto& myFactories = Share::GetMyFactory();

		const auto& enFactories = Share::GetEnFactory();
		const auto& neFactories = Share::GetNeFactory();

		setRiskTable();

		/*
		for (const auto& vec : riskTable)
		{
			for (const auto& v : vec)
				cerr << setw(3) << right << v << ",";
			cerr << endl;
		}
		cerr << resetiosflags(ios_base::floatfield);
		//*/

		string com = WaitCommand();

		return com;
	}

	void makeTree() {

		struct Range {
			int begin;
			int end;
			int range = Inf;
			const bool operator<(const Range& o) const {
				return range < o.range;
			}
		};

		const auto& distance = Share::GetDistance();
		const size_t FNum = Share::GetFactoryNumber();

		for (auto& v : pathTable) v.fill(0);

		set<int> tree;
		tree.insert(0);

		vector<int> check(FNum, 0);
		check[0] = 1;

		const auto getRange = [&](int n) {

			Range range;
			for (const auto& t : tree)
			{
				if (range.range > distance[t][n])
				{
					range.range = distance[t][n];
					range.begin = t;
					range.end = n;
				}
			}

			return range;
		};

		while (tree.size() < FNum)
		{
			Range m;
			for (size_t j = 1; j < FNum; j++)
			{
				if (check[j] == 0)
				{
					const auto r = getRange(j);
					if (r < m)
					{
						m = r;
					}
				}
			}

			pathTable[m.begin][m.end] = 1;
			pathTable[m.end][m.begin] = 1;

			check[m.end] = 1;
			tree.insert(m.end);
		}
		/*
		for (const auto& vec : pathTable)
		{
			for (const auto& v : vec)
				cerr << v << ",";
			cerr << endl;
		}
		//*/
	}

private:


	RiskType riskTable;
	TreeArray pathTable;

	void setRiskTable() {

		for (auto& vec : riskTable) vec.fill(0);

		const auto& factories = Share::GetFactory();
		const auto& troops = Share::GetTroop();

		const auto& battle = [&](int o1, int o2) {
			return o1 == o2 ? 1 : -1;
		};

		for (const auto& troop : troops)
		{
			riskTable[troop.arrive][troop.target] += troop.number*battle(troop.owns, factories[troop.target].owns);
		}
	}

	const int getRiskSum(const int id) {
		int sum = 0;
		for (int i = 0; i < (int)riskTable.size(); i++)
			sum += riskTable[i][id];
		return sum;
	}

};

int main() {

	Input::Setup();

	AI ai;
	Stopwatch sw;

	ai.makeTree();

	while (true)
	{
		Input::Loop();

		sw.start();
		const string com = ai.think();
		sw.stop();

		cout << com << MessageCommand(to_string(sw.millisecond()) + "ms") << endl;
	}
}
