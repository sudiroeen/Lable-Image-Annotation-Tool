/*

Copyright:
  
  Sudiro
			[at] SudiroEEN@gmail.com

  available:
  		on my github:
  			github.com/sudiroeen
*/



#include <opencv2/opencv.hpp>
#include <iostream>
#include <dirent.h>
#include <cstring> 


using namespace std;
using namespace cv;


// #define INITIAL

#define KOLOM 640
#define BARIS 480

Mat gambar;
Mat gambarClone;
Point titikStart;
bool afterDownBeforeUp = false;
Rect rectROI;
bool _EV;

static void readMouse(int event, int x, int y, int, void*){
    int xrs, yrs, lx, ly;

    _EV = false;

    if(afterDownBeforeUp){
        gambar = gambarClone.clone();
        xrs = min(titikStart.x, x);
        yrs = min(titikStart.y, y);
        lx = max(titikStart.x, x) - min(titikStart.x, x);
        ly = max(titikStart.y, y) - min(titikStart.y, y);
        rectROI = Rect(xrs, yrs, lx+1, ly+1);

        rectangle(gambar, rectROI,Scalar(255, 0, 0), 1);
    }
    if(event == EVENT_LBUTTONDOWN){
        titikStart = Point(x,y);
        rectROI = Rect(x,y,0,0);
        afterDownBeforeUp = true;

    }else if(event == EVENT_LBUTTONUP){
        Mat roi(gambarClone.clone(), rectROI);
        imshow("roi", roi);

        _EV = true;

        afterDownBeforeUp = false;
    }

}

Mat bacaYAML(string _path){
	FileStorage _fs(_path, FileStorage::READ);
	Mat _matFS;
	_fs["TabelMat"] >> _matFS;
	_fs.release();
	
	return _matFS;
}

void simpan(const string& path, Mat& TabelMat, const Mat& _baris, const Mat& _index){
	// cout << TabelMat.row(_baris.at<int>(0)) << "\t";
	
	_baris.copyTo(TabelMat.row(_baris.at<int>(0)));

	// cout << _baris.at<int>(0) << endl;
	// cout << TabelMat.row(_baris.at<int>(0)) << endl;

	// cout << "_baris: " << _baris << endl;

	FileStorage fsw(path, FileStorage::WRITE);
	fsw << "TabelMat" << TabelMat;
	fsw << "_index" << _index;
	fsw.release();
}

void initializeYAML(string _path, int ukuran){
	FileStorage __fs(_path, FileStorage::WRITE);
	__fs << "TabelMat" << -1 * Mat::ones(ukuran, 5, CV_32SC1);
	__fs << "_index" << -1 * Mat::ones(1, ukuran, CV_32SC1);
	__fs.release();
}

int cvtToInt(const string& _str){
	std::stringstream ss0, ss1, ss2;
	std::string out_str;

	ss0 << _str;

	while(std::getline(ss0, out_str, '/'))continue;
	ss1 << out_str;
	std::getline(ss1, out_str, '.');
	ss2 << out_str;
	while(std::getline(ss2, out_str, '_'))continue;

	int number = std::atoi(out_str.c_str());
	return number;
}


int main(){
	string namaFolder  = "../datasetMata/";

	string pathYAML = "rectImage.yaml";

	vector<string> vs;
	vector<int> vs_index;
/*
	DIR* direktori;
	direktori = opendir(namaFolder.c_str());

	struct dirent *str_dr;
	str_dr = readdir(direktori);

	while(str_dr != NULL){
		stringstream ss(str_dr->d_name);
		if(ss.str().find("mata_") == std::string::npos){
			str_dr = readdir(direktori);
			continue;
		}

		vs_index.push_back(cvtToInt(ss.str()));
		stringstream namaFile;
		namaFile << namaFolder << (str_dr->d_name);
		vs.push_back(namaFile.str());

		str_dr = readdir(direktori);
	}
*/
	for(int i=0; i<160; i++){
		stringstream ss;
		ss << "../datasetMata/mata_" << i << ".jpg";
		vs.push_back(ss.str());
		vs_index.push_back(cvtToInt(ss.str()));
	}
	if(!vs.size()){cout << "size of vs[] is 0" << endl; return -1;}

#ifdef INITIAL
	initializeYAML(pathYAML, vs.size());
#endif

	Mat M_Nx5 = bacaYAML(pathYAML);
	Mat Index_v = Mat(1, vs_index.size(), CV_32S, vs_index.data());

	for(int s=0; s< vs.size(); s++){
		if(s)destroyWindow(vs[s-1]);

		label:
		namedWindow(vs[s], CV_WINDOW_NORMAL);
		setMouseCallback(vs[s], readMouse);

		Mat img = cv::imread(vs[s]);

		gambar = img;
        gambarClone = gambar.clone();

		// cout << img<< endl;
		Mat _row;

		while(true){
			cv::imshow(vs[s], gambar);
			int key =  waitKey(10);

			if((char)key == 'a'){
				if(s){
					destroyWindow(vs[s]);
					if((rectROI.width != 0) || (rectROI.height != 0))
						try{destroyWindow("roi");}catch(...){}
					s--;
				}
				goto label;
			}
			else if((char)key == 'd'){
				if(s != vs.size()-1){
					destroyWindow(vs[s]);
					if((rectROI.width != 0) || (rectROI.height != 0))
						try{destroyWindow("roi");}catch(...){}
					s++;
				}
				goto label;
			}

			bool state_crop;

			if(_EV){
				_EV = false;
				state_crop = true;
				_row = (Mat_<int>(1,5) << vs_index[s], rectROI.x, rectROI.y, rectROI.width, rectROI.height);
				// cout << _row << endl;
			}

			if(state_crop){
				if((char)key == 's'){
					cout << "Rect-" << vs_index[s] << " saved!!!" << endl;
					simpan(pathYAML, M_Nx5, _row, Index_v);
					state_crop = false;
				}
				else if((char)key == 'q'){
					simpan(pathYAML, M_Nx5, _row, Index_v);
					state_crop = false;
					break;
				}
			}
		}
	}
}

