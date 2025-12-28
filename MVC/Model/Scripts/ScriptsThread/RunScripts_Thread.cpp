#include "RunScripts_Thread.h"
#include <iostream>
#include <regex>
#include <QRegularExpression>

void RunScripts_Thread::run() {
    std::cout << "Running Scripts_Thread" << std::endl;
    for (const auto& script : m_scripts) {
        if (script.enabled) {
            executeScript(script.text);
        }
    }
    while (!isInterruptionRequested()) {
        msleep(100);
    }
}

void RunScripts_Thread::executeScript(const std::string& scriptText) {
    QString qScript = QString::fromStdString(scriptText);
    QStringList lines = qScript.split(QRegularExpression("[\n;]"), Qt::SkipEmptyParts);
    for (const QString& line : lines) {
        if (isInterruptionRequested()) return;
        parseAndExecute(line.trimmed().toStdString());
    }
}

void RunScripts_Thread::parseAndExecute(const std::string& line) {
    QString qLine = QString::fromStdString(line);
    if (qLine.isEmpty() || qLine.startsWith("//")) return;

    // Handle proto->talk("message")
    static QRegularExpression talkRegex(R"(proto\s*->\s*talk\s*\(\s*\"(.*)\"\s*\)\s*)");
    auto talkMatch = talkRegex.match(qLine);
    if (talkMatch.hasMatch()) {
        QString message = talkMatch.captured(1);
        proto->talk(message.toStdString());
        return;
    }

    // Handle msleep(ms)
    static QRegularExpression sleepRegex(R"(msleep\s*\(\s*(\d+)\s*\)\s*)");
    auto sleepMatch = sleepRegex.match(qLine);
    if (sleepMatch.hasMatch()) {
        int ms = sleepMatch.captured(1).toInt();
        msleep(ms);
        return;
    }
}
