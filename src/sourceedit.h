/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */
#ifndef SOURCEEDIT_H
#define SOURCEEDIT_H

#include "textedit.h"
#include "cudacompil.h"
class SourceEdit : public TextEdit
{
     Q_OBJECT
public:
    SourceEdit(QWidget *parent = 0);
    void setEditingModule(GPUModule * module) { editingModule = module; setText( module->getSrc()); }

protected:
    void keyPressEvent(QKeyEvent *e);
    GPUModule * editingModule;
    CudaCompiler * compiler;

public slots:
    void notifyTextChanged();
};

#endif // SOURCEEDIT_H
