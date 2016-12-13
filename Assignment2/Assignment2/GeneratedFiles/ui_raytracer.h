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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RayTracerClass
{
public:
    QWidget *centralWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtoncreate;
    QPushButton *pushButtonsave;
    QCheckBox *checkBoxAnti;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RayTracerClass)
    {
        if (RayTracerClass->objectName().isEmpty())
            RayTracerClass->setObjectName(QStringLiteral("RayTracerClass"));
        RayTracerClass->resize(993, 686);
        centralWidget = new QWidget(RayTracerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 421, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButtoncreate = new QPushButton(horizontalLayoutWidget);
        pushButtoncreate->setObjectName(QStringLiteral("pushButtoncreate"));

        horizontalLayout->addWidget(pushButtoncreate);

        pushButtonsave = new QPushButton(horizontalLayoutWidget);
        pushButtonsave->setObjectName(QStringLiteral("pushButtonsave"));

        horizontalLayout->addWidget(pushButtonsave);

        checkBoxAnti = new QCheckBox(horizontalLayoutWidget);
        checkBoxAnti->setObjectName(QStringLiteral("checkBoxAnti"));

        horizontalLayout->addWidget(checkBoxAnti);

        RayTracerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(RayTracerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RayTracerClass->setStatusBar(statusBar);

        retranslateUi(RayTracerClass);
        QObject::connect(pushButtoncreate, SIGNAL(clicked()), RayTracerClass, SLOT(CreateImage()));
        QObject::connect(pushButtonsave, SIGNAL(clicked()), RayTracerClass, SLOT(SaveImage()));
        QObject::connect(checkBoxAnti, SIGNAL(toggled(bool)), RayTracerClass, SLOT(TrigerAntiAliasing(bool)));

        QMetaObject::connectSlotsByName(RayTracerClass);
    } // setupUi

    void retranslateUi(QMainWindow *RayTracerClass)
    {
        RayTracerClass->setWindowTitle(QApplication::translate("RayTracerClass", "RayTracer", 0));
#ifndef QT_NO_STATUSTIP
        pushButtoncreate->setStatusTip(QApplication::translate("RayTracerClass", "Create Image", 0));
#endif // QT_NO_STATUSTIP
        pushButtoncreate->setText(QApplication::translate("RayTracerClass", "Create", 0));
#ifndef QT_NO_STATUSTIP
        pushButtonsave->setStatusTip(QApplication::translate("RayTracerClass", "Save to .ppm", 0));
#endif // QT_NO_STATUSTIP
        pushButtonsave->setText(QApplication::translate("RayTracerClass", "Save", 0));
#ifndef QT_NO_STATUSTIP
        checkBoxAnti->setStatusTip(QApplication::translate("RayTracerClass", "open anti-aliasing", 0));
#endif // QT_NO_STATUSTIP
        checkBoxAnti->setText(QApplication::translate("RayTracerClass", "Anti-Aliasing", 0));
    } // retranslateUi

};

namespace Ui {
    class RayTracerClass: public Ui_RayTracerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RAYTRACER_H
