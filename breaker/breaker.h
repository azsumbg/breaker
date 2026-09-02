#pragma once

#ifdef BREAKER_EXPORTS
#define BREAKER_API __declspec(dllexport)
#else
#define BREAKER_API __declspec(dllimport)
#endif

#include <d2d1.h>
#include <random>

#pragma comment(lib, "d2d1.lib")

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

constexpr float ball_default_speed{ 2.0f };

constexpr D2D1_RECT_F FULL_SCREEN{ 0, 0, 1000.0f, 800.0f };
constexpr D2D1_RECT_F GAME_SCREEN{ 0, 50.0f, 1000.0f, 750.0f };

constexpr int BAD_PTR{ 6001 };
constexpr int BAD_PARAM{ 6002 };
constexpr int BAD_INDEX{ 6003 };
constexpr int BAD_MEMORY{ 6004 };

enum class dirs { stop = 0, left = 1, right = 2, up = 3, down = 4 };
enum class bricks { yellow = 0, blue = 1, red = 2, green = 3, orange = 4, stone = 5 };
enum class assets { triple = 0, life = 1, shorten = 2, strech = 3 };
enum class pads { normal = 0, large = 1, shorter = 2 };
enum class bumps { no_bump = 0, on_pad = 1, on_brick = 2, on_left = 3, on_right = 4, on_top = 5, out = 6 };
enum class fields { space = 0, fantasy = 1, planes = 2, forest = 3 };


struct BRICK
{
	bricks type{ bricks::yellow };

	D2D1_RECT_F rect;

	int number = 0;
	int lifes = 1;
	int value = 1;
};

namespace dll
{
	class BREAKER_API RANDIT
	{
	private:
		std::mt19937* twister{ nullptr };

	public:
		RANDIT();
		~RANDIT();

		int operator()(int min, int max);
		float operator()(float min, float max);
	};

	// BAG and EXCEPTIOM CLASSES ***************

	class BREAKER_API EXCEPTION
	{
	private:
		int error{ 0 };

	public:
		EXCEPTION(int err_code);

		const wchar_t* getErr()const;
	};

	#pragma warning(disable : 26439)

