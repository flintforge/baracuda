/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.

 Source  Highlighter for cuda

*/

#include "highlighter.h"


Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::yellow);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    // shall we use wordlist.txt ?
    QStringList keywords;
    keywords
    <<"_gpu_"
    <<"__device__"
    <<"__global__"
    <<"atomicAdd"
    <<"blockDim"
    <<"blockIdx"
    <<"bool"
    <<"break"
    <<"case"
    <<"char"
    <<"class"
    <<"const"
    <<"continue"
    <<"double"
    <<"else"
    <<"enum"
    <<"extern"
    <<"false"
    <<"friend"
    <<"float"
    <<"for"
    <<"goto"
    <<"gridDim"
    <<"if"
    <<"inline"
    <<"int"
    <<"long"
    <<"namespace"
    <<"operator"
    <<"private"
    <<"protected"
    <<"public"
    <<"return"
    <<"rgba8"
    <<"short"
    <<"signed"
    <<"sizeof"
    <<"static"
    <<"struct"
    <<"switch"
    <<"template"
    <<"this"
    <<"threadIdx"
    <<"true"
    <<"typedef"
    <<"typename"
    <<"unsigned"
    <<"uint"
    <<"uchar4"
    <<"void"
    <<"volatile"
    <<"while";

    foreach (QString s, keywords)
        keywordPatterns << "\\b"+s+"\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);
//! [3]

//! [4]
    quotationFormat.setForeground(Qt::green);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    //functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::yellow);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}


void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
