#include "paint.h"

Paint::Paint(QWidget *parent): QWidget{parent}
{
    newData = false;
    currentType = FigureType::CURVE;
    currentPen.setWidth(5);
    currentPen.setColor(QColor(Qt::black));
    setMouseTracking(true);
}

void Paint::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    foreach(FigureData element, figures)
    {
        painter.setBrush(element.brush);
        painter.setPen(element.pen);

        if(element.type==FigureType::LINE)
            painter.drawLine(element.dots[0], element.dots[1]);

        else if(element.type==FigureType::CURVE)
        {
            QPointF first;
            bool bOk = false;
            foreach(QPointF second, element.dots)
            {
                if(bOk)
                    painter.drawLine(first, second);
                first = second;
                bOk = true;
            }
        }

        else if(element.type==FigureType::RECTANGLE)
            painter.drawRect(element.dots[0].x(), element.dots[0].y(), element.size.width(), element.size.height());

        else if(element.type==FigureType::ELLIPSE)
            painter.drawEllipse(element.dots[0].x(), element.dots[0].y(), element.size.width(), element.size.height());
    }
}

void Paint::resizeEvent(QResizeEvent *event)
{
    xProp = (double)width()/(double)event->oldSize().width();
    yProp = (double)height()/(double)event->oldSize().height();
    curWid = width();
    curHeig = height();
    resizeFigures();
    update();
}

void Paint::mousePressEvent (QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        newData = true;
        FigureData newFig;
        newFig.dots.append(event->pos());

        if(currentType == FigureType::LINE)
        {
            newFig.dots.append(QPointF(0,0));
            newFig.type = FigureType::LINE;
        }
        if(currentType == FigureType::CURVE)
            newFig.type = FigureType::CURVE;
        if(currentType == FigureType::RECTANGLE)
            newFig.type = FigureType::RECTANGLE;
        if(currentType == FigureType::ELLIPSE)
            newFig.type = FigureType::ELLIPSE;

        newFig.brush = currentBrush;
        newFig.pen = currentPen;
        figures.append(newFig);
    }
}

void Paint::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
        setFigureData(event->pos());
}

void Paint::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton)
        setFigureData(event->pos());
}

void Paint::setFigureData(QPointF p)
{
    if(currentType == FigureType::LINE)
        figures[figures.size()-1].dots[1] = p;
    else if(currentType == FigureType::CURVE)
        figures[figures.size()-1].dots.append(p);
    else if(currentType == FigureType::RECTANGLE || currentType == FigureType::ELLIPSE)
        figures[figures.size()-1].size = QSize(p.x() - figures[figures.size()-1].dots[0].x(),
                p.y() - figures[figures.size()-1].dots[0].y());
    update();
}

void Paint::resizeFigures()
{
    for(auto &element : figures)
    {
        for(int i=0;i<element.dots.size();i++)
        {
            element.dots[i].setX(element.dots[i].x()*xProp);
            element.dots[i].setY(element.dots[i].y()*yProp);
        }
        element.size.setWidth(element.size.width()*xProp);
        element.size.setHeight(element.size.height()*yProp);
    }
    update();
}

bool Paint::openImg(const QString &fileName)
{
    bool bOk = false;
    QFile file(fileName);
    if( file.open(QIODevice::ReadOnly))
    {
        figures.clear();
        QDataStream in(&file);

        double w, h;
        in >> w >> h;
        xProp = (double)width()/w;
        yProp = (double)height()/h;

        FigureData el;
        int dotsSize;
        while(!in.atEnd())
        {
            in >> el.type >> el.size >> el.brush >> el.pen;
            in >> dotsSize;
            QPointF x;
            for(int i=0; i<dotsSize;i++)
            {
                in >> x;
                el.dots.append(x);
            }

            figures.append(el);
            el.dots.clear();
        }
        resizeFigures();
        bOk = true;
    }
    file.close();
    newData = false;
    update();
    return bOk;
}

bool Paint::saveImg(const QString &fileName)
{
    bool bOk = false;
    QFile file(fileName);
    if( file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out << curWid << curHeig;
        foreach(FigureData element, figures)
        {
            out << element.type << element.size << element.brush << element.pen;
            int s = element.dots.size();
            out << s;
            foreach(QPointF x, element.dots)
                out << x;
        }

        bOk = true;
    }
    file.close();
    newData = false;
    return bOk;
}

Paint::~Paint(){}
