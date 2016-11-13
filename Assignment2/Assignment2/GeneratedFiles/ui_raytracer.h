/********************************************************************************
** Form generated from reading UI file 'raytracer.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RAYTRACER_H
#define UI_RAYTRACER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RayTracerClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RayTracerClass)
    {
        if (RayTracerClass->objectName().isEmpty())
            RayTracerClass->setObjectName(QStringLiteral("RayTracerClass"));
        RayTracerClass->resize(920, 627);
        centralWidget = new QWidget(RayTracerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        RayTracerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RayTracerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 920, 23));
        RayTracerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RayTracerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        RayTracerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RayTracerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RayTracerClass->setStatusBar(statusBar);

        retranslateUi(RayTracerClass);

        QMetaObject::connectSlotsByName(RayTracerClass);
    } // setupUi

    void retranslateUi(QMainWindow *RayTracerClass)
    {
        RayTracerClass->setWindowTitle(QApplication::translate("RayTracerClass", "RayTracer", 0));
    } // retranslateUi

};

namespace Ui {
    class RayTracerClass: public Ui_RayTracerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RAYTRACER_H
