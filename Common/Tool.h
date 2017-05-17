/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    Tool.h

Abstract:

    This file contains tool define.
 */

#ifndef TOOL_H
#define TOOL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#include <QObject>
#include <memory>
#include <string>
#include <QWidget>

#include <QFileDialog>
#include "Global/Global.h"

/**
  * @defgroup RABBITGIS_TOOL 工具类模块  
  */

/**
 * @brief 工具类。提供一此工具函数  
 * @ingroup RABBITGIS_TOOL
 */
class  CTool : public QObject
{
    Q_OBJECT
public:
    explicit CTool(QObject *parent = 0);
    ~CTool();

public:
    
    /**
     * @brief 删除指定的目录  
     *
     * @param dirName 要删除的目录名  
     * @return bool 成功返回 true，否则返回 false  
     */
    static bool removeDirectory(QString dirName);
    
    /**
     * @brief 调整窗口的位置，手机上，让窗口全屏，在pc上，把窗口调整到屏幕中间  
     *
     * @fn SetWindowsGeometry
     * @param pWindow
     * @return int
     */
    static int SetWindowsGeometry(QWidget* pWindow);
    
    static QString FileDialog(QWidget* pParent, const QString &szDir,
                              const QString &szFilter,
                              const QString &szTilte = tr("Open file"), const QFileDialog::AcceptMode mode = QFileDialog::AcceptOpen);
    static std::string DoubleToString(double d);
    /**
     * 得到文件的校验和  
     */
    static QByteArray GetFileMd5Sum(QString filePath);
    static QString GetFileMd5SumString(QString filePath);

    static QPixmap ComposePixmap(const QString& szFile1,
                                 const QString& szFile2);
    static int ComposePixmap(QPixmap &src1, const QPixmap &src2);
    static int ComposeAvatarStatus(QPixmap &src1, const QPixmap& src2);

    static QImage ConvertToGray(QImage image);
    
    /**
     * @brief 禁用锁屏  
     * @param bWake：true，禁用锁屏，false，允许锁屏  
     * @return 成功返回 true，否则返回 false
     */
    static bool EnableWake(bool bWake = true);

signals:

public slots:

};

#endif // TOOL_H
