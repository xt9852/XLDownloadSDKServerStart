DownloadSDKServer.exe ִ�й���,�����в����̶�

00E09FC0              ��ʼ��ַ

GetCommandLineW       "C:\***\DownloadSDKServer.exe" BDAF7A63-568C-43ab-9406-D145CF03B08C:2824
CommandLineToArgvW    "DAF7A63-568C-43ab-9406-D145CF03B08C:2824"
SetProcessShutdownParameters

GetTempPathW          "C:\Users\Administrator\AppData\Local\Temp\"
PathAppendW           "Thunder Network\XLSDK\"

XLBugHan.XL_SetBugReportRootDir("C:\Users\Administrator\AppData\Local\Temp\Thunder Network\XLSDK\")
XLBugHan.XL_SetPeerID("11111111111111111")

GetModuleFileNameW    "C:\***\DownloadSDKServer.exe"
GetFileVersionInfoW   "2.85.110.32"

XLBugHan.XL_InitBugHandler("Ѹ������SDK", 0, 0, "DownloadSDK", "2.85.110.32")
XLBugHan.XL_SetReportShowMode(0)

CreateThread(0010A210)  �����߳�ֻ���ڴ���������Ϣ
    LoadCursorW(NULL, IDC_ARROW)
    RegisterClassExW(mainWindowProc.0010A190, "XL_SleepWindow")
    CreateWindowExW()
    ShowWindow(SW_HIDE)
    UpdateWindow
    GetMessageW
    do
        TranslateMessage
        DispatchMessageW
        GetMessageW
    while
    return

    LRESULT CALLBACK mainWindowProc(wnd, msg, wParam, lParam)
    switch (msg)
    {
        case WM_DESTROY:PostQuitMessage();break;
        case WM_POWERBROADCAST:
            if (wParam == PBT _APMSUSPEND) // 4���߹���
            {
                _time64(&curTime)
                DownloadSDK.XL_SetWindowsSleepInfo(1, msg, curTime)
            }
            else if (wParam == PBT _APMRESUMEAUTOMATIC) // 12����״̬�лָ�
            {
                _time64(NULL)
                DownloadSDK.XL_SetWindowsSleepInfo(2, msg, curTime)
            }
    }
    DefWindowProcW
CloseHandle(thread1_id)

GetCurrentProcessId() 3896
snprintf("BDAF7A63-568C-43ab-9406-D145CF03B08C:2824:3896")  // 2824������ID,3896������ID

call 00272220
    call 0027A360
        call 0027A540
            call 0027AD90
                call 0027A780                                   edi=01E59850
                    CreateMutexW("|ServerAliveMutex")          [edi+0x28=01E59878]=D8
                    CreateEventW("|ServerCloseEvent")          [edi+0x2C=01E5987C]=E0
                    call 0027B140                               esi=01E59884
                        CreateFileMappingW("|SendShareMemory") [esi+0x4=01E59888] =E4
                        MapViewOfFile(1M)                      [esi+0x8=01E5988C] =022D0000
                        CreateEventW("|SendBufferEmptyEvent")  [esi+0xC=01E59890] =E8
                        CreateEventW("|SendBufferFullEvent")   [esi+0x10=01E59894]=EC
                    call 0027B140                               esi=01E598A0
                        CreateFileMappingW("|RecvShareMemory") [esi+0x4=01E598A0] =F0
                        MapViewOfFile(1M)                      [esi+0x8=01E598A4] =023D0000
                        CreateEventW("|RecvBufferEmptyEvent")  [esi+0xC=01E598A8] =F4
                        CreateEventW("|RecvBufferFullEvent")   [esi+0x10=01E598AC]=F8

                CreateMutexW("|ConnectMutex")                  [edi=01E59850+0x24=01E59874]=FC

    OpenEventW("|ServerStartUpEvent") = 100
    SetEvent("|ServerStartUpEvent")
    CloseHandle("|ServerStartUpEvent")

    OpenMutexW("|ProxyAliveMutex") ss:[esp=0047F578+0x1C=0047F594]=100
    CreateEventW("|AccetpReturnEvent") = 104

    CreateThread(00102590) ThreadID2=10C
        WaitForMultipleObjects("|ProxyAliveMutex", "|AccetpReturnEvent", INFINITE)
        if (WAIT_OBJECT_0 || WAIT_ABANDONED)
        {
            ReleaseMutex("|ProxyAliveMutex") // ����
            SetEvent("|ServerCloseEvent")
        }

    call 0027A9E0
        WaitForMultipleObjects("|ServerCloseEvent", "|RecvBufferFullEvent", INFINITE)
        if (WAIT_OBJECT_1)
        {
            ResetEvent("|RecvBufferFullEvent")
            SetEvent("|RecvBufferEmptyEvent")
            if (RecvBuffer�����Ƿ�Ϊ1)
                new
                memset()
                _ui64tow_s()

                call 0027A540
                    call 0027AD90
                        call 0027A780                              edi=01E59928
                            CreateMutexW("|ServerAliveMutex")     [edi+0x28=01E59950]=10C
                            CreateEventW("|ServerCloseEvent")     [edi+0x2C=01E59954]=110
                            call 0027B140                              esi=01E5995C
                                CreateFileMappingW("|SendShareMemory")[esi+0x4=01E59960]=114
                                MapViewOfFile(1M)                     [esi+0x8=01E59964]=02530000
                                CreateEventW("|SendBufferEmptyEvent") [esi+0xC=01E59968]=118
                                CreateEventW("|SendBufferFullEvent")  [esi+0x10=01E5996C]=11C
                            call 0027B140                              esi=01E59974
                                CreateFileMappingW("|RecvShareMemory")[esi+0x4=01E59978]=120
                                MapViewOfFile(1M)                     [esi+0x8=01E5997C]=02630000
                                CreateEventW("|RecvBufferEmptyEvent") [esi+0xC=01E59980]=124
                                CreateEventW("|RecvBufferFullEvent")  [esi+0x10=01E59984]=128
        }
        CreateMutex("|ClientAliveMutex") ds:[edi=01E59928+0x30=01E59958]=12C

        WaitForMultipleObjects("|ServerCloseEvent", "|RecvBufferFullEvent", 12000ms)
        if (WAIT_OBJECT_1)
        {
            ResetEvent("|RecvBufferFullEvent")
            SetEvent("|RecvBufferEmptyEvent")
            if (RecvBuffer�����Ƿ�Ϊ3) return
        }

    SetEvent("|AccetpReturnEvent") 104
    WaitForSingleObject(ThreadID2, INFINITE)
    CloseHandle(ThreadID2)
    CloseHandle("|AccetpReturnEvent")
    CloseHandle("|ProxyAliveMutex")
    SetEvent("|ServerCloseEvent")
    CloseHandle
    UnmapViewOfFile

if (eax != 0)
    GetCurrentProcess
    TerminateProcess

do
    WaitForMultipleObjects("|ServerCloseEvent", "|RecvBufferFullEvent", "|ClientAliveMutex")
    memcpy(00C60020, 028C0004, 8) 0C
    ResetEvent("|RecvBufferFullEvent") 128
    SetEvent("|RecvBufferEmptyEvent") 124

    // ----����Э�������Ӧ�ĺ���

    WaitForMultipleObjects("|ServerCloseEvent", "|ClientAliveMutex", "|SendBufferEmptyEvent")
    memcpy(027C0004, 00C60020, 4) 0C
    ResetEvent("|SendBufferEmptyEvent") 118
    SetEvent("|SendBufferFullEvent") 11C
while (eax == 0)

//------------------------------------------------------
// ��ʼ������

caller:                                         sdk:

CreateEventW("|ServerStartUpEvent")
WaitForMultipleObjects("|ServerStartUpEvent")
                                                OpenEventW("|ServerStartUpEvent")
                                                SetEvent("|ServerStartUpEvent")
                                                WaitForMultipleObjects("|RecvBufferFullEvent")
OpenFileMappingW("|RecvShareMemory")
MapViewOfFile(1M)
set 01
OpenEventW("|RecvBufferFullEvent")
SetEvent("|RecvBufferFullEvent")
                                                set 02 28 99 6A "6986024" // �������ڹ����ڴ�����
                                                ResetEvent("|SendBufferEmptyEvent")
                                                SetEvent("|SendBufferFullEvent")
                                                WaitForMultipleObjects("|RecvBufferFullEvent")
WaitForMultipleObjects("|SendBufferFullEvent");
set 03
SetEvent("|RecvBufferFullEvent")
                                                WaitForSingleObject(ThreadID2)

OpenEventW("|AccetpReturnEvent")
SetEvent("|AccetpReturnEvent")

ReleaseMutex("|ProxyAliveMutex")
                                                for WaitForMultipleObjects("��3����|RecvBufferFullEvent")

//------------------------------------------------------
// ����ID

    /* 1*/ XL_Init = 1,
    /* 2*/ XL_UnInit,
    /* 3*/ XL_SetDownloadSpeedLimit,
    /* 4*/ XL_SetUploadSpeedLimit,
    /* 5*/ XL_SetProxy,
    /* 6*/ XL_SetUserAgent,
    /* 7*/ XL_CreateP2spTask,
    /* 8*/ XL_SetTaskStrategy,
    /* 9*/ XL_StartTask,
    /* A*/ XL_StopTask,
    /* B*/ XL_DeleteTask,
    /* C*/ XL_QueryTaskInfo,
    /* D*/ XL_AddServer,
    /* E*/ XL_DiscardServer,
    /* F*/ XL_CreateEmuleTask,
    /*10*/ XL_CreateBTTask,
    /*11*/ XL_BTStartUpload,
    /*12*/ XL_BTStopUpload,
    /*13*/ XT_Func_NULL_1,
    /*14*/ XL_AddPeer,
    /*15*/ XL_DiscardPeer,
    /*16*/ XL_QueryTaskIndex,
    /*17*/ XL_QueryTaskFlow,
    /*18*/ XL_QueryGlobalStat,
    /*19*/ XL_EnableDcdn,
    /*1A*/ XL_DisableDcdn,
    /*1B*/ XL_SetUserInfo,
    /*1C*/ XL_SetOriginConnectCount,
    /*1D*/ XL_QueryBTSubFileInfo,
    /*1E*/ XT_Func_NULL_2,
    /*1F*/ XL_SetDownloadStrategy,
    /*20*/ XL_CreateMagnetTask,
    /*21*/ XL_SetGlobalExtInfo,
    /*22*/ XL_SetTaskExtInfo,
    /*23*/ XL_SetP2spTaskIndex,
    /*24*/ XL_SetEmuleTaskIndex,
    /*25*/ XL_SetBTSubTaskIndex,
    /*26*/ XL_QueryPlayInfo,
    /*27*/ XL_SetAccelerateCertification,
    /*28*/ XL_RenameP2spTaskFile,
    /*29*/ XL_SetTaskExtStat,
    /*2A*/ XL_EnableFreeDcdn,
    /*2B*/ XL_DisableFreeDcdn,
    /*2C*/ XL_QueryFreeDcdnAccelerate,
    /*2D*/ XL_SetCacheSize,
    /*2E*/ XL_SetFreeDcdnDownloadSpeedLimit,
    /*2F*/ XL_EnableDcdnWithToken,
    /*30*/ XL_EnableDcdnWithSession,
    /*31*/ XL_SetP2SPTaskIdxURL,
    /*32*/ XL_GetFilePlayInfo,
    /*33*/ XL_SetTaskPriorityLevel,
    /*34*/ XL_BatchAddPeer,
    /*35*/ XL_BatchDiscardPeer,
    /*36*/ XL_GetUnRecvdRangeArray,
    /*37*/ XL_EnableDcdnWithVipCert,
    /*38*/ XL_UpdateDcdnWithVipCert,
    /*39*/ XL_DisableDcdnWithVipCert,
    /*3A*/ XL_GetPeerId,
    /*3B*/ XL_BatchAddBTTracker,
    /*3C*/ XL_SetTaskUserAgent,
    /*3D*/ XL_GetSumOfRemotePeerBeBenefited,
    /*3E*/ XL_IsFileSizeSetterWorking,
    /*3F*/ XL_LaunchFileAssistant,
    /*40*/ XL_GetTaskProfileLog,
    /*41*/ XL_SetGlobalConnectionLimit,
    /*42*/ XL_AddHttpHeaderField,
    /*43*/ XL_GetSubNetUploader,
    /*44*/ XL_IsDownloadTaskCFGFileExit,
    /*45*/ XL_UpdateNetDiscVODCachePath,
    /*46*/ XL_SetupNetDiskFetchTaskFlag,
    /*47*/ XL_UpdateTaskVideoByteRatio

//------------------------------------------------------
// Э�����ݰ�

1, XL_CreateEmuleTask

�������ݰ�:���ݳ���+����ID+��������URL����+��������URL+�����ļ�������+�����ļ���
�������ݰ�:���ݳ���+����ID+����ֵ+����ID

���ݳ��Ȳ����������4�ֽ�,�����ĳ��ȶ���4�ֽ�
��������URL,�����ļ���ΪUNICODE
��������URL����,�����ļ�������ΪUNICODE����
����ֵ,0�ɹ�,����ʧ��

2, XL_CreateBTTask

�������ݰ�:���ݳ���+����ID+����·������+����·��+����Ŀ¼����+����Ŀ¼+�����б���+�����б�
�������ݰ�:���ݳ���+����ID+����ֵ+����ID

����·��,����Ŀ¼ΪUNICODE
����·������,����Ŀ¼����ΪUNICODE����
�����б�Ϊchar��
��:��������10���ļ�,ֻ���ص�1�͵�3���ļ�,�������б�Ϊ"101",1����,0������,
   ��3λ��1�����0����Ҫ����,˳��Ϊ�������ļ��б��˳��,���ǰ�ҳ������ʾ��˳��

3, XL_CreateP2spTask

�������ݰ�:���ݳ���+����ID+URL��ַ����+URL��ַ+����Ŀ¼����+����Ŀ¼
�������ݰ�:���ݳ���+����ID+����ֵ+����ID

URL��ַ,����Ŀ¼ΪUNICODE
URL��ַ����,����Ŀ¼����ΪUNICODE����

4, XL_SetTaskStrategy
�������ݰ�:���ݳ���+����ID+����ID+7
�������ݰ�:���ݳ���+����ID+����ֵ

5, XL_SetTaskExtInfo

�������ݰ�:���ݳ���+����ID+����ID+"parentid=1736383318,taskorigin=Magnet"+0
�������ݰ�:���ݳ���+����ID+����ֵ

taskorigin=Magnet��newwindow_url

6, XL_BatchAddBTTracker

�������ݰ�:���ݳ���+����ID+����ID+Tracker����+Tracker��ַ����+Tracker��ַ+...+..
�������ݰ�:���ݳ���+����ID+����ֵ

Tracker��ַ����ΪUNICODE

7, XL_StartTask

�������ݰ�:���ݳ���+����ID+����ID
�������ݰ�:���ݳ���+����ID+����ֵ

8, XL_StopTask

�������ݰ�:���ݳ���+����ID+����ID
�������ݰ�:���ݳ���+����ID+����ֵ

9, XL_DeleteTask

�������ݰ�:���ݳ���+����ID+����ID
�������ݰ�:���ݳ���+����ID+����ֵ

10, XL_QueryTaskInfo(����ID)

�������ݰ�:���ݳ���+����ID+����ID
�������ݰ�:���ݳ���+����ID+���ݳ�(37C)+4�ֽ�+�ܴ�С(64)

���ݳ�:37C
�ܴ�С:8�ֽ�
��У���С:8�ֽ�
�����ش�С:8�ֽ�
������ʱ:4�ֽ�,��λΪ��


XL_UpdateDcdnWithVipCert(����ID, FALSE, "")
����8�ֽ�
00B20020  22 00 00 00 00 00 00 00

XL_SetOriginConnectCount(����ID, 5)
00B20020  1C 00 00 00 09 (����ID) 05 00 00 00
����8�ֽ�
00B20020  1C 00 00 00 00 00 00 00

XL_QueryTaskIndex(ID, 0, buffer)
00B20020  16 00 00 00 08 00 00 00 00 00 00 00
����60�ֽ�
00B20020  16 00 00 00 00 00 00 00 34 00 00 00 88 6B CD 90  .......4...�k͐
00B20030  00 00 00 00 2B 1B 03 B9 95 07 50 3C 2A 74 92 26  ....+��P<*t?
00B20040  53 03 21 6C 63 BB 66 BE 24 9A C3 7A FD 19 54 A2  S!lc�f?��z?T?
00B20050  2F A4 AF 3D 3D 63 F8 F7 7A 52 63 75              /��==c��zRcu��?

XL_QueryTaskFlow(����ID, FALSE)
00D20020  17 00 00 00 05 00 00 00 00 00 00 00              ..........
����48�ֽ�
00D20020  17 00 00 00 00 00 00 00 01 00 00 00 C9 00 00 00  ..........?..
00D20030  01 00 00 00 00 00 04 00 00 C0 91 06 00 00 00 00  .......��....
00D20040  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00D20050  B0 C2 DA 07 38 D4 23 00 02 00                    ��?8?....��.

XL_IsDownloadTaskCFGFileExit(TRUE, "D:\5.downloads\sdk.htmlҪ���ص��ļ���ַ")
00B20020  44 00 00 00 01 00 00 00 17 (���ݳ�) 44 00 3A 00  D.........D.:.
00B20030  5C 00 35 00 2E 00 64 00 6F 00 77 00 6E 00 6C 00  \.5...d.o.w.n.l.
00B20040  6F 00 61 00 64 00 73 00 5C 00 73 00 64 00 6B 00  o.a.d.s.\.s.d.k.
00B20050  2E 00 68 00 74 00 6D 00 6C 00                    ..h.t.m.l.
����9�ֽ�
00B20020  44 00 00 00 00 00 00 00 00(FALSE,����ֵ)

XL_IsFileSizeSetterWorking
02350020  3E 00 00 00 00 00 00 00 1C 00 00 00 BC 5A D4 1A  >..........�Z?
02350030  00 00 00 00 12 52 12 01 00 00 00 00 D5 00 00 00  ....R....?..
02350040  01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ...............
02350050  00 00 00 00 00 00 00 00 00 00 00 00 C4 F6 BD 02  ............��?
02350060  00 00 00 00 00 00 05 02 50 09 A1 10 DC F4 BD 02  ......P.?��?
02350070  00 00 00 00 AF 01 28 06 B9 19 6E C9 CA D6 28 3E  ....?(?n��?>
����5�ֽ�
02350020  3E 00 00 00 01
