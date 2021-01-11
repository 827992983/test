#include <ntddk.h>

#define DEVICE_NAME L"\\Device\\HbgProcessKillerDriverDev"
#define DRIVER_LINK L"\\??\\HbgProcessKillerDriverLnk"

#define OPERKILLPID CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_BUFFERED,FILE_ANY_ACCESS)

// �ṹ����
typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	UINT32 SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	UINT32 Flags;
	UINT16 LoadCount;
	UINT16 TlsIndex;
	LIST_ENTRY HashLinks;
	PVOID SectionPointer;
	UINT32 CheckSum;
	UINT32 TimeDateStamp;
	PVOID LoadedImports;
	PVOID EntryPointActivationContext;
	PVOID PatchInformation;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

// ȫ�ֱ���
PDRIVER_OBJECT g_pDriver;

// ��������
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING RegPath);
VOID DriverUnload(PDRIVER_OBJECT pDriver);
NTSTATUS IrpCreateProc(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS IrpCloseProc(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS IrpDeviceControlProc(PDEVICE_OBJECT pDevObj, PIRP pIrp);
VOID GetKernelBase(PDRIVER_OBJECT driver, PVOID *pKrnlBase, PUINT32 uKrnlImageSize);
PVOID MemorySearch(PVOID bytecode, UINT32 bytecodeLen, PVOID pBeginAddress, PVOID pEndAddress);
void KillPid(ULONG uPid, PDRIVER_OBJECT driver);

// ��ں���
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING RegPath)
{
	NTSTATUS status;
	ULONG uIndex = 0;
	PDEVICE_OBJECT pDeviceObj = NULL; // �豸����ָ��
	UNICODE_STRING DeviceName; // �豸����0����
	UNICODE_STRING SymbolicLinkName; // ������������3����

	g_pDriver = pDriver;

	// �����豸����
	RtlInitUnicodeString(&DeviceName,DEVICE_NAME);
	// �����豸
	status = IoCreateDevice(pDriver,0,&DeviceName,FILE_DEVICE_UNKNOWN,FILE_DEVICE_SECURE_OPEN,FALSE,&pDeviceObj);
	if (status != STATUS_SUCCESS)
	{
		IoDeleteDevice(pDeviceObj);
		DbgPrint("�����豸ʧ��.\n");
		return status;
	}
	DbgPrint("�����豸�ɹ�.\n");
	// ���ý������ݵķ�ʽ
	pDeviceObj->Flags |= DO_BUFFERED_IO;
	// ������������
	RtlInitUnicodeString(&SymbolicLinkName, DRIVER_LINK);
	IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName);
	// ���÷ַ�����
	pDriver->MajorFunction[IRP_MJ_CREATE] = IrpCreateProc;
	pDriver->MajorFunction[IRP_MJ_CLOSE] = IrpCloseProc;
	pDriver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IrpDeviceControlProc;
	// ����ж�غ���
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}

// ж������
VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	UNICODE_STRING SymbolicLinkName;
	// ɾ���������ӣ�ɾ���豸
	RtlInitUnicodeString(&SymbolicLinkName, DRIVER_LINK);
	IoDeleteSymbolicLink(&SymbolicLinkName);
	IoDeleteDevice(pDriver->DeviceObject);
	DbgPrint("����ж�سɹ�\n");
}

