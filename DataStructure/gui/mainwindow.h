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
    void help();
    void expandAndCollapase();

private:
    QSplitter *splitterUp, *splitterDown;
    QTextEdit *leftUp, *rightUp;
    QTreeView *rightDown, *leftDown;
    QString curFileName;
    QToolBar *toolBar;
    QLabel *lut, *rut, *rdt, *ldt;
    QWidget *mainWidget;
    int treeStatus;

    void loadFile(const QString& fileName);
    void createActions();
    void createStatusBar();
    void splitMainWin();
    void setMainLayout();
    void do_mlpc(const QStringList& argv, QString& ret);
};
#endif // MAINWINDOW_H
