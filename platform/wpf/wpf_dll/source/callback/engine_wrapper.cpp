#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#include "callback/callbacks.hpp"
#include "input/wpf_input_handler.hpp"
#include "render/wpf_renderer.hpp"
#include "engine/engine.hpp"
#include "callback/engine_wrapper.hpp"
#include <thread>

static Callbacks cbs;
static Engine* game_engine = nullptr;
static thread game_worker;

extern "C" 
{

	// C#(WPF)의 콜백함수 등록
	DLL_EXPORT void register_callbacks(
		InputScanCallback scan_callback,
		RenderBackgroundCallback background_callback,
		RenderBoardCallback board_callback,
		RenderTimerCallback timer_callback,
		RenderScoreCallback score_callback,
		RenderLevelCallback level_callback
	) {
		cbs.scan_callback = scan_callback;
		cbs.background_callback = background_callback;
		cbs.board_callback = board_callback;
		cbs.timer_callback = timer_callback;
		cbs.score_callback = score_callback;
		cbs.level_callback = level_callback;
	}

	// 반드시 콜백 등록(register_callback) 후 시작
	DLL_EXPORT void init_engine()
	{

		auto* inputHandler = new WpfInputHandler();
		auto* renderer = new WpfRenderer();

		inputHandler->set_callback(&cbs);
		renderer->set_callback(&cbs);

		game_engine = new Engine(inputHandler, renderer);
	}

	// 쓰레드 생성 후 메인함수 실행
	DLL_EXPORT void run_engine()
	{
		game_worker = thread([]()
			{
				if (!game_engine) return;
				game_engine->run();
			});
	}

	DLL_EXPORT void stop_engine()
	{
		game_engine->stop();
	}

	// 포인터 정리
	DLL_EXPORT void finish_engine()
	{
		game_engine->finish();
		game_engine = nullptr;

		cbs.scan_callback = nullptr;
		cbs.background_callback = nullptr;
		cbs.board_callback = nullptr;
		cbs.timer_callback = nullptr;
		cbs.score_callback = nullptr;
		cbs.level_callback = nullptr;

		if (game_worker.joinable())
			game_worker.join();
	}
}