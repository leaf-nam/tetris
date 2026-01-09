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
	void register_callbacks(
		const char* (*scan_callback)(),
		void (*background_callback)(int),
		void (*board_callback)(int),
		void (*timer_callback)(int),
		void (*next_block_callback)(int),
		void (*hold_callback)(int),
		void (*score_callback)(int),
		void (*level_callback)(int)
	) {
		cbs.scan_callback = scan_callback;
		cbs.background_callback = background_callback;
		cbs.board_callback = board_callback;
		cbs.timer_callback = timer_callback;
		cbs.next_block_callback = next_block_callback;
		cbs.hold_callback = hold_callback;
		cbs.score_callback = score_callback;
		cbs.level_callback = level_callback;
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