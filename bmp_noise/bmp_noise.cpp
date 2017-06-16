#include <string>
#include<sstream> //�����X�g���[��
#include<fstream>
#include<iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>


using namespace std;


int cols;
int rows;

char Output_Image_name[255];
char Input_Image_name[255];						//���̓t�@�C�����E���͌��̐ݒ�
char input_noise_name[255];
char inputimage_directory[120]="..\\input_Image\\";							//inputimage�̏ꏊ
char outputimage_directory[120]="..\\output_Image\\";						//outputiamge�̏ꏊ
char inputnoise_directory[120]="..\\input_noise_csv\\";
char input_csv_name[255];
char output_csv_name[255];
char sd[255];
int repaet_max=5;
int repeat_count=0;



//main
int main(int argc, char** argv) {

	for(int repeat=1;repeat<=repaet_max;++repeat){
////////////////////////////////�����ݒ�//////////////////////////////////////////////////////////////////

	int no_repeat_flag=0;					//�J��Ԃ��Ȃ��Ƃ�1�ɂ���.���̂Ƃ��CIndependent_noise_name���w��D

	char *Input_Image_s = "3-3�g_usui_11.4";			//���̓t�@�C�����̎w�� .bmp������
	int sd_number=10*repeat;				//�W���΍���

	cols=256;				//����
	rows=256;				//�c��

	

//////////////////���o�̓t�@�C���̃f�B���N�g������//////////////////////////////////////////////////////////
	if(repeat==1){
	sprintf(Input_Image_name,"%s%s.bmp",inputimage_directory,Input_Image_s);						//���͉摜.bmp
	sprintf(input_csv_name,"%s%s.csv",inputimage_directory,Input_Image_s);							//���͉摜�̉�f�l.csv
	}
	sprintf(Output_Image_name,"%s%s_sd%d.bmp",outputimage_directory,Input_Image_s,sd_number);		//�o�͉摜.bmp
	sprintf(output_csv_name,"%s%s_sd%d.csv",outputimage_directory,Input_Image_s,sd_number);			//�o�͉摜�̉�f�l.csv
	sprintf(input_noise_name,"%s256�~256\\sd%d.csv",inputnoise_directory,sd_number);							//�m�C�Y�̉�f�l.csv


	if(no_repeat_flag==1){
	char Independent_noise_name[120]="no_noise.csv";
	sprintf(Output_Image_name,"%s%s_%s.bmp",outputimage_directory,Input_Image_s,Independent_noise_name);	//�o�͉摜.bmp
	sprintf(output_csv_name,"%s%s_%s",outputimage_directory,Input_Image_s,Independent_noise_name);			//�o�͉摜�̉�f�l.csv
	sprintf(input_noise_name,"%s%s",inputnoise_directory,Independent_noise_name);							//�o�͉摜�̉�f�l.csv
	}

////////////////////////InputImage�̉�f�l�̎擾////////////////////////////////////////////////////////////////////////////////////
	//int InputR[256][256];
	///int InputG[256][256];
	//int InputB[256][256];
	int InputRGB[256][256];
	int OutputRGB[256][256];
	FILE *fp_InputRGB;

	cv::Mat ImputImage = cv::imread(Input_Image_name);
	for(int y=0;y<rows;y++){
		for(int x=0;x<cols;x++){
		int R=3*x;
		int G=3*x+1;
		int B=3*x+2;
			//InputR[x][y] = ImputImage.at<unsigned char>(y, R);
			//InputG[x][y] = ImputImage.at<unsigned char>(y, G);
			//InputB[x][y] = ImputImage.at<unsigned char>(y, B);
			//InputRGB[x][y] = (InputR[x][y]+InputB[x][y]+InputB[x][y])/3;						//�J���[�摜�𔒍��ɕϊ�
		InputRGB[x][y] = ImputImage.at<unsigned char>(y, R);
			//printf("%d,",InputRGB[x][y]);

		}//printf("\n");
	}
	fp_InputRGB = fopen( input_csv_name, "w" );
	if( fp_InputRGB == NULL ){
		printf( "%s�t�@�C�����J���܂���\n", input_csv_name );
		return -1;
	}
	for(int y=0;y<rows;y++){
		for(int x=0;x<cols;x++){

			fprintf( fp_InputRGB, "%d,",InputRGB[x][y]);
		}
			fprintf( fp_InputRGB, "\n");
	}

	fclose( fp_InputRGB );

	printf( "%s�t�@�C���������݂��I���܂���\n", input_csv_name );

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	cv::Mat noise_image = cv::Mat::zeros(cols, rows, CV_8UC1);


	int i=0,j=0;

	int count_small=0;
	ifstream noise(input_noise_name);
	if(!noise){printf("%s������܂���\n",input_noise_name);return 1;}

	string str_noise;
		while(getline(noise,str_noise)){
		string token_noise;
        istringstream stream_noise(str_noise);
		
        while(getline(stream_noise,token_noise,',')){	//��s�ǂݎ��Dsd�̂݁C�J��Ԃ��͈͎̔w��ɗp����
			double tmp_noise=stof(token_noise);			//�����𐔎��ɕϊ�
			sd[count_small]=tmp_noise;				//�z��ɑ��
			
			OutputRGB[count_small][j]=InputRGB[count_small][j]+sd[count_small];
			if(OutputRGB[count_small][j]>255)OutputRGB[count_small][j]=255;
			if(OutputRGB[count_small][j]<0)OutputRGB[count_small][j]=0;

			noise_image.at<uchar>(j, i) = OutputRGB[count_small][j];

			++count_small;
			++i;
		}++j;count_small=0;i=0;
		}				
   


		FILE *fp_Output_image;
		fp_Output_image = fopen( output_csv_name, "w" );
	if( fp_Output_image == NULL ){
		printf( "%s�t�@�C�����J���܂���\n", output_csv_name );
		return -1;
	}
	for(int y=0;y<rows;y++){
		for(int x=0;x<cols;x++){

			fprintf( fp_Output_image, "%d,",OutputRGB[x][y]);
		}
			fprintf( fp_Output_image, "\n");
	}

	fclose( fp_Output_image );

	printf( "%s�t�@�C���������݂��I���܂���\n", output_csv_name );

	imwrite(Output_Image_name,noise_image); 

	 //���O������
	std::string windowName[10] = {"1","2","3","4","5","6"};
	cv::namedWindow(windowName[repeat]);

	//��ʕ\��
	cv::imshow(windowName[repeat],noise_image);
	 
 
   // cv::destroyWindow(windowName);
	if(no_repeat_flag==1){
		cv::waitKey(0);
		cv::destroyWindow(windowName[repeat]);
		break;
	}
	++repeat_count;
	if(repeat_count==repaet_max){
		cv::waitKey(0);
	//cv::destroyWindow(windowName);
	}
	
	}
   
	
}