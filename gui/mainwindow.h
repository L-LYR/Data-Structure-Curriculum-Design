#ifndef MAINWINDOW_H
#define MAINWINDOW_H
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
    void subprocess(const QStringList& argv);

private:
    QSplitter* splitterMain;
    QTextEdit *left, *right;
    QString curFileName;
    QString mlpc_main;
};
#endif // MAINWINDOW_H
