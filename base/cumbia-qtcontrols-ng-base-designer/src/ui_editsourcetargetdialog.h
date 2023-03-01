/********************************************************************************
** Form generated from reading UI file 'editsourcetargetdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITSOURCETARGETDIALOG_H
#define UI_EDITSOURCETARGETDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditSourceTarget
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout1;
    QGroupBox *groupBox_2;
    QVBoxLayout *vboxLayout;
    QLineEdit *lineEdit;
    QWidget *tab_2;
    QGridLayout *gridLayout2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout3;
    QToolButton *moveItemDownButton;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QLabel *textLabel;
    QLineEdit *itemTextLineEdit;
    QToolButton *moveItemUpButton;
    QHBoxLayout *hboxLayout1;
    QToolButton *newItemButton;
    QToolButton *deleteItemButton;
    QSpacerItem *spacerItem1;
    QListWidget *listWidget;

    void setupUi(QDialog *EditSourceTarget)
    {
        if (EditSourceTarget->objectName().isEmpty())
            EditSourceTarget->setObjectName(QString::fromUtf8("EditSourceTarget"));
        EditSourceTarget->resize(313, 300);
        gridLayout = new QGridLayout(EditSourceTarget);
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(9, 9, 9, 9);
        buttonBox = new QDialogButtonBox(EditSourceTarget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);

        tabWidget = new QTabWidget(EditSourceTarget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout1 = new QGridLayout(tab);
        gridLayout1->setSpacing(6);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        gridLayout1->setContentsMargins(9, 9, 9, 9);
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        vboxLayout = new QVBoxLayout(groupBox_2);
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(9, 9, 9, 9);
        lineEdit = new QLineEdit(groupBox_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        vboxLayout->addWidget(lineEdit);


        gridLayout1->addWidget(groupBox_2, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout2 = new QGridLayout(tab_2);
        gridLayout2->setSpacing(6);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        gridLayout2->setContentsMargins(9, 9, 9, 9);
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout3 = new QGridLayout(groupBox);
        gridLayout3->setSpacing(6);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        gridLayout3->setContentsMargins(9, 9, 9, 9);
        moveItemDownButton = new QToolButton(groupBox);
        moveItemDownButton->setObjectName(QString::fromUtf8("moveItemDownButton"));

        gridLayout3->addWidget(moveItemDownButton, 2, 1, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        textLabel = new QLabel(groupBox);
        textLabel->setObjectName(QString::fromUtf8("textLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textLabel->sizePolicy().hasHeightForWidth());
        textLabel->setSizePolicy(sizePolicy1);

        hboxLayout->addWidget(textLabel);

        itemTextLineEdit = new QLineEdit(groupBox);
        itemTextLineEdit->setObjectName(QString::fromUtf8("itemTextLineEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(itemTextLineEdit->sizePolicy().hasHeightForWidth());
        itemTextLineEdit->setSizePolicy(sizePolicy2);

        hboxLayout->addWidget(itemTextLineEdit);


        gridLayout3->addLayout(hboxLayout, 2, 0, 1, 1);

        moveItemUpButton = new QToolButton(groupBox);
        moveItemUpButton->setObjectName(QString::fromUtf8("moveItemUpButton"));

        gridLayout3->addWidget(moveItemUpButton, 1, 1, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        newItemButton = new QToolButton(groupBox);
        newItemButton->setObjectName(QString::fromUtf8("newItemButton"));

        hboxLayout1->addWidget(newItemButton);

        deleteItemButton = new QToolButton(groupBox);
        deleteItemButton->setObjectName(QString::fromUtf8("deleteItemButton"));

        hboxLayout1->addWidget(deleteItemButton);

        spacerItem1 = new QSpacerItem(16, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);


        gridLayout3->addLayout(hboxLayout1, 1, 0, 1, 1);

        listWidget = new QListWidget(groupBox);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout3->addWidget(listWidget, 0, 0, 1, 2);


        gridLayout2->addWidget(groupBox, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

#if QT_CONFIG(shortcut)
        textLabel->setBuddy(itemTextLineEdit);
#endif // QT_CONFIG(shortcut)

        retranslateUi(EditSourceTarget);
        QObject::connect(buttonBox, SIGNAL(accepted()), EditSourceTarget, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EditSourceTarget, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(EditSourceTarget);
    } // setupUi

    void retranslateUi(QDialog *EditSourceTarget)
    {
        EditSourceTarget->setWindowTitle(QCoreApplication::translate("EditSourceTarget", "Edit Source and Target", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("EditSourceTarget", "Set Source Point", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit->setToolTip(QCoreApplication::translate("EditSourceTarget", "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\" white-space: pre-wrap; font-family:Sans Serif; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Valid formats are:</p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for attributes: <span style=\" font-weight:600;\">tango/device/server/attribute</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for commands: <span style=\" font-weight:600;\">tango/device/server-&gt;command</span></p><p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent"
                        ":0px;\">You can also specify a Tango Database:</p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for attributes: <span style=\" font-weight:600;\">host:port/tango/device/server/attribute</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for commands: <span style=\" font-weight:600;\">host:port/tango/device/server-&gt;command</span></p><p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("EditSourceTarget", "Source", nullptr));
        groupBox->setTitle(QCoreApplication::translate("EditSourceTarget", "Set Targets", nullptr));
#if QT_CONFIG(tooltip)
        moveItemDownButton->setToolTip(QCoreApplication::translate("EditSourceTarget", "Move Item Down", nullptr));
#endif // QT_CONFIG(tooltip)
        moveItemDownButton->setText(QCoreApplication::translate("EditSourceTarget", "D", nullptr));
        textLabel->setText(QCoreApplication::translate("EditSourceTarget", "&Text", nullptr));
#if QT_CONFIG(tooltip)
        itemTextLineEdit->setToolTip(QCoreApplication::translate("EditSourceTarget", "Set Item Text", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        moveItemUpButton->setToolTip(QCoreApplication::translate("EditSourceTarget", "Move Item Up", nullptr));
#endif // QT_CONFIG(tooltip)
        moveItemUpButton->setText(QCoreApplication::translate("EditSourceTarget", "U", nullptr));
#if QT_CONFIG(tooltip)
        newItemButton->setToolTip(QCoreApplication::translate("EditSourceTarget", "New Item", nullptr));
#endif // QT_CONFIG(tooltip)
        newItemButton->setText(QCoreApplication::translate("EditSourceTarget", "&New Item", nullptr));
#if QT_CONFIG(tooltip)
        deleteItemButton->setToolTip(QCoreApplication::translate("EditSourceTarget", "Delete Item", nullptr));
#endif // QT_CONFIG(tooltip)
        deleteItemButton->setText(QCoreApplication::translate("EditSourceTarget", "&Delete Item", nullptr));
#if QT_CONFIG(tooltip)
        listWidget->setToolTip(QCoreApplication::translate("EditSourceTarget", "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\" white-space: pre-wrap; font-family:Sans Serif; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Valid formats are:</p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for attributes: <span style=\" font-weight:600;\">tango/device/server/attribute</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for commands: <span style=\" font-weight:600;\">tango/device/server-&gt;command</span></p><p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent"
                        ":0px;\">You can also specify a Tango Database:</p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for attributes: <span style=\" font-weight:600;\">host:port/tango/device/server/attribute</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for commands: <span style=\" font-weight:600;\">host:port/tango/device/server-&gt;command</span></p><p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("EditSourceTarget", "Target", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditSourceTarget: public Ui_EditSourceTarget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITSOURCETARGETDIALOG_H
