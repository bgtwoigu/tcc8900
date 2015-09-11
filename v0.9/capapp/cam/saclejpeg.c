#include "scalejpeg.h"


int scalejpeg(char *srcName, char *destName, unsigned int width, unsigned int height)
{
	IplImage *src = 0;			//Դͼ��ָ��
	IplImage *dst = 0;			//Ŀ��ͼ��ָ��
	IplImage *mid = 0;
    CvSize dst_cvsize;			//Ŀ��ͼ��ߴ�
 
    src = cvLoadImage(srcName, -1);    //���빤��Ŀ¼���ļ���Ϊ��tiger.jpg����ͼƬ��
    
    dst_cvsize.width = width;		//Ŀ��ͼ��Ŀ�
    dst_cvsize.height = height;		//Ŀ��ͼ��ĸ�

 	printf("before !\n");
    //dst = cvCreateImage(dst_cvsize, src->depth, src->nChannels);	//����Ŀ��ͼ��

    int img_width = src->width;
    int img_height = src->height;

    cvSetImageROI(src , cvRect(0 + 10, 0, img_width - 20, img_height - 10));
    mid = cvCreateImage(cvSize(img_width - 20, img_height - 10), src->depth, src->nChannels);
    cvCopy(src, mid, 0);

    dst = cvCreateImage(dst_cvsize, src->depth, src->nChannels);	//����Ŀ��ͼ��
    cvResize(src, dst, CV_INTER_LINEAR);							//����Դͼ��Ŀ��ͼ��
    cvSaveImage(destName, dst, NULL);
	printf("end !\n");

    cvReleaseImage(&src);	//�ͷ�Դͼ��ռ�õ��ڴ�
    cvReleaseImage(&dst);	//�ͷ�Ŀ��ͼ��ռ�õ��ڴ�	
    cvReleaseImage(&mid);	//�ͷ�Ŀ��ͼ��ռ�õ��ڴ�	
}

