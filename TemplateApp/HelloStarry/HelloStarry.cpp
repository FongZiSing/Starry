// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     HelloStarry.cpp
//

#include "Starry/Starry.hpp"

#include <graphics.h>
#include <format>
#include <random>
#include <Windows.h>



class scene_canvas
{
private:
	se::vec2i_t window_size;
	se::vec2i_t canvas_size;
	se::vec2i_t origin;
	se::vec2_t dpi;

public:
	scene_canvas(se::vec2i_t in_window_size, se::vec2i_t in_canvas_size, se::vec2i_t in_scene_size)
		: window_size{ in_window_size }
		, canvas_size{ in_canvas_size }
		, origin{ (in_window_size - in_canvas_size) / 2 }
		, dpi{ (se::vec2_t)in_canvas_size / (se::vec2_t)in_scene_size }
	{
		::initgraph(window_size.x, window_size.y);
		::setorigin(origin.x, origin.y);
		::setbkcolor(RGB(0, 0, 0));
		::settextcolor(RGB(0, 255, 0));
		::setfillcolor(RGB(255, 255, 255));
	}

	~scene_canvas()
	{
		::closegraph();
	}

	void begin_paint()
	{
		::BeginBatchDraw();
		::cleardevice();
	}

	void draw_circle(se::vec2_t position, COLORREF color, float radius)
	{
		//::setfillcolor(color);
		se::vec2i_t p = (position * dpi);
		p.y = canvas_size.y - p.y;
		::solidcircle(p.x, p.y, (int)radius);
	}

	void draw_text(LPCTSTR text)
	{
		// TODO
		RECT rect{ -800, -100, 256, 24 };
		::drawtext(text, &rect, DT_CENTER);
	}

	void end_paint()
	{
		::EndBatchDraw();
	}

	bool exit()
	{
		return false;
		// TODO: getmessage 有时候会卡很久，需要换一种方式捕获事件
		//ExMessage m = ::getmessage(EX_MOUSE | EX_KEY);
		//return m.message == WM_KEYDOWN && m.vkcode == VK_ESCAPE;
	}
};



struct particle_generator
{
	std::vector<se::particle_t> operator() (se::vec2i_t scene_size)
	{
		int particle_num = 10000;
		std::vector<se::particle_t> particles;
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
				se::particle_t
				{
					{ x, y }, // 粒子初始位置
					{ 0, 0 }, // 例子初始速度
					{  }      // 预留给用户自定义的属性
				}
			);
		}

		return particles;
	}
};



struct particle_position_updater
{
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<float> distribution{ -1.f, 1.f };

	particle_position_updater()
	{
		new (&gen) std::mt19937(rd());
	}

	void operator() (se::particle_t::position_t& position)
	{
		position += { distribution(gen), distribution(gen) };
	}
};



struct particle_renderer
{
	scene_canvas& canvas;

	particle_renderer(scene_canvas& in_canvas) noexcept
		: canvas{ in_canvas }
	{}

	void operator() (se::vec2_t const& position) noexcept
	{
		canvas.draw_circle(position, RGB(255, 255, 255), 2);
	}
};



int main()
{
	se::vec2i_t scene_size{ 1000, 1000 };  // 场景大小

	//********************************
	// Step 1: Creates a canvas for visualization.
	//         创建一个画布，用于绘制绘制结果
	//********************************
	se::vec2i_t window_size{ 1600, 1080 }; // 窗口大小
	se::vec2i_t canvas_size{ 800, 800 };   // 位于窗口中间的画布的大小
	scene_canvas canvas{ window_size, canvas_size, scene_size };


	//********************************
	// Step 2: Prepares a virtual scene for simulation.
	//         创建一个场景
	//********************************
	se::scene2d_t scene = se::make_scene(scene_size);


	//********************************
	// Step 3: Generates particle here.
	//         在场景中生成粒子
	//********************************
	scene.spawn_particle(particle_generator{});


	//********************************
	// Step 4: Executes rendering loop.
	//         循环渲染
	//********************************
	particle_position_updater position_updater;
	while (!canvas.exit())
	{
		canvas.begin_paint();


		//********************************
		// Step 5: Update scene.
		//         更新场景
		//********************************
		{
			long long nanoseconds = scene.tick<se::visitor_flag::position_only>(position_updater);

			// 输出时间
			std::wstring microseconds = std::format(L"update time:   {:.5f} ms", nanoseconds / 1000000.0);
			canvas.draw_text(microseconds.c_str());
		}


		//********************************
		// Step 6: Render scene.
		//         绘制场景
		//********************************
		scene.render<se::visitor_flag::position_only>(particle_renderer{ canvas });


		canvas.end_paint();
	}

	return 0;
}
