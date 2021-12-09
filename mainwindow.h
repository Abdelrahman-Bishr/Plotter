#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include "graph.h"
#include "custom_form_layout.h"
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void draw();
    void set_error_message(QString message);
    void error_message_box_bttn_clicked(QAbstractButton * button);

private:
    QWidget * central_widget;
    Custom_form_layout * function_layout,*min_x_layout,*max_x_layout;
    QVBoxLayout * central_widget_layout;
    QHBoxLayout * x_extremes_layout;
    Graph * chart;
    QPushButton * plot_bttn;
    QMessageBox * error_message_box;

    void keyPressEvent(QKeyEvent * event);
    void init();
    void position_items();
    void adjust_graph();
};
#endif // MAINWINDOW_H
