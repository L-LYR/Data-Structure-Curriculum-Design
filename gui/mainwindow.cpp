#include "mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    mlpc_main = "bin/mlpc";
    createActions();
    createStatusBar();
    splitMainWin();
}

void MainWindow::open()
{
    qDebug() << "open() is triggered";
    curFileName = QFileDialog::getOpenFileName(this);
    if (!curFileName.isEmpty())
        loadFile(curFileName);
}

void MainWindow::subprocess(const QStringList& argv)
{
    QProcess* subprocess = new QProcess(this);
    subprocess->start(mlpc_main, argv);
    if (!subprocess->waitForStarted()) {
        QMessageBox::warning(this, tr("Warning"), tr("Fail to start!"));
        return;
    }
    while (subprocess->waitForFinished() == false) {
        QMessageBox::warning(this, tr("Warning"), tr("Fail to finish!"));
        subprocess->kill();
        return;
    }
    QString output = subprocess->readAll();
    right->setPlainText(output);
}
void MainWindow::lexicalAnalysis()
{
    if (curFileName == "") {
        QMessageBox::warning(this, tr("Warning"), tr("No input!"));
        return;
    }
    QStringList argv;
    argv << "-l"
         << curFileName;
    subprocess(argv);
    statusBar()->showMessage(tr("Lexical Analysis Finished"), 2000);
}

void MainWindow::syntaxAnalysis()
{
    if (curFileName == "") {
        QMessageBox::warning(this, tr("Warning"), tr("No input!"));
        return;
    }
    QStringList argv;
    argv << "-s"
         << curFileName;
    subprocess(argv);
    statusBar()->showMessage(tr("Syntax Analysis Finished"), 2000);
}

void MainWindow::rearrange()
{
    if (curFileName == "") {
        QMessageBox::warning(this, tr("Warning"), tr("No input!"));
        return;
    }
    QStringList argv;
    argv << "-r"
         << curFileName;
    subprocess(argv);
    statusBar()->showMessage(tr("Code Rearranged"), 2000);
}

void MainWindow::createActions()
{
    setFixedSize(960, 540);
    QToolBar* toolBar = addToolBar(tr("File"));
    toolBar->setMovable(false);

    QAction* openAct = new QAction(QIcon(":/img/Open.png"), tr("Open"), this);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    toolBar->addAction(openAct);

    QAction* lexicalAnalysisAct = new QAction(QIcon(":/img/L.png"), tr("Lexical Analysis"), this);
    lexicalAnalysisAct->setStatusTip(tr("Do lexing"));
    connect(lexicalAnalysisAct, &QAction::triggered, this, &MainWindow::lexicalAnalysis);
    toolBar->addAction(lexicalAnalysisAct);

    QAction* syntaxAnalysisAct = new QAction(QIcon(":/img/S.png"), tr("Syntax Analysis"), this);
    syntaxAnalysisAct->setStatusTip(tr("Parse"));
    connect(syntaxAnalysisAct, &QAction::triggered, this, &MainWindow::syntaxAnalysis);
    toolBar->addAction(syntaxAnalysisAct);

    QAction* rearrangeAct = new QAction(QIcon(":/img/Rearrange.png"), tr("Rearrange"), this);
    rearrangeAct->setStatusTip(tr("Rearrange the current file"));
    connect(rearrangeAct, &QAction::triggered, this, &MainWindow::rearrange);
    toolBar->addAction(rearrangeAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready..."));
}

void MainWindow::splitMainWin()
{
    splitterMain = new QSplitter(Qt::Horizontal, this);
    left = new QTextEdit(splitterMain);
    right = new QTextEdit(splitterMain);
    left->setReadOnly(true);
    right->setReadOnly(true);
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

    QTextStream in(&file);
    int i = 1;
    while (!in.atEnd()) {
        left->append(QString("%1  ").arg(i, 3) + in.readLine());
        i++;
    }

    statusBar()->showMessage(tr("File Loaded"), 2000);
}

MainWindow::~MainWindow() {}
