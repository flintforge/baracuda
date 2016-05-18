/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

/**

  A source editor derived from Textedit

 */


#include "sourceedit.h"

SourceEdit::SourceEdit(QWidget *parent): TextEdit(parent)
{
    QObject::connect(this, SIGNAL(textChanged()),
                     this, SLOT(notifyTextChanged()));
}

void SourceEdit::notifyTextChanged() {
    qDebug()<<"text changed";
}


void SourceEdit::keyPressEvent(QKeyEvent *e)
{
    TextEdit::keyPressEvent(e);

    switch (e->key()) {

    case Qt::Key_F5:
        if(editingModule != NULL) {
            qDebug() << getNextVersionNumFile(editingModule->fileInfo());
        }
        /*
        save("kernels/autosave/"+newfileName+".cu", this->toPlainText());
        CudaCompiler compiler;
        editingModule  = compiler.createGPUModuleFromFile(newfileName);
        */
        break;
    }
}

