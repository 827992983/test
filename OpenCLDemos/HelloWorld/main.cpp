#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>                 // OpenCL ��ͷ�ļ�

int main()
{
	cl_platform_id *pPlatform;  // ƽ̨���ݽṹ���飬ÿ���ṹ��ʾһ���������ϵ� OpenCL ִ��ƽ̨��ִ���豸��
	cl_uint nPlatform;
	cl_int i, err;
	char* extraInformation;     // ������Ϣ������
	size_t extraSize;           // ������Ϣ��������С
	bool supportICD = false;    // ��¼�����Ƿ�֧�� OpenCL Installable Client Driver (ICD)

								// platform�����Ĳ��裺0��ѯ���������ж��ټ���ƽ̨��1��Ϊƽ̨���ݽṹ�����ڴ�ռ䣻2����ʼ����Щ���ݽṹ
	if (err = clGetPlatformIDs(2, NULL, &nPlatform) < 0)// ��ѯƽ̨��������ֵ����˵�����ô��󣬲�����1����Ҫ��ƽ̨�����ޣ�2��NULL ���ѯƽ̨������3�����ƽ̨���ı���ָ��
	{
		perror("Couldn't find any pPlatform.");                             // ���������Ϣ�õĺ���
		exit(1);
	}
	printf("Platform count: %d\n", nPlatform);

	pPlatform = (cl_platform_id*)malloc(sizeof(cl_platform_id) * nPlatform);// �����������ݽṹ
	clGetPlatformIDs(nPlatform, pPlatform, NULL);                           // ��ʼ���ṹ

	for (i = 0; i < nPlatform; i++)                                         // ѭ����ȡƽ̨��Ϣ
	{
		if (err = clGetPlatformInfo(pPlatform[i], CL_PLATFORM_EXTENSIONS, 0, NULL, &extraSize) < 0) // ��ȡ������Ϣ�����������Ϊ��Ϣ����
		{
			perror("Couldn't read extension data.");
			exit(1);
		}
		printf("\nExtension data size: %d\n", extraSize);

		extraInformation = (char*)malloc(extraSize);                                                // �����Ϣ����
		clGetPlatformInfo(pPlatform[i], CL_PLATFORM_EXTENSIONS, extraSize, extraInformation, NULL); // ��ȡ�����Ϣ���ڶ�����Ϊ��Ϣ���ݣ�������OpenCL֧�ֵ���չ������Ϣ
		printf("Platform %d supports extensions: %s\n", i, extraInformation);
		if (strstr(extraInformation, "cl_khr_icd") != NULL)                                         // ����Ƿ�֧�� ICD��֧�������ƽ̨���
		{
			printf("\nPlatform %d supports ICD extension.\n", i);
			supportICD = true;
		}
		clGetPlatformInfo(pPlatform[i], CL_PLATFORM_NAME, extraSize, extraInformation, NULL);       // ������
		printf("Platform %d name: %s\n", i, extraInformation);
		clGetPlatformInfo(pPlatform[i], CL_PLATFORM_VENDOR, extraSize, extraInformation, NULL);     // ��Ӧ����
		printf("Platform %d vendor: %s\n", i, extraInformation);
		clGetPlatformInfo(pPlatform[i], CL_PLATFORM_VERSION, extraSize, extraInformation, NULL);    // OpenCL�汾
		printf("Platform %d version: %s\n", i, extraInformation);
		clGetPlatformInfo(pPlatform[i], CL_PLATFORM_PROFILE, extraSize, extraInformation, NULL);    // ����ģʽ / Ƕ��ʽ
		printf("Profile: %s\n", extraInformation);
		free(extraInformation);
	}
	if (!supportICD)
		printf("\nNo platform support ICD extension.\n");
	free(pPlatform);
	getchar();
	return 0;
}