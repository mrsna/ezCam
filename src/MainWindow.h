//
// Created by silas on 25.09.2025.
//

#ifndef EZCAM_MAINWINDOW_H
#define EZCAM_MAINWINDOW_H

#endif //EZCAM_MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <QVideoWidget>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QCameraDevice>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void showContextMenu(const QPoint &pos);

    ~MainWindow() override = default;

private:
    QVideoWidget *videoWidget{nullptr};
    QCamera *camera{nullptr};
    QMediaCaptureSession *captureSession{nullptr};

    void setupMenus();

    void keyPressEvent(QKeyEvent *event);

    void setCamera(const QCameraDevice &device);
};