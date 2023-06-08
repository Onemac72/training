#ifndef UNICODE
#define UNICODE
#endif
#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

double randInRange(double min, double max)
{
    double random = (double)rand() / RAND_MAX;
    double range = max - min;
    return min + range * random;
}

double **randm(int rows, int cols)
{
    double **mT = (double **)malloc(sizeof(double *) * rows);
    int i, j;
    for (i = 0; i < rows; i++) {
        mT[i] = (double *)malloc(sizeof(double) * cols);
        for (j = 0; j < cols; j++)
            mT[i][j] = randInRange(0.0, 2.0);
    }
    return mT;
}

int **mulmr(int rows, int cols, double **mT, double n3, double n4)
{
    int **mA = (int **)malloc(sizeof(int *) * rows);
    double coefficient = 1.0 - n3 * 0.02 - n4 * 0.005 - 0.25;
    int i, j;
    for (i = 0; i < rows; i++) {
        mA[i] = (int *)malloc(sizeof(int) * cols);
        for (j = 0; j < cols; j++)
            mA[i][j] = (int)(mT[i][j] * coefficient);
    }
    return mA;
}

int **makeMSymmetric(int rows, int cols, int **mA)
{
    int **mS = (int **)malloc(sizeof(int *) * rows);
    int i, j;
    for (i = 0; i < rows; i++)
        mS[i] = (int *)calloc(cols, sizeof(int));
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (mA[i][j] == 1 || mA[j][i] == 1) mS[i][j] = mS[j][i] = 1;
        }
    }
    return mS;
}

void printDoubleM(int rows, int cols, double **m)
{
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%lf ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printIntM(int rows, int cols, int **m)
{
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void deleteDoubleM(int rows, double **m)
{
    int i;
    for (i = 0; i < rows; i++) free(m[i]);
    free(m);
}

void deleteIntM(int rows, int **m)
{
    int i;
    for (i = 0; i < rows; i++) free(m[i]);
    free(m);
}

// N1N2 - номер групи, N3N4 - порядковий номер у списку групи
#define N1 2
#define N2 2
#define N3 1
#define N4 3
// Для позначення осей координат (позиції елементів записані у векторах)
#define x 0
#define y 1
// n = 10 + N3
#define COLS 11
#define ROWS 11
// Значення, які часто використовуються в обчисленнях
#define PI 3.1415926536
#define SQRT_2 1.4142135624

const wchar_t *vNames[11] = {
        L"1", L"2", L"3",
        L"4", L"5", L"6",
        L"7", L"8", L"9",
        L"10", L"11",
};

const int vCoords[11][2] = {
        {110, 110},
        {290, 110},
        {470, 110},
        {650, 110},
        {110, 380},
        {650, 290},
        {650, 470},
        {110, 650},
        {290, 650},
        {470, 650},
        {650, 650},
};

const int mPos[2] = {815, 250};
const int distanceBetweenNums = 20;
const int printOffset[2] = {5, 8};

const int vRadius = 40;
const int lRadius = ((5 * vRadius) / 4);
const int aAngleOffset = 3;

// Визначають зміщення центру ребер для огинання вершин або вже намальованих ребер
const double eOffsetDividers[5] = {0, 4.5, 7, 6, 6.5};


// Допоміжні функції
int checkTypeOfGraph(int **m, int rows, int cols);
double pow2(int value);
double getDistance(const int v1Pos[2], const int v2Pos[2]);
double convertDegreeToRad(double degreeValue);

typedef struct {
    int edgeType;
    int start[2];
    int center[2];
    int end[2];
    double angle;
    int aEnd[2];
} metadata;

metadata setDrawData(int v1, int v2, int isDirected, int drawnLines[ROWS][COLS])
{
    const int oneHopLen = vCoords[1][x] - vCoords[0][x];
    const int maxOneHopLen = 3 * oneHopLen / 2;
    metadata drawMD;
    drawMD.edgeType = 1;
    drawMD.start[x] = vCoords[v1][x];
    drawMD.start[y] = vCoords[v1][y];
    drawMD.end[x] = vCoords[v2][x];
    drawMD.end[y] = vCoords[v2][y];
    drawMD.center[x] = (drawMD.start[x] + drawMD.end[x]) / 2;
    drawMD.center[y] = (drawMD.start[y] + drawMD.end[y]) / 2;
    int dx = drawMD.end[x] - drawMD.start[x];
    int dy = drawMD.end[y] - drawMD.start[y];
    double vDistance = getDistance(drawMD.start, drawMD.end);
    int centerOffset[2] = {
            abs((int)(dy / eOffsetDividers[(int)round(vDistance / oneHopLen)])),
            abs((int)(dx / eOffsetDividers[(int)round(vDistance / oneHopLen)]))
    };
    int isDrawn = 0;
    if (drawnLines[v1][v2] == 1 &&
        drawnLines[v2][v1] == 1) isDrawn = 1;
    if (!isDrawn) {
        drawnLines[v1][v2] = drawnLines[v2][v1] = 1;
    } else {
        if (!isDirected) {
            drawMD.edgeType = 0;
            return drawMD;
        } else {
            drawMD.center[x] += centerOffset[x];
            drawMD.center[y] += centerOffset[y];
        }
    }
    if (v1 == v2) {
        drawMD.edgeType = 2;
        int lOffset[2] = {0};
        int aDirection[2] = {0};
        if (drawMD.center[x] > vCoords[0][x] &&
            drawMD.center[y] == vCoords[0][y]) {
            --lOffset[x];
            --lOffset[y];
            --aDirection[y];
            drawMD.angle = convertDegreeToRad(-90 + aAngleOffset);
        } else if (drawMD.center[x] == vCoords[ROWS - 1][x] &&
                   drawMD.center[y] > vCoords[0][y]) {
            ++lOffset[x];
            --lOffset[y];
            ++aDirection[x];
            drawMD.angle = convertDegreeToRad(180 + aAngleOffset);
        } else if (drawMD.center[x] < vCoords[ROWS - 1][x] &&
                   drawMD.center[y] == vCoords[ROWS - 1][y]) {
            ++lOffset[x];
            ++lOffset[y];
            ++aDirection[y];
            drawMD.angle = convertDegreeToRad(90 + aAngleOffset);
        } else if (drawMD.center[x] == vCoords[0][x] &&
                   drawMD.center[y] < vCoords[ROWS - 1][y]) {
            --lOffset[x];
            ++lOffset[y];
            --aDirection[x];
            drawMD.angle = convertDegreeToRad(0 + aAngleOffset);
        }
        int lCenterOffset = (int)round(
                (vRadius * SQRT_2 / 2 +
                 sqrt(pow2(lRadius) - pow2(vRadius) / 2))
                / SQRT_2);
        drawMD.center[x] += lCenterOffset * lOffset[x];
        drawMD.center[y] += lCenterOffset * lOffset[y];
        drawMD.aEnd[x] = drawMD.end[x] + vRadius * aDirection[x];
        drawMD.aEnd[y] = drawMD.end[y] + vRadius * aDirection[y];
        return drawMD;
    } else if (dy == 0 || dx == 0) {
        if (abs(dx) > maxOneHopLen || abs(dy) > maxOneHopLen) {
            if (!isDrawn) {
                drawMD.center[x] += centerOffset[x];
                drawMD.center[y] += centerOffset[y];
            } else {
                drawMD.center[x] -= 2 * centerOffset[x];
                drawMD.center[y] -= 2 * centerOffset[y];
            }
        }
    } else {
        if (isDrawn) {
            int graphCenter[2] = {
                    ((vCoords[COLS - 1][x] + vCoords[0][x]) / 2),
                    ((vCoords[ROWS - 1][y] + vCoords[0][y]) / 2)
            };
            int alternativeECenter[2] = {
                    (drawMD.center[x] - 2 * centerOffset[x]),
                    (drawMD.center[y] - 2 * centerOffset[y])
            };
            if (getDistance(drawMD.center, graphCenter) >
                getDistance(alternativeECenter, graphCenter)) {
                drawMD.center[x] = alternativeECenter[x];
                drawMD.center[y] = alternativeECenter[y];
            }
        }
    }
    if (isDirected) {
        int newDX = drawMD.end[x] - drawMD.center[x];
        int newDY = drawMD.end[y] - drawMD.center[y];
        double hypotenuse = getDistance(drawMD.center, drawMD.end);
        if (newDX >= 0 && newDY >= 0) {
            drawMD.angle = acos(abs(newDX) / hypotenuse) * -1;
        } else if (newDX >= 0 && newDY < 0) {
            drawMD.angle = acos(abs(newDX) / hypotenuse);
        } else if (newDX < 0 && newDY >= 0) {
            drawMD.angle = (PI - acos(abs(newDX) / hypotenuse)) * -1;
        } else if (newDX < 0 && newDY < 0) {
            drawMD.angle = (PI - acos(abs(newDX) / hypotenuse));
        }
        drawMD.aEnd[x] = drawMD.end[x] - (int)round((double)vRadius * cos(drawMD.angle));
        drawMD.aEnd[y] = drawMD.end[y] + (int)round((double)vRadius * sin(drawMD.angle));
    }
    return drawMD;
}

int checkTypeOfGraph(int **m, int rows, int cols)
{
    int isDirected = 0;
    int i, j;
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            if (m[i][j] != m[j][i]) {
                isDirected = 1;
                break;
            }
        }
    }
    return isDirected;
}

double pow2(int value)
{
    return (double)(value * value);
}

double getDistance(const int v1Pos[2], const int v2Pos[2])
{
    int a = v2Pos[x] - v1Pos[x];
    int b = v2Pos[y] - v1Pos[y];
    return sqrt(pow2(a) + pow2(b));
}

double convertDegreeToRad(double degreeValue)
{
    return PI * degreeValue / 180.0;
}


void showIntM(int rows, int cols, int **m, const int mStartPos[2], HDC hdc)
{
    int currentOutputPos[2] = {mStartPos[x], mStartPos[y]};
    wchar_t *output;
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (m[i][j] == 1) output = L"1";
            else output = L"0";
            TextOut(hdc,
                    currentOutputPos[x],
                    currentOutputPos[y],
                    output,
                    1);
            currentOutputPos[x] += distanceBetweenNums;
        }
        currentOutputPos[x] = mStartPos[x];
        currentOutputPos[y] += distanceBetweenNums;
    }
}

void drawVertices(HBRUSH brush, HPEN pen, HDC hdc)
{
    int left, top, right, bottom;
    int printPos[2];
    int i;
    for (i = 0; i < ROWS; ++i) {
        left = (vCoords[i][x] - vRadius);
        top = (vCoords[i][y] - vRadius);
        right = (vCoords[i][x] + vRadius);
        bottom = (vCoords[i][y] + vRadius);
        if (i > 8) printPos[x] = vCoords[i][x] - (int)(1.5 * printOffset[x]);
            // 9-й та 10-й елементи складаються з двох цифр, тому зміщення має бути більшим
        else printPos[x] = vCoords[i][x] - printOffset[x];
        printPos[y] = vCoords[i][y] - printOffset[y];
        SelectObject(hdc, brush);
        Ellipse(hdc, left, top, right, bottom);
        SelectObject(hdc, pen);
        Ellipse(hdc, left, top, right, bottom);
        TextOut(hdc, printPos[x], printPos[y], vNames[i], 2);
    }
}

void drawArrow(HDC hdc, double angle, int aEnd[2]) {
    int leftLineEnd[2], rightLineEnd[2];
    angle = PI - angle;
    rightLineEnd[x] = aEnd[x] + (int)(30 * cos(angle + 0.3));
    rightLineEnd[y] = aEnd[y] + (int)(30 * sin(angle + 0.3));
    leftLineEnd[x] = aEnd[x] + (int)(30 * cos(angle - 0.3));
    leftLineEnd[y] = aEnd[y] + (int)(30 * sin(angle - 0.3));
    MoveToEx(hdc, leftLineEnd[x], leftLineEnd[y], NULL);
    LineTo(hdc, aEnd[x], aEnd[y]);
    LineTo(hdc, rightLineEnd[x], rightLineEnd[y]);
}

void drawLoop(HDC hdc, int center[2])
{
    Ellipse(hdc,
            (center[x] - lRadius),
            (center[y] - lRadius),
            (center[x] + lRadius),
            (center[y] + lRadius));
}

void drawEdge(HDC hdc, int start[2], int center[2], int end[2])
{
    MoveToEx(hdc, start[x], start[y], NULL);
    LineTo(hdc, center[x], center[y]);
    MoveToEx(hdc, center[x], center[y], NULL);
    LineTo(hdc, end[x], end[y]);
}

void drawEdges(HDC hdc, HPEN hpen, int **m) {
    int isDirected = checkTypeOfGraph(m, ROWS, COLS);
    int drawnLines[ROWS][COLS] = { 0 };
    SelectObject(hdc, hpen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (m[i][j] == 1) {
                metadata data = setDrawData(i, j, isDirected, drawnLines);
                switch (data.edgeType)
                {
                    case 0:
                        break;
                    case 1:
                        drawEdge(hdc, data.start, data.center, data.end);
                        break;
                    case 2:
                        drawLoop(hdc, data.center);
                        break;
                }
                if (isDirected) drawArrow(hdc, data.angle, data.aEnd);
            }
        }
    }
}


#define DRAW_OTHER_GRAPH 0

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void createButton(HWND);
void createTips(HWND);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASS wndClass;
    wndClass.lpszClassName = L"Лабораторна робота 2.3";
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hIcon = 0;
    wndClass.lpszMenuName = 0;
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    if (!RegisterClass(&wndClass)) return 0;
    HWND hWnd;
    MSG lpMsg;
    hWnd = CreateWindow(L"Лабораторна робота 2.3",
                        L"Лабораторна робота 2.3, виконав М.М.Кушнір",
                        WS_OVERLAPPEDWINDOW,
                        10, 10, 1100, 800,
                        (HWND)NULL,
                        (HMENU)NULL,
                        (HINSTANCE)hInstance,
                        (HINSTANCE)NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    int GetMessageRes;
    while ((GetMessageRes = GetMessage(&lpMsg, hWnd, 0, 0)) != 0) {
        if (GetMessageRes == -1) return lpMsg.wParam;
        else {
            TranslateMessage(&lpMsg);
            DispatchMessage(&lpMsg);
        }
    }
}

int **A;
int **S;
int **currentM;

HWND hButton;
HWND hLabel1;
HWND hLabel2;

LRESULT CALLBACK WndProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (message)
    {
        case WM_COMMAND:
            if (wParam == DRAW_OTHER_GRAPH) {
                if (currentM == S) currentM = A;       // Якщо натиснуто кнопку, відбувається
                else if (currentM == A) currentM = S;  // заміна поточної матриці графа
                RedrawWindow(hWnd, NULL, NULL,
                             RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
                DestroyWindow(hButton); // Видалення старих та створення
                DestroyWindow(hLabel1); // нових контролерів зі
                DestroyWindow(hLabel2); // зміненими відповідно до
                createButton(hWnd);           // поточного стану програми
                createTips(hWnd);             // текстовими назвами
                break;
            }
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            SetBkMode(hdc, TRANSPARENT);
            HPEN ePen = CreatePen(PS_SOLID, 1, RGB(0, 38, 0));
            HPEN vOutlinePen = CreatePen(PS_SOLID, 3, RGB(3, 104, 65));
            HBRUSH vFillBrush = CreateSolidBrush(RGB(37, 255, 127));
            drawEdges(hdc, ePen, currentM);
            drawVertices(vFillBrush, vOutlinePen, hdc);
            showIntM(ROWS, COLS, currentM, mPos, hdc);
            DeleteObject(ePen);
            DeleteObject(vOutlinePen);
            DeleteObject(vFillBrush);
            EndPaint(hWnd, &ps);
            break;
        case WM_CREATE:
            srand(1000 * N1 + 100 * N2 + 10 * N3 + N4);
            double **T = randm(ROWS, COLS);
            A = mulmr(ROWS, COLS, T, N3, N4);
            S = makeMSymmetric(ROWS, COLS, A);
            printf("Matrix T\n");               // Логування створених
            printDoubleM(ROWS, COLS, T); // матриць до консолі
            printf("Matrix A\n");               // для перевірки коректності
            printIntM(ROWS, COLS, A);    // роботи функцій
            printf("Matrix S\n");               // randm(), mulmr()
            printIntM(ROWS, COLS, S);    // та makeMSymmetric()
            currentM = S; // На початку роботи програми завжди буде виводитися ненапрямлений граф
            deleteDoubleM(ROWS, T);
            createButton(hWnd);
            createTips(hWnd);
            break;
        case WM_DESTROY:
            deleteIntM(ROWS, A);
            deleteIntM(ROWS, S);
            currentM = NULL;
            PostQuitMessage(0);
            break;
        default :
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

void createButton(HWND hWnd)
{
    wchar_t buttonForDGraph[] = L"Намалювати напрямлений граф";
    wchar_t buttonForUGraph[] = L"Намалювати ненапрямлений граф";
    wchar_t *buttonText;
    if (currentM == A) buttonText = buttonForUGraph;
    else buttonText = buttonForDGraph;
    hButton = CreateWindowW(L"Button", buttonText,
                            WS_VISIBLE | WS_CHILD | WS_BORDER |
                            BS_FLAT | BS_MULTILINE | BS_CENTER,
                            815, 550, 210, 50,
                            hWnd,
                            (HMENU)DRAW_OTHER_GRAPH,
                            NULL, NULL);
}

void createTips(HWND hWnd)
{
    wchar_t label1ForDGraph[] = L"Матриця A";
    wchar_t label1ForUGraph[] = L"Матриця S";
    wchar_t *label1Text;
    wchar_t label2ForDGraph[] = L"( напрямлений граф )";
    wchar_t label2ForUGraph[] = L"( ненапрямлений граф )";
    wchar_t *label2Text;
    if (currentM == A) {
        label1Text = label1ForDGraph;
        label2Text = label2ForDGraph;
    } else {
        label1Text = label1ForUGraph;
        label2Text = label2ForUGraph;
    }
    hLabel1 = CreateWindowW(L"Static", label1Text,
                            WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                            820, 175, 200, 20,
                            hWnd,
                            NULL, NULL, NULL);
    hLabel2 = CreateWindowW(L"Static", label2Text,
                            WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                            820, 200, 200, 20,
                            hWnd,
                            NULL, NULL, NULL);
}
