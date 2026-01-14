#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#include "callback/callbacks.hpp"
#include "input/wpf_input_handler.hpp"
#include "render/wpf_renderer.hpp"
#include "network/wpf_network.hpp"
#include "engine/multi_engine.hpp"
#include "callback/multi_engine_wrapper.hpp"
#include <thread>

static Callbacks cbs;
static Engine* game_engine = nullptr;
static thread game_worker;

extern "C" 
{

	// C#(WPF)의 콜백함수 등록
	DLL_EXPORT void register_multi_callbacks(
		InputScanCallback scan_callback,
		RenderBackgroundCallback background_callback,
		RenderBoardCallback board_callback,
		RenderHoldCallback hold_callback,
		RenderNextBlockCallback next_callback,
		RenderTimerCallback timer_callback,
		RenderScoreCallback score_callback,
		RenderLevelCallback level_callback,
		RenderOtherBoardCallback other_board_callback,
		FinishCallback finish_callback,
		SendUdpCallback send_callback,
		ReceiveUdpCallback receive_callback
	) {
		cbs.scan_callback = scan_callback;
		cbs.background_callback = background_callback;
		cbs.board_callback = board_callback;
		cbs.hold_callback = hold_callback;
		cbs.next_block_callback = next_callback;
		cbs.timer_callback = timer_callback;
		cbs.score_callback = score_callback;
		cbs.level_callback = level_callback;
		cbs.other_board_callback = other_board_callback;
		cbs.finish_callback = finish_callback;
		cbs.send_callback = send_callback;
		cbs.receive_callback = receive_callback;
	}

	// 반드시 콜백 등록(register_callback) 후 시작
	DLL_EXPORT void init_multi_engine()
	{

		auto* inputHandler = new WpfInputHandler();
		auto* renderer = new WpfRenderer();
		auto* network = new WpfNetwork();

		inputHandler->set_callback(&cbs);
		renderer->set_callback(&cbs);
		network->set_callback(&cbs);

		game_engine = new MultiEngine(inputHandler, renderer, network);
	}

	// 쓰레드 생성 후 메인함수 실행
	DLL_EXPORT void run_multi_engine()
	{
		game_worker = thread([]()
			{
				if (!game_engine) return;
				game_engine->run();
				cbs.finish_callback();
			});
	}

	DLL_EXPORT void stop_multi_engine()
	{
		game_engine->stop();
	}

	// 포인터 정리
	DLL_EXPORT void finish_multi_engine()
	{
		game_engine->finish();
		game_engine = nullptr;
		
		cbs.scan_callback = nullptr;
		cbs.background_callback = nullptr;
		cbs.board_callback = nullptr;
		cbs.hold_callback = nullptr;
		cbs.next_block_callback = nullptr;
		cbs.timer_callback = nullptr;
		cbs.score_callback = nullptr;
		cbs.level_callback = nullptr;
		cbs.other_board_callback = nullptr;
		cbs.finish_callback = nullptr;
		cbs.send_callback = nullptr;
		cbs.receive_callback = nullptr;

		if (game_worker.joinable()) {
			game_worker.join();
		}
	}
}