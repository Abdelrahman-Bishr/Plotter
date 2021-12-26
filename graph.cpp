#include "graph.h"

Graph::Graph()
{
    init();
    set_attributes();
    set_resolution(10000);
    set_graph_ticks(15,15);
    set_x_extremes(-10,10);
    min_y=-1;
    max_y=1;
    set_y_extremes();
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

void Graph::set_y_extremes()
{
    if (evaluator->is_const()){
//        if (min_y>0){
            y_axis->setMin(evaluator->evaluate(0)-10);
            y_axis->setMax(evaluator->evaluate(0)+10);
//        }else{
//            y_axis->setMax(0);
//            y_axis->setMin(1.5*max_y);
//        }
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
    min_y = INT_MAX;
    max_y = INT_MIN;
    if (!can_draw){
        delete series;
        series=nullptr;
        return;
    }

    graph->removeAllSeries();
    series = new QLineSeries();
    double x ;
    if(max_x>=0 && min_x<=0){
        try {
            get_y(0);
        } catch (std::invalid_argument e) {
            set_error_message(QString(e.what()));
            return;
        }
    }
    double y;
    for (int step=0;step<num_points && can_draw;step++){
        x = min_x + step* (max_x-min_x)/num_points;
        try {
            y=get_y(x);
            this->max_y = std::max(max_y,y);
            this->min_y = std::min(min_y,y);
            series->append(x,y);
        } catch (std::invalid_argument e) {
            set_error_message(QString(e.what()));
            return;
        }
        if (!can_draw){
            delete series;
            series=nullptr;
            return;
        }
    }
    if (!can_draw){
        delete series;
        series=nullptr;
        return;
    }
    x = max_x;
    try {
        set_y_extremes();
    } catch (std::invalid_argument e) {
        set_error_message(QString(e.what()));
        return;
    }
    graph->addSeries(series);
    series->attachAxis(x_axis);
    series->attachAxis(y_axis);

}

void Graph::set_graph_ticks(int x, int y)
{
    graph_x_tick=x;
    graph_y_ticks=y;
}

void Graph::set_function(QString func)
{
    can_draw=true;
    try {
        evaluator->set_expression(func.toStdString());
    } catch (std::invalid_argument e) {
        cout<<e.what();
    }
//    cout<<"function set\n";
}

double Graph::get_y(double x)
{
    try {
        return evaluator->evaluate(x);
    } catch (std::invalid_argument e) {
        throw e;
    }
}

void Graph::set_error_message(QString message)
{
    can_draw=false;
    emit error_message(message);
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
