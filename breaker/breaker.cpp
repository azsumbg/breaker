#include "pch.h"
#include "breaker.h"

// EXCEPTION CLASS ************************

dll::EXCEPTION::EXCEPTION(int err_code)
{
	error = err_code;
}

const wchar_t* dll::EXCEPTION::getErr()const
{
	switch (error)
	{
	case BAD_INDEX:
		return L"Bad index passed to a BAG method !";

	case BAD_PARAM:
		return L"Bad parameter passed to a BAG method !";

	case BAD_PTR:
		return L"Nullptr pointer passed to a BAG method !";

	case BAD_MEMORY:
		return L"Bad memory allocation in realloc() !";
	}

	return L"Unknown error occurred in a BAG method !";
}

///////////////////////////////////////////

// RANDIT CLASS ****************************

dll::RANDIT::RANDIT()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd(), rd() };
	twister = new std::mt19937(sq);
}
dll::RANDIT::~RANDIT()
{
	if (twister)delete twister;
}

int dll::RANDIT::operator()(int min, int max)
{
	if (max < min)return 0;

	std::uniform_int_distribution distrib(min, max);

	return distrib(*twister);
}
float dll::RANDIT::operator()(float min, float max)
{
	if (max < min)return 0;

	std::uniform_real_distribution<float> distrib(min, max);

	return distrib(*twister);
}

////////////////////////////////////////////

// PROTON CLASS ***************************

dll::PROTON::PROTON()
{
	start.x = 0;
	start.y = 0;

	_width = 1.0f;
	_height = 1.0f;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	end.x = start.x + _width;
	end.y = start.y + _height;

	myRect.left = start.x;
	myRect.top = start.y;
	myRect.right = end.x;
	myRect.bottom = end.y;
}
dll::PROTON::PROTON(float first_x, float first_y)
{
	start.x = first_x;
	start.y = first_y;

	_width = 1.0f;
	_height = 1.0f;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	end.x = start.x + _width;
	end.y = start.y + _height;

	myRect.left = start.x;
	myRect.top = start.y;
	myRect.right = end.x;
	myRect.bottom = end.y;
}
dll::PROTON::PROTON(float first_x, float first_y, float first_width, float first_height)
{
	start.x = first_x;
	start.y = first_y;

	_width = first_width;
	_height = first_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	end.x = start.x + _width;
	end.y = start.y + _height;

	myRect.left = start.x;
	myRect.top = start.y;
	myRect.right = end.x;
	myRect.bottom = end.y;
}

void dll::PROTON::set_edges()
{
	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	end.x = start.x + _width;
	end.y = start.y + _height;

	myRect.left = start.x;
	myRect.top = start.y;
	myRect.right = end.x;
	myRect.bottom = end.y;
}
void dll::PROTON::set_width(float new_width)
{
	_width = new_width;
	
	x_rad = _width / 2.0f;
	
	center.x = start.x + x_rad;
	
	end.x = start.x + _width;
	
	myRect.left = start.x;
	myRect.right = end.x;
}
void dll::PROTON::set_height(float new_height)
{
	_height = new_height;

	y_rad = _height / 2.0f;

	center.y = start.y + y_rad;

	end.y = start.y + _height;

	myRect.top = start.y;
	myRect.bottom = end.y;
}
void dll::PROTON::set_dims(float new_width, float new_height)
{
	_width = new_width;
	_height = new_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	end.x = start.x + _width;
	end.y = start.y + _height;

	myRect.left = start.x;
	myRect.top = start.y;
	myRect.right = end.x;
	myRect.bottom = end.y;
}

float dll::PROTON::get_width() const
{
	return _width;
}
float dll::PROTON::get_height() const
{
	return _height;
}
D2D1_RECT_F dll::PROTON::get_rect()const
{
	return myRect;
}

bool dll::PROTON::created_in_heap()const
{
	return in_heap;
}

dll::PROTON* dll::PROTON::create(float first_x, float first_y, float first_width, float first_height)
{
	PROTON* ret{ new PROTON(first_x, first_y, first_width, first_height) };

	ret->in_heap = true;

	return ret;
}

///////////////////////////////////////////

// GRID CLASS ****************************

dll::GRID::GRID(int number_of_bricks)
{
	Bricks.clear();

	int brick_counter = 0;
	int stone_bricks = number_of_bricks / 10;

	float brick_x = 5.0f;
	float brick_y = 60.0f;

	while (brick_counter < number_of_bricks)
	{
		for (int cols = 0; cols < 15; ++cols)
		{
			BRICK dummy{};
			
			dummy.rect.left = brick_x;
			dummy.rect.top = brick_y;
			dummy.rect.right = brick_x + 50.0f;
			dummy.rect.bottom = brick_y + 20.0f;

			dummy.type = static_cast<bricks>(_randerer(0, 4));
			dummy.number = brick_counter;

			switch (dummy.type)
			{
			case bricks::yellow:
				dummy.lifes = 1;
				break;

			case bricks::red:
				dummy.lifes = 2;
				break;

			case bricks::blue:
				dummy.lifes = 4;
				break;

			case bricks::green:
				dummy.lifes = 3;
				break;
			}
			dummy.value = dummy.lifes;

			Bricks.push_back(dummy);

			brick_x += 52.5f;

			++brick_counter;
		}

		brick_x = 5.0f;
		brick_y += 22.5f;
	}

	for (int i = 0; i < stone_bricks; ++i)
	{
		bool ok = false;

		while (!ok)
		{
			ok = true;

			int stone_number = _randerer(0, brick_counter - 1);
			if (Bricks[stone_number].type != bricks::stone)Bricks[stone_number].type = bricks::stone;
			else ok = false;
		}
	}

	bricks_in_grid = number_of_bricks;
	bricks_to_crush = bricks_in_grid - stone_bricks;
}

void dll::GRID::create_grid(int all_bricks)
{
	Bricks.clear();

	int brick_counter = 0;
	int stone_bricks = all_bricks / 10;

	float brick_x = 5.0f;
	float brick_y = 60.0f;

	while (brick_counter < all_bricks)
	{
		for (int cols = 0; cols < 15; ++cols)
		{
			BRICK dummy{};

			dummy.rect.left = brick_x;
			dummy.rect.top = brick_y;
			dummy.rect.right = brick_x + 50.0f;
			dummy.rect.bottom = brick_y + 20.0f;

			dummy.type = static_cast<bricks>(_randerer(0, 4));
			dummy.number = brick_counter;

			switch (dummy.type)
			{
			case bricks::yellow:
				dummy.lifes = 1;
				break;

			case bricks::red:
				dummy.lifes = 2;
				break;

			case bricks::blue:
				dummy.lifes = 4;
				break;

			case bricks::green:
				dummy.lifes = 3;
				break;
			}
			dummy.value = dummy.lifes;

			Bricks.push_back(dummy);

			brick_x += 52.5f;

			++brick_counter;
		}

		brick_x = 5.0f;
		brick_y += 22.5f;
	}

	for (int i = 0; i < stone_bricks; ++i)
	{
		bool ok = false;

		while (!ok)
		{
			ok = true;

			int stone_number = _randerer(0, brick_counter - 1);
			if (Bricks[stone_number].type != bricks::stone)Bricks[stone_number].type = bricks::stone;
			else ok = false;
		}
	}

	bricks_in_grid = all_bricks;
	bricks_to_crush = bricks_in_grid - stone_bricks;
}

BRICK dll::GRID::get_brick(int number)
{
	BRICK ret{};

	if (number < 0 || number >= Bricks.size())return ret;

	ret = Bricks[number];

	return ret;
}
BRICK dll::GRID::get_brick(D2D1_RECT_F brick_rect)
{
	BRICK ret{};

	ret.number = -1;

	for (int i = 0; i < Bricks.size(); ++i)
	{
		D2D1_RECT_F dummy = Bricks[i].rect;

		if (dummy.left == brick_rect.left && dummy.right == brick_rect.right
			&& dummy.top == brick_rect.top && dummy.bottom == brick_rect.bottom)
		{
			ret = Bricks[i];
			break;
		}
	}

	return ret;
}

void dll::GRID::remove_brick(int number)
{
	if (number < 0 || number >= Bricks.size())return;

	Bricks.erase(number);
}
void dll::GRID::remove_brick(D2D1_RECT_F brick_rect)
{
	BRICK ret{};

	ret.number = -1;

	for (int i = 0; i < Bricks.size(); ++i)
	{
		D2D1_RECT_F dummy = Bricks[i].rect;

		if (dummy.left == brick_rect.left && dummy.right == brick_rect.right
			&& dummy.top == brick_rect.top && dummy.bottom == brick_rect.bottom)
		{
			ret = Bricks[i];
			break;
		}
	}

	if (ret.number != -1)Bricks.erase(ret.number);
}

//////////////////////////////////////////