/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    Tool.cpp

Abstract:

    This file contains tool implement.
 */

#include "Tool.h"
#include "Global/Global.h"
#include <QFileInfo>
#include <QDir>
#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QApplication>
#include <QFileDialog>
#include <sstream>
#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <QFile>
#include <QPainter>

#ifdef ANDROID
    #include <QtAndroidExtras/QAndroidJniObject>
#endif
#ifdef WINDOWS
    #include <windows.h>
    #pragma comment(lib, "User32.lib")
#endif

CTool::CTool(QObject *parent) :
    QObject(parent)
{
}

CTool::~CTool()
{
}

//设置日志的回调函数  
void Log(void*, int, const char* fmt, va_list vl)
{
    LOG_MODEL_DEBUG("ffmpeg", fmt, vl);
}

bool CTool::removeDirectory(QString dirName)
{
    QDir dir(dirName);
    QString tmpdir ="";
    if(!dir.exists()){
        return false;
    }
    
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName()=="."|| fileInfo.fileName()=="..")
            continue;
        
        if(fileInfo.isDir()){
            tmpdir = dirName +("/")+ fileInfo.fileName();
            removeDirectory(tmpdir);
            dir.rmdir(fileInfo.fileName());/**< 移除子目录 */
        }
        else if(fileInfo.isFile()){
            QFile tmpFile(fileInfo.fileName());
            dir.remove(tmpFile.fileName());/**< 删除临时文件 */
        }
    }
    
    dir.cdUp();            /**< 返回上级目录，因为只有返回上级目录，才可以删除这个目录 */
    if(dir.exists(dirName)){
        if(!dir.rmdir(dirName))
            return false;
    }
    return true;
}

int CTool::SetWindowsGeometry(QWidget *pWindow)
{
    /*
    QDesktopWidget *pDesk = QApplication::desktop();
    LOG_MODEL_DEBUG("CTool", "availableGeometry:%d,%d; screenGeometry:%d,%d; geometry:%d,%d",
                    pDesk->availableGeometry().width(),
                    pDesk->availableGeometry().height(),
                    pDesk->screenGeometry().width(),
                    pDesk->screenGeometry().height(),
                    pDesk->geometry().width(),
                    pDesk->geometry().height());
#ifdef MOBILE
    pWindow->setGeometry(pDesk->availableGeometry());
#else
    pWindow->move((pDesk->width() - pWindow->width()) / 2,
         (pDesk->height() - pWindow->height()) / 2);
#endif
    //*/
    //*
    QScreen *pScreen = QGuiApplication::primaryScreen();
    LOG_MODEL_DEBUG("CTool", "availableGeometry:%d,%d; geometry:%d,%d",
                    pScreen->availableGeometry().width(),
                    pScreen->availableGeometry().height(),
                    pScreen->geometry().width(),
                    pScreen->geometry().height());
#ifdef MOBILE
    pWindow->setGeometry(pScreen->availableGeometry());
#else
    if(!(pScreen->availableGeometry().width() > pWindow->width()
            && pScreen->availableGeometry().height() > pWindow->height()))
    {
        pWindow->setGeometry(0, 0, pScreen->availableGeometry().width() - 80,
                             pScreen->availableGeometry().height() - 80);
    }
    pWindow->move((pScreen->availableGeometry().width() - pWindow->width()) >> 1,
                  (pScreen->availableGeometry().height() - pWindow->height()) >> 1);
#endif
    //*/
    return 0;
}

QString CTool::FileDialog(QWidget *pParent, const QString &szDir,
                          const QString &szFilter, const QString &szTilte, const QFileDialog::AcceptMode mode)
{
    QString szFile;
    QFileDialog dlg(pParent, szTilte, szDir, szFilter);
    dlg.setAcceptMode(mode);
#ifdef MOBILE
    dlg.setOption(QFileDialog::DontUseNativeDialog, true);
#endif
    CTool::SetWindowsGeometry(&dlg);
    QStringList fileNames;
    if(dlg.exec())
        fileNames = dlg.selectedFiles();
    else
        return szFile;
    if(fileNames.isEmpty())
        return szFile;
    szFile = *fileNames.begin();

    return szFile;
}

std::string CTool::DoubleToString(double d)
{
    //Need #include <sstream>
    std::string str;
    std::stringstream ss;
    ss << d;
    ss >> str;
    return str;
}

