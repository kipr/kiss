#ifndef ADD_TO_PROJECT_DIALOG_HPP
#define ADD_TO_PROJECT_DIALOG_HPP

#include <QDialog>

#include "ui_AddToProjectDialog.h"

class AddToProjectDialog : public QDialog, private Ui::AddToProjectDialog
{
   Q_OBJECT
    
public:
	enum AddType {
    	Move,
    	Copy,
    	AbsoluteReference,
    	RelativeReference
    };

    AddToProjectDialog(QWidget *parent = 0);
    ~AddToProjectDialog();

    AddToProjectDialog::AddType type() const;
};

#endif // ADD_TO_PROJECT_DIALOG_HPP
