#include "mainwindow.h"
#include "treemodel.h"
#include <QDebug>
#include <QtWidgets>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    setWindowIcon(QIcon(":/img/Hammer.ico"));
    resize(1200, 800);
    treeStatus = 0;

    createActions();
    createStatusBar();
    splitMainWin();
    setMainLayout();

    leftUp->append("How to use:");
    leftUp->append("    Step 1: Click the first button to open a C source file.");
    leftUp->append("            Here are some test files in the folder \"test\"");
    leftUp->append("            under current directory.");
    leftUp->append("    Step 2: Click the second button to do all the analyses.");
}

void MainWindow::open()
{
    curFileName = QFileDialog::getOpenFileName(this);
    if (!curFileName.isEmpty())
        loadFile(curFileName);
}

void MainWindow::help()
{
    QMessageBox::about(this, tr("How to use?"), tr(
      "Step 1: click the first button to open a C source file. "
      "Here are some test files in the folder \"test\" under the current directory.\n"
      "Step 2: click the second button to do all the analyses.\n"
    ));
}

void MainWindow::expandAndCollapase()
{
    if(treeStatus == 1){
        leftDown->collapseAll();
        rightDown->collapseAll();
        treeStatus = 2;
    }else if(treeStatus == 2){
        leftDown->expandAll();
        rightDown->expandAll();
        treeStatus = 1;
    }
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

    QString ret = "";
    do_mlpc(QStringList({"-l", curFileName}), ret);
    if(ret == "")
    {
        QMessageBox::warning(this, tr("Warning"), tr("Fail to do lexical analysis!"));
        return;
    }
    TreeModel* tree = new TreeModel(QStringList({ "Token", "Type" }), ret);
    leftDown->setModel(tree);


    ret = "";
    do_mlpc(QStringList({"-s", curFileName}), ret);
    if(ret == "")
    {
        QMessageBox::warning(this, tr("Warning"), tr("Fail to do syntax analysis!"));
        return;
    }
    tree = new TreeModel(QStringList({"AST"}), ret);
    rightDown->setModel(tree);

    ret = "";
    do_mlpc(QStringList({"-r", curFileName}), ret);
    rightUp->clear();
    if(ret == "")
    {
        QMessageBox::warning(this, tr("Warning"), tr("Fail to rearrange!"));
        return;
    }
    QTextStream in(&ret);
    int i = 1;
    while (!in.atEnd()) {
        rightUp->append(QString("%1  ").arg(i, 3) + in.readLine());
        i++;
    }

    treeStatus = 2;
    expandAndCollapase();
    for (int column = 0; column < tree->columnCount(); ++column)
        leftDown->resizeColumnToContents(column);
}

void MainWindow::createActions()
{
    toolBar = addToolBar(tr("File"));
    toolBar->setMovable(false);

    QAction* openAct = new QAction(QIcon(":/img/Open.png"), tr("Open File"), this);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    toolBar->addAction(openAct);

    QAction* mlpc = new QAction(QIcon(":/img/Hammer.ico"), tr("Do lexing, parse and rearrange"), this);
    mlpc->setStatusTip(tr("Do lexing, parse and rearrange"));
    connect(mlpc, &QAction::triggered, this, &MainWindow::mlpc);
    toolBar->addAction(mlpc);

    QAction* expandAct = new QAction(QIcon(":/img/Expand.png"), tr("Expand & collapse"), this);
    expandAct->setStatusTip(tr("Expand all & Collapse all"));
    connect(expandAct, &QAction::triggered, this, &MainWindow::expandAndCollapase);
    toolBar->addAction(expandAct);

    QAction* helpAct = new QAction(QIcon(":/img/Help.png"), tr("Help message"), this);
    helpAct->setStatusTip(tr("Help message"));
    connect(helpAct, &QAction::triggered, this, &MainWindow::help);
    toolBar->addAction(helpAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready..."));
}

void MainWindow::splitMainWin()
{
    splitterUp = new QSplitter(Qt::Horizontal, mainWidget);
    leftUp = new QTextEdit(splitterUp);
    rightUp = new QTextEdit(splitterUp);

    splitterDown = new QSplitter(Qt::Horizontal, mainWidget);
    leftDown = new QTreeView(splitterDown);
    rightDown = new QTreeView(splitterDown);
    leftUp->setReadOnly(true);
    rightUp->setReadOnly(true);
}

void MainWindow::setMainLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *h4 = new QHBoxLayout,
        *h3 = new QHBoxLayout,
        *h2 = new QHBoxLayout,
        *h1 = new QHBoxLayout;
    QFont labelFont("consolas", 14);
    lut = new QLabel(mainWidget);
    lut->setText("Source Code");
    lut->setFont(labelFont);
    lut->setFixedHeight(24);

    rut = new QLabel(mainWidget);
    rut->setText("Rearranged Code");
    rut->setFont(labelFont);
    rut->setFixedHeight(24);

    h1->addWidget(lut, 1);
    h1->addWidget(rut, 1);

    ldt = new QLabel(mainWidget);
    ldt->setText("Lexical Analysis");
    ldt->setFont(labelFont);
    ldt->setFixedHeight(24);

    rdt = new QLabel(mainWidget);
    rdt->setText("Syntax Analysis");
    rdt->setFont(labelFont);
    rdt->setFixedHeight(24);

    h3->addWidget(ldt, 1);
    h3->addWidget(rdt, 1);

    h2->addWidget(splitterUp);
    h4->addWidget(splitterDown);

    mainLayout->addLayout(h1);
    mainLayout->addLayout(h2);
    mainLayout->addLayout(h3);
    mainLayout->addLayout(h4);

    mainWidget->setLayout(mainLayout);
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

