#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdafx.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:




private:
    Ui::MainWindow *ui;
    QTimer *myTimer;


private slots:
    void photo_window_update(Mat, int);
    void un_redo();
    void detectAndDisplay(Mat);

    void on_actionOpen_File_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionSave_File_triggered();
    void on_shapen_btn_clicked();
    void on_blur_btn_clicked();
    void on_magic_set_btn_clicked();
    void on_contrast_slider_sliderReleased();
    void on_brightness_slider_sliderReleased();
    void on_set_bright_btn_clicked();
    void on_set_con_btn_clicked();
};

#endif // MAINWINDOW_H
