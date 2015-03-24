#define M_PI 3.14159265358979323846
#define M_G 9.81
#define EARTH_RADIUS_M 6371000.0
#define EQUATOR_LENGTH_KM 40075.0
#define EARTH_TURN_TIME_S 31557600.0
#define LAND_UP 1000
#define MAX_STR_LEN 256

int currentCalculationTime;
int calculationTime;
int outTime;
bool isProcessing = false;
int mapSizeX, mapSizeY;
double startX_dgr, endX_dgr;
double startY_dgr, endY_dgr;
double isobath;
double maxHeight;
double **terrian;
double **waveFrontCurrent;
double **waveFrontOld;
double **heightsFront;
double **uCurrent;
double **uOld;
double **vCurrent;
double **vOld;
int hearthBricksQ;
int watchingPointsQ;
double **terr_points;
double **point_points;

double **h;
double *delta_t;
double delta_x, delta_y;
double delta_x_m, *delta_y_m;
double **t_h_v_up;
int **terr_up;
double **terr_tmp;
double **point_tmp;