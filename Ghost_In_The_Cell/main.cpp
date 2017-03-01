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

const string CMove = "MOVE ";
const string CWait = "WAIT;";
const string CMsg = "MSG ";
const string CBomb = "BOMB ";

const string Factory = "FACTORY";
const string Troop = "TROOP";
const string Bomb = "BOMB";

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

	inline static const vector<Entity>& GetFactory() { return factory; }
	inline static const vector<Entity>& GetMyFactory() { return myFactory; }
	inline static const vector<Entity>& GetEnFactory() { return enFactory; }
	inline static const vector<Entity>& GetNeFactory() { return neFactory; }

	inline static const vector<Entity>& GetMyTroop() { return myTroop; }
	inline static const vector<Entity>& GetEnTroop() { return enTroop; }

	inline static const vector<Entity>& GetMyBomb() { return myBomb; }
	inline static const vector<Entity>& GetEnBomb() { return enBomb; }

private:

	friend Input;

	/// <summary>工場の総数</summary>
	static int factoryNumber;
	/// <summary>工場の間の距離</summary>
	static vector<vector<int>> distance;

	/// <summary>工場データ</summary>
	static vector<Entity> factory;
	/// <summary>自分工場データ</summary>
	static vector<Entity> myFactory;
	/// <summary>相手工場データ</summary>
	static vector<Entity> enFactory;
	/// <summary>放置工場データ</summary>
	static vector<Entity> neFactory;
	/// <summary>自分軍隊データ</summary>
	static vector<Entity> myTroop;
	/// <summary>相手軍隊データ</summary>
	static vector<Entity> enTroop;
	/// <summary>自分爆弾データ</summary>
	static vector<Entity> myBomb;
	/// <summary>相手爆弾データ</summary>
	static vector<Entity> enBomb;

};

int Share::factoryNumber;
vector<vector<int>> Share::distance;
vector<Entity> Share::factory;
vector<Entity> Share::myFactory;
vector<Entity> Share::enFactory;
vector<Entity> Share::neFactory;
vector<Entity> Share::myTroop;
vector<Entity> Share::enTroop;
vector<Entity> Share::myBomb;
vector<Entity> Share::enBomb;

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
				Share::factory.push_back(entity);
				if (entity.arg1 == My)
					Share::myFactory.push_back(move(entity));
				else if (entity.arg1 == Enemy)
					Share::enFactory.push_back(move(entity));
				else
					Share::neFactory.push_back(move(entity));
			}
			else if (entity.type == Troop)
			{
				if (entity.arg1 == My)
					Share::myTroop.push_back(move(entity));
				else if (entity.arg1 == Enemy)
					Share::enTroop.push_back(move(entity));
			}
			else if (entity.type == Bomb)
			{
				if (entity.arg1 == My)
					Share::myBomb.push_back(move(entity));
				else if (entity.arg1 == Enemy)
					Share::enBomb.push_back(move(entity));
			}


		}

	}

};

class AI {
public:

	const string think() {

		const auto& distance = Share::GetDistance();
		const auto& factories = Share::GetFactory();
		const int size = distance.size();

		vector<pair<int, int>> cyborgMap;
		cyborgMap.resize(size, { 0,0 });

		const auto& myTroops = Share::GetMyTroop();
		const auto& enTroops = Share::GetEnTroop();
		for (const auto& troop : myTroops) cyborgMap[troop.arg3].first += troop.arg4;
		for (const auto& troop : enTroops) cyborgMap[troop.arg3].second += troop.arg4;

		stringstream com;
		for (int s = 0; s < size; s++)
		{
			if (factories[s].arg1 == My)
			{
				for (int e = 0; e < size; e++)
				{
					if (s != e)
					{
						if (factories[e].arg1 == Enemy || factories[e].arg1 == Neutral)
						{
							int enemyNumber = factories[e].arg2 + cyborgMap[e].second;
							if (factories[e].arg1 == Enemy)
								enemyNumber += factories[e].arg3*distance[s][e];
							else
								enemyNumber += 1;

							//int myNumber = entities[s].arg2 + cyborgMap[e].first;
							int myNumber = factories[s].arg2;

							if (myNumber > enemyNumber)
							{
								com << CMove << s << " " << e << " " << max(enemyNumber, 1) << ";";
							}
						}
					}
				}
			}
		}

		if (!com.str().empty())
		{
			return com.str();
		}

		return CWait;
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

		cout << com << CMsg << sw.millisecond() << "ms" << endl;
	}
}

/*メモ
工場ユニット:FACTORY
arg1:　所有者　1:自分, -1:相手 0:中立
arg2:　サイボーグ数
arg3:　生産量
arg4:　
arg5:　

部隊ユニット:TROOP
arg1:　所有者　1:自分, -1:相手
arg2:　部隊が出発した工場
arg3:　部隊が目指す工場
arg4:　サイボーグ数
arg5:　到着までのターン数

爆弾ユニット:BOMB
arg1:　所有者　1:自分, -1:相手
arg2:　爆弾が出発した工場
arg3:　爆弾が目指す工場(相手の爆弾ならば-1)
arg4:　到着までのターン数(相手の爆弾ならば-1)
arg5:　

出力
MOVE　サイボーグを送り出す
MOVE 2 4 12:　12のサイボーグを2から4に送る

WAIT　何もしない

MSG　メッセージを表示する

BOMB　爆弾を発射する
BOMB 2 4:　爆弾を2から4に送る

INC　生産量を増やす　コスト:サイボーグ10体
INC 2:　2の生産数を1つ増やす

爆弾
ボムは2つだけ
工場から工場に送られる
着弾した工場のサイボーグ数をmin(n/2,n-10)にする　(33->16, 13->3)
着弾後5ターンの間サイボーグを生産できない
相手の爆弾が発射されることはわかるが、どこにいつ着弾するかはわからない
同じ工場から爆弾と軍隊を同時に送ろうとすると、爆弾だけ送られる

*/
