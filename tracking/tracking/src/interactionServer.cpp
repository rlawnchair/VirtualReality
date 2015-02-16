#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "vrpnTrackerColors.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{

    Mat CameraMatrix;
    Mat DistoMatrix;
    const string inputSettingsFile = argc > 1 ? argv[1] : "default.xml";
    FileStorage fs(inputSettingsFile, FileStorage::READ); // Read the settings
    if (!fs.isOpened())
    {
        cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
        return -1;
    }
    fs["Camera_Matrix"] >> CameraMatrix;
    fs["Distortion_Coefficients"] >> DistoMatrix;
    fs.release();                                         // close Settings file

    // cree la connexion vrpn
    vrpn_Connection* connection = vrpn_create_server_connection();

    // cree un button tracker colors
    vrpnTrackerColors* tracker = new vrpnTrackerColors("Tracker",connection,CameraMatrix,DistoMatrix);

    while(true) {

        // met à jour le device
        tracker->mainloop();

        // met à jour la connexion
        connection->mainloop();

        char c = (char)waitKey(10);
        if( c  == 27 /*ESC*/ || c == 'q' || c == 'Q' )
            break;

        vrpn_SleepMsecs(1);
    }

    return 0;
}

