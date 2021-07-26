#include "../external/ChiraEngine/src/core/engine.h"
#include "../external/ChiraEngine/src/render/freecam.h"

int main() {
    engine engine;
    virtualFileSystem::addResourceDirectory("resources/editor/");
    engine.getSettingsLoader()->setValue("engine", "title", std::string("Chira Editor"), true, true);

    freecam player{&engine}; // todo: make own camera class
    engine.setCamera(&player);

#if DEBUG
    engine.addKeybind(keybind(GLFW_KEY_ESCAPE, GLFW_PRESS, [](class engine* e) {
        e->stop();
    }));
#endif

    engine.addInitFunction([&player](class engine* e) {
        engine::setBackgroundColor(0.9098f, 0.9137f, 0.9098f, 1.0f);
        e->captureMouse(false);
        player.init(e);
    });
    engine.init();
    engine.run();
    return 0;
}
