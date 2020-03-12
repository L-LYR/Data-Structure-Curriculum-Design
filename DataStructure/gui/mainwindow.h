#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "treemodel.h"
#include <QMainWindow>
#include <QtWidgets>
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void loadFile(const QString& fileName);

private slots:
    void open();
    void lexicalAnalysis();
    void syntaxAnalysis();
    void rearrange();

private:
    void createActions();
    void createStatusBar();
    void splitMainWin();
    QString subprocess(const QStringList& argv);

private:
    QSplitter *splitterMain, *splitterRight, *splitterLeft;
    QTextEdit *leftUp, *rightUp;
    QTreeView *rightDown, *leftDown;
    QString curFileName;
    QString mlpc_main;
};
#endif // MAINWINDOW_H
