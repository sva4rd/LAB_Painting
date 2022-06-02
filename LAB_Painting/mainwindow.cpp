#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    setWindowTitle("Painting");
    setMinimumSize(500, 500);
    move(0, 0);
    resize(QGuiApplication::screens().at(0)->geometry().width(),
           QGuiApplication::screens().at(0)->geometry().height());

    paintWGT = new Paint(this);
    setCentralWidget(paintWGT);

    fileMenu = new QMenu("File", this);
    fileMenu->addAction("&Open file", this, SLOT(openFile()), QKeySequence("CTRL+O"));
    fileMenu->addAction("&Save file", this, SLOT(saveFile()), QKeySequence("CTRL+S"));
    fileMenu->addSeparator();
    fileMenu->addAction("&Exit", qApp, SLOT(quit()), QKeySequence("CTRL+Q"));

    settingsMenu = new QMenu("Settings", this);
    QMenu *typeSubMenu = new QMenu("Type", this);
    typeSubMenu->addAction("&Line", this, SLOT(lineSelected()));
    typeSubMenu->addAction("&Curve", this, SLOT(curveSelected()));
    typeSubMenu->addAction("&Rectangle", this, SLOT(rectangleSelected()));
    typeSubMenu->addAction("&Ellipse", this, SLOT(ellipseSelected()));
    settingsMenu->addMenu(typeSubMenu);
    settingsMenu->addAction("&Brush color", this, SLOT(setCurrentBrushCol()), QKeySequence("CTRL+B"));
    settingsMenu->addAction("&Pen color", this, SLOT(setCurrentPenCol()), QKeySequence("CTRL+P"));
    settingsMenu->addAction("&Pen width", this, SLOT(setCurrentStyles()), QKeySequence("CTRL+W"));
    brushStyle = settingsMenu->addAction("&No brush", this, SLOT(setBrushStyle()));
    brushStyle->setCheckable(true);
    brushStyle->setChecked(true) ;
    settingsMenu->addSeparator();
    settingsMenu->addAction("&Clear", this, SLOT(clearFigures()), QKeySequence("CTRL+L"));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(settingsMenu);

    curFigure = new QLabel("Current figure: CURVE", this);
    statusBar()->addWidget(curFigure);


    widDial = new QDialog(this);
    widDial->setModal(true);
    widDial->setFixedSize(170, 200);
    QVBoxLayout *dialboxV = new QVBoxLayout();
    ok = new QPushButton("OK",widDial);
    width1 = new QRadioButton("1 px",widDial);
    width2 = new QRadioButton("3 px",widDial);
    width3 = new QRadioButton("5 px",widDial);
    width4 = new QRadioButton("10 px",widDial);

    ok->resize(100,30);
    dialboxV->addWidget(width1);
    dialboxV->addWidget(width2);
    dialboxV->addWidget(width3);
    dialboxV->addWidget(width4);
    dialboxV->addWidget(ok);
    connect(ok,&QPushButton::clicked,this,  &MainWindow::setPenWidth);
    widDial->setLayout(dialboxV);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(unsavedData())
        event->accept();
    else event->ignore();
}

bool MainWindow::unsavedData()
{
    if(paintWGT->newData)
    {
        QMessageBox::StandardButton rezult = QMessageBox::warning(this, "Warning", "Do you want to save the data?",
                                                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (rezult == QMessageBox::Yes)
            saveFile();
        else if (rezult == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::setFigureType(FigureType t, QString s)
{
    paintWGT->currentType = t;
    curFigure->setText(QString("Current figure: %1").arg(s));
}

void MainWindow::openFile()
{
    if(unsavedData())
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath());
        if(!fileName.isEmpty())
            paintWGT->openImg(fileName);
    }
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath(), "BIN (*.bin)");
    if(!fileName.isEmpty())
        paintWGT->saveImg(fileName);
}

void MainWindow::lineSelected()
{
    setFigureType(FigureType::LINE, "LINE");
}

void MainWindow::curveSelected()
{
    setFigureType(FigureType::CURVE, "CURVE");
}

void MainWindow::rectangleSelected()
{
    setFigureType(FigureType::RECTANGLE, "RECTANGLE");
}

void MainWindow::ellipseSelected()
{
    setFigureType(FigureType::ELLIPSE, "ELLIPSE");
}

void MainWindow::setCurrentBrushCol()
{
    QColor col = QColorDialog::getColor(paintWGT->currentBrush.color());
    if(col.isValid())
        paintWGT->currentBrush.setColor(col);
}

void MainWindow::setCurrentPenCol()
{
    QColor col = QColorDialog::getColor(paintWGT->currentPen.color());
    if(col.isValid())
        paintWGT->currentPen.setColor(col);
}

void MainWindow::setCurrentStyles()
{
    if(paintWGT->currentPen.width() == 1)
        width1->setChecked(1);
    else if(paintWGT->currentPen.width() == 3)
        width2->setChecked(1);
    else if(paintWGT->currentPen.width() == 5)
        width3->setChecked(1);
    else if(paintWGT->currentPen.width() == 10)
        width4->setChecked(1);
    widDial->exec();

}

void MainWindow::setPenWidth()
{
    widDial->close();
    if(width1->isChecked())
        paintWGT->currentPen.setWidth(1);
    if(width2->isChecked())
        paintWGT->currentPen.setWidth(3);
    if(width3->isChecked())
        paintWGT->currentPen.setWidth(5);
    if(width4->isChecked())
        paintWGT->currentPen.setWidth(10);
}

void MainWindow::setBrushStyle()
{
    if(brushStyle->isChecked())
        paintWGT->currentBrush.setStyle(Qt::NoBrush);
    else
        paintWGT->currentBrush.setStyle(Qt::SolidPattern);
}

void MainWindow::clearFigures()
{
    paintWGT->figures.clear();
    paintWGT->newData = false;
    paintWGT->update();
}

MainWindow::~MainWindow()
{
    delete paintWGT;
}
