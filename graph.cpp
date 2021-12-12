#include "graph.h"

Graph::Graph()
{
    init();
    set_attributes();
    set_resolution(10000);
    set_graph_ticks(15,15);
    set_x_extremes(-10,10);
    set_y_extremes(-10,10);
//    draw();
    connect(evaluator,SIGNAL(error_message(QString)),this,SLOT(set_error_message(QString)));
}

Graph::~Graph()
{
    delete graph;
    delete series;
    delete x_axis;
    delete y_axis;
}



void Graph::set_x_extremes(double min_x, double max_x)
{
    this->min_x =min_x;
    this->max_x =max_x;

    x_axis->setMin(min_x);
    x_axis->setMax(max_x);
    x_axis->setTickCount(graph_y_ticks);
}

void Graph::set_y_extremes(double min_y, double max_y)
{
    if (evaluator->is_const()){
        if (min_y>0){
            y_axis->setMin(0);
            y_axis->setMax(1.5*max_y);
        }else{
            y_axis->setMax(0);
            y_axis->setMin(1.5*max_y);
        }
        }
    else{
        y_axis->setMin(min_y);
        y_axis->setMax(max_y);
    }
    y_axis->setTickCount(graph_y_ticks);
}

void Graph::set_resolution(int num_points)
{
    this->num_points = num_points;
}

void Graph::draw()
{
    if (!can_draw){
        qDebug()<<"BAD EXPRESSION , CEASING TO DRAW";
        delete series;
        series=nullptr;
        return;
    }
    set_y_extremes(get_y(min_x),get_y(max_x));
    graph->removeAllSeries();
    series = new QLineSeries();
    double x ;
    if(max_x>=0 && min_x<=0){
        get_y(0);
    }
    for (int step=0;step<num_points && can_draw;step++){
        x = min_x + step* (max_x-min_x)/num_points;
        series->append(x,get_y(x));
        if (!can_draw){
            qDebug()<<"BAD EXPRESSION , CEASING TO DRAW";
            delete series;
            series=nullptr;
            return;
        }
    }
    if (!can_draw){
        qDebug()<<"BAD EXPRESSION , CEASING TO DRAW";
        delete series;
        series=nullptr;
        return;
    }
    x = max_x;
    graph->addSeries(series);
    series->attachAxis(x_axis);
    series->attachAxis(y_axis);
    qDebug()<<"==================================================";
}

void Graph::set_graph_ticks(int x, int y)
{
    graph_x_tick=x;
    graph_y_ticks=y;
}

void Graph::set_function(QString func)
{
    can_draw=true;
    evaluator->set_expression(func);
}

double Graph::get_y(double x)
{
//    qDebug()<<"in get_y , x = "<<x<<"\t y = "<<evaluator->evaluate(x);
    return evaluator->evaluate(x);
}

void Graph::set_error_message(QString message)
{
    can_draw=false;
    emit error_message(message);
//    evaluator->set_expression(0);
}

void Graph::init()
{
    graph = new QChart();
    evaluator = new ExpressionEvaluator();
    series = new QLineSeries();

    x_axis = new QValueAxis(graph);
    y_axis = new QValueAxis(graph);
    can_draw=true;

}

void Graph::set_attributes()
{
    graph->legend()->hide();
    graph->setTitle("Function plotter");
    graph->setTheme(QChart::ChartTheme::ChartThemeBrownSand);
    graph->setTitleFont(QFont("Serif",-1,-1,true));

    this->setChart(graph);
    this->setRenderHint(QPainter::Antialiasing);

    x_axis->setTitleText("X");
    y_axis->setTitleText("Y");

    x_axis->setTitleFont(QFont("Serif",-1,-1,true));
    y_axis->setTitleFont(QFont("Serif",-1,-1,true));

    graph->addAxis(x_axis,Qt::AlignBottom);
    graph->addAxis(y_axis,Qt::AlignLeft);

}

//void Graph::check_zero_inrange()
//{
//    if (min_x<=0 && max_x>=0){
//        set_graph_ticks(graph_x_tick+1,graph_y_ticks+1);
//    }
//}

