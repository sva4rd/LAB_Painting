#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "paint.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Paint *paintWGT;
    QMenu *fileMenu;
    QMenu *settingsMenu;
    QMenu *figureMenu;
    QLabel *curFigure;
    QAction* brushStyle;

    QDialog *widDial;
    QRadioButton *width1;
    QRadioButton *width2;
    QRadioButton *width3;
    QRadioButton *width4;
    QPushButton *ok;

    void closeEvent(QCloseEvent *event) override;
    bool unsavedData();
    void setFigureType(FigureType, QString);

private slots:
    void openFile();
    void saveFile();

    void lineSelected();
    void curveSelected();
    void rectangleSelected();
    void ellipseSelected();

    void setCurrentBrushCol();
    void setCurrentPenCol();
    void setCurrentStyles();
    void setPenWidth();
    void setBrushStyle();
    void clearFigures();

};

#endif // MAINWINDOW_H
