/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    GlobalDir.h

Abstract:

    This file contains global directory define.
 */

#ifndef CGLOBALDIR_H
#define CGLOBALDIR_H

#include <QString>

class CGlobalDir
{
public:
    CGlobalDir();
    
    static CGlobalDir* Instance();
     
    QString GetDirApplication();

    QString GetDirDocument();
    int SetDirDocument(QString szPath);
    
    QString GetDirData();
    QString GetDirImage();
    
    QString GetDirTranslate();

    QString GetApplicationConfigureFile();
   
private:
    QString m_szDocumentPath;
};

#endif // CGLOBALDIR_H
