/*
 * Author: KangLin(Email:kl222@126.com)
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
