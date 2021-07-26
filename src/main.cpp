#include "../external/ChiraEngine/src/core/engine.h"
#include "../external/ChiraEngine/src/render/freecam.h"
#include "../external/ChiraEngine/src/loader/objMeshLoader.h"

int main() {
    engine engine;
    virtualFileSystem::addResourceDirectory("resources/editor/");
    engine.getSettingsLoader()->setValue("engine", "title", std::string("Chira Editor"), true, true);
    objMeshLoader meshLoader;

    freecam player{&engine}; // todo: make own camera class
    engine.setCamera(&player);

#if DEBUG
    engine.addKeybind(keybind(GLFW_KEY_ESCAPE, GLFW_PRESS, [](class engine* e) {
        e->stop();
    }));
#endif

    engine.addShader("unlit", new shader("unlit.vsh", "unlit.fsh"));
    engine.addMesh("teapot", new mesh(&meshLoader, "teapot.obj"));

    engine.addInitFunction([](class engine* e) {
        engine::setBackgroundColor(0.9098f, 0.9137f, 0.9098f, 1.0f);
        e->captureMouse(false);
    });
    engine.init();

    engine.addRenderFunction([](class engine* e) {
        e->getMesh("teapot")->render(e->getShader("unlit"));
    });
    engine.run();
    return 0;
}
