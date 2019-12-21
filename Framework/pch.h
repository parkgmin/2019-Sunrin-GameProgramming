// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.

#ifndef PCH_H
#define PCH_H

#include <stdio.h>
#include <list>
#include <string>
#include <map>
#include <iostream>
#include <vector>

#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodec.h>
#include <timeapi.h>
//#include <dwrite.h>
//#include <Xinput.h>
#include <xaudio2.h>
#include <mfapi.h>		//wmf
#include <mfidl.h>
#include <mfreadwrite.h>


//라이브러리
#pragma comment (lib,"d2d1.lib")
#pragma comment (lib,"WindowsCodecs.lib")
#pragma comment (lib, "Winmm.lib")
#pragma comment (lib, "Xaudio2.lib")
#pragma comment (lib, "mfreadwrite.lib")
#pragma comment (lib, "mfplat.lib")
#pragma comment (lib, "mfuuid")
//#pragma comment (lib,"dwrite.lib")
//#pragma comment (lib,"xinput.lib")

#define PI 3.14159265358979323846f
#define FULLSCREEN 0
#define WINDOWWIDTH 800
#define WINDOWHEIGHT 600

#define SAFE_RELEASE(p) { if(p){p->Release();} p=NULL; }
#define SAFE_DELETE(p) {if(p!=nullptr){delete(p);} p=nullptr; }

using namespace std;

#endif //PCH_H
