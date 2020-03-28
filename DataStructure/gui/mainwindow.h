#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void open();
    void mlpc();

private:
    QSplitter *splitterMain, *splitterRight, *splitterLeft;
    QTextEdit *leftUp, *rightUp;
    QTreeView *rightDown, *leftDown;
    QString curFileName;

    void loadFile(const QString& fileName);
    void createActions();
    void createStatusBar();
    void splitMainWin();
    void do_mlpc(const QStringList& argv, QString& ret);
};
#endif // MAINWINDOW_H
