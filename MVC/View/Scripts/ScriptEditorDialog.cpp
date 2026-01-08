#include "ScriptEditorDialog.h"
#include "Highlighter.h"

#include <QLineEdit>
#include <QLabel>

ScriptEditorDialog::ScriptEditorDialog(const QString &name, const QString &script, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Script Editor");
    resize(700, 600);

    auto *layout = new QVBoxLayout(this);

    auto *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Script Name:", this));
    m_nameEdit = new QLineEdit(name, this);
    nameLayout->addWidget(m_nameEdit);
    layout->addLayout(nameLayout);

    m_codeEditor = new CodeEditor(this);
    m_codeEditor->setPlainText(script);
    new Highlighter(m_codeEditor->document());

    layout->addWidget(m_codeEditor);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(buttonBox);
}

QString ScriptEditorDialog::getScript() const {
    return m_codeEditor->toPlainText();
}

QString ScriptEditorDialog::getName() const {
    return m_nameEdit->text();
}
