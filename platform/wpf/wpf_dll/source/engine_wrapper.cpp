#include "callbacks.hpp"
#include "input/wpf_input_handler.hpp"
#include "render/wpf_renderer.hpp"
#include "engine/engine.hpp"
#include "engine_wrapper.hpp"
#include <thread>

static Callbacks cbs;
static Engine* game_engine = nullptr;
static thread game_worker;

extern "C" 
{

	// C#(WPF)의 콜백함수 등록
	void register_callback(const Callbacks cbs_)
	{
		cbs = cbs_;
	}

	// 반드시 콜백 등록(register_callback) 후 시작
	void init_engine() 
	{

		auto* inputHandler = new WpfInputHandler();
		auto* renderer = new WpfRenderer();

		inputHandler->set_callback(cbs);
		renderer->set_callback(cbs);

		Engine* game = new Engine(inputHandler, renderer);
	}

	// 쓰레드 생성 후 메인함수 실행
	void run_engine() 
	{
		game_worker = thread([]()
			{
				game_engine->run();
			});
	}

	void stop_engine() 
	{
		game_engine->stop();
	}

	void finish_engine() 
	{
		game_engine->finish();
	}
}