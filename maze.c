#include "H03_maze.h"

extern queuePtr front, rear;   // pointer for FIFO queue
extern int sR, sC; // starting position = (sR, sC) = (1,1) (fixed)
extern int dR, dC;         // destination position = (dR, dC)
extern int allocSize;      // size of allocated memory currently used
extern int allocSizeMax;   // store the max size of allocated memory at each test

// queue 관련
// 1. queue는 반드시 (singly) linked list로 구현해야 한다.
// 2. queue init : front = NULL
// 3. empty check: front == NULL 인지 확인
// use the following two functions

void addq(int r, int c) {
    queuePtr tmp;
    MALLOC(tmp, queuePtr, sizeof(queue));
    tmp->row = r; 
    tmp->col = c;
    tmp->p = NULL;
    

    if (front) //front에 뭐가 있을 때
        rear->p = tmp; //rear에 연결
    else //front=NULL일 때
        front = tmp; //front에 저장
    rear = tmp; //rear에 저장

    allocSize += sizeof(queue);
    allocSizeMax = MAX2(allocSize, allocSizeMax); //더 큰 수 저장

    return;
}

void deleteq(int* r, int* c) {
    queuePtr tmp = front; //fifo이므로
   
    if (!tmp) { //front가 비어있을 때
        fprintf(stderr, "Queue is empty"); //오류
        exit(EXIT_FAILURE);
    }
    else {
        *r = tmp->row;
        *c = tmp->col;
        front = tmp->p; //front에 연결된 걸 front에 저장
        free(tmp); 
    }

    allocSize -= sizeof(queue);
    allocSizeMax = MAX2(allocSize, allocSizeMax);
    
    return;
}

char** readMaze(int* Rm, int* Cm) {
    int C, R;
    char** maze;
    static char staticbuffer[2];

    scanf("%d %d", &C, &R); //width C와 height R 입력받기
    fgets(staticbuffer, sizeof(staticbuffer), stdin); //\n 제거
   
    *Rm = 2 * R + 1, *Cm = 2 * C + 1;

    MALLOC(maze, char**, (*Rm) * sizeof(char*)); //maze 동적할당
    allocSize += *Rm * sizeof(char*); //메모리 변화 저장
    for (int i = 0; i < (*Rm); i++)
    {
        MALLOC(maze[i], char*, (*Cm + 2) * sizeof(char)); //maze[i] 동적할당
        fgets(maze[i], (*Cm)+2, stdin); //정보 입력
        allocSize += (*Cm+2) * sizeof(char); //메모리 변화 저장
    }
    allocSizeMax = MAX2(allocSize, allocSizeMax); //더 큰 수 저장

    dR = *Rm - 2; //목적지 서정
    dC = *Cm - 2; //목적지 설정
        
    return maze;
}

int findShortest(char **maze, int **mark, int Rm, int Cm) {
    int d = 0;
    int distance;
    bool found = false;
    int front_row=0, front_col = 0; //front 값 저장할 변수
    front = NULL;
    addq(sR, sC); 
    mark[sR][sC] = 0;

    while (front!=NULL) {
        if (d > (Rm) * (Cm + 2))
            break;
        if (mark[front->row][front->col]==d) {
            front_row = front->row; //front->row 값 저장
            front_col = front->col; //front->col 값 저장
            deleteq(&(front->row), &(front->col)); //front delete
            if (((front_row >= 0) && (front_row < Rm)) && ((front_col - 1 >= 0) && (front_col < Cm))) { //범위 내에 있는지 확인
                if (((maze[front_row][front_col - 1] == ' ') && (mark[front_row][front_col - 1] == UNVISITED))) { //좌 체크
                    mark[front_row][front_col - 1] = d + 1;
                    addq(front_row, front_col - 1);
                    if ((rear->row == dR) && (rear->col == dC)) {//목적지 도착 시
                        found = true; 
                        break; //종료
                    }
                }
            }
            if (((front_row >= 0) && (front_row < Rm)) && ((front_col >= 0) && (front_col + 1 < Cm))) {
                if (((maze[front_row][front_col + 1] == ' ') && (mark[front_row][front_col + 1] == UNVISITED))) { //우 체크
                    mark[front_row][front_col + 1] = d + 1;
                    addq(front_row, front_col + 1);
                    if ((rear->row == dR) && (rear->col == dC)) {
                        found = true;
                        break;
                    }
                }
            }
            if (((front_row-1 >= 0) && (front_row < Rm)) && ((front_col >= 0) && (front_col < Cm))) {
                if (((maze[front_row - 1][front_col] == ' ') && (mark[front_row - 1][front_col] == UNVISITED))) { //상 체크
                    mark[front_row - 1][front_col] = d + 1;
                    addq(front_row - 1, front_col);
                    if ((rear->row == dR) && (rear->col == dC)) {
                        found = true;
                        break;
                    }
                }
            }
            if (((front_row >= 0) && (front_row +1 < Rm)) && ((front_col >= 0) && (front_col < Cm))) {
                if (((maze[front_row + 1][front_col] == ' ') && (mark[front_row + 1][front_col] == UNVISITED))) { //하 체크
                    mark[front_row + 1][front_col] = d + 1;
                    addq(front_row + 1, front_col);
                    if ((rear->row == dR) && (rear->col == dC)) {
                        found = true;
                        break;
                    }
                }
            }
            
        }
        else {
            d +=1; // 해당 d에 대한 모든 셀을 완료 시 +1
        }
    }

    while (front) { //front에 정보가 남아있는데 종료됐을 시
        deleteq(&(front->row), &(front->col)); //비워주기
    }
    if (found == false) return (UNVISITED); //경로 못 찾았을 시 종료


    addq(dR, dC);
    maze[dR][dC] = 'T';
    d += 1; //도착지 distance 값 계산
    distance = d; //distance 값 저장
    while (true) {
        if ((front->row==sR)&&(front->col==sC)){ //출발점 도착 시
            maze[sR][sC] = 'S';
            break; //종료
        }
        if ((mark[front->row][front->col-1]) == d - 1) { //좌 체크
            maze[front->row][front->col-1] = '*'; //경로 표시
            addq(front->row, front->col-1);
            deleteq(&(front->row), &(front->col));
            d = d - 1; //다음 경로 탐색
            continue;
        }
        else if ((mark[front->row][front->col+1]) == d - 1) { //우 체크
            maze[front->row][front->col+1] = '*';
            addq(front->row, front->col + 1);
            deleteq(&(front->row), &(front->col));
            d = d - 1;
            continue;
        }
        else if ((mark[front->row-1][front->col]) == d  - 1) { //상 체크
            maze[front->row-1][front->col] = '*';
            addq(front->row-1, front->col);
            deleteq(&(front->row), &(front->col));
            d = d - 1;
            continue;
        }
        else if ((mark[front->row+1][front->col]) == d - 1) {// 하 체크
            maze[front->row+1][front->col] = '*';
            addq(front->row+1, front->col);
            deleteq(&(front->row), &(front->col));
            d = d - 1;
            continue;
        }
        else
            fprintf(stderr, "error"); //에러 출력
    }
    deleteq(&(front->row), &(front->col)); //sR,sC 메모리 DELETE

    return distance;
}