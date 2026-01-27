#ifndef BOTENGINE_H
#define BOTENGINE_H
#include <vector>
#include <mutex>

#define engine BotEngine::getInstance()

class BotEngine {
private:
    static BotEngine* instance;
    static std::mutex mutex;
protected:
    BotEngine();
    ~BotEngine(){}
public:
    BotEngine(BotEngine const&) = delete;
    void operator=(const BotEngine&) = delete;
    static BotEngine* getInstance();

    // Engine variables
    bool hasTarget = false;
    bool isLooting = false;
    bool isWalking = false;

    // Client Custom Functions
    std::vector<int> countItems(std::vector<int> itemId);
    void depositItems(std::vector<int> itemId, std::string containerToDeposit);
    void findDeposit(std::vector<int> depoId);
};



#endif //BOTENGINE_H
