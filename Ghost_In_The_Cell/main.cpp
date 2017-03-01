#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <array>
#include <chrono>

using namespace std;

const string Move = "MOVE ";
const string Wait = "WAIT";

const string Factory = "FACTORY";
const string Troop = "TROOP";

const int My = 1;
const int Enemy = -1;
const int Neutral = 0;

const int Inf = 999999;

class Stopwatch {
public:

	void start() {
		s = std::chrono::high_resolution_clock::now();
		e = s;
	}
	void stop() {
		e = std::chrono::high_resolution_clock::now();
	}

	const long long nanoseconds() const { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
	const long long microseconds() const { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
	const long long millisecond() const { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }
	const long long second() const { return std::chrono::duration_cast<std::chrono::seconds>(e - s).count(); }
	const long long minutes() const { return std::chrono::duration_cast<std::chrono::minutes>(e - s).count(); }
	const long long hours() const { return std::chrono::duration_cast<std::chrono::hours>(e - s).count(); }

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

	inline static const vector<Entity>& GetEntity() { return entities; }

private:

	friend Input;

	/// <summary>工場の総数</summary>
	static int factoryNumber;
	/// <summary>工場の間の距離</summary>
	static vector<vector<int>> distance;

	/// <summary>ユニットデータ</summary>
	static vector<Entity> entities;

};

int Share::factoryNumber;
vector<vector<int>> Share::distance;
vector<Entity> Share::entities;

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

		Share::entities.clear();
		for (int i = 0; i < number; i++)
		{
			Entity entity;
			cin >> entity.id >> entity.type >> entity.arg1 >> entity.arg2 >> entity.arg3 >> entity.arg4 >> entity.arg5;
			cin.ignore();
			Share::entities.push_back(move(entity));
		}

	}

};

class AI {
public:

	const string think() {

		const auto& distance = Share::GetDistance();
		const auto& entity = Share::GetEntity();
		const int size = distance.size();

		pair<int, int> route(Inf, Inf);
		int range = Inf;
		int send = 0;
		for (int s = 0; s < size; s++)
		{
			if (entity[s].arg1 == My)
			{
				for (int e = 0; e < size; e++)
				{
					if (s != e)
					{
						if (entity[e].arg1 == Enemy || entity[e].arg1 == Neutral)
						{
							if (distance[s][e] < range)
							{
								const int enemyNumber = entity[e].arg2 + entity[e].arg3*distance[s][e];

								if (entity[s].arg2 > enemyNumber)
								{
									cerr << "STATE:" << entity[e].arg1 << "in" << entity[e].arg2 << "+" << entity[e].arg3*distance[s][e] << endl;
									cerr << "COM:" << s << "->" << e << "-" << enemyNumber << endl;
									range = distance[s][e];
									route = { s,e };
									send = max(enemyNumber, 1);
								}
							}
						}
					}
				}
			}
		}

		if (range != Inf)
		{
			stringstream com;
			com << Move << route.first << " " << route.second << " " << send;
			return com.str();
		}

		return Wait;
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

		cout << com << endl;
		cerr << sw.millisecond() << "ms" << endl;
	}
}

/*メモ
工場ユニット:FACTORY
arg1:　所有者　1:自分, -1:相手 0:中立
arg2:　サイボーグ数
arg3:　生産量
arg4:　未使用
arg5:　未使用

部隊ユニット:TROOP
arg1:　所有者　1:自分, -1:相手
arg2:　部隊が出発した工場
arg3:　部隊が目指す工場
arg4:　サイボーグ数
arg5:　到着までのターン数

出力
MOVE　サイボーグを送り出す
MOVE 2 4 12:　12のサイボーグを2から4に送る

WAIT　何もしない

*/
