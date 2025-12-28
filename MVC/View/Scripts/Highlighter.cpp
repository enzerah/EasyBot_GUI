#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    QTextCharFormat keywordFormat;
    // Keywords (Blue)
    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\band\\b" << "\\bbreak\\b" << "\\bdo\\b" << "\\belse\\b"
                    << "\\belseif\\b" << "\\bend\\b" << "\\bfor\\b" << "\\bfunction\\b"
                    << "\\bgoto\\b" << "\\bif\\b" << "\\bin\\b" << "\\blocal\\b"
                    << "\\bnil\\b" << "\\bnot\\b" << "\\bor\\b" << "\\brepeat\\b"
                    << "\\breturn\\b" << "\\bthen\\b" << "\\btrue\\b" << "\\buntil\\b"
                    << "\\bwhile\\b" << "\\bproto\\b" << "\\bOtc\\b" << "\\bmsleep\\b";
    
    for (const QString &pattern : keywordPatterns) {
        rules.append({QRegularExpression(pattern), keywordFormat});
    }

    QTextCharFormat stringFormat;
    // Strings (Dark Red)
    stringFormat.setForeground(Qt::darkRed);
    rules.append({
        QRegularExpression("\".*?\""),
        stringFormat
    });
    rules.append({
        QRegularExpression("'.*?'"),
        stringFormat
    });

    QTextCharFormat commentFormat;
    // Comments (Dark Green)
    commentFormat.setForeground(Qt::darkGreen);
    rules.append({
        QRegularExpression("--[^\n]*"),
        commentFormat
    });

    QTextCharFormat numberFormat;
    // Numbers (Magenta)
    numberFormat.setForeground(Qt::magenta);
    rules.append({
        QRegularExpression("\\b\\d+\\b"),
        numberFormat
    });
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const Rule &rule : rules) {
        auto it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            auto match = it.next();
            setFormat(match.capturedStart(),
                      match.capturedLength(),
                      rule.format);
        }
    }
}
