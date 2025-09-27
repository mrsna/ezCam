//
// Created by silas on 25.09.2025.
//

#include "MainWindow.h"
#include <QMenuBar>
#include <QActionGroup>
#include <QScreen>
#include <QKeyEvent>
#include <QShortcut>
#include <QMessageBox>
#include <QDebug>
#include <qguiapplication.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("ezCam");
    //resize(720, 480);
    setMinimumSize(350, 200);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    QSize halfSize(screenGeometry.width() / 2, screenGeometry.height() / 2);

    resize(halfSize);

    captureSession = new QMediaCaptureSession(this);

    videoWidget = new QVideoWidget(this);
    setCentralWidget(videoWidget);

    captureSession->setVideoOutput(videoWidget);

    setupMenus();

    if (auto cameras = QMediaDevices::videoInputs(); !cameras.isEmpty()) {
        setCamera(cameras.first());
    }
}

void MainWindow::setupMenus()
{
    //Cam menu
    QMenu *cameraMenu = menuBar()->addMenu(tr("Camera"));
    auto *group = new QActionGroup(this);
    group->setExclusive(true);

    const auto cameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &device : cameras) {
        auto *action = new QAction(device.description(), cameraMenu);
        action->setCheckable(true);
        cameraMenu->addAction(action);
        group->addAction(action);

        connect(action, &QAction::triggered, this, [this, device, action]() {
            setCamera(device);
            action->setChecked(true);
        });
    }

    //menuBar()->addSeparator();

    //Options Menu
    QMenu *optionsMenu = menuBar()->addMenu(tr("Options"));

    //fullscreen
    auto *fullscreenAction = new QAction(tr("Fullscreen"), optionsMenu);
    fullscreenAction->setObjectName("fullscreenAction");
    fullscreenAction->setCheckable(true);
    optionsMenu->addAction(fullscreenAction);

    connect(fullscreenAction, &QAction::toggled, this, [this](bool on) {
        if (on) {
            showFullScreen();
            menuBar()->setVisible(false);
        } else {
            showNormal();
            menuBar()->setVisible(true);
        }
    });

    auto *fsShortcut = new QShortcut(QKeySequence(QKeySequence::FullScreen), this);
    connect(fsShortcut, &QShortcut::activated, this, [this, fullscreenAction]() {
        fullscreenAction->toggle();
    });

    //about cam
    auto *cameraInfoAction = new QAction(tr("About Camera..."), optionsMenu);
    optionsMenu->addAction(cameraInfoAction);

    connect(cameraInfoAction, &QAction::triggered, this, [this]() {
        if (!camera) {
            QMessageBox::warning(this, tr("Camera Info"), tr("No camera is active."));
            return;
        }

        QCameraDevice device = camera->cameraDevice();

        QString info;
        info += tr("Description: %1\n").arg(device.description());
        info += tr("ID: %1\n").arg(QString::fromUtf8(device.id()));
        info += tr("Position: %1\n").arg(
            device.position() == QCameraDevice::FrontFace ? "Front" : "Back"
        );

        //get formats
        info += tr("\nSupported video formats:\n");
        for (const QCameraFormat &format : device.videoFormats()) {
            info += tr("  - %1 x %2 @ %3 fps\n")
                    .arg(format.resolution().width())
                    .arg(format.resolution().height())
                    .arg(format.maxFrameRate());
        }

        QMessageBox::about(this, tr("Camera Info"), info);
    });

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && isFullScreen()) {
        showNormal();
        menuBar()->setVisible(true);

        auto *fsAction = findChild<QAction*>("fullscreenAction");
        if (fsAction)
            fsAction->setChecked(false);
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::setCamera(const QCameraDevice &device)
{
    if (camera) {
        camera->stop();
        captureSession->setCamera(nullptr);
        camera->deleteLater();
        camera = nullptr;
    }

    camera = new QCamera(device, this);
    captureSession->setCamera(camera);
    camera->start();


}