	template<typename T> class BAG
	{
	private:
		size_t max_size{ 1 };
		size_t next_pos{ 0 };
		T* mPtr{ nullptr };

	public:
		BAG()
		{
			mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
			if (!mPtr)throw EXCEPTION(BAD_PTR);
		}
		BAG(size_t capacity)
		{
			max_size = capacity;
			mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
			if (!mPtr)throw EXCEPTION(BAD_PTR);
		}
		BAG(BAG& other)
		{
			if (other.mPtr == nullptr)throw EXCEPTION(BAD_PTR);
			else
			{
				max_size = other.max_size;
				next_pos = other.next_pos;
				mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
				if (mPtr == nullptr)throw EXCEPTION(BAD_PTR);
				else
				{
					if (next_pos > 0)for (size_t i = 0; i < next_pos; ++i)mPtr[i] = other.mPtr[i];
				}
			}
		}
		BAG(BAG&& other)
		{
			if (other.mPtr == nullptr)throw EXCEPTION(BAD_PTR);
			else
			{
				max_size = other.max_size;
				next_pos = other.next_pos;
				mPtr = other.mPtr;
				
				other.mPtr = nullptr;
			}
		}

		~BAG()
		{
			free(mPtr);
		}

		bool empty()const
		{
			return(next_pos == 0);
		}
		size_t size()const
		{
			return next_pos;
		}
		size_t capacity()const
		{
			return max_size;
		}

		BAG& operator=(BAG& other)
		{
			if (!other.mPtr)throw EXCEPTION(BAD_PTR);
			else
			{
				free(mPtr);

				max_size = other.max_size;
				next_pos = other.next_pos;
				mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
				if (mPtr == nullptr)throw EXCEPTION(BAD_PTR);
				else
				{
					if (next_pos > 0)for (size_t i = 0; i < next_pos; ++i)mPtr[i] = other.mPtr[i];
				}

			}

			return *this;
		}
		BAG& operator=(BAG&& other)
		{
			if (!other.mPtr)throw EXCEPTION(BAD_PTR);
			else
			{
				free(mPtr);

				max_size = other.max_size;
				next_pos = other.next_pos;
				mPtr = other.mPtr;

				other.mPtr = nullptr;
			}

			return *this;
		}

		T& operator[](size_t index)
		{
			if (index < 0 || index >= next_pos)throw EXCEPTION(BAD_INDEX);
			if (!mPtr)throw EXCEPTION(BAD_PTR);

			return mPtr[index];
		}

		T& front()
		{
			if (!mPtr)throw EXCEPTION(BAD_PTR);
			if (next_pos == 0)throw EXCEPTION(BAD_INDEX);

			return *mPtr;
		}
		T& back()
		{
			if (!mPtr)throw EXCEPTION(BAD_PTR);
			if (next_pos == 0)throw EXCEPTION(BAD_INDEX);

			return mPtr[next_pos-1];
		}

		void clear()
		{
			free(mPtr);
			
			max_size = 1;
			next_pos = 0;

			mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
		}

		void push_back(T element)
		{
			if (!mPtr)throw EXCEPTION(BAD_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					mPtr[next_pos] = element;
					++next_pos;
				}
				else
				{
					++max_size;

					T* dummy = reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T)));
					if (!dummy)throw EXCEPTION(BAD_MEMORY);
					else
					{
						mPtr = dummy;
						dummy = nullptr;
						mPtr[next_pos] = element;
						++next_pos;
					}
				}
			}
		}
		void push_back(T* element)
		{
			if (!mPtr)throw EXCEPTION(BAD_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					mPtr[next_pos] = *element;
					++next_pos;
				}
				else
				{
					++max_size;

					T* dummy = reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T)));
					if (!dummy)throw EXCEPTION(BAD_MEMORY);
					else
					{
						mPtr = dummy;
						dummy = nullptr;
						mPtr[next_pos] = *element;
						++next_pos;
					}
				}
			}
		}

		void push_front(T element)
		{
			if (!mPtr)throw EXCEPTION(BAD_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					*mPtr = element;
					++next_pos;
				}
				else
				{
					++max_size;

					T* dummy = reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T)));
					if (!dummy)throw EXCEPTION(BAD_MEMORY);
					else
					{
						mPtr = dummy;
						dummy = nullptr;
						*mPtr = element;
						++next_pos;
					}
				}
			}
		}
		void push_front(T* element)
		{
			if (!mPtr)throw EXCEPTION(BAD_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					*mPtr = *element;
					++next_pos;
				}
				else
				{
					++max_size;

					T* dummy = reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T)));
					if (!dummy)throw EXCEPTION(BAD_MEMORY);
					else
					{
						mPtr = dummy;
						dummy = nullptr;
						*mPtr = *element;
						++next_pos;
					}
				}
			}
		}
		
		void erase(size_t index)
		{
			if (index < 0 || index >= next_pos)throw EXCEPTION(BAD_INDEX);
			if (!mPtr)throw EXCEPTION(BAD_PTR);
			else
			{
				for (size_t i = index; i < next_pos - 1; ++i)mPtr[i] = mPtr[i + 1];

				--max_size;

				T* dummy = reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T)));
				if (!dummy)throw EXCEPTION(BAD_MEMORY);
				else
				{
					mPtr = dummy;
					dummy = nullptr;
				}

				--next_pos;
			}
		}

		void insert(T element, size_t index)
		{
			if (index < 0 || index > next_pos)throw EXCEPTION(BAD_INDEX);
			if (!mPtr)throw EXCEPTION(BAD_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					if (index == next_pos)
					{
						mPtr[next_pos] = element;
						++next_pos;
					}
					else
					{
						for (size_t i = next_pos; i > index; --i)mPtr[i] = mPtr[i - 1];
						mPtr[index] = element;
						++next_pos;
					}
				}
				else
				{
					++max_size;

					T* dummy = reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T)));
					if (!dummy)throw EXCEPTION(BAD_MEMORY);
					else
					{
						mPtr = dummy;
						dummy = nullptr;
						
						if (index == next_pos)
						{
							mPtr[next_pos] = element;
							++next_pos;
						}
						else
						{
							for (size_t i = next_pos; i > index; --i)mPtr[i] = mPtr[i - 1];
							mPtr[index] = element;
							++next_pos;
						}
					}
				}
			}
		}
		void insert(T* element, size_t index)
		{
			if (index < 0 || index > next_pos)throw EXCEPTION(BAD_INDEX);
			if (!mPtr)throw EXCEPTION(BAD_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					if (index == next_pos)
					{
						mPtr[next_pos] = *element;
						++next_pos;
					}
					else
					{
						for (size_t i = next_pos; i > index; --i)mPtr[i] = mPtr[i - 1];
						mPtr[index] = *element;
						++next_pos;
					}
				}
				else
				{
					++max_size;

					T* dummy = reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T)));
					if (!dummy)throw EXCEPTION(BAD_MEMORY);
					else
					{
						mPtr = dummy;
						dummy = nullptr;

						if (index == next_pos)
						{
							mPtr[next_pos] = *element;
							++next_pos;
						}
						else
						{
							for (size_t i = next_pos; i > index; --i)mPtr[i] = mPtr[i - 1];
							mPtr[index] = *element;
							++next_pos;
						}
					}
				}
			}
		}
	};

	///////////////////////////////////////////

	class BREAKER_API PROTON
	{
	protected:
		float _width{ 0 };
		float _height{ 0 };
		D2D1_RECT_F myRect{};

		bool in_heap{ false };

	public:
		D2D1_POINT_2F start{};
		D2D1_POINT_2F end{};
		D2D1_POINT_2F center{};

		float x_rad{ 0 };
		float y_rad{ 0 };
		
		PROTON();
		PROTON(float first_x, float first_y);
		PROTON(float first_x, float first_y, float first_width, float first_height);

		virtual ~PROTON() {};

		void set_edges();
		void set_width(float new_width);
		void set_height(float new_height);
		void set_dims(float new_width, float new_height);

		float get_width() const;
		float get_height() const;
		D2D1_RECT_F get_rect()const;

		bool created_in_heap()const;

		static PROTON* create(float first_x, float first_y, float first_width, float first_height);
	};

	class BREAKER_API GRID
	{
	private:
		BAG<BRICK> Bricks{};
		RANDIT _randerer{};

	public:
		int bricks_in_grid{ 0 };
		int bricks_to_crush{ 0 };
		
		GRID(int number_of_bricks);

		void create_grid(int all_bricks);

		BRICK get_brick(int number);
		BRICK get_brick(D2D1_RECT_F brick_rect);

		int hit_brick(int number);

		void remove_brick(int number);
		void remove_brick(D2D1_RECT_F brick_rect);
	};

	class BREAKER_API PAD :public PROTON
	{
	private:
		pads type{ pads::normal };

		float speed = 5.0f;

		PAD(float _sx, float _sy);

	public:
		dirs dir = dirs::stop;

		void move(float gear, dirs to_where);

		pads get_pad() const;
		void set_pad(pads new_pad);

		void set_speed(float new_speed);

		void Release();

		static PAD* create(float sx, float sy);
	};

	class BREAKER_API BALL :public PROTON
	{
	private:
		float speed{ ball_default_speed };

		float delta_x{ 0 };
		float delta_y{ 0 };
		float vec_lenght{ 0 };

		float vec_next_x{ 0 };
		float vec_next_y{ 0 };

		float move_sx{ 0 };
		float move_ex{ 0 };
		float move_sy{ 0 };
		float move_ey{ 0 };

		int frame{ 0 };
		int max_frames{ 3 };
		int frame_delay{ 20 };
		int max_frame_delay{ 20 };

		BALL(float _sx, float _sy);

	public:

		float get_init_x() const;
		float get_init_y() const;
		float get_target_x() const;
		float get_target_y() const;

		int get_frame();

		void set_speed(float new_speed);

		void set_path(float target_x, float target_y);

		bumps move(float gear);

		void Release();

		static BALL* create(float sx, float sy);
	};

	class BREAKER_API FIELD
	{
	private:
		int frame = 0;
		int max_frames = 0;
		int frame_delay = 0;
		int max_frame_delay = 0;

		RANDIT _randerer{};

		FIELD(fields _what);

	public:
		fields type{ fields::fantasy };
		D2D1_RECT_F rect{ GAME_SCREEN };

		int get_frame();
		void Release();

		static FIELD* create(fields what);
	};

	class BREAKER_API ASSET :public PROTON
	{
	private:
		float speed{ 1.5f };

		ASSET(assets _what, float _sx, float _sy);

	public:
		assets type{};

		bool move(float gear);

		void Release();

		static ASSET* create(assets what, float sx, float sy);
	};

	void BREAKER_API BallDispatcher(BALL& Ball, bumps where, float bump_x, float bump_y, float pad_sx, float pad_ex);

	bool BREAKER_API Intersect(D2D1_RECT_F first, D2D1_RECT_F second);
}