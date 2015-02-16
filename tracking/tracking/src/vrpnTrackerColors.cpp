#include "vrpnTrackerColors.hpp"

using namespace std;
using namespace cv;

vrpnTrackerColors::vrpnTrackerColors(std::string trackerName, 
                                     vrpn_Connection * con, cv::Mat CameraMatrix, cv::Mat DistoMatrix):
    vrpn_Tracker(trackerName.c_str(), con) {

    this->cameraMatrix = CameraMatrix;
    this->distCoeffs = DistoMatrix;
    // On ouvre la premiere camera
    m_cap.open(0);
    if(!m_cap.isOpened()){
        printf("Can't find the camera\n");
    }else{
        printf("Camera found\n");
    }

    m_cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    m_cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);

    // On ouvre une fenetre pour l'affichage des etapes du traitement
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );

    this->taillePostIt = 76;
}

vrpnTrackerColors::~vrpnTrackerColors(){
}

void vrpnTrackerColors::mainloop() {

    //mise a jour du serveur
    server_mainloop();

    Mat frame;
    m_cap>>frame;

    //Conversion de l'image de Bayer vers BGR => nécessaire pour les cameras Firewire
    cvtColor(frame, frame, CV_BayerBG2BGR);

    Mat image_src = frame.clone();
    undistort(image_src, frame, this->cameraMatrix, this->distCoeffs);

    //Affichage de l'image brute
    imshow( "videoInputRaw", frame);

    //2. TODO : CONVERTISSEZ L'IMAGE EN HSV avec cvtColor
    cvtColor(frame, frame, CV_BGR2HSV);
    imshow( "videoInput", frame);
    //3. TODO : REMPLISSEZ AVEC LES BONNES COULEURS POUR TRACKER LE MARQUEUR
    // (LE DERNIER PARAMETRE CORRESPOND A L'IDENTIFIANT DU CAPTEUR DU TRACKER)
    // Hue -> 0 - 180   | S & V  0 - 255
    int TeinteMin = 5;
    int TeinteMax = 15;
    int SaturationMin = 140;
    int SaturationMax = 180;
    int ValeurMin = 200;
    int ValeurMax = 255;
    trackColor(frame, Scalar(TeinteMin, SaturationMin, ValeurMin, 0),
               Scalar(TeinteMax, SaturationMax, ValeurMax, 0), 0);

    //BONUS : PERMETTEZ A L'UTILISATEUR DE SELECTIONNER LA COULEUR A TRACKER
    // EN CLIQUANT SUR L'IMAGE

}

void vrpnTrackerColors::trackColor(Mat& frame, cv::Scalar hsv_min, 
                                   cv::Scalar hsv_max, int sensor_id) {

    Mat frameRange;
    //1. TODO : Filtrer l'intervalle de couleur HSV avec inRange
    inRange(frame,hsv_min,hsv_max,frameRange);

    //AFFICHAGE APRES INTERVALLE COULEURS HSV
    imshow( "Intervalle", frameRange);

    Mat frameThreshold;// = frameRange.clone();
    //2. TODO : EFFECTUEZ LE SEUILLAGE BINAIRE avec la fonction threshold
    threshold(frameRange,frameThreshold,0.5,255.0,THRESH_BINARY);

    //3. TODO : ELIMINEZ LES ZONES BRUITEES avec les fonctions erode et dilate
    int erosion_type = MORPH_ELLIPSE;
    int erosion_size = 2;
    Mat element = getStructuringElement( erosion_type,
                                         Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                         Point( erosion_size, erosion_size ) );

    /// Apply the erosion operation
    dilate(frameThreshold, frameThreshold, element);
    erode(frameThreshold, frameThreshold, element);

    //AFFICHAGE APRES SEUILLAGE
    imshow( "Thresh", frameThreshold);

    Mat drawing = Mat::zeros( frameThreshold.size(), CV_8UC3 );

    ///4. TODO : EXTRAYEZ LES CONTOURS DE L'IMAGE SEUILLEE AVEC findContours et contour_poly
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(frameThreshold,contours,hierarchy,RETR_TREE,CHAIN_APPROX_TC89_KCOS);

    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );
    bool found=false;
    RNG rng(12345);
    Scalar color(rng.uniform(0, 255),rng.uniform(0, 255),rng.uniform(0, 255));
    int idMax = 0;
    float radiusmax = 0.0;
    for( unsigned int i = 0; i < contours.size(); i++ ){
        //5. TODO : RECUPEREZ LES CERCLES ENGLOBANTS avec approxPolyDP et minEnclosingCircle
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );

        //6. TODO : TESTER LA TAILLE DES CERCLES, NE CONSERVEZ QUE LA PLUS
        //GRANDE RECUPEREZ LA POSITION DU CENTRE DES BOITES AVEC center[i]
        //LEUR TAILLE avec radius[i] ET REMPLISSEZ LE TABLEAU POS[3]
        if(radius[i]>radiusmax){
            idMax = i;
            radiusmax = radius[idMax];
            found = true;
        }

        //circle(frame,center[i],(int)radius[i],color);
    }
    if(found){
        //std::cout << idMax << "Center " << center[idMax] << "Radius " << radius[idMax] << " " <<radiusmax << std::endl;

        //7. TODO : DESSINER LES CERCLES
        drawContours(frameThreshold,contours_poly,idMax,color);
        circle(frame,center[idMax],(int)radius[idMax],color);

        pos[0] = center[idMax].x;
        pos[1] = center[idMax].y;
        pos[2] = radius[idMax];

        float ab = 2*radius[idMax];

        char msgbuf[1000];
        vrpn_Tracker::timestamp = _timestamp;
        d_sensor=sensor_id;
        int len = vrpn_Tracker::encode_to(msgbuf);
        if (d_connection->pack_message(len, _timestamp, position_m_id,
                                       d_sender_id, msgbuf, vrpn_CONNECTION_LOW_LATENCY)) {
            cerr<<"Color Tracker: can't write message: tossing"<<endl;
        }
    }

    //AFFICHE APRES DETECTION CERCLES sur frame en hsv (d'où les couleurs)
    imshow("Circles", frame);

}

