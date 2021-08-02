#include "../external/ChiraEngine/src/core/engine.h"
#include "../external/ChiraEngine/src/render/freecam.h"
#include "../external/ChiraEngine/src/loader/objMeshLoader.h"
#include "../external/ChiraEngine/src/render/unlitMaterial.h"

int main() {
    engine engine;
    virtualFileSystem::addResourceDirectory("resources/editor/");

    engine.getSettingsLoader()->setValue("engine", "title", std::string("Chira Editor"), true, true);
    objMeshLoader meshLoader;

#if DEBUG
    engine.addKeybind(keybind(GLFW_KEY_ESCAPE, GLFW_PRESS, [](class engine* e) {
        e->stop();
    }));
#endif

    engine::addShader("unlit", new shader{"unlit.vsh", "unlit.fsh"});
    engine::addMaterial("unlit", new unlitMaterial{"unlit"});
    engine::addMesh("teapot", new mesh{&meshLoader, "teapot.obj", "unlit"});

    engine.addInitFunction([](class engine* e) {
        // todo: make own camera class
        e->setWorld(new world{e, new freecam{e}});
        e->getWorld()->addMesh("teapot");
        engine::setBackgroundColor(0.9098f, 0.9137f, 0.9098f, 1.0f);
        e->captureMouse(false);
    });
    engine.init();

    engine.addRenderFunction([](class engine* e) {
        engine::getMesh("teapot")->render();
    });
    engine.run();
    return 0;
}
