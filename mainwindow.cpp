/**
 * @file mainwindow.cpp
 * @brief This program comprises the class for the program's user interface, which consists of the camera feed and options for the user to adjust settings
 * @bug no known bugs
 */

/** -- Includes -- **/
#include "mainwindow.hpp"

using namespace cv;
using namespace std;


// global variables are declared because they need to be accessed within a class' lambda function for painting the screen which cannot be edited
// therefore we cannot include the relavent classes in that file to maintain OOP best practices in this case
VideoCapture cap(0);

QLabel *imageFeed;
QLabel *compliancePercentLabel;
QLabel *peopleNumberLabel;

float noMaskCount = 0.0;
float maskCount = 0.0;
float compliancePercent = 0.0;

bool paused = false;
bool recording = false;

VideoWriter video;
Mat initialFrame;

int zoomValue = 0;
int maxPeople = 0;

/**
 * @brief Sets up the main window for the Qt interface, which uses a grid layout to organize the design
 *
 * There are 3 columns for the grid. The first column mainly features the video feed, the second column is a placeholder for spacing,
 * and the third column features settings for the user to interact with the program (i.e. adjusting zoom, play/pause, and summary statistics)
 */
MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    
    // do camera initialization on first paint
    if(!cap.isOpened()){
        cap.open(0);
        cap >> initialFrame;
    }

    // ensures the frame is repainted and updated every 20ms to make the video feed live
    timer = new QTimer;
    timer->setInterval(20);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    
    // check if the video feed is initialized
    if(!cap.isOpened()){
        QMessageBox::critical(
            this,
            tr("Big Brother"),
            tr("Could not initialize camera, please check to see if your camera is installed properly."));
    }

    // holds the content for the main page
    QWidget *mainPage = new QWidget;

    QStackedWidget *stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(mainPage);

    liveFeedLabel = new QLabel;
    liveFeedLabel->setText("Live Feed");

    imageFeed = new QLabel();

    statisticsLabel = new QLabel;
    statisticsLabel->setText("Statistics");
    statisticsLabel->setStyleSheet("font-weight: bold; font-size: 24px;");

    complianceLabel = new QLabel;
    complianceLabel->setText("Mask Compliance");
    complianceLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    compliancePercentLabel = new QLabel;
    compliancePercentLabel->setStyleSheet("font-weight: bold; color: red; font-size: 20px; text-align: center;");
    compliancePercentLabel->setText("100%");
    
    peopleLabel = new QLabel;
    peopleLabel->setText("Total People Detected");
    peopleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    peopleNumberLabel = new QLabel;
    peopleNumberLabel->setStyleSheet("font-weight: bold; font-size: 20px; text-align: center;");
    peopleNumberLabel->setText("0");

    // pause button
    pauseButton = new QPushButton("Pause");
    connect(pauseButton, &QPushButton::released, this, &MainWindow::pauseClicked);
    
    // record button
    recordButton = new QPushButton("Record");
    connect(recordButton, &QPushButton::released, this, &MainWindow::recordClicked);
    
    // export report button
    exportButton = new QPushButton("Export Report");
    connect(exportButton, &QPushButton::released, this, &MainWindow::exportClicked);

    // controls label
    controlsLabel = new QLabel;
    controlsLabel->setStyleSheet("font-weight: bold; font-size: 20px;");
    controlsLabel->setText("Controls");
    
    // zoom label
    zoomLabel = new QLabel;
    zoomLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    zoomLabel->setText("Zoom");
    
    // zoom slider
    zoomSlider = new QSlider(Qt::Horizontal);
    connect(zoomSlider, &QSlider::valueChanged, this, &MainWindow::sliderChanged);
    
    // allow up to a 8x zoom based on height
    int zoomX = initialFrame.size().height;
    
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(zoomX);
    zoomSlider->setValue(0);
    zoomSlider->setTickPosition(QSlider::TicksBelow);
    zoomSlider->setTickInterval(50);
    
    // sensitivity label
    sensitivityLabel = new QLabel;
    sensitivityLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    sensitivityLabel->setText("Mask Detection Sensitivity");
    
    // setup sensitivity
    sensitivityInput = new QDoubleSpinBox;
    connect(sensitivityInput, &QDoubleSpinBox::valueChanged, this, &MainWindow::sensitivityChanged);
    
    sensitivityInput->setValue(MaskDetector::getInstance()->getMaskSensitivity());
    sensitivityInput->setSingleStep(0.05);
    sensitivityInput->setAccelerated(true);
    sensitivityInput->setMinimum(0.01);
    sensitivityInput->setMaximum(0.99);
    sensitivityInput->setDecimals(2);
    
    // layout to support multiple column display
    mainLayout = new QGridLayout(this);
    
    // COLUMN #1
    mainLayout->addWidget(liveFeedLabel, 0, 0, 1, 1);
    
    // this is our main image view
    mainLayout->addWidget(imageFeed, 1, 0, 9, 1);

    mainLayout->setColumnMinimumWidth(1,25);

    // COLUMN #2
    mainLayout->addWidget(statisticsLabel, 0, 2, 1, 1);
    
    mainLayout->addWidget(complianceLabel, 1, 2, 1, 1);
    mainLayout->addWidget(compliancePercentLabel, 2, 2, 1, 1);
    
    mainLayout->addWidget(peopleLabel, 3, 2, 1, 1);
    mainLayout->addWidget(peopleNumberLabel, 4, 2, 1, 1);

    mainLayout->addWidget(controlsLabel, 5, 2, 1, 1);

    mainLayout->addWidget(zoomLabel, 6, 2, 1, 1);
    
    // add action items
    mainLayout->addWidget(zoomSlider, 7, 2, 1, 1);
    
    mainLayout->addWidget(sensitivityLabel, 8, 2, 1, 1);
    mainLayout->addWidget(sensitivityInput, 9, 2, 1, 1);

    mainLayout->addWidget(pauseButton, 10, 2, 1, 1);
    mainLayout->addWidget(recordButton, 11, 2, 1, 1);
    
    mainLayout->addWidget(exportButton, 12, 2, 1, 1);

    mainLayout->setColumnStretch(0,10);
    mainLayout->setColumnStretch(2,5);
    
}

/**
 * @brief Called every time the paint event is fired, we are triggering it every 20ms to ensure the camera view is updated on the UI
 */
void QWidget::paintEvent(QPaintEvent*) {
        
    if(!paused){
    
        // get the frame data
        Mat frame_in;
        cap >> frame_in;

        // flip the video frame so it feels more natural
        Mat frame;
        flip(frame_in, frame, 1);
        
        // extract the current faces that exist on frame
        vector<Rect> faces = FaceDetector().getInstance()->getFaces(frame);
        
        // count faces and add them to the max people
        if((int)faces.size() > maxPeople){
            maxPeople = (int)faces.size();
            peopleNumberLabel->setText(QString(format("%d",maxPeople).c_str()));
        }

        // resize the frame to reduce load on the CPU
        Mat resized;
        cv::resize(frame, resized, Size(frame.cols/RESIZE_SCALE, frame.rows/RESIZE_SCALE));

        // iterate through the faces we have
        for (Rect area : faces)
        {
            
            // setup the face to do heavy lifting behind the scenes
            Face* currentFace = new Face(resized.clone(), area);

            // get the mask status from the face object
            bool hasMask = currentFace->detectMask();

            Scalar drawColor = Scalar(255, 0, 0);

            // text that will be added to screen
            string text = "";
            
            // if they are wear/not wearing a mask we display different statuses
            if(hasMask){
                drawColor = Scalar(0, 255, 0);
                string textString = format("Mask - %d %%", currentFace->getProbabilityOfMask());
                text = textString.c_str();
                maskCount += 1.0;
            }else{
                drawColor = Scalar(0, 0, 255);
                string textString = format("No Mask - %d %%", (100 - currentFace->getProbabilityOfMask()));
                text = textString.c_str();
                noMaskCount += 1.0;
            }

            // add face rectangle
            rectangle(frame, currentFace->getTopLeftPoint(), currentFace->getBottomRightPoint(), drawColor);

            // add text for mask status
            Point coordinates = currentFace->getBottomRightPoint();
            auto font = FONT_HERSHEY_SIMPLEX;
            double fontScale = 1.0;
            
            // add the text object to the frame
            putText(frame, text, coordinates, font, fontScale, drawColor);

        }
        
        // record the frame in our video file if recording
        if(recording){
            video.write(frame);
            circle(frame, Point(40,40), 15, Scalar(0,0,255), FILLED);
        }
        
        // use a crop ratio to ensure the image doesn't scale awkwardly when zoomiinig
        const float cropRatio = (float)frame.size().height/(float)frame.size().width;
        
        // compute the height/width of the new frame after cropping
        const int cropHeight = frame.size().height-((int)((float)zoomValue*cropRatio));
        const int cropWidth = frame.size().width-(zoomValue);
        
        const int offsetW = (frame.cols - cropWidth) / 2;
        const int offsetH = (frame.rows - cropHeight) / 2;
        const Rect roi(offsetW, offsetH, cropWidth, cropHeight);
        frame = frame(roi).clone();
        
        // factor to scale down to fit the image on screen
        const float scaleDown = 0.8;
        
        // scale down the resolution to fit more appropriately
        cv::resize(frame, frame, Size(1280*scaleDown, 720*scaleDown), 0, 0, INTER_CUBIC);

        // Since OpenCV uses BGR order, we need to convert it to RGB
        // NOTE: OpenCV 2.x uses CV_BGR2RGB, OpenCV 3.x uses cv::COLOR_BGR2RGB
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        QImage image = QImage((const unsigned char*)frame.data,frame.cols,
                       frame.rows,frame.step,QImage::Format_RGB888);

        QPainter painter(this);
        
        // display our image inside a label
        imageFeed->setPixmap(QPixmap::fromImage(image));
        
        // this value helps us estimate the compliance of the class
        // add a bias towards compliance to prevent non-compliance from rapidly running down the score
        compliancePercent = (maskCount*3) / (noMaskCount+(maskCount*3));
        
        float value = 0.0;
        value = std::ceil(compliancePercent * 10000.0) / 100.0;

        QString complianceText = QString("%1%").arg(value);
        
        compliancePercentLabel->setText(QString(complianceText));
        
        if(compliancePercent > 0.75){
            compliancePercentLabel->setStyleSheet("font-weight: bold; color: green; font-size: 20px; text-align: center;");
        }else{
            compliancePercentLabel->setStyleSheet("font-weight: bold; color: red; font-size: 20px; text-align: center;");
        }
        
    }
    
}
/**
 * @brief Called when the pause button is clicked
 */
void MainWindow::pauseClicked(){
        
    if(!paused){
        // change the text
        pauseButton->setText("Play");
        // pause video
        paused = true;
        timer->stop();
        
        // stop video recording on pause of video
        video.release();
                
    }else{
        // change the text
        pauseButton->setText("Pause");
        // play video
        paused = false;
        timer->start();
        
        if(recording){
            // stop the video recording
            recordButton->setText("Record");
            // end video recording
            recording = false;
            
            // save the video
            video.release();
                    
            string message = format("Video file saved at %s", outputLocation.c_str());
                    
            // popup that video was saved
            QMessageBox::information(
                this,
                tr("Big Brother"),
                tr(message.c_str()));
        }
        
    }
    
}

/**
 * @brief Called when the record button is pressed
 */
void MainWindow::recordClicked(){
        
    if(!recording){
        // change the text
        recordButton->setText("Stop");
        // pause video
        recording = true;
        
        // create video writer to record the camera output
        // get current time for use as file name
        time_t curtime;
        time(&curtime);
        
        string dateTime = format("%s", ctime(&curtime));
        
        dateTime.erase(dateTime.length()-1);
        
        std::replace( dateTime.begin(), dateTime.end(), ' ', '_'); // replace all spaces with _


        MainWindow::outputLocation = format("%s/Mask_Recording_%s.avi", OUTPUT_FOLDER, dateTime.c_str());
        
        cap >> initialFrame;
                
        video.open(outputLocation.c_str(), cv::VideoWriter::fourcc('a','v','c','1'), 10, Size(initialFrame.size().width,initialFrame.size().height));
        
    }else{
        // change the text
        recordButton->setText("Record");
        // end video recording
        recording = false;
        
        // save the video
        video.release();
                
        string message = format("Video file saved at %s", outputLocation.c_str());
                
        // popup that video was saved
        QMessageBox::information(
            this,
            tr("Big Brother"),
            tr(message.c_str()));

    }
    
}
/**
 * @brief Called when the slider is dragged and updated
 */
void MainWindow::sliderChanged(int value){
    
    zoomValue = value;
        
}
/**
 * @brief Called when the user updates the sensitivity
 */
void MainWindow::sensitivityChanged(double value){
    
    // set the sensitivity in our maskdetector class
    MaskDetector::getInstance()->setMaskSensitivity(value);
        
}

/**
 * @brief Is called when the export button is clicked
 */
void MainWindow::exportClicked(){
    
    Report *r = new Report(maxPeople, compliancePercent);
    
    r->exportFile();
    
    string message = format("Report file saved at %s", r->getOutputLocation().c_str());
    
    // popup that report was saved
    QMessageBox::information(
        this,
        tr("Big Brother"),
        tr(message.c_str()));
    
}
