#ifndef SCRIPTEDITORDIALOG_H
#define SCRIPTEDITORDIALOG_H

#include <QDialog>
#include "CodeEditor.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>

class ScriptEditorDialog : public QDialog {
    Q_OBJECT

public:
    explicit ScriptEditorDialog(const QString &script, QWidget *parent = nullptr);

    QString getScript() const;

private:
    CodeEditor *m_codeEditor;
};

#endif // SCRIPTEDITORDIALOG_H
