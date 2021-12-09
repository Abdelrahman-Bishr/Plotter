#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    init();
    position_items();
    adjust_graph();
    chart->show();
    connect(chart,SIGNAL(error_message(QString)),this,SLOT(set_error_message(QString)));
    connect(plot_bttn,SIGNAL(clicked()),this,SLOT(draw()));
    connect(error_message_box,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(error_message_box_bttn_clicked(QAbstractButton*)));
}

MainWindow::~MainWindow()
{
    delete central_widget;
    delete function_layout;
    delete min_x_layout ;
    delete max_x_layout;
    delete central_widget_layout;
    delete x_extremes_layout;

    central_widget = nullptr;
    function_layout = nullptr;
    min_x_layout = nullptr;
    max_x_layout = nullptr;
    central_widget_layout = nullptr;
    x_extremes_layout = nullptr;
}

void MainWindow::draw()
{
    if (function_layout->get_function()==""){
        set_error_message("Empty Expression");
        return;
    }
    chart->set_x_extremes(min_x_layout->get_val(),max_x_layout->get_val());
    chart->set_function(function_layout->get_function());
    chart->draw();
}

void MainWindow::set_error_message(QString message)
{
    error_message_box->setText(message);
    error_message_box->show();
    qDebug()<<"error message is"<<message;
}

void MainWindow::error_message_box_bttn_clicked(QAbstractButton *button)
{
    qDebug()<<"function_layout_edit_box should grab keyboard";
    function_layout->edit_box_grab_keyboard();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return)
        draw();
}

void MainWindow::init()
{
    this->resize(800,600);
    central_widget = new QWidget(this);

    chart = new Graph();

    function_layout = new Custom_form_layout("f(x) = ");
    min_x_layout = new Custom_form_layout("Min x");
    max_x_layout = new Custom_form_layout("Max x");
    central_widget_layout = new QVBoxLayout();
    x_extremes_layout = new QHBoxLayout();
    plot_bttn = new QPushButton("Plot",central_widget);

    error_message_box = new QMessageBox();
}

void MainWindow::position_items()
{
    this->setCentralWidget(central_widget);
    central_widget->setLayout(central_widget_layout);
    central_widget_layout->addWidget(chart);
    central_widget_layout->addLayout(function_layout);
    central_widget_layout->addLayout(x_extremes_layout);

    x_extremes_layout->addLayout(min_x_layout);
    x_extremes_layout->addLayout(max_x_layout);
    central_widget_layout->addWidget(plot_bttn,0,Qt::AlignTrailing);
}

void MainWindow::adjust_graph()
{

}



