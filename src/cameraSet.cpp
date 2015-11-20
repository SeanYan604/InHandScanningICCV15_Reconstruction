// Author: Dimitrios Tzionas
//
// source code for the work:
//
// Dimitrios Tzionas and Juergen Gall
// 3D Object Reconstruction from Hand-Object Interactions
// International Conference on Computer Vision (ICCV) 2015
// http://files.is.tue.mpg.de/dtzionas/In-Hand-Scanning


#include "cameraSet.h"


CameraSet::CameraSet()
{
}


CameraSet::CameraSet( QString INPUT_PATH_CAMERA_SET )
{
        setupCameras( INPUT_PATH_CAMERA_SET );

        currentCameraID = 0;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void CameraSet::setupCameras( QString INPUT_PATH_CAMERA_SET )
{

        QFile myFile( INPUT_PATH_CAMERA_SET );

        myFile.open(QIODevice::ReadOnly | QIODevice::Text);

        if( !myFile.isOpen() )
        {
            qDebug() << "CameraSet::setupCameras - ERROR, unable to open **" << INPUT_PATH_CAMERA_SET << "** for CameraConf Input";
            return;
        }

        QTextStream myTextStream(&myFile);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        double tempDoubleToInt;

        myTextStream >> tempDoubleToInt;

        totalCameras = tempDoubleToInt;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        cameraMatricesStruct tempCameraStruct;

        tempCameraStruct.KKK    = Eigen::MatrixXd::Zero(3,3);
        tempCameraStruct.RRR    = Eigen::MatrixXd::Zero(3,3);
        tempCameraStruct.TTT    = Eigen::MatrixXd::Zero(3,1);
        tempCameraStruct.RT_3x4 = Eigen::MatrixXd::Zero(3,4);
        tempCameraStruct.RT_4x4 = Eigen::MatrixXd::Zero(4,4);
        tempCameraStruct.P_3x4  = Eigen::MatrixXd::Zero(3,4);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        cameras.clear();
        cameras.resize( totalCameras     );
        cameras.fill(   tempCameraStruct );

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        for (int camID=0; camID<totalCameras; camID++)
        {

                for     (int iii=0; iii<3; iii++)
                {   for (int jjj=0; jjj<3; jjj++)     myTextStream >> cameras[camID].KKK(iii,jjj);
                }

                ////////////////////////////////////////////////////////////////////////////////////
                ////////////////////////////////////////////////////////////////////////////////////

                for     (int iii=0; iii<3; iii++)
                {   for (int jjj=0; jjj<3; jjj++)     myTextStream >> cameras[camID].RRR(iii,jjj);
                                                      myTextStream >> cameras[camID].TTT(iii);
                }

                cameras[camID].RT_3x4 << cameras[camID].RRR, cameras[camID].TTT;

                cameras[camID].RT_4x4 << cameras[camID].RT_3x4(0,0), cameras[camID].RT_3x4(0,1), cameras[camID].RT_3x4(0,2), cameras[camID].RT_3x4(0,3),
                                         cameras[camID].RT_3x4(1,0), cameras[camID].RT_3x4(1,1), cameras[camID].RT_3x4(1,2), cameras[camID].RT_3x4(1,3),
                                         cameras[camID].RT_3x4(2,0), cameras[camID].RT_3x4(2,1), cameras[camID].RT_3x4(2,2), cameras[camID].RT_3x4(2,3),
                                         0,                          0,                          0,                          1;


                cameras[camID].P_3x4 =  cameras[camID].KKK * cameras[camID].RT_3x4;

        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        myFile.close();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void CameraSet::print()
{

        std::cout << totalCameras << std::endl << std::endl;

        for (int cam=0; cam<totalCameras; cam++)
        {
            std::cout << cameras[cam].KKK    << std::endl << std::endl;
            std::cout << cameras[cam].RT_4x4 << std::endl << std::endl << std::endl;
        }

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



bool CameraSet::switchCamera( int newCurrCamID )
{
        /////////////////////////////////////////////////
        if (newCurrCamID >= totalCameras)   return false;
        /////////////////////////////////////////////////

        currentCameraID = newCurrCamID;

        ////////////
        return true;
        ////////////
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



Eigen::Vector2d     CameraSet::project__3D_Vertex__to__2D_Point(const Eigen::Vector3d &vert_3x1, const int camID)
{

    Eigen::MatrixXd vert_4x1(4,1);
                    vert_4x1 << vert_3x1(0),
                                vert_3x1(1),
                                vert_3x1(2),
                                1;

    Eigen::Vector3d proj_3x1 = cameras[ camID ].P_3x4 * vert_4x1;

    Eigen::Vector2d proj_2x1;
                    proj_2x1 << proj_3x1(0) / proj_3x1(2),
                                proj_3x1(1) / proj_3x1(2);
             ////////////////
             return proj_2x1;
             ////////////////

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



