/********************************************************************************
** Form generated from reading UI file 'editsourcewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITSOURCEWIDGET_H
#define UI_EDITSOURCEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditSourceWidget
{
public:
    QGridLayout *gridLayout;
    QToolButton *toolDevice;
    QGroupBox *gbFormula;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QLineEdit *leScriptPath;
    QTextEdit *teJsCode;
    QPushButton *pbFromJs;
    QLineEdit *leAlias;
    QLabel *labelPluginErr;
    QLineEdit *lineEdit;

    void setupUi(QWidget *EditSourceWidget)
    {
        if (EditSourceWidget->objectName().isEmpty())
            EditSourceWidget->setObjectName(QString::fromUtf8("EditSourceWidget"));
        EditSourceWidget->resize(400, 300);
        gridLayout = new QGridLayout(EditSourceWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        toolDevice = new QToolButton(EditSourceWidget);
        toolDevice->setObjectName(QString::fromUtf8("toolDevice"));
        toolDevice->setEnabled(false);

        gridLayout->addWidget(toolDevice, 0, 1, 1, 1);

        gbFormula = new QGroupBox(EditSourceWidget);
        gbFormula->setObjectName(QString::fromUtf8("gbFormula"));
        gbFormula->setAutoFillBackground(false);
        gbFormula->setStyleSheet(QString::fromUtf8("border-color: rgb(231, 55, 23);"));
        gbFormula->setCheckable(true);
        gbFormula->setChecked(true);
        gridLayout_2 = new QGridLayout(gbFormula);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(gbFormula);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        leScriptPath = new QLineEdit(gbFormula);
        leScriptPath->setObjectName(QString::fromUtf8("leScriptPath"));
        leScriptPath->setReadOnly(true);

        gridLayout_2->addWidget(leScriptPath, 2, 0, 1, 2);

        teJsCode = new QTextEdit(gbFormula);
        teJsCode->setObjectName(QString::fromUtf8("teJsCode"));

        gridLayout_2->addWidget(teJsCode, 1, 0, 1, 3);

        pbFromJs = new QPushButton(gbFormula);
        pbFromJs->setObjectName(QString::fromUtf8("pbFromJs"));

        gridLayout_2->addWidget(pbFromJs, 2, 2, 1, 1);

        leAlias = new QLineEdit(gbFormula);
        leAlias->setObjectName(QString::fromUtf8("leAlias"));

        gridLayout_2->addWidget(leAlias, 0, 1, 1, 2);

        labelPluginErr = new QLabel(gbFormula);
        labelPluginErr->setObjectName(QString::fromUtf8("labelPluginErr"));

        gridLayout_2->addWidget(labelPluginErr, 3, 0, 1, 3);


        gridLayout->addWidget(gbFormula, 1, 0, 1, 2);

        lineEdit = new QLineEdit(EditSourceWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 0, 1, 1);


        retranslateUi(EditSourceWidget);

        QMetaObject::connectSlotsByName(EditSourceWidget);
    } // setupUi

    void retranslateUi(QWidget *EditSourceWidget)
    {
        EditSourceWidget->setWindowTitle(QCoreApplication::translate("EditSourceWidget", "Form", nullptr));
        toolDevice->setText(QCoreApplication::translate("EditSourceWidget", "...", nullptr));
        gbFormula->setTitle(QCoreApplication::translate("EditSourceWidget", "Formula", nullptr));
        label->setText(QCoreApplication::translate("EditSourceWidget", "Source alias", nullptr));
        pbFromJs->setText(QCoreApplication::translate("EditSourceWidget", "From js script...", nullptr));
#if QT_CONFIG(tooltip)
        leAlias->setToolTip(QCoreApplication::translate("EditSourceWidget", "<html><head/><body><p align=\"justify\">An alias for the source can be provided.</p><p align=\"justify\"><br/></p><p align=\"justify\">If not empty, it will be stored into the &quot;src&quot; key of the data bundle. For example, if the formula sums the value of two sources, you could provide a short alias named <span style=\" font-style:italic;\">sum</span> for the source. If empty, the &quot;src&quot; value will contain the comma separated list of sources</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        labelPluginErr->setText(QCoreApplication::translate("EditSourceWidget", "plugin error", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit->setToolTip(QCoreApplication::translate("EditSourceWidget", "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\" white-space: pre-wrap; font-family:Sans Serif; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Valid formats are:</p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for attributes: <span style=\" font-weight:600;\">tango/device/server/attribute</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for commands: <span style=\" font-weight:600;\">tango/device/server-&gt;command</span></p><p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent"
                        ":0px;\">You can also specify a Tango Database:</p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for attributes: <span style=\" font-weight:600;\">host:port/tango/device/server/attribute</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">for commands: <span style=\" font-weight:600;\">host:port/tango/device/server-&gt;command</span></p><p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
    } // retranslateUi

};

namespace Ui {
    class EditSourceWidget: public Ui_EditSourceWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITSOURCEWIDGET_H
