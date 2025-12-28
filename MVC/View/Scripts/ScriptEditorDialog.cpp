#include "ScriptEditorDialog.h"
#include "Highlighter.h"

ScriptEditorDialog::ScriptEditorDialog(const QString &script, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Script Editor");
    resize(700, 600);

    auto *layout = new QVBoxLayout(this);

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
