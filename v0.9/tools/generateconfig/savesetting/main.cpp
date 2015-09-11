#include <QtGui>
#include <QTextCodec>
#include "savesetting.h"
#include "mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

    QApplication a(argc, argv);
    //SaveSetting w;
    QFont font = a.font();
    font.setPointSize(9);
    a.setFont(font);

    mainwidget w;
    w.setWindowTitle("�������ؼ�¼�������ļ�");
//    w.setWindowIcon();
    w.show();
    qApp->setStyle(new QWindowsStyle);
    return a.exec();
}
