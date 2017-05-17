/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    DlgAbout.h

Abstract:

    This file contains about dialog define.
 */

#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QDialog>
#include <QTextEdit>

namespace Ui {
class CDlgAbout;
}

class CDlgAbout : public QDialog
{
    Q_OBJECT
    
public:
    explicit CDlgAbout(QWidget *parent = 0);
    ~CDlgAbout();

private:
    Ui::CDlgAbout *ui;

    int AppendFile(QTextEdit *pEdit, const QString &szFile);
};

#endif // DLGABOUT_H