// �����������������Ring3����CreateFile�᷵��1
// IRP_MJ_CREATE ������
NTSTATUS IrpCreateProc(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	DbgPrint("Ӧ�ò������豸.\n");
	// ����״̬��������ã�Ring3����ֵ��ʧ��
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

// IRP_MJ_CLOSE ������
NTSTATUS IrpCloseProc(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	DbgPrint("Ӧ�ò�Ͽ������豸.\n");
	// ����״̬��������ã�Ring3����ֵ��ʧ��
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

// IRP_MJ_DEVICE_CONTROL ������
NTSTATUS IrpDeviceControlProc(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	// DbgPrint("IrpDeviceControlProc.\n");
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
	PIO_STACK_LOCATION pIrpStack;
	ULONG uIoControlCode;
	PVOID pIoBuffer;
	ULONG uInLength;
	ULONG uOutLength;
	ULONG uPid; // Ҫ�رյĽ��̺�
	ULONG uRet; // PspTerminateProcess ����ֵ���ظ�3��

	// ������ʱ������ֵ
	uPid = uRet = 0;
	// ��ȡIRP����
	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	// ��ȡ������
	uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	// ��ȡ��������ַ�����������ͬһ����
	pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	// Ring3 �������ݵĳ���
	uInLength = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	// Ring0 �������ݵĳ���
	uOutLength = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	switch (uIoControlCode)
	{
	case OPERKILLPID:
		{
			// ��ȡ������
			memcpy(&uPid,pIoBuffer,4);
			// ���� PspTerminateProcess
			KillPid(uPid,g_pDriver);
			// д�뻺����
			memcpy(pIoBuffer, &uRet, 4);
			// ����״̬
			pIrp->IoStatus.Information = 4;
			status = STATUS_SUCCESS;
			break;
		}
	}

	// ����״̬��������ã�Ring3����ֵ��ʧ��
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

// ��ȡ�ں˻�ַ����С
VOID GetKernelBase(PDRIVER_OBJECT driver, PVOID *pKrnlBase, PUINT32 uKrnlImageSize)
{
	PLDR_DATA_TABLE_ENTRY pLdteHead; // �ں�ģ������ͷ
	PLDR_DATA_TABLE_ENTRY pLdteCur; // ����ָ��
	UNICODE_STRING usKrnlBaseDllName; // �ں�ģ����

	RtlInitUnicodeString(&usKrnlBaseDllName,L"ntoskrnl.exe");
	pLdteHead = (PLDR_DATA_TABLE_ENTRY)driver->DriverSection;
	pLdteCur = pLdteHead;
	do 
	{
		PLDR_DATA_TABLE_ENTRY pLdte = CONTAINING_RECORD(pLdteCur, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		//DbgPrint("DllBase: %p, SizeOfImage: %08X %wZ\n", pLdteCur->DllBase, pLdteCur->SizeOfImage, &(pLdteCur->FullDllName));
		if (RtlCompareUnicodeString(&pLdteCur->BaseDllName, &usKrnlBaseDllName, TRUE) == 0)
		{
			*pKrnlBase = pLdteCur->DllBase;
			*uKrnlImageSize = pLdteCur->SizeOfImage;
			return;
		}
		pLdteCur = (PLDR_DATA_TABLE_ENTRY)pLdteCur->InLoadOrderLinks.Flink;
	} while (pLdteHead != pLdteCur);
	return;
}

// ����������
PVOID MemorySearch(PVOID bytecode, UINT32 bytecodeLen, PVOID pBeginAddress, PVOID pEndAddress)
{
	PVOID pCur = pBeginAddress;
	while (pCur != pEndAddress)
	{
		if (RtlCompareMemory(bytecode,pCur,bytecodeLen) == bytecodeLen)
		{
			return pCur;
		}
		((UINT32)pCur)++;
	}
	return 0;
}

// ɱ����
void KillPid(ULONG uPid, PDRIVER_OBJECT driver)
{
	typedef NTSTATUS (*_PspTerminateProcess)(PEPROCESS pEprocess, NTSTATUS ExitCode);
	_PspTerminateProcess PspTerminateProcess;
	UINT32 bytecode[] = {
		0x0124a164, 0x758b0000, 0x44703b08, 0x0db80775,
		0xebc00000, 0xbe8d575a, 0x00000248, 0x200147f6,
		0x868d1274, 0x00000174
	};
	PVOID pKrnlBase; // �ں˻�ַ
	UINT32 uKrnlImageSize; // �ں˴�С
	PEPROCESS pEprocess; // Ҫ�رյĽ��̵�EPROCESS

	// ��ȡ�ں�ģ���ַ�ʹ�С
	GetKernelBase(driver, &pKrnlBase, &uKrnlImageSize);
	//DbgPrint("�ں˻�ַ: %p����С: %X\n", pKrnlBase, uKrnlImageSize);
	// ��ȡ PspTerminateProcess ������ַ
	PspTerminateProcess = (_PspTerminateProcess)((UINT32)MemorySearch( \
		bytecode,sizeof(bytecode),pKrnlBase,(PVOID)((UINT32)pKrnlBase+uKrnlImageSize)) - 6);
	//DbgPrint("PspTerminateProcess: %p\n", PspTerminateProcess);
	// ����PID��ȡEPROCESS
	PsLookupProcessByProcessId((HANDLE)uPid,&pEprocess);
	// ���� PspTerminateProcess �رս���
	PspTerminateProcess(pEprocess, 0);
}
