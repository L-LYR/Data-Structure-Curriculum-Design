#include "mainwindow.h"
#include "treemodel.h"
#include <QDebug>
#include <QtWidgets>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/img/Hammer.ico"));

    createActions();
    createStatusBar();
    splitMainWin();
}

void MainWindow::open()
{
    curFileName = QFileDialog::getOpenFileName(this);
    if (!curFileName.isEmpty())
        loadFile(curFileName);
}

void MainWindow::do_mlpc(const QStringList &argv, QString &ret)
{
    QProcess *sp = new QProcess(this);
    sp->start("./mlpc.exe", argv);
    if (!sp->waitForStarted()) {
        QMessageBox::warning(this, tr("Warning"), tr("Fail to start!"));
        return;
    }
    while (sp->waitForFinished() == false) {
        QMessageBox::warning(this, tr("Warning"), tr("Fail to finish!"));
        sp->kill();
        return;
    }
    ret = sp->readAll();
}

void MainWindow::mlpc()
{
    if(curFileName == "")
    {
        QMessageBox::warning(this, tr("Warning"),
                             tr("No input file!"));
        return ;
    }

    QString ret;
    do_mlpc(QStringList({"-l", curFileName}), ret);
    TreeModel* tree = new TreeModel(QStringList({ "Token", "Type" }), ret);
    leftDown->setModel(tree);

    do_mlpc(QStringList({"-s", curFileName}), ret);
    tree = new TreeModel(QStringList({"AST"}), ret);
    rightDown->setModel(tree);

    do_mlpc(QStringList({"-r", curFileName}), ret);
    rightUp->clear();
    QTextStream in(&ret);
    int i = 1;
    while (!in.atEnd()) {
        rightUp->append(QString("%1  ").arg(i, 3) + in.readLine());
        i++;
    }
}

void MainWindow::createActions()
{
    QToolBar* toolBar = addToolBar(tr("File"));
    toolBar->setMovable(false);

    QAction* openAct = new QAction(QIcon(":/img/Open.png"), tr("Open File"), this);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    toolBar->addAction(openAct);

    QAction* mlpc = new QAction(QIcon(":/img/Hammer.ico"), tr("Do lexing, parse and rearrange"), this);
    mlpc->setStatusTip(tr("Do lexing, parse and rearrange"));
    connect(mlpc, &QAction::triggered, this, &MainWindow::mlpc);
    toolBar->addAction(mlpc);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready..."));
}

void MainWindow::splitMainWin()
{
    splitterMain = new QSplitter(Qt::Horizontal, this);

    splitterLeft = new QSplitter(Qt::Vertical, splitterMain);
    leftUp = new QTextEdit(splitterLeft);
    leftDown = new QTreeView(splitterLeft);

    splitterRight = new QSplitter(Qt::Vertical, splitterMain);
    rightUp = new QTextEdit(splitterRight);
    rightDown = new QTreeView(splitterRight);

    leftUp->setReadOnly(true);
    rightUp->setReadOnly(true);

    setCentralWidget(splitterMain);
}

void MainWindow::loadFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }
    leftUp->clear();
    QTextStream in(&file);
    int i = 1;
    while (!in.atEnd()) {
        leftUp->append(QString("%1  ").arg(i, 3) + in.readLine());
        i++;
    }

    statusBar()->showMessage(tr("File Loaded"), 2000);
}

MainWindow::~MainWindow() {}

