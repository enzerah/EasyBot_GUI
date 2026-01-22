#include "LuaEngine.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <regex>
#include <sstream>


void LuaEngine::run() {
    initLua();

    std::string scriptToExecute = m_script;
    if (m_script.size() > 4 && m_script.substr(m_script.size() - 4) == ".lua") {
        QString path = QDir::current().absoluteFilePath("Save/" + QString::fromStdString(m_script));
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            scriptToExecute = in.readAll().toStdString();
            std::cout << "Loading script from file: " << path.toStdString() << std::endl;
        } else {
            std::cerr << "Could not open script file: " << path.toStdString() << std::endl;
            // Optionally: execute m_script as raw lua if file not found, 
            // but usually a .lua filename isn't valid lua code alone.
        }
    }

    executeLuaScript(preprocess(scriptToExecute));
    closeLua();
}

void LuaEngine::initLua() {
    std::cout << "Creating Lua" << std::endl;
    L = luaL_newstate();
    luaL_openlibs(L);
    
    lua_pushlightuserdata(L, this);
    lua_setfield(L, LUA_REGISTRYINDEX, "LuaEngine_instance");
    
    LuaBindings::registerAll(L);

    // Globalize proto functions
    const char* globalizeSnippet =
        "setmetatable(_G, {"
        "  __index = function(_, key)"
        "    local val = proto[key]"
        "    if type(val) == 'function' then"
        "      return function(...) return val(proto, ...) end"
        "    end"
        "    return val"
        "  end"
        "})";

    if (luaL_dostring(L, globalizeSnippet) != LUA_OK) {
        std::cerr << "Globalize Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
    
    // Set hook to check for interruption every 100 instructions
    lua_sethook(L, luaHookCallback, LUA_MASKCOUNT, 100);
}

void LuaEngine::closeLua() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
    std::cout << "Closing Lua" << std::endl;
}



void LuaEngine::executeLuaScript(const std::string &scriptText) {
    if (!L) return;
    
    m_returnedString.clear();  // Reset before execution
    
    if (luaL_dostring(L, scriptText.c_str()) != LUA_OK) {
        const char* error = lua_tostring(L, -1);
        // Don't print error if it was intentional interruption
        if (std::string(error) != "Script interrupted") {
            std::cerr << "Lua Error: " << error << std::endl;
        }
        lua_pop(L, 1);
    } else {
        // Check if script returned a value
        if (lua_gettop(L) > 0) {
            // Check if the returned value is a string
            if (lua_isstring(L, -1)) {
                m_returnedString = lua_tostring(L, -1);
                std::cout << "Lua script returned: '" << m_returnedString << "'" << std::endl;
            }
            lua_pop(L, 1);  // Pop the return value
        }
    }
}

void LuaEngine::requestStop() {
    m_shouldStop.store(true);
}

void LuaEngine::luaHookCallback(lua_State* L, lua_Debug* ar) {
    // Retrieve the LuaEngine instance from registry
    lua_getfield(L, LUA_REGISTRYINDEX, "LuaEngine_instance");
    LuaEngine* engine = static_cast<LuaEngine*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    
    if (engine && engine->m_shouldStop.load()) {
        // Throw a Lua error to stop execution
        luaL_error(L, "Script interrupted");
    }
}

std::string LuaEngine::preprocess(const std::string& script) {
    std::istringstream stream(script);
    std::string line;
    std::string result;
    int blockLevel = 0;

    // Regex to match if/elseif/while/for/function/repeat
    std::regex blockStartRegex(R"(^\s*(if|elseif|while|for|function|repeat)\b)", std::regex_constants::icase);
    // Regex to match then/do/until/end
    std::regex blockEndRegex(R"(\b(then|do|until|end)\s*$)", std::regex_constants::icase);
    // Regex to match end specifically
    std::regex endRegex(R"(^\s*end\b)", std::regex_constants::icase);
    // Regex to match repeat specifically
    std::regex repeatRegex(R"(^\s*repeat\b)", std::regex_constants::icase);

    while (std::getline(stream, line)) {
        std::smatch match;
        std::string trimmedLine = line;
        trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t\r\n"));
        trimmedLine.erase(trimmedLine.find_last_not_of(" \t\r\n") + 1);

        if (trimmedLine.empty()) {
            result += line + "\n";
            continue;
        }

        // Check for end
        if (std::regex_search(trimmedLine, match, endRegex)) {
            blockLevel--;
        } else if (std::regex_search(trimmedLine, match, blockStartRegex)) {
            std::string keyword = match[1];
            bool hasBlockEnd = std::regex_search(trimmedLine, match, blockEndRegex);

            if (keyword == "if" || keyword == "elseif") {
                if (!hasBlockEnd && trimmedLine.find(" then") == std::string::npos && trimmedLine.back() != ':') {
                    line += " then";
                }
            } else if (keyword == "while" || keyword == "for") {
                if (!hasBlockEnd && trimmedLine.find(" do") == std::string::npos) {
                    line += " do";
                }
            }

            if (keyword != "elseif") {
                 blockLevel++;
            }
        }

        result += line + "\n";
    }

    // Close remaining blocks
    while (blockLevel > 0) {
        result += "end\n";
        blockLevel--;
    }

    return result;
}
