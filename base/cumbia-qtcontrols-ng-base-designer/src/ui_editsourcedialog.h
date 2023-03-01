/********************************************************************************
** Form generated from reading UI file 'editsourcedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITSOURCEDIALOG_H
#define UI_EDITSOURCEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditSourceDialog
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QPushButton *pbSrcAdd;
    QPushButton *pbSrcRemove;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QSpacerItem *spacerItem;

    void setupUi(QDialog *EditSourceDialog)
    {
        if (EditSourceDialog->objectName().isEmpty())
            EditSourceDialog->setObjectName(QString::fromUtf8("EditSourceDialog"));
        EditSourceDialog->resize(364, 438);
        gridLayout = new QGridLayout(EditSourceDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(EditSourceDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tabWidget->addTab(tab, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 5);

        pbSrcAdd = new QPushButton(EditSourceDialog);
        pbSrcAdd->setObjectName(QString::fromUtf8("pbSrcAdd"));

        gridLayout->addWidget(pbSrcAdd, 2, 0, 1, 1);

        pbSrcRemove = new QPushButton(EditSourceDialog);
        pbSrcRemove->setObjectName(QString::fromUtf8("pbSrcRemove"));

        gridLayout->addWidget(pbSrcRemove, 2, 1, 1, 1);

        okButton = new QPushButton(EditSourceDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 2, 3, 1, 1);

        cancelButton = new QPushButton(EditSourceDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 2, 4, 1, 1);

        spacerItem = new QSpacerItem(71, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 1, 2, 2, 1);


        retranslateUi(EditSourceDialog);
        QObject::connect(okButton, SIGNAL(clicked()), EditSourceDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), EditSourceDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(EditSourceDialog);
    } // setupUi

    void retranslateUi(QDialog *EditSourceDialog)
    {
        EditSourceDialog->setWindowTitle(QCoreApplication::translate("EditSourceDialog", "Edit Source", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("EditSourceDialog", "Set connection source", nullptr));
        pbSrcAdd->setText(QCoreApplication::translate("EditSourceDialog", "Add", nullptr));
        pbSrcRemove->setText(QCoreApplication::translate("EditSourceDialog", "Remove", nullptr));
        okButton->setText(QCoreApplication::translate("EditSourceDialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("EditSourceDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditSourceDialog: public Ui_EditSourceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITSOURCEDIALOG_H
