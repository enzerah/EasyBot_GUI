#ifndef OTHERS_THREAD_H
#define OTHERS_THREAD_H
#include <QThread>
#include <QMutex>
#include <map>
#include "../../const.h"
#include "../../proto_functions_client.h"


class Others_Thread : public QThread {
    Q_OBJECT
    public:
    struct OtherData {
        int itemId;
        int condition;
        int value;
    };
    explicit Others_Thread(QObject *parent = nullptr)
        : QThread(parent){};
    ~Others_Thread() override = default;

    void updateCondition(const std::string& optionType, const int &itemId, const int &condition, const int &value, bool state);
    bool hasActiveOthers();

protected:
    void run() override;

    private:
    std::map<std::string, OtherData> activeOthers;
    QMutex mutex;

};



#endif //OTHERS_THREAD_H
