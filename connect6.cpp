// Samsung Go Tournament Form C (g++-4.8.3)
//좌표로 출력하는 
/*
[AI 코드 작성 방법]

1. char info[]의 배열 안에					"TeamName:자신의 팀명,Department:자신의 소속"					순서로 작성합니다.
( 주의 ) Teamname:과 Department:는 꼭 들어가야 합니다.
"자신의 팀명", "자신의 소속"을 수정해야 합니다.

2. 아래의 myturn() 함수 안에 자신만의 AI 코드를 작성합니다.


[변수 및 함수]
myturn(int cnt) : 자신의 AI 코드를 작성하는 메인 함수 입니다.
int cnt (myturn()함수의 파라미터) : 돌을 몇 수 둬야하는지 정하는 변수, cnt가 1이면 육목 시작 시  한 번만  두는 상황(한 번), cnt가 2이면 그 이후 돌을 두는 상황(두 번)
int  x[0], y[0] : 자신이 둘 첫 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
int  x[1], y[1] : 자신이 둘 두 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.


//int board[BOARD_SIZE][BOARD_SIZE]; 바둑판 현재상황 담고 있어 바로사용 가능함. 단, 원본데이터로 수정 절대금지
// 놓을수 없는 위치에 바둑돌을 놓으면 실격패 처리.

boolean ifFree(int x, int y) : 현재 [x,y]좌표에 바둑돌이 있는지 확인하는 함수 (없으면 true, 있으면 false)
int showBoard(int x, nt y) : [x, y] 좌표에 무슨 돌이 존재하는지 보여주는 함수 (1 = 자신의 돌, 2 = 상대의 돌)

*/

#include <stdio.h>
#include <Windows.h>
#include <time.h>
int isFree(int, int);
void mymove(int x[], int y[], int);
int showBoard(int, int);
int isInside(int x, int y);
// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:DUST,Department:Handong University" };
int board[20][20], height = 19, width = 19;
//상수 지정
#define MAXXY 19
//1이 몇개냐에 따라 좌우대칭이면 1이 n개면 n/2개 대칭 아니면 n개 놓으면 된다.
const char pattern[80][12] = {//0이 아무거나 상관 없음, 1이 내돌, 2가 상대돌, 3이 빈곳, 이번차례 6번째칸이 자기가 놓는 위치
	"00000111111","00001111110","00011111100",//6
	"00003111113","00031111130","00311111300",//5 사이빈칸 0
	"00000131111","00001311110","00131111000","01311110000","13111100000",//5 사이빈칸 1
	"00000113111","00001131110","00113111000","01131110000","11311100000",
	"00003111112","00031111120","00311111200","03111112000","31111120000",
	"00033111133","00331111330",//4 사이빈칸 0
	"00003111133","00031111330","00311113300","03111133000",
	"00002111133","00021111330","00211113300","02111133000",
	"00003111313","00031113130","00311131300","31113130000",//4 사이빈칸 1  --3131113 할 필요 x
	"00003111312","00031113120","00311131200","31113120000",
	"00002111313","00021113130","00211131300","21113130000",
	"00003113113","00031131130",
	"00002113113","00021131130","02113113000","21131130000",
	"00000131131","00013113100",//4 사이빈칸 2
	"00000113311","00001133110",
	"00000133111","00133111000","01331110000","13311100000",
	"00000131311","00013131100","01313110000","13131100000",
	"00033111330","00331113300",//3 사이빈칸 0  ==>3부터 상대돌 막힘 없음 
	"00003111330","00003111330","00311133000",
	"00003131130","00313113000","03131130000",//3 사이빈칸 1
	"00003131313","00313131300",//3 사이빈칸 2
	"00333113330",//2 사이빈칸 0
	"00033113330","00331133300",
	"00003113330","00031133300",
	"00033113300",
	"00033131330",//2 사이빈칸 1
	//"00003131333","00313133300",
	"00003131330","00313133000",


};

const int stoneNum[80] = {
	6,6,6,
	5,5,5,
	5,5,5,5,5,
	5,5,5,5,5,
	5,5,5,5,5,
	4,4,
	4,4,4,4,
	4,4,4,4,
	4,4,4,4,
	4,4,4,4,
	4,4,4,4,
	4,4,
	4,4,4,4,
	4,4,
	4,4,
	4,4,4,4,
	4,4,4,4,
	3,3,
	3,3,3,
	3,3,3,
	3,3,
	2,
	2,2,
	2,2,
	2,
	2,
	2,2//여기까지 80개
};

const bool connected[80] = {
	true, true, true,
	true, true, true,
	false, false, false, false, false,
	false, false, false, false, false,
	false, false, false, false, false,
	true, true,
	true, true, true, true,
	true, true, true, true,
	false, false, false, false,
	false, false, false, false,
	false, false, false, false,
	false, false,
	false, false, false, false,
	false, false,
	false, false,
	false, false, false, false,
	false, false, false, false,
	true, true,
	true, true, true,
	false, false, false,
	false, false,
	true,
	true, true,
	true, true,
	true,
	false,
	false, false
};
/*
const int category[]={

};
*/
const int score1[81] = {//첫 번째 둘 때 가중치
	2100,2100,2100,
	1000,1000,1000,
	1000,1000,1000,1000,1000,
	1000,1000,1000,1000,1000,
	1000,1000,1000,1000,1000,
	110,110,
	109,109,109,109,
	106,106,106,106,
	108,108,108,108,
	105,105,105,105,
	105,105,105,105,
	107,107,
	105,105,105,105,
	103,103,
	103,103,
	104,104,104,104,
	103,103,103,103,
	54,54,
	52,52,52,
	51,51,51,
	53,53,
	24,
	23,23,
	21,21,
	22,
	20,
	20,20,
	0
};
const int score2[81] = {//두 번째 둘 때 가중치
	1000,1000,1000,
	19,19,19,
	19,19,19,19,19,
	19,19,19,19,19,
	19,19,19,19,19,
	110,110,
	109,109,109,109,
	106,106,106,106,
	108,108,108,108,
	105,105,105,105,
	105,105,105,105,
	107,107,
	105,105,105,105,
	103,103,
	103,103,
	104,104,104,104,
	103,103,103,103,
	54,54,
	52,52,52,
	51,51,51,
	53,53,
	24,
	23,23,
	21,21,
	22,
	20,
	20,20,
	0
};
const int dy[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
const int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };//상, 우상, 우, 우하, 하, 좌하, 좌, 좌상

int findpattern(int user, int d, int xpos, int ypos) {
	int curuser = user == 1 ? 2 : 1;//받은ㄹ
	int ptidx, idx;
	char cur;
	int cur2;
	bool check;
	for (ptidx = 0; ptidx < 80; ++ptidx) {
		check = true;
		for (idx = -5; idx < 6; ++idx) {//패턴 11칸 다 확인
			cur = pattern[ptidx][idx + 5];
			if (idx == 0 || cur == '0')//0일때는 볼 필요가 없다.(idx: 내가 놓을 자리니까, cur:뭔돌이든 상관 없으니까)
				continue;
			cur2 = (cur == '1' ? user : (cur == '2' ? curuser : 0));//상대 돌 확인 할 때는 user가 이니까 돌이 2인지 아닌지 확인
			if ( !isInside(xpos + dx[d] * idx, ypos + dy[d] * idx) || showBoard(xpos + dx[d] * idx, ypos + dy[d] * idx) != cur2) {
				check = false; // 패턴이랑 일치 안하거나 바둑판 벗어나면 false
				break;
			}
		}
		if (check) {
			return ptidx;//패턴과 일치하면 어떤 패턴인지 반환(패턴 일치는 하나밖에 없다)(가중치가 낮을 때도 반환하나?)
		}
	}
	return -1;
}
void myturn(int cnt) {

	int x[2], y[2];


	// 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.
	// 현재 Sample code의 AI는 Random으로 돌을 놓는 Algorithm이 작성되어 있습니다.

	srand((unsigned)time(NULL));
	/*
	for (int i = 0; i < cnt; i++) {
	do {
	x[i] = rand() % width;
	y[i] = rand() % height;
	if (terminateAI) return;
	} while (!isFree(x[i], y[i]));

	if (x[1] == x[0] && y[1] == y[0]) i--;
	}
	*/
	for (int i = 0; i < cnt; i++) {
		int iy, ix;
		int patt[8];
		int mybesty = 0, mybestx = 0;
		int mybestRank = 11, mybestscore = 0;

		int opbesty = 0, opbestx = 0;
		int opbestRank = 11, opbestscore = 0;

		int idx, d;
		int p1, p2;
		int rank, score;
		bool fourCheck, threeCheck;
		if (cnt == 1) {
			x[0] = 9;
			y[0] = 9;
		}
		else {
			for (iy = 0; iy < height; iy++) {//내 돌 패턴 확인
				for (ix = 0; ix < width; ix++) {
					if (isFree(ix, iy)) {
						rank = 11;
						score = 0;
						fourCheck = false;
						threeCheck = false;
						for (d = 0; d < 8; d++)
							patt[d] = findpattern(1, d, ix, iy);
						for (d = 0; d < 4; d++) {//카테고리로 패턴 나눔 33, 44 등 확인
							p1 = patt[d];//정방향
							p2 = patt[d + 4];//역방향
						}
						for (d = 0; d < 4; d++) {
							p1 = patt[d];
							p2 = patt[d + 4];
							if (p1 != -1 && p2 != -1) {
								if (i == 0) {
									score += score1[p1] > score1[p2] ? score1[p1] : score1[p2];
								}
								else {
									score += score2[p1] > score2[p2] ? score2[p1] : score2[p2];
								}
							}
							else if (p1 != -1 && p2 == -1) {
								if (i == 0) {
									score += score1[p1];
								}
								else {
									score += score2[p1];
								}
							}
							else if (p1 == -1 && p2 != -1) {
								if (i == 0) {
									score += score1[p2];
								}
								else {
									score += score2[p2];
								}
							}

						}
						if (mybestscore < score) {
							mybestscore = score;
							mybesty = iy;
							mybestx = ix;
						}
					}
				}
			}

			for (iy = 0; iy < height; iy++) {//상대 돌 패턴 확인
				for (ix = 0; ix < width; ix++) {
					if (isFree(ix, iy)) {
						rank = 11;
						score = 0;
						fourCheck = false;
						threeCheck = false;
						for (d = 0; d < 8; d++)
							patt[d] = findpattern(2, d, ix, iy);
						for (d = 0; d < 4; d++) {//카테고리로 패턴 나눔 33, 44 등 확인
							p1 = patt[d];//정방향
							p2 = patt[d + 4];//역방향
						}
						for (d = 0; d < 4; d++) {
							p1 = patt[d];
							p2 = patt[d + 4];
							if (p1 != -1 && p2 != -1) {
								score += score1[p1] > score1[p2] ? score1[p1] : score1[p2];
							}
							else if (p1 != -1 && p2 == -1) {
								score += score1[p1];
							}
							else if (p1 == -1 && p2 != -1) {
								score += score1[p2];
							}
						}
						if (opbestscore < score) {
							opbestscore = score;
							opbesty = iy;
							opbestx = ix;
						}
					}
				}
			}
			//총계
			if (mybestscore > opbestscore) {
				x[i] = mybestx;
				y[i] = mybesty;
			}
			else {
				x[i] = opbestx;
				y[i] = opbesty;
			}
			printf("score : %d %d", mybestscore, opbestscore);

		}
		if (i == 0)
			board[x[0]][y[0]] = 1;
	}
	board[x[0]][y[0]] = 0;
	// 이 부분에서 자신이 놓을 돌을 출력하십시오.
	// 필수 함수 : domymove(x배열,y배열,배열크기)
	// 여기서 배열크기(cnt)는 myturn()의 파라미터 cnt를 그대로 넣어야합니다.
	mymove(x, y, cnt);
	int a, b;
	for (int j = 0; j < 2; ++j) {
		printf("send :  ");
		scanf("%d %d", &a, &b);
		board[a][b] = 2;
	}
	printf("\n");
}
void mymove(int x[], int y[], int cnt) {
	for (int i = 0; i < cnt; i++) {
		if (isFree(x[i], y[i])) {
			board[x[i]][y[i]] = 1;
		}
	}
	printf("my turn :%d, %d  %d, %d\n", x[0], y[0], x[1], y[1]);
}

int isFree(int x, int y)
{
	return x >= 0 && y >= 0 && x < width && y < height && board[x][y] == 0;
}
int isInside(int x, int y)
{
	return x >= 0 && y >= 0 && x < width && y < height;
}

int showBoard(int x, int y) {
	return board[x][y];
}

int main() {
	printf("p1: %d , p2: %d\n", score1[0], score1[7]);
	myturn(1);
	while (1) {
		myturn(2);
	}
	return 0;
}
