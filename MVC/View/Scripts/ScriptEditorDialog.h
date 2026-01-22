#ifndef SCRIPTEDITORDIALOG_H
#define SCRIPTEDITORDIALOG_H

#include <QDialog>
#include "CodeEditor.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QLineEdit>

class ScriptEditorDialog : public QDialog {
    Q_OBJECT

public:
    explicit ScriptEditorDialog(const QString &name, const QString &script, QWidget *parent = nullptr);

    QString getScript() const;
    QString getName() const;

private:
    void populateLists();
    QLineEdit *m_nameEdit;
    CodeEditor *m_codeEditor;

    QListWidget *m_gameList;
    QListWidget *m_localPlayerList;
    QListWidget *m_containerList;
    QListWidget *m_creatureList;
    QListWidget *m_itemList;
    QListWidget *m_mapList;
    QListWidget *m_thingList;
    QListWidget *m_tileList;
};

#endif // SCRIPTEDITORDIALOG_H
