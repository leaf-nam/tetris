#include "engine/Engine.hpp"

int main()
{
    const char* argv[] = {"tetris"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    Engine engine(argc, argv);

    engine.run();

    engine.stop();

    engine.finish();

    return 0;
}
