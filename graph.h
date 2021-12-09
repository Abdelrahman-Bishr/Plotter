#ifndef GRAPH_H
#define GRAPH_H
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QDebug>
#include "expressionevaluator.h"

using namespace QtCharts ;

class Graph : public QChartView
{
    Q_OBJECT
public:
    Graph();
    ~Graph();
    void set_x_extremes(double min_x,double max_x);
    void set_y_extremes(double min_y,double max_y);
    void set_resolution(int num_points=100);
    void draw();
    void set_graph_ticks(int x=10,int y=10);
    void set_function(QString func);
    double get_y(double x);
signals:
    void error_message(QString message);
//    add a slot to receive error messages from evaluator , and propagate them to
//    main window so that they can be presented to user , and interrupt evaluation and drawing
public slots:
    void set_error_message(QString message);

private:
    QChart * graph;
    QLineSeries * series;
    QValueAxis *x_axis,* y_axis;
    ExpressionEvaluator * evaluator;
    bool can_draw;
    int num_points , graph_x_tick , graph_y_ticks;
    double min_x,max_x;

    void init();
    void set_attributes();
//    void check_zero_inrange();
};

#endif // GRAPH_H
