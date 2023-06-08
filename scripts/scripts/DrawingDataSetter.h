#include <math.h>
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