QByteArray CTool::GetFileMd5Sum(QString filePath)
{
    QFile localFile(filePath);
    
    if (!localFile.open(QFile::ReadOnly))
    {
        LOG_MODEL_ERROR("CTool", "file open error.");
        return 0;
    }
    
    QCryptographicHash ch(QCryptographicHash::Md5);
    
    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesToWrite = 0;
    quint64 loadSize = 1024 * 4;
    QByteArray buf;
    
    totalBytes = localFile.size();
    bytesToWrite = totalBytes;
    
    while (1)
    {
        if(bytesToWrite > 0)
        {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }
        
        if(bytesWritten == totalBytes)
        {
            break;
        }
    }
    
    localFile.close();
    QByteArray md5 = ch.result();
    return md5;
}

QString CTool::GetFileMd5SumString(QString filePath)
{
    return GetFileMd5Sum(filePath).toHex();
}

QPixmap CTool::ComposePixmap(const QString &szFile1, const QString &szFile2)
{
    QPixmap src1(szFile1);
    QPixmap src2(szFile2);
    ComposePixmap(src1, src2);
    return src1;
}

int CTool::ComposePixmap(QPixmap &src1, const QPixmap &src2)
{
    QPainter painter(&src1);
    //painter.setCompositionMode(QPainter::CompositionMode_Xor);
    painter.drawPixmap(0, 0,  src2);
    return 0;
}

int CTool::ComposeAvatarStatus(QPixmap &src1, const QPixmap &src2)
{
    QPainter painter(&src1);
    painter.drawPixmap((src1.width() * 3) >> 2,  (src1.height() * 3) >> 2,
                       src1.width() >> 2, src1.height() >> 2,  src2);
    return 0;
}

uchar RGBtoGRAY(uchar r, uchar g, uchar b)  
{  
    return (uchar)((((qint32)((r << 5) + (r << 2) + (r << 1)))
                    + (qint32)((g << 6) + (g << 3) + (g << 1) + g) 
                    + (qint32)((b << 4) - b)) >> 7);  
}  
//转为灰度图  
QImage CTool::ConvertToGray(QImage image)
{
    QSize size = image.size();
    QImage grayImage = QImage(size, QImage::Format_Indexed8);
    int width = size.width();
    int height = size.height();
    uchar * rgbImageData = image.bits();
    uchar * grayImageData = grayImage.bits();
    if(image.allGray())
    {
        grayImage = QImage(image);
        return grayImage;
    }
    //若width不是4的倍数，会自动添加字节，使之对齐到4的倍数  
    int realWidth1=image.bytesPerLine();
    int realWidth2=grayImage.bytesPerLine();
    uchar * backup1 = rgbImageData;
    uchar * backup2 = grayImageData;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            uchar R = *rgbImageData;
            uchar G = *(rgbImageData + 1);
            uchar B = *(rgbImageData + 2);
            //*grayImageData = (uchar)(0.29900*R + 0.58700*G + 0.11400*B);
            *grayImageData = RGBtoGRAY(R, G, B); 
            rgbImageData += 3;
            grayImageData++;
        }
        rgbImageData = backup1 + realWidth1 * i;
        grayImageData = backup2 + realWidth2 * i;
    }
    
    return grayImage;
    //下面算法可以达到效果，但性能很差  
    //    outGrayImage = rgbImage;
    //    for(int x = 0; x < rgbImage.width(); x++)
    //        for(int y = 0; y < rgbImage.width(); y++)
    //        {
    //            QRgb pix = rgbImage.pixel(x, y);
    //            int gray = qGray(pix);
    //            QRgb grayPixel = qRgb(gray, gray, gray);
    //            outGrayImage.setPixel(x, y, grayPixel);
    //        }
    //return true;
}

bool CTool::EnableWake(bool bWake)
{
    static bool bSet = false;
/*
#ifdef ANDROID
    jboolean bPara = bWake;
    jboolean bRet = QAndroidJniObject::callStaticMethod<jboolean>(
                "org/KangLinStudio/RabbitIm/RabbitImActivity",
                "EnableWake",
                "(Z)Z",
                bPara);
    return bRet;
#elif WINDOWS
    if(bWake)
    {
        BOOL bScrActive;
        SystemParametersInfo(SPI_GETSCREENSAVEACTIVE,0,&bScrActive,0);
        if(bScrActive)
        {
            bSet = true;
            SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, NULL, 0);
        }
    } else if(bSet) {
        SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, 0);
        bSet = false;
    }
    return true;
#endif //*/
    return false;
}
