/********************************************************************************
** Form generated from reading UI file 'LYD_Player.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LYD_PLAYER_H
#define UI_LYD_PLAYER_H

#include <ContrastSlider.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "CenterWindow.h"
#include "LydSlider.h"
#include "OpenglWidget.h"
#include "SplitWindowLine.h"

QT_BEGIN_NAMESPACE

class Ui_LYD_PlayerClass
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    CenterWindow *centralWidget;
    OpenglWidget *_gl;
    QWidget *_edit;
    SplitWindowLine *_split;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *_text;
    QLabel *label;
    QLineEdit *_barrage;
    QFrame *_line;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *_filterDock;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_2;
    ContrastSlider *_contrast;
    QLabel *label_8;
    QDockWidget *_pictureDock;
    QWidget *dockWidgetContents_2;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents_4;
    QGridLayout *gridLayout;
    QFrame *line_2;
    LydSlider *_progress;
    QLabel *label_3;
    QSlider *_tempo;
    QLabel *label_5;
    QLabel *label_4;
    QPushButton *_isPause;
    LydSlider *_volume;
    QSlider *_rate;
    QSlider *_pitch;
    QLabel *label_6;
    QLabel *label_7;

    void setupUi(QMainWindow *LYD_PlayerClass)
    {
        if (LYD_PlayerClass->objectName().isEmpty())
            LYD_PlayerClass->setObjectName(QStringLiteral("LYD_PlayerClass"));
        LYD_PlayerClass->resize(1094, 844);
        actionOpen = new QAction(LYD_PlayerClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(LYD_PlayerClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        centralWidget = new CenterWindow(LYD_PlayerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMinimumSize(QSize(200, 0));
        centralWidget->setStyleSheet(QStringLiteral("background-color: rgb(222, 222, 222);"));
        _gl = new OpenglWidget(centralWidget);
        _gl->setObjectName(QStringLiteral("_gl"));
        _gl->setGeometry(QRect(10, 30, 802, 331));
        _gl->setMinimumSize(QSize(100, 100));
        _gl->setStyleSheet(QStringLiteral("background-color: rgb(212, 212, 212);"));
        _edit = new QWidget(centralWidget);
        _edit->setObjectName(QStringLiteral("_edit"));
        _edit->setGeometry(QRect(20, 430, 802, 200));
        _edit->setMinimumSize(QSize(100, 100));
        _edit->setStyleSheet(QStringLiteral("background-color: rgb(200, 190, 181);"));
        _split = new SplitWindowLine(centralWidget);
        _split->setObjectName(QStringLiteral("_split"));
        _split->setGeometry(QRect(10, 415, 801, 2));
        _split->setMinimumSize(QSize(0, 1));
        _split->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        _split->setFrameShape(QFrame::StyledPanel);
        _split->setFrameShadow(QFrame::Raised);
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(20, 722, 791, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(horizontalLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("font: 16pt \"\346\245\267\344\275\223\";"));

        horizontalLayout->addWidget(label_2);

        _text = new QLineEdit(horizontalLayoutWidget);
        _text->setObjectName(QStringLiteral("_text"));
        _text->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

        horizontalLayout->addWidget(_text);

        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QString::fromUtf8("font: 16pt \"\346\245\267\344\275\223\";"));

        horizontalLayout->addWidget(label);

        _barrage = new QLineEdit(horizontalLayoutWidget);
        _barrage->setObjectName(QStringLiteral("_barrage"));
        _barrage->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

        horizontalLayout->addWidget(_barrage);

        _line = new QFrame(centralWidget);
        _line->setObjectName(QStringLiteral("_line"));
        _line->setGeometry(QRect(10, 680, 801, 2));
        _line->setMinimumSize(QSize(0, 1));
        _line->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        _line->setFrameShape(QFrame::StyledPanel);
        _line->setFrameShadow(QFrame::Raised);
        LYD_PlayerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(LYD_PlayerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1094, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        LYD_PlayerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(LYD_PlayerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        LYD_PlayerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(LYD_PlayerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        LYD_PlayerClass->setStatusBar(statusBar);
        _filterDock = new QDockWidget(LYD_PlayerClass);
        _filterDock->setObjectName(QStringLiteral("_filterDock"));
        _filterDock->setMinimumSize(QSize(200, 62));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        gridLayout_2 = new QGridLayout(dockWidgetContents);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        _contrast = new ContrastSlider(dockWidgetContents);
        _contrast->setObjectName(QStringLiteral("_contrast"));
        _contrast->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(_contrast, 0, 1, 1, 1);

        label_8 = new QLabel(dockWidgetContents);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));

        gridLayout_2->addWidget(label_8, 0, 0, 1, 1);

        _filterDock->setWidget(dockWidgetContents);
        LYD_PlayerClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), _filterDock);
        _pictureDock = new QDockWidget(LYD_PlayerClass);
        _pictureDock->setObjectName(QStringLiteral("_pictureDock"));
        _pictureDock->setMinimumSize(QSize(200, 38));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        _pictureDock->setWidget(dockWidgetContents_2);
        LYD_PlayerClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), _pictureDock);
        dockWidget = new QDockWidget(LYD_PlayerClass);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setMinimumSize(QSize(200, 235));
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        gridLayout = new QGridLayout(dockWidgetContents_4);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        line_2 = new QFrame(dockWidgetContents_4);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 6, 1, 1, 1);

        _progress = new LydSlider(dockWidgetContents_4);
        _progress->setObjectName(QStringLiteral("_progress"));
        _progress->setMaximum(999);
        _progress->setValue(0);
        _progress->setSliderPosition(0);
        _progress->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(_progress, 0, 1, 1, 1);

        label_3 = new QLabel(dockWidgetContents_4);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        _tempo = new QSlider(dockWidgetContents_4);
        _tempo->setObjectName(QStringLiteral("_tempo"));
        _tempo->setMinimum(-100);
        _tempo->setMaximum(100);
        _tempo->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(_tempo, 1, 1, 1, 1);

        label_5 = new QLabel(dockWidgetContents_4);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        label_4 = new QLabel(dockWidgetContents_4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        _isPause = new QPushButton(dockWidgetContents_4);
        _isPause->setObjectName(QStringLiteral("_isPause"));

        gridLayout->addWidget(_isPause, 7, 1, 1, 1);

        _volume = new LydSlider(dockWidgetContents_4);
        _volume->setObjectName(QStringLiteral("_volume"));
        _volume->setMaximum(999);
        _volume->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(_volume, 4, 1, 1, 1);

        _rate = new QSlider(dockWidgetContents_4);
        _rate->setObjectName(QStringLiteral("_rate"));
        _rate->setMinimum(-100);
        _rate->setMaximum(100);
        _rate->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(_rate, 2, 1, 1, 1);

        _pitch = new QSlider(dockWidgetContents_4);
        _pitch->setObjectName(QStringLiteral("_pitch"));
        _pitch->setMinimum(-25);
        _pitch->setMaximum(25);
        _pitch->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(_pitch, 3, 1, 1, 1);

        label_6 = new QLabel(dockWidgetContents_4);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        label_7 = new QLabel(dockWidgetContents_4);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));

        gridLayout->addWidget(label_7, 3, 0, 1, 1);

        dockWidget->setWidget(dockWidgetContents_4);
        LYD_PlayerClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);

        retranslateUi(LYD_PlayerClass);

        QMetaObject::connectSlotsByName(LYD_PlayerClass);
    } // setupUi

    void retranslateUi(QMainWindow *LYD_PlayerClass)
    {
        LYD_PlayerClass->setWindowTitle(QApplication::translate("LYD_PlayerClass", "LYD_Player", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("LYD_PlayerClass", "Open", Q_NULLPTR));
        actionSave->setText(QApplication::translate("LYD_PlayerClass", "Export", Q_NULLPTR));
        label_2->setText(QApplication::translate("LYD_PlayerClass", "\346\226\207\345\255\227\357\274\232", Q_NULLPTR));
        label->setText(QApplication::translate("LYD_PlayerClass", "\345\274\271\345\271\225\357\274\232", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("LYD_PlayerClass", "File", Q_NULLPTR));
        _filterDock->setWindowTitle(QApplication::translate("LYD_PlayerClass", "Filter", Q_NULLPTR));
        label_8->setText(QApplication::translate("LYD_PlayerClass", "\345\257\271\346\257\224\345\272\246", Q_NULLPTR));
        _pictureDock->setWindowTitle(QApplication::translate("LYD_PlayerClass", "Picture", Q_NULLPTR));
        dockWidget->setWindowTitle(QApplication::translate("LYD_PlayerClass", "Ctrl", Q_NULLPTR));
        label_3->setText(QApplication::translate("LYD_PlayerClass", "\350\277\233\345\272\246", Q_NULLPTR));
        label_5->setText(QApplication::translate("LYD_PlayerClass", "\351\237\263\351\207\217", Q_NULLPTR));
        label_4->setText(QApplication::translate("LYD_PlayerClass", "\350\212\202\346\213\215", Q_NULLPTR));
        _isPause->setText(QApplication::translate("LYD_PlayerClass", "PushButton", Q_NULLPTR));
        label_6->setText(QApplication::translate("LYD_PlayerClass", "\351\200\237\347\216\207", Q_NULLPTR));
        label_7->setText(QApplication::translate("LYD_PlayerClass", "\351\237\263\350\260\203", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LYD_PlayerClass: public Ui_LYD_PlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LYD_PLAYER_H
