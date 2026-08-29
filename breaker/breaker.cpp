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

// PAD CLASS ****************************

dll::PAD::PAD(float _sx, float _sy) :PROTON(_sx, _sy, 71.0f, 24.0f) {};

void dll::PAD::move(float gear, dirs to_where)
{
	float my_speed = speed + gear / 10.0f;

	dir = to_where;

	switch (to_where)
	{
	case dirs::left:
		if (start.x > 0)
		{
			start.x -= my_speed;
			set_edges();
		}
		else
		{
			start.x = 0;
			set_edges();
			dir = dirs::stop;
		}
		break;

	case dirs::right:
		if (end.x < scr_width)
		{
			start.x += my_speed;
			set_edges();
		}
		else
		{
			end.x = scr_width;
			start.x = end.x - get_width();
			set_edges();
			dir = dirs::stop;
		}
		break;
	}
}

pads dll::PAD::get_pad() const
{
	return(type);
}
void dll::PAD::set_pad(pads new_pad)
{
	switch (new_pad)
	{
	case pads::normal:
		set_dims(71.0f, 24.0f);
		break;

	case pads::large:
		set_dims(138.0f, 23.0f);
		break;

	case pads::extra_large:
		set_dims(204.0f, 24.0f);
		break;
	}
	
	type = new_pad;
}

void dll::PAD::set_speed(float new_speed)
{
	speed = new_speed;
}

void dll::PAD::Release()
{
	delete this;
}

dll::PAD* dll::PAD::create(float sx, float sy)
{
	return new PAD(sx, sy);
}

////////////////////////////////////////

// BALL CLASS **************************

dll::BALL::BALL(float _sx, float _sy) :PROTON(_sx, _sy, 20.0f, 13.0f) {};

float dll::BALL::get_init_x() const
{
	return move_sx;
}
float dll::BALL::get_init_y() const
{
	return move_sy;
}
float dll::BALL::get_target_x() const
{
	return move_ex;
}
float dll::BALL::get_target_y() const
{
	return move_sy;
}

int dll::BALL::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++frame;
		if (frame > max_frames)frame = 0;
	}

	return frame;
}
void dll::BALL::set_speed(float new_speed)
{
	speed = new_speed;
}

void dll::BALL::set_path(float target_x, float target_y)
{
	ver_dir = false;
	hor_dir = false;

	move_sx = start.x;
	move_ex = target_x;

	move_sy = start.y;
	move_ey = target_y;

	if (move_sx == move_ex || (move_ex > move_sx && move_ex <= end.x))
	{
		ver_dir = true;
		return;
	}
	if (move_sy == move_ey || (move_ey > move_sy && move_ey <= end.y))
	{
		hor_dir = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sx);
	intercept = start.y - start.x * slope;
}

bumps dll::BALL::move(float gear)
{
	float my_speed = speed + gear / 5.0f;

	if (hor_dir)
	{
		if (move_sx > move_ex)
		{
			start.x -= my_speed;
			set_edges();
			if (start.x <= 0)
			{
				start.x = 0;
				set_edges();
				return bumps::on_left;
			}
		}
		else if (move_sx < move_ex)
		{
			start.x += my_speed;
			set_edges();
			if (end.x >= scr_width)
			{
				end.x = scr_width;
				start.x = end.x - _width;
				set_edges();
				return bumps::on_right;
			}
		}
	}
	else if (ver_dir)
	{
		if (move_sy > move_ey)
		{
			start.y -= my_speed;
			set_edges();
			if (start.y <= sky)
			{
				start.y = sky;
				set_edges();
				return bumps::on_top;
			}
		}
		else if (move_sy < move_ey)
		{
			start.y += my_speed;
			set_edges();
			if (end.y >= ground) return bumps::out;
		}
	}
	else
	{
		if (move_sx > move_ex)
		{
			start.x -= my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (start.x <= 0)
			{
				start.x = 0;
				set_edges();
				return bumps::on_left;
			}
			if (start.y <= sky)
			{
				start.y = sky;
				set_edges();
				return bumps::on_top;
			}
			if (end.y >= ground) return bumps::out;
		}
		else if (move_sx < move_ex)
		{
			start.x += my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.x >= scr_width)
			{
				end.x = scr_width;
				start.x = end.x - _width;
				set_edges();
				return bumps::on_right;
			}
			if (start.y <= sky)
			{
				start.y = sky;
				set_edges();
				return bumps::on_top;
			}
			if (end.y >= ground) return bumps::out;
		}
	}
	
	return bumps::no_bump;
}

void dll::BALL::Release() 
{
	delete this;
}

dll::BALL* dll::BALL::create(float sx, float sy)
{
	return new BALL(sx, sy);
}

