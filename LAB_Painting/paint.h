#ifndef PAINT_H
#define PAINT_H

#include <QtWidgets>

enum class FigureType
{
    LINE,
    CURVE,
    RECTANGLE,
    ELLIPSE
};

class Paint : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
private:
    Paint(QWidget *parent = nullptr);
    ~Paint();

    struct FigureData
    {
        FigureType type;
        QList<QPointF> dots;
        QSize size = QSize(0,0);
        QBrush brush;
        QPen pen;
    };
    QList<FigureData> figures;
    bool newData;
    FigureType currentType;
    QBrush currentBrush;
    QPen currentPen;
    QSize currentSize;

    double xProp;
    double yProp;

    double curWid;
    double curHeig;

    void setFigureData(QPointF);
    void resizeFigures();
    bool openImg(const QString &fileName);
    bool saveImg(const QString &fileName);

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent (QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // PAINT_H
