#include "RunScripts_Thread.h"
#include "../../../../LuaEngine.h"
#include <iostream>
#include <regex>
#include <vector>
#include <QRegularExpression>

void RunScripts_Thread::run() {
    std::vector<LuaEngine*> luaEngines;
    
    // Create and start a thread for each enabled script
    for (const auto& script : m_scripts) {
        if (script.enabled) {
            std::string wrappedScript = "while true do\n";
            wrappedScript += "    sleep(" + std::to_string(script.sleepTime) + ")\n";
            wrappedScript += script.text + "\n";
            wrappedScript += "end";
            
            auto* luaEngine = new LuaEngine(wrappedScript, nullptr);
            luaEngines.push_back(luaEngine);
            luaEngine->start();
        }
    }
    
    std::cout << "Starting RunScripts_Thread with " << luaEngines.size() << " Lua scripts" << std::endl;
    
    // Wait until interruption is requested
    while (!isInterruptionRequested()) {
        msleep(10);
    }
    
    std::cout << "Stopping all Lua scripts..." << std::endl;
    
    // Request all Lua threads to stop
    for (auto* luaEngine : luaEngines) {
        luaEngine->requestStop();
    }
    
    // Wait for all Lua threads to finish (with timeout)
    for (auto* luaEngine : luaEngines) {
        if (!luaEngine->wait(1000)) { // 1 second timeout
            std::cerr << "Warning: Lua thread did not finish in time, terminating forcefully" << std::endl;
            luaEngine->terminate();
            luaEngine->wait();
        }
    }
    
    // Clean up
    for (auto* luaEngine : luaEngines) {
        delete luaEngine;
    }
    luaEngines.clear();
    
    std::cout << "RunScripts_Thread ended" << std::endl;
}