////////////////////////////////////////

// CLASS FIELD *************************

dll::FIELD::FIELD(fields _what)
{
	type = _what;

	switch (type)
	{
	case fields::space:
		max_frames = 5;
		frame_delay = 13;
		break;

	case fields::fantasy:
		max_frames = 71;
		break;

	case fields::planes:
		max_frames = 3;
		frame_delay = 22;
		break;

	case fields::forest:
		max_frames = 17;
		frame_delay = 4;
		break;
	}

	max_frame_delay = frame_delay;
}

int dll::FIELD::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++frame;
		if (frame > max_frames)frame = 0;
	}

	return frame;
}
void dll::FIELD::Release()
{
	delete this;
}

dll::FIELD* dll::FIELD::create(fields what)
{
	return new FIELD(what);
}

////////////////////////////////////////

// CLASS ASSET *************************

dll::ASSET::ASSET(assets _what, float _sx, float _sy) :PROTON(_sx, _sy, 40.0f, 35.0f)
{
	type = _what;
}

bool dll::ASSET::move(float gear)
{
	float my_speed = speed + gear / 10.0f;

	start.y += my_speed;
	set_edges();

	if (start.y >= ground)return false;

	return true;
}

void dll::ASSET::Release()
{
	delete this;
}

dll::ASSET* dll::ASSET::create(assets what, float sx, float sy)
{
	return new ASSET(what, sx, sy);
}

////////////////////////////////////////

// FUNCTIONS **************************

