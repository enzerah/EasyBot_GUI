#ifndef SCRIPTEDITORDIALOG_H
#define SCRIPTEDITORDIALOG_H

#include <QDialog>
#include "CodeEditor.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLineEdit>

class ScriptEditorDialog : public QDialog {
    Q_OBJECT

public:
    explicit ScriptEditorDialog(const QString &name, const QString &script, QWidget *parent = nullptr);

    QString getScript() const;
    QString getName() const;

private:
    QLineEdit *m_nameEdit;
    CodeEditor *m_codeEditor;
};

#endif // SCRIPTEDITORDIALOG_H
