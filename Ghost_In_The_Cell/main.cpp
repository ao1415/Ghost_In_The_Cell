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

inline const string MoveCommand(int begin, int end, int num) { return CMove + to_string(begin) + " " + to_string(end) + " " + to_string(num) + ";"; }
inline const string WaitCommand() { return CWait + ";"; }
inline const string MessageCommand(const string& msg) { return CMsg + msg + ";"; }
inline const string BombCommand(int begin, int end) { return CBomb + to_string(begin) + " " + to_string(end) + ";"; }
inline const string IncCommand(int id) { return CInc + to_string(id) + ";"; }

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

using FactoryDistance = array<array<int, 15>, 15>;

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

private:

	friend Input;

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

struct Input {

	static void Setup() {

		int number;
		cin >> number;
		cin.ignore();

		Share::factoryNumber = number;

		for (auto& d : Share::distance) d.fill(0);

		int link;
		cin >> link;
		cin.ignore();

		//cerr << "Unit:" << number << ", Link:" << link << endl;

		for (int i = 0; i < link; i++)
		{
			int factory1, factory2;
			int distance;
			cin >> factory1 >> factory2 >> distance;
			cin.ignore();
			Share::distance[factory1][factory2] = distance;
			Share::distance[factory2][factory1] = distance;
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

class Game {
public:

	Game() = default;
	Game(const Game&) = default;
	Game(Game&&) = default;

	void input(const vector<FactoryEntity>& _factories, const vector<TroopEntity>& _troops) {
		factories = _factories;
		troops = _troops;
	}

	Game next(const vector<Command>& coms) const {

		Game game(*this);

		const auto& distance = Share::GetDistance();

		for (auto& troop : game.troops)
			troop.arrive--;

		for (const auto& com : coms)
		{
			switch (com.type)
			{
			case CommandType::Move:
			{
				game.factories[com.arg1].number -= com.arg3;
				TroopEntity troop;
				troop.id = Inf;
				troop.owns = game.factories[com.arg1].owns;
				troop.leave = com.arg1;
				troop.target = com.arg2;
				troop.number = com.arg3;
				troop.arrive = distance[com.arg1][com.arg2];
				game.troops.push_back(move(troop));
				break;
			}
			case CommandType::Inc:
				game.factories[com.arg1].number -= IncCost;
				game.factories[com.arg1].production = min(IncLimit, game.factories[com.arg1].production + 1);
				break;
			}
		}

		for (auto& fac : game.factories)
		{
			if (fac.owns != Neutral)
			{
				if (fac.restart <= 0)
					fac.number += fac.production;
				fac.restart--;
			}
		}

		vector<TroopEntity> nextTroop;
		for (auto& troop : game.troops)
		{
			if (troop.arrive == 0)
			{
				if (game.factories[troop.target].owns == troop.owns)
				{
					game.factories[troop.target].number += troop.number;
				}
				else
				{
					game.factories[troop.target].number -= troop.number;
					if (game.factories[troop.target].number <= 0)
					{
						game.factories[troop.target].number *= -1;
						game.factories[troop.target].owns = troop.owns;
					}
				}
			}
			else
			{
				troop.arrive--;
				nextTroop.push_back(move(troop));
			}
		}
		game.troops = move(nextTroop);

		return game;
	}

private:

	vector<FactoryEntity> factories;
	vector<TroopEntity> troops;

};

class AI {
public:

	const string think() {

		const auto& distance = Share::GetDistance();

		const auto& factories = Share::GetFactory();
		const auto& myFactories = Share::GetMyFactory();

		string com = WaitCommand();

		if (myFactories.size() == 1)
		{
			cerr << "初動ルーチン" << endl;

			const auto& enFactories = Share::GetEnFactory();
			const auto& neFactories = Share::GetNeFactory();

			const auto nextBaseEval = [&](const FactoryEntity& f) {
				int enRange = Inf;
				int myRange = Inf;
				int pro = f.production;

				for (const auto& fac : enFactories)
					enRange = min(enRange, distance[f.id][fac.id]);
				for (const auto& fac : myFactories)
					myRange = min(myRange, distance[f.id][fac.id]);

				const double er = enRange / 20.0;
				const double mr = myRange / 20.0;
				const double pr = pro / 3.0;

				return (int)((er - mr + pr) * 10000);
			};

			if (neFactories.empty())
			{
				cerr << "詰みです" << endl;
				return WaitCommand();
			}

			int maxScore = nextBaseEval(neFactories[0]);
			int id = neFactories[0].id;
			for (const auto& nefac : neFactories)
			{
				const int score = nextBaseEval(nefac);
				if (maxScore < score)
				{
					maxScore = score;
					id = nefac.id;
				}
			}

			if (myFactories[0].production > 0)
			{
				cerr << "移動:" << myFactories[0].id << "->" << id << "=" << myFactories[0].number << endl;
				com += MoveCommand(myFactories[0].id, id, myFactories[0].number);
			}
			else
			{
				if (myFactories[0].number > IncCost)
				{
					cerr << "移動:" << myFactories[0].id << "->" << id << "=" << myFactories[0].number - IncCost << endl;
					com += MoveCommand(myFactories[0].id, id, myFactories[0].number - IncCost);
				}
			}
		}
		else
		{
			for (const auto& myfac : myFactories)
			{
				if (myfac.production < IncLimit)
				{
					if (myfac.number >= IncCost)
					{
						com += IncCommand(myfac.id);
					}
				}
				else
				{

				}
			}
		}

		return com;
	}

private:



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