void dll::BallDispatcher(BALL& Ball, bumps where, float bump_x, float bump_y, float pad_sx, float pad_ex)
{
	RANDIT _randerer{};

	float to_where_x{ 0 };
	float to_where_y{ 0 };

	float bump_angle{ 0 };

	float oppos{ 0 };
	float adjanced{ 0 };

	float temp_x{ 0 };

	switch (where)
	{
	case bumps::on_left:
		to_where_y = sky;
		adjanced = Ball.start.y - bump_y;
		oppos = Ball.start.x;
		bump_angle = atan2f(oppos, adjanced) * 180.0f / 3.14f;
		bump_angle = 180.0f - bump_angle;
		bump_angle = bump_angle * 3.14f / 180.0f;
		to_where_x = tanf(bump_angle) * Ball.start.y;
		break;

	case bumps::on_right:
		to_where_y = sky;
		adjanced = Ball.start.y - bump_y;
		oppos = scr_width - Ball.start.x;
		bump_angle = atan2f(oppos, adjanced) * 180.0f / 3.14f;
		bump_angle = 180.0f - bump_angle;
		bump_angle = bump_angle * 3.14f / 180.0f;
		to_where_x = tanf(bump_angle) * Ball.start.y;
		break;

	case bumps::on_top:
		Ball.set_speed(ball_default_speed);
		if (Ball.get_init_x() > Ball.get_target_x())
		{
			to_where_y = ground;
			adjanced = Ball.get_init_x() - bump_x;
			oppos = Ball.get_init_y();
			bump_angle = atan2f(oppos, adjanced);
			temp_x = oppos / tanf(bump_angle);
			to_where_x = Ball.start.x - temp_x;
			break;
		}
		else if (Ball.get_init_x() < Ball.get_target_x())
		{
			to_where_y = ground;
			adjanced = Ball.get_init_x() + bump_x;
			oppos = Ball.get_init_y();
			bump_angle = atan2f(oppos, adjanced);
			temp_x = oppos / tanf(bump_angle);
			to_where_x = Ball.start.x + temp_x;
			break;
		}
		else
		{
			to_where_y = ground;
			if (_randerer(0, 2) == 1)to_where_x = Ball.end.x + 100.0f + _randerer(0.0f, 50.0f);
			else to_where_x = Ball.start.x - (100.0f + _randerer(0.0f, 50.0f));
		}
		break;

	case bumps::on_brick:
		Ball.set_speed(ball_default_speed);
		if (bump_y < Ball.center.y)
		{
			if (Ball.get_init_x() > Ball.get_target_x())
			{
				to_where_y = ground;
				adjanced = Ball.get_init_x() - bump_x;
				oppos = Ball.get_init_y();
				bump_angle = atan2f(oppos, adjanced);
				temp_x = oppos / tanf(bump_angle);
				to_where_x = Ball.start.x - temp_x;
				break;
			}
			else if (Ball.get_init_x() < Ball.get_target_x())
			{
				to_where_y = ground;
				adjanced = Ball.get_init_x() + bump_x;
				oppos = Ball.get_init_y();
				bump_angle = atan2f(oppos, adjanced);
				temp_x = oppos / tanf(bump_angle);
				to_where_x = Ball.start.x + temp_x;
				break;
			}
			else
			{
				to_where_y = ground;
				if (_randerer(0, 2) == 1)to_where_x = Ball.end.x + 100.0f + _randerer(0.0f, 50.0f);
				else to_where_x = Ball.start.x - (100.0f + _randerer(0.0f, 50.0f));
			}
		}
		else if (bump_y > Ball.center.y)
		{
			if (Ball.get_init_x() > Ball.get_target_x())
			{
				to_where_y = sky;
				adjanced = Ball.get_init_x() - bump_x;
				oppos = bump_y - Ball.get_init_y();
				bump_angle = atan2f(oppos, adjanced);
				temp_x = oppos / tanf(bump_angle);
				to_where_x = Ball.start.x - temp_x;
				break;
			}
			else if (Ball.get_init_x() < Ball.get_target_x())
			{
				to_where_y = sky;
				adjanced = Ball.get_init_x() + bump_x;
				oppos = bump_y - Ball.get_init_y();
				bump_angle = atan2f(oppos, adjanced);
				temp_x = oppos / tanf(bump_angle);
				to_where_x = Ball.start.x + temp_x;
				break;
			}
			else
			{
				to_where_y = sky;
				if (_randerer(0, 2) == 1)to_where_x = Ball.end.x + 100.0f + _randerer(0.0f, 50.0f);
				else to_where_x = Ball.start.x - (100.0f + _randerer(0.0f, 50.0f));
			}
		}
		else
		{
			to_where_y = sky;
			if (Ball.start.x >= bump_x)to_where_x = Ball.end.x + 100.0f + _randerer(0.0f, 50.0f);
			else to_where_x = Ball.start.x - (100.0f + _randerer(0.0f, 50.0f));
		}
		break;

	case bumps::on_pad:
		temp_x = (pad_ex - pad_sx) / 3.0f;
		if ((bump_x >= pad_sx && bump_x < pad_sx + temp_x)
			|| (bump_x >= pad_sx + 2.0f * temp_x && bump_x < pad_ex))Ball.set_speed(ball_default_speed + 0.5f);
		
		if (bump_y < Ball.center.y)
		{
			if (Ball.get_init_x() > Ball.get_target_x())
			{
				to_where_y = ground;
				adjanced = Ball.get_init_x() - bump_x;
				oppos = Ball.get_init_y();
				bump_angle = atan2f(oppos, adjanced);
				temp_x = oppos / tanf(bump_angle);
				to_where_x = Ball.start.x - temp_x;
				break;
			}
			else if (Ball.get_init_x() < Ball.get_target_x())
			{
				to_where_y = ground;
				adjanced = Ball.get_init_x() + bump_x;
				oppos = Ball.get_init_y();
				bump_angle = atan2f(oppos, adjanced);
				temp_x = oppos / tanf(bump_angle);
				to_where_x = Ball.start.x + temp_x;
				break;
			}
			else
			{
				to_where_y = ground;
				if (_randerer(0, 2) == 1)to_where_x = Ball.end.x + 100.0f + _randerer(0.0f, 50.0f);
				else to_where_x = Ball.start.x - (100.0f + _randerer(0.0f, 50.0f));
			}
		}
		else if (bump_y > Ball.center.y)
		{
			if (Ball.get_init_x() > Ball.get_target_x())
			{
				to_where_y = sky;
				adjanced = Ball.get_init_x() - bump_x;
				oppos = bump_y - Ball.get_init_y();
				bump_angle = atan2f(oppos, adjanced);
				temp_x = oppos / tanf(bump_angle);
				to_where_x = Ball.start.x - temp_x;
				break;
			}
			else if (Ball.get_init_x() < Ball.get_target_x())
			{
				to_where_y = sky;
				adjanced = Ball.get_init_x() + bump_x;
				oppos = bump_y - Ball.get_init_y();
				bump_angle = atan2f(oppos, adjanced);
				temp_x = oppos / tanf(bump_angle);
				to_where_x = Ball.start.x + temp_x;
				break;
			}
			else
			{
				to_where_y = sky;
				if (_randerer(0, 2) == 1)to_where_x = Ball.end.x + 100.0f + _randerer(0.0f, 50.0f);
				else to_where_x = Ball.start.x - (100.0f + _randerer(0.0f, 50.0f));
			}
		}
		else
		{
			to_where_y = sky;
			if (Ball.start.x >= bump_x)to_where_x = Ball.end.x + 100.0f + _randerer(0.0f, 50.0f);
			else to_where_x = Ball.start.x - (100.0f + _randerer(0.0f, 50.0f));
		}
		break;
	}

	Ball.set_path(to_where_x, to_where_y);
}