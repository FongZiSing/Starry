// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     HelloStarry.cpp
//

#include "Starry/Starry.hpp"

#include <graphics.h>
#include <format>
#include <random>
#include <chrono>
#include <array>

#define NOMINMAX
#include <Windows.h>



class scene_canvas
{
private:
	se::vec2i window_size;
	se::vec2i canvas_size;
	se::vec2i origin;
	se::vec2 dpi;
	RECT rect_for_text; // TODO

public:
	scene_canvas(se::vec2i in_window_size, se::vec2i in_canvas_size, se::vec2i in_scene_size)
		: window_size{ in_window_size }
		, canvas_size{ in_canvas_size }
		, origin{ (in_window_size - in_canvas_size) / 2 }
		, dpi{ (se::vec2)in_canvas_size / (se::vec2)in_scene_size }
		, rect_for_text{ -390, -130, 512, 24 }
	{
		::initgraph(window_size.x, window_size.y);
		::setorigin(origin.x, origin.y);
		::setbkcolor(RGB(0, 0, 0));
		::settextcolor(RGB(0, 255, 0));
	}

	~scene_canvas()
	{
		::closegraph();
	}

	void begin_paint()
	{
		::BeginBatchDraw();
		::cleardevice();
		rect_for_text = { -390, -130, -390 + 512, -130 + 24 };
	}

	se::vec2 put_circle(se::vec2i screen_position, COLORREF color, float radius)
	{
		screen_position -= origin;
		// 设置 linecolor 帧率会降低很多，影响调试，暂时用白色
		// 后续换成其他库，不再使用 easyx，有两个选择：
		//     1. raylib
		//     2. 使用 vulkan/directx 自己写一个简单的可视化工具，因为 raylib 没有充分使用 instanced draw 的方式
		// 
		//COLORREF old_color = getlinecolor();
		//::setlinecolor(color);
		::circle(screen_position.x, screen_position.y, int(radius * dpi.x));
		//setlinecolor(old_color);

		screen_position.y = canvas_size.y - screen_position.y;
		se::vec2 position = se::vec2(screen_position) / dpi;
		return position;
	}

	void draw_square(se::vec2 position, COLORREF color, float size)
	{
		::setfillcolor(color);
		se::vec2i p = (position * dpi);
		p.y = canvas_size.y - p.y;
		int r = (int)size;
		::fillrectangle(p.x - r, p.y - r, p.x + r, p.y + r);
	}

	void draw_text(LPCTSTR text)
	{
		::drawtext(text, &rect_for_text, DT_LEFT | DT_EXPANDTABS);
		::OffsetRect(&rect_for_text, 0, 24);
	}

	void end_paint()
	{
		::EndBatchDraw();
	}

	bool exit()
	{
		ExMessage m;
		if (::peekmessage(&m, EX_KEY))
		{
			return m.vkcode == VK_ESCAPE;
		}
		return false;
	}
};



class moving_average
{
	static constexpr long long size = 60;
	long long sum = 0;
	long long index = 0;
	std::array<long long, size> queue;


public:
	moving_average()
	{
		for (long long i = 0; i < size; ++i)
		{
			queue[i] = 0;
		}
	}

	long long next(long long val)
	{
		sum -= queue[index % size];
		queue[index % size] = val;
		sum += val;
		index++;
		return sum / ((index < size) ? index : size);
	}
};



template <std::size_t n>
struct trace_cpu_profiler_tag
{
	wchar_t str[n];
	consteval trace_cpu_profiler_tag(const wchar_t(&in_str)[n]) { for (std::size_t i = 0; i < n; ++i) str[i] = in_str[i]; }
};

template<trace_cpu_profiler_tag tag>
struct trace_cpu_profiler
{
	std::chrono::high_resolution_clock::time_point start;
	std::wstring_view title;
	scene_canvas& canvas;

	trace_cpu_profiler(std::wstring_view in_title, scene_canvas& in_canvas)
		: start(std::chrono::high_resolution_clock::now())
		, title(in_title)
		, canvas(in_canvas)
	{}

	~trace_cpu_profiler()
	{
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

		static moving_average averages;
		long long nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		long long nanoseconds_mean = averages.next(nanoseconds);
		std::wstring milliseconds = std::format(L"{}:\t {:0>9.5f} ms", title, nanoseconds / 1000000.0);
		std::wstring milliseconds_mean = std::format(L"{} avr:\t {:0>9.5f} ms", title, nanoseconds_mean / 1000000.0);
		canvas.draw_text(milliseconds.c_str());
		canvas.draw_text(milliseconds_mean.c_str());
	}
};

#define TRACE_CPU_PROFILER(_name, _canvas) trace_cpu_profiler<L##_name> __profiler { L##_name, _canvas };



//********************************
// Define user particle.
// 定义用户的粒子具有哪些属性
//********************************
struct my_particle_t
{
	se::vec2 position;
	se::vec2 velocity;
	se::vec2 F;
	se::vec2 Q;

	//********************************
	// Config particle's attributes.
	// 配置粒子具有哪些属性
	//********************************
	consteval static auto config()
	{
		return se_register_particle_attributes(
			se_make_position(&my_particle_t::position),
			se_make_attribute("velocity", &my_particle_t::velocity),
			se_make_attribute("Q", &my_particle_t::Q)
		);
	}
};



struct particle_generator
{
	std::vector<my_particle_t> operator() (se::vec2i scene_size)
	{
		int particle_num = 10'001;
		std::vector<my_particle_t> particles;
		particles.reserve(particle_num);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> distribution{ 0.f, 1.f };

		// Generate particle randomly.
		// 随机生成粒子
		while (particle_num-- > 0)
		{
			float x = distribution(gen) * scene_size.x;
			float y = distribution(gen) * scene_size.y;
			particles.push_back(
				my_particle_t
				{
					{ x, y }, // 粒子初始位置
					{ 0, 0 }, // 粒子初始速度
					0, // F
					0, // Q
				}
			);
		}

		return particles;
	}
};



struct particle_position_updater
{
	const se::vec2 scene_size;
	const se::grid2d_accelerator* accel;

	particle_position_updater(const se::vec2i& in_scene_size, const se::grid2d_accelerator* in_accelerator)
		: scene_size(in_scene_size)
		, accel(in_accelerator)
	{
	}

	void operator() (std::size_t index, se::vec2& position, se::vec2& velocity, se::vec2& Q)
	{
		se::vec2 F = 0;
		accel->query_near_of(index, position, 5, [&](int index, float distance_squared, se::vec2 const& nearly_position)
			{
				float distance = sqrtf(distance_squared);
				if (distance_squared < 25)
				{
					distance_squared = 25;
					distance = 5;
				}
				
				se::vec2 factor1 = (nearly_position - position) / distance;
				constexpr float KE = 10;
				constexpr float Q = 5;
				float factor2 = KE * Q * Q / distance_squared;
				F += -factor1 * factor2;
			});

		// Collides the boundary.
		position.x = std::clamp(position.x, 0.f, scene_size.x);
		position.y = std::clamp(position.y, 0.f, scene_size.y);

		// Update velocity.
		constexpr float dt = 0.1f;
		velocity.x += F.x * 0.2f * dt;
		velocity.y += F.y * 0.2f * dt;

		// Inverts the velocity if outside the scene.
		se::math::chgsign_if(&velocity.x, position.x <= 0 || position.x >= scene_size.x);
		se::math::chgsign_if(&velocity.y, position.y <= 0 || position.y >= scene_size.y);

		// Update position.
		position.x += velocity.x * dt;
		position.y += velocity.y * dt;
	}
};



int main()
{
	se::vec2i scene_size{ 1000, 1000 };  // 场景大小

	//********************************
	// Step 1: Creates a canvas for visualization.
	//         创建一个画布，用于绘制绘制结果
	//********************************
	se::vec2i window_size{ 1600, 1080 }; // 窗口大小
	se::vec2i canvas_size{ 800, 800 };   // 位于窗口中间的画布的大小
	scene_canvas canvas{ window_size, canvas_size, scene_size };


	//********************************
	// Step 2: Prepares a virtual scene for simulation.
	//         创建一个场景
	//********************************
	se::scene2d scene = se::make_scene(scene_size);
	const se::grid2d_accelerator* accel = scene.get_accelerator();

	//********************************
	// Step 3: Generates particle here.
	//         在场景中生成粒子
	//********************************
	scene.generate_particle<my_particle_t>(particle_generator{});


	//********************************
	// Step 4: Executes rendering loop.
	//         循环渲染
	//********************************
	particle_position_updater position_updater{ scene_size, accel };
	while (!canvas.exit())
	{
		canvas.begin_paint();


		//********************************
		// Step 5: Update scene.
		//         更新场景
		//********************************
		{
			TRACE_CPU_PROFILER("querier rebuild time", canvas);
			scene.begin_update<my_particle_t>();
		}
		{
			TRACE_CPU_PROFILER("update time", canvas);
			scene.update_for_each<my_particle_t>(position_updater);
		}
		{
			// TODO
			scene.end_update<my_particle_t>();
		}


		//********************************
		// Step 6: Render scene.
		//         绘制场景
		//********************************
		{
			TRACE_CPU_PROFILER("render time", canvas);
			std::span<const se::vec2> positions = scene.query_any_of<my_particle_t, "position">();
			for (se::vec2 const& position : positions)
			{
				canvas.draw_square(position, RGB(255, 255, 255), 2);
			}
		}

		{
			//// 获取一条鼠标或按键消息
			MOUSEMSG m;
			PeekMouseMsg(&m);
			TRACE_CPU_PROFILER("query time", canvas);
			se::vec2 position = canvas.put_circle(se::vec2(m.x, m.y), RED, 50);
			accel->query_near_of(-1, position, 50, [&canvas](int, float, se::vec2 const& nearly_position)
				{
					canvas.draw_square(nearly_position, GREEN, 2);
				});
		}

		canvas.end_paint();
	}

	return 0;
}
