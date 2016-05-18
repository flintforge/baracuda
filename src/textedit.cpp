/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

/**

  text editor derived from the QT examples

  */

#include "textedit.h"
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QStringListModel>
#include <QScrollBar>
#include <QCompleter>
#include <QMessageBox>

TextEdit::TextEdit(QWidget *parent)
    : QTextEdit(parent), c(0)
{

    setTabStopWidth(20);

    // let's have some ascii fun
    QString imagetext;
    QFile file("baracuda.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        imagetext = in.readAll();
        file.close();
    }

    setFontFamily("Monospace");

    String title = String(
"█████╗  ████╗ █████╗  ████╗  ████╗██╗ ██╗█████╗  ████╗ \n"
"██╔═██╗██╔═██╗██╔═██╗██╔═██╗██╔══╝██║ ██║██╔═██╗██╔═██╗\n"
"█████╔╝██████║█████╔╝██████║██║   ██║ ██║██║ ██║██████║\n"
"██╔═██╗██╔═██║██╔═██╗██╔═██║██║   ██║ ██║██║ ██║██╔═██║\n"
"█████╔╝██║ ██║██║ ██║██║ ██║╚████╗╚████╔╝█████╔╝██║ ██║\n"
"╚════╝ ╚═╝ ╚═╝╚═╝ ╚═╝╚═╝ ╚═╝ ╚═══╝ ╚═══╝ ╚════╝ ╚═╝ ╚═╝\n"
"");
    setFontPointSize(10);
    //String introtext=
    setPlainText(String("\nv0.5 'generator' \n"
                        " a Cuda kernel compositor\n\n"
                        "The Code editor provides autocompletions. Trigger it with ")
                 + QKeySequence("Ctrl+E").toString(QKeySequence::NativeText)
                 + String("\n\nDrag blocks in the composer to start rendering it")
                 + String("\nDouble click a block in the composer to start rendering it")
                 );

    //setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    setFontPointSize(2);
    insertPlainText(imagetext);
    setFontPointSize(7);
    insertPlainText(title);


    QCompleter * completer = new QCompleter(this);
    completer->setModel(modelFromFile("wordlist.txt", completer));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    setCompleter(completer);
    highlighter = new Highlighter(this->document());
    txtChanged=false;

    QObject::connect(this, SIGNAL(textChanged()),
                     this, SLOT(notifyTextChanged()));
    setFontPointSize(10);
}

TextEdit::~TextEdit()
{
}


void TextEdit::setCompleter(QCompleter *completer)
{
    if (c)
        QObject::disconnect(c, 0, this, 0);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}
//! [2]

//! [3]
QCompleter *TextEdit::completer() const
{
    return c;
}
//! [3]

//! [4]
void TextEdit::insertCompletion(const QString& completion)
{
    qDebug()<< "Ctrl+E";
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}
//! [4]

//! [5]
QString TextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}
//! [5]

//! [6]
void TextEdit::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QTextEdit::focusInEvent(e);
}
//! [6]

//! [7]
void TextEdit::keyPressEvent(QKeyEvent *e)
{

    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }


    switch (e->key()) {
    case Qt::Key_F5:
        if(!txtChanged) {
            QMessageBox::warning(NULL, "No","no recompilation, the module hasn't changed",
                                  QMessageBox::Ok );
        }
        else if (txtChanged && brick != NULL) {
            qDebug()<< brick;
            QFileInfo * finfo = brick->getOp()->getModule()->fileInfo();
            qDebug() << finfo->fileName();
            String filename=getNextVersionNumFile(finfo);
            qDebug() << finfo->absoluteDir().absolutePath()+ "/" + filename;
            save ( filename , toPlainText());
            emit(UpdateBrick(brick,filename));
            txtChanged=false;
        }
    }


    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                      || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}

//! [8]


QAbstractItemModel *TextEdit::modelFromFile(const QString& fileName, QCompleter * completer)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        qDebug()<< "Wordlist " << fileName << " not found.";
        return new QStringListModel(completer);
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer);
}


// create kernel.X.cu with incremental X
String TextEdit::getNextVersionNumFile(FileInfo * f) {
    int i=0;
    QString fpath = f->absoluteFilePath();
    // shouldnt happen
    if(!QFile::exists(fpath)) {
        qWarning() << fpath << " doesnt exist. It may have been deleted";
        return f->fileName();
    }
    else {
        i=-1;
        QString str,nextname;
        do
        {   ++i;
            nextname = QString(__KERNEL_DIR)+"/"+f->completeBaseName()+"."+ str.setNum(i) +"."+f->completeSuffix();
        }while (QFile::exists(nextname));

        return nextname;
    }
}


bool TextEdit::save(String file, String src) {
    QFile data(file);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        out << src;
        data.close();
        qDebug()<< file << " saved";
        return true;
    }
    else qDebug()<< file << " not saved";
    return false;
   
}
