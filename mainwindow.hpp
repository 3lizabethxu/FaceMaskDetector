/**
 * @file mainwindow.hpp
 * @brief Header file for the user interface, mainly used to import Qt widgets and set up pointers to be used by the interface
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsView>
#include <QWidget>
#include <QGridLayout>
#include <QTextEdit>
#include <QStackedWidget>
#include <QRadioButton>
#include <QListWidget>
#include <QComboBox>
#include <QTimer>
#include <QMessageBox>
#include <QSlider>
#include <QDoubleSpinBox>
#include <time.h>

#include "opencv.hpp"
#include "environment.hpp"
#include "Face.hpp"
#include "FaceDetector.hpp"
#include "Report.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    // button clicks
    void pauseClicked();
    void recordClicked();
    void exportClicked();

    // zoom slider changed
    void sliderChanged(int value);

    // sensitivity updated
    void sensitivityChanged(double value);

private:
    // utilities
    QTimer *timer;
    QGridLayout *mainLayout;

    // labels
    QLabel *liveFeedLabel;
    QLabel *statisticsLabel;
    QLabel *complianceLabel;
    QLabel *peopleLabel;

    QLabel *controlsLabel;
    QLabel *zoomLabel;
    QLabel *sensitivityLabel;

    QPushButton *pauseButton;
    QPushButton *recordButton;
    QPushButton *exportButton;

    QSlider *zoomSlider;
    QDoubleSpinBox *sensitivityInput;

    std::string outputLocation;

};
#endif // MAINWINDOW_H
