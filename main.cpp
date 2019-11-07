#include <iostream>
#include <random>

using std::cout;
using std::endl;
using std::default_random_engine;
using std::random_device;
using std::uniform_int_distribution;

unsigned randomchoose(unsigned n_min, unsigned n_max)
{
	random_device r;
	default_random_engine e{r()};
	uniform_int_distribution<unsigned> u(n_min, n_max);
	return u(e);
}

class ThreeDoors
{
private:
	const unsigned n_door = 3;
	unsigned door_goat;
	unsigned door_player_first_choosed;
	unsigned door_player_choosed;
	unsigned door_host_opened;
	bool bingo;

public:
	ThreeDoors();
	void PlaceGoatBehindDoor();
	void PlayerChooseDoor();
	void HostOpenDoor(bool b_known);
	void PlayerChange();
	bool Judge();
	bool Play(bool b_known, unsigned n_policy);
	void Output();
	void Reset();
	~ThreeDoors(){};
};

ThreeDoors::ThreeDoors()
{
	door_goat = 0;
	door_player_first_choosed = 0;
	door_player_choosed = 0;
	door_host_opened = 0;
	bingo = true;
}

void ThreeDoors::Reset()
{
	door_goat = 0;
	door_player_first_choosed = 0;
	door_player_choosed = 0;
	door_host_opened = 0;
	bingo = true;
}

void ThreeDoors::PlaceGoatBehindDoor()
{
	door_goat = randomchoose(1, 3);
}

void ThreeDoors::PlayerChooseDoor()
{
	door_player_first_choosed = randomchoose(1, 3);
}

void ThreeDoors::HostOpenDoor(bool b_known)
{
	if (b_known)
	{
		for (unsigned i = 1; i < n_door + 1; i++)
		{
			if ((i != door_goat) && (i != door_player_first_choosed))
			{
				door_host_opened = i;
				break;
			}
		}
	}
	else
	{
		unsigned n1, n2;
		switch (door_player_first_choosed) {
		case 1:
			n1 = 2;
			n2 = 3;
			break;
		case 2:
			n1 = 1;
			n2 = 3;
			break;
		case 3:
			n1 = 1;
			n2 = 2;
			break;
		}
		door_host_opened = randomchoose(0, 1) ? n1 : n2;
		if (door_host_opened == door_goat)
		{
			bingo = false;
		}
	}
}

void ThreeDoors::PlayerChange()
{
	for (unsigned i = 1; i < n_door + 1; i++)
	{
		if ((i != door_host_opened) && (i != door_player_first_choosed))
		{
			door_player_choosed = i;
			break;
		}
	}
}

bool ThreeDoors::Judge()
{
	return door_player_choosed == door_goat;
}

bool ThreeDoors::Play(bool b_known, unsigned n_policy)
{
	PlaceGoatBehindDoor();
	PlayerChooseDoor();
	HostOpenDoor(b_known);
	if (bingo)
	{
		switch (n_policy)
		{
		case 1: // Remain the first choice
			door_player_choosed = door_player_first_choosed;
			break;
		case 2: // Change to the other closed door
			PlayerChange();
			break;
		case 3: // Choose again from the two closed doors
			PlayerChange();
			door_player_choosed = randomchoose(0, 1) ? door_player_first_choosed : door_player_choosed;
			break;
		}
		return Judge();
	}
	else
	{
		return false;
	}
}

void ThreeDoors::Output()
{
	cout << door_goat << "\t" << door_player_first_choosed << "\t" << door_host_opened << "\t" << door_player_choosed << endl;
}

void PlayGame(ThreeDoors thrdoors, bool b_known, unsigned n_policy, size_t n)
{
	size_t n_bingo;
	n_bingo = 0;
	for (size_t i = 0; i < n; i++)
	{
		if (thrdoors.Play(b_known, n_policy))
		{
			n_bingo++;
		};
		thrdoors.Reset();
	}
	if (b_known)
	{
		cout << "When the host known behind which door the car is: " << endl;
	}
	else
	{
		cout << "When the host don't known behind which door the car is: " << endl;
	}
	switch (n_policy)
	{
	case 1:
		cout << "\t Win rate of remaining the first choice is " << (1.0 * n_bingo) / n * 100 << "%" << endl;
		break;
	case 2:
		cout << "\t Win rate of changing to the other door is " << (1.0 * n_bingo) / n * 100 << "%" << endl;
		break;
	case 3:
		cout << "\t Win rate of randomly choosing between the unopened doors is " << (1.0 * n_bingo) / n * 100 << "%" << endl;
		break;
	}
}

int main(int argc, char **argv)
{
	ThreeDoors thrdoors;
	size_t n = 1000000;
	PlayGame(thrdoors, true, 1, n);
	PlayGame(thrdoors, true, 2, n);
	PlayGame(thrdoors, true, 3, n);

	PlayGame(thrdoors, false, 1, n);
	PlayGame(thrdoors, false, 2, n);
	PlayGame(thrdoors, false, 3, n);

	return 0;
}