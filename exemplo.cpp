#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <vector>

#define POSITIONX (screen_width - WINDOW_WIDTH) / 2
#define POSITIONY (screen_height - WINDOW_WIDTH) / 2

GLint WINDOW_WIDTH  = 500,
      WINDOW_HEIGHT = 500;

typedef struct Point{
  GLint x;
  GLint y;
  int set = 0;
}Point;

typedef struct Figure{
  std::vector<Point> points;
}Figure;

const static char* TITLE = "Manipulando com OpenGL";
struct Point firstLPoint;
GLint drawMode = 0;
GLint openedMenu = 0;

GLint screen_height, screen_width;
std::vector<Point> savedPoints;
std::vector<Figure> figures; 

void init();      
void draw_test();

void mouse_test(GLint button, GLint action, GLint x, GLint y);
void mouse_test2(GLint x, GLint y);
void mouse_test3(GLint x, GLint y);

void keybord_test(GLubyte key, GLint x, GLint y);
void keybord_test2(GLint key, GLint x, GLint y);

void test_create_menu();
void menu_test(GLint item_number);


void savePoint(GLint x, GLint y) {
  firstLPoint.x = x;
  firstLPoint.y = y;
  firstLPoint.set = 1;
}

GLint mouseCoord2Bottom(GLint y) {
  return (WINDOW_HEIGHT-y);
}

void clearFirstLPoint() {
  firstLPoint.set = 0;
}

int isSetFirstLPoint() {
  return firstLPoint.set;
}



void dda(GLint x, GLint y, GLint x0, GLint y0) { 
  int i, steps;
  int dx, dy;
  GLfloat xIncrement, yIncrement;
  GLfloat lastY, lastX;
  
  dx = x - x0;
  dy = y - y0;
  lastY = y0;
  lastX = x0;

  steps = fabs(dx);
  if(fabs(dx) < fabs(dy))
    steps = fabs(dy);
  xIncrement = float(dx) / float(steps);
  yIncrement = float(dy) / float(steps);
  
  for(i = 0; i < steps; i++) {
    lastY += yIncrement;
    lastX += xIncrement;
    glPointSize(5.0f);
    glBegin(GL_POINTS);
      glVertex2i(round(lastX), round(lastY));
    glEnd();
  }
//glFlush();
}


std::vector<Point> drawTriangle(Point *a, Point *b, Point *c) {
  std::vector<Point> createdPoints;
  dda(a->x, a->y, b->x, b->y);
  dda(b->x, b->y, c->x, c->y);
  dda(c->x, c->y, a->x, a->y);
  createdPoints.push_back(*a);
  createdPoints.push_back(*b);
  createdPoints.push_back(*c);
  return createdPoints;
}
 
std::vector<Point> drawSquare(Point *a, GLint length) {
  std::vector<Point> createdPoints;
  Point p0, p1, p2, p3; 
  dda(a->x, a->y, (a->x)+length, a->y);
  dda((a->x)+length, a->y, (a->x)+length, (a->y)+length);
  dda((a->x)+length, (a->y)+length, (a->x), (a->y)+length);
  dda((a->x), (a->y)+length, a->x, a->y);
  p0.x = a->x; 
  p0.y = a->y;
  std::cout << "\n valor em p0 "<<p0.x<<" "<<p0.y<<"\n";
  p1.x = (a->x)+length; 
  p1.y = a->y;
  p2.x = (a->x)+length; 
  p2.y = (a->y)+length;
  p3.x = a->x; 
  p3.y = (a->y)+length;
  createdPoints.push_back(p0);
  createdPoints.push_back(p1);
  createdPoints.push_back(p2);
  createdPoints.push_back(p3);
  return createdPoints;
}

std::vector<Point> drawCircle(Point *pc, GLint radius, GLint nLines) {
  std::vector<Point> createdPoints;
  int i;
  GLfloat k, theta;
  GLint x0, y0, x, y;
  k = 2*M_PI/GLfloat(nLines);
  x0 = pc->x + radius;
  y0 = pc->y;
  for(i = 1; i <= nLines; i++){
    theta = k*i;
    x = round(pc->x + radius*cos(theta));
    y = round(pc->y + radius*sin(theta));
    dda(x, y, x0, y0);
    Point p;
    p.x = x;
    p.y = y;
    createdPoints.push_back(p);
    x0 = x;
    y0 = y;
  }
  return createdPoints; 
}

std::vector<Point> drawEllipse(Point *pc, GLint radiusX, GLint radiusY, GLint nLines) {
  std::vector<Point> createdPoints; 
  int i;
  GLfloat k, theta;
  GLint x0, y0, x, y;
  k = 2*M_PI/GLfloat(nLines);
  x0 = pc->x + radiusX;
  y0 = pc->y;
  for(i = 1; i <= nLines; i++){
    theta = k*i;
    x = pc->x + radiusX*cos(theta);
    y = pc->y + radiusY*sin(theta);
    dda(x, y, x0, y0);
    Point p;
    p.x = x;
    p.y = y;
    createdPoints.push_back(p);
    x0 = x;
    y0 = y;
  }
  return createdPoints;
}

void drawLine(GLint x, GLint y) {
  
  if(!isSetFirstLPoint()){
    firstLPoint.x = x;
    firstLPoint.y = y;
    firstLPoint.set = 1;
    glColor3f(0.0f,0.0f,0.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
      glVertex2i(x, y);
    glEnd();
  }else{
     std::vector<Point> createdPoints;
    dda(x, y, firstLPoint.x, firstLPoint.y);
    Point p1; 
    p1.x = x;
    p1.y = y; 
    createdPoints.push_back(firstLPoint);
    createdPoints.push_back(p1);
    savedPoints = createdPoints;
    Figure figure;
    figure.points = savedPoints;
    figures.push_back(figure);
    clearFirstLPoint();
  }
}

void drawPolygon(std::vector<Point> &obj) {
  int i;
  for(i = 0; i < obj.size() - 1; i++){
    dda(obj[i+1].x, obj[i+1].y, obj[i].x, obj[i].y);
  }
  dda(obj[0].x, obj[0].y, obj[i].x, obj[i].y);
}

void translate(std::vector<Point> &obj, GLint x, GLint y) {
  for(int i = 0; i < obj.size(); i++){
    obj[i].x += x;
    obj[i].y += y;
  }
}

void scale(std::vector<Point> &obj, GLfloat factor) {
 for(int i = 0; i < obj.size(); i++){
    obj[i].x = round(obj[i].x*factor);
    obj[i].y = round(obj[i].y*factor);
  } 
  std::cout<<"fator é ="<<factor<<"\n";
}

void rotate(std::vector<Point> &obj, GLfloat angle, GLint dir) {
  GLfloat theta = dir*angle*2*M_PI/360;
  GLint auxX;
  for(int i = 0; i < obj.size(); i++){
    auxX = obj[i].x;
    obj[i].x = round(auxX*cos(theta) - obj[i].y*sin(theta));
    obj[i].y = round(auxX*sin(theta) + obj[i].y*cos(theta));
  }
}

void ellipse(GLint x, GLint y) {
  if(isSetFirstLPoint()){
    GLint aAxis, bAxis;
    aAxis = abs(firstLPoint.x - x);
    bAxis = abs(firstLPoint.y - y);
    savedPoints = drawEllipse(&firstLPoint, aAxis, bAxis, 40);  
    Figure figure;
    figure.points = savedPoints;
    figures.push_back(figure);
  }
  else{
    savePoint(x,y);
  }
}

void circle(GLint x, GLint y) {
  if(isSetFirstLPoint()){
    GLint radius = round(sqrt(pow((x - firstLPoint.x),2)+pow((y - firstLPoint.y),2)));
    savedPoints = drawCircle(&firstLPoint,radius,40);
    Figure figure;
    figure.points = savedPoints;
    figures.push_back(figure);
  }
  else{
    savePoint(x,y);
  }

}

void square(GLint x, GLint y) {
  if(isSetFirstLPoint()){
   // std::cout<<"\nnao entrou --------------\n";
    GLint length = round(sqrt(pow((x - firstLPoint.x),2) + pow((y - firstLPoint.y),2)));
    GLfloat m = GLfloat(x - firstLPoint.x)/GLfloat(y - firstLPoint.y);
    GLint chooseTan = (m > 0)?0:180;
    GLint dx = (x - firstLPoint.x)<0?180:0;
    GLfloat angle = (atan(m)*360/(2*M_PI))-90+chooseTan;
    angle = -angle;
    std::cout <<"\nangulo "<<angle<<"\n";
    //std::cout<<"Angulo eh dado por m "<<m<<" e angle "<<angle<<" ternario "<<((m > 0)?1:-1)<<"\n";
    savedPoints = drawSquare(&firstLPoint, length);
   // std::cout<<"\nDiferença entre pontos "<<savedPoints[0].x<<"e original "<<firstLPoint.x<<"\n";
    translate(savedPoints, -firstLPoint.x, -firstLPoint.y);
    rotate(savedPoints, angle, 1);
    if((x - firstLPoint.x)<0)
      rotate(savedPoints, 90, 1);
    translate(savedPoints, firstLPoint.x, firstLPoint.y);
    drawPolygon(savedPoints);
    Figure figure;
    figure.points = savedPoints;
    figures.push_back(figure);
  }
  else{
   // std::cout << "===here==";
    savePoint(x, y);
  }
}


void triangle(GLint x, GLint y) {
  if(isSetFirstLPoint()){
    Point a, b, c;
    GLfloat m, theta, l; 
    a.x = x;
    a.y = y;
    b.x = firstLPoint.x;
    b.y = firstLPoint.y;
    m = float(a.y - b.y)/ float(a.x - b.x);
    theta = (atan(m)*360/(2.0*M_PI)+(180-60))*2*M_PI/360;
    l = sqrt(pow(a.x - b.x, 2)+pow(a.y - b.y, 2));
    Point bigger = a;
    if(a.x < b.x)
      bigger = b;

    c.x = round(bigger.x + l*cos(theta));
    c.y = round(bigger.y + l*sin(theta));
    savedPoints = drawTriangle(&a, &b, &c);
   // std::cout<<"O ponto esta setado" <<firstLPoint.set<<"\n";
    Figure figure;
    figure.points = savedPoints;
    figures.push_back(figure);
    clearFirstLPoint();
  //  std::cout<<"O ponto esta setado" <<firstLPoint.set<<"\n";
  }else{
  //  std::cout<<"hereeeeeeeeeeeeeeeeeeeee";
    savePoint(x, y);
  }
  std::cout<<"Chamando triangulo";
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);                            // Inicializa a biblioteca GLUT e negocia uma seção com o sistema de janelas 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);      // Define qual será o modo inicial de display
  
  screen_width  = glutGet(GLUT_SCREEN_WIDTH),
  screen_height = glutGet(GLUT_SCREEN_HEIGHT);  
  
  glutInitWindowPosition(POSITIONX, POSITIONY);  //Utilizada para definir a posição inicial da janela, sendo que os parâmetros representam a posição do canto superior esquerdo
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_WIDTH);                                                 // Define a largura e altura da janela
  glutCreateWindow(TITLE);                             // Cria a janela, sendo que o parâmetro será o título dela
  
  init();                                                  
  glutDisplayFunc(draw_test);
  
  glutMouseFunc(mouse_test);
  glutPassiveMotionFunc(mouse_test2);
  glutMotionFunc(mouse_test3);
  
  glutKeyboardFunc(keybord_test);
  glutSpecialFunc(keybord_test2);
  

  test_create_menu();

  
  
  glutMainLoop();
  
  
  return EXIT_SUCCESS;
}

void init()
{
  glClearColor(1.0, 1.0, 1.0, 1.0);     // Define cor de fundo da tela que será usado em matrix mode 
  glMatrixMode(GL_PROJECTION);          
  
  gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

  clearFirstLPoint();
}

void draw_test()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.0f, 0.0f, 0.0f);
 // glBegin(GL_POINTS);
  for(int i = 0; i < figures.size(); i++)
  {
    drawPolygon(figures[i].points);
  }
 // glEnd();
/*
  glBegin(GL_LINES);
    glVertex2i(100,100);
    glVertex2i(200,200);
  glEnd();

  glFlush();
  */
/*  Point a,b,c, d;
  a.x = 100;
  a.y = 100;
  b.x = 200;
  b.y = 200;
  c.x = 200;
  c.y = 100;
  d.x = 100;
  d.y = 200;
  glColor3f(0.0f, 0.0f, 0.0f);
 // drawTriangle(&a,&b,&c);
  savedPoints = drawSquare(&a, 30);
  std::cout<<"pontos salvos"<<savedPoints[0].x;*/
//  drawCircle(&d, 100, 40); 
//  drawElipse(&c, 20,100,40); 
  glFlush();
}

void mouse_test(GLint button, GLint action, GLint x, GLint y)
{
  if(openedMenu){
    openedMenu = 0;
    return;
  }
  switch(button)
  {
    case GLUT_LEFT_BUTTON: 
    {
      std::cout<<"Esquerda";
      break;
    }
    case GLUT_MIDDLE_BUTTON:
    {
      std::cout<<"Meio";
      break;
    }      
    case GLUT_RIGHT_BUTTON:
    {
      std::cout<<"Direita";
      break;
    }      
    default: 
    break;
  }
  
  if(action == GLUT_DOWN){
    std::cout<<" preciona";
   // drawLine(x, mouseCoord2Bottom(y));
/*    translate(savedPoints, -savedPoints[0].x, -savedPoints[0].y);
    translate(savedPoints, x, mouseCoord2Bottom(y));
    drawPolygon(savedPoints);*/

/*      translate(savedPoints, x, mouseCoord2Bottom(y));
      std::cout<<"Pontos trocados"<<savedPoints[0].x<<"\n";
      drawPolygon(savedPoints); */
    clearFirstLPoint();
    switch(drawMode) {
      case 0:
        drawLine(x, mouseCoord2Bottom(y));
        break;
      case 1:
        triangle(x, mouseCoord2Bottom(y));
        break;
      case 2:
        circle(x, mouseCoord2Bottom(y));
        break;
      case 3:
        ellipse(x, mouseCoord2Bottom(y));
        break;
      case 4:
        square(x, mouseCoord2Bottom(y));
        break;
    }
  }
  else {//GLUT_UP
    std::cout<<" libera";
    switch(drawMode) {
      case 0:
        drawLine(x, mouseCoord2Bottom(y));
        break;
      case 1:
        triangle(x, mouseCoord2Bottom(y));
        break;
      case 2:
        circle(x, mouseCoord2Bottom(y));
        break;
      case 3:
        ellipse(x, mouseCoord2Bottom(y));
        break;
      case 4:
        square(x, mouseCoord2Bottom(y));
        break;
    }
  }
// x cresce da esquerda pra direita. O y cresce de cima para baixo  
  std::cout<<" em (x:"<<x<<", y:"<<y<<")";
    
  std::cout<<"\n"; 
  draw_test();
}

void mouse_test2(GLint x, GLint y)
{
  std::cout<<"Movendo mouse sem clicar para posicao (x:"<<x<<", y:"<<y<<")\n"; 
}

void mouse_test3(GLint x, GLint y)
{
  std::cout<<"Arrastando o mouse para posicao (x:"<<x<<", y:"<<y<<")\n";

}

void keybord_test(GLubyte key, GLint x, GLint y)
{
  GLint m = glutGetModifiers();
  
  if(m == GLUT_ACTIVE_SHIFT){
    std::cout<<"Shift ou Caps ";
    if(key == 43){ // + 
      GLint x0, y0;
      x0 = savedPoints[0].x;
      y0 = savedPoints[0].y;
      translate(savedPoints, -x0, -y0);
      scale(savedPoints, 2.0f);
      translate(savedPoints, x0, y0);
      drawPolygon(savedPoints);
      std::cout<<"Aumentando escala + \n";
  }
  }

  else if(m == GLUT_ACTIVE_CTRL)
    std::cout<<"Control ";
  else if(m == GLUT_ACTIVE_ALT)
    std::cout<<"Alt "; 
  
//VERIFICAR TABELA ASCII QUANDO O CTRL ESTIVER PRECIONADO COM ALGUMA LETRA  
  if(m == GLUT_ACTIVE_CTRL && (GLint) key == 4)
    exit(EXIT_SUCCESS);

  std::cout<<"Tecla: "<<(GLint) key<<" (x:"<<x<<", y:"<<y<<")\n"; 
  
  //ESC = 27
  if (key == 27)
    glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
  else if(key == 45){ // -
      GLint x0, y0;
      x0 = savedPoints[0].x;
      y0 = savedPoints[0].y;
      translate(savedPoints, -x0, -y0);
      scale(savedPoints, GLfloat(1.0/2.0));
      translate(savedPoints, x0, y0);
      drawPolygon(savedPoints);
      std::cout<<"Diminuindo escala - \n"<<savedPoints[1].x;
  }

}

void keybord_test2(GLint key, GLint x, GLint y)
{
//GLUT_KEY_F1 .. GLUT_KEY_F12
//GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT
//GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME etc.  
  
  std::cout<<"Tecla especial: "<<key<<" (x:"<<x<<", y:"<<y<<")\n"; 
  
  if(key == GLUT_KEY_F11)
    glutFullScreen();
  else if(key == GLUT_KEY_LEFT){
    GLint x0, y0;
      x0 = savedPoints[0].x;
      y0 = savedPoints[0].y;
      translate(savedPoints, -x0, -y0);
      rotate(savedPoints, 90.0, 1);
      translate(savedPoints, x0, y0);
      drawPolygon(savedPoints);
      std::cout<<"Rotacionando para esquerda \n"<<savedPoints[1].x;
  }
  else if(key == GLUT_KEY_RIGHT){
    GLint x0, y0;
      x0 = savedPoints[0].x;
      y0 = savedPoints[0].y;
      translate(savedPoints, -x0, -y0);
      rotate(savedPoints, 90.0, -1);
      translate(savedPoints, x0, y0);
      drawPolygon(savedPoints);
      std::cout<<"Rotacionando para esquerda \n"<<savedPoints[1].x;
  }

}


void clear() {
  std::vector<Figure> newFigures;  
  figures = newFigures;
}

void menu_test(GLint item_number)
{
  std::cout<<"Item "<<item_number<<"\n";
  glutPostRedisplay();
  openedMenu = 1;
  switch(item_number){
    case 0:
      drawMode = 0;
      clearFirstLPoint();
      break;
    case 1:
      drawMode = 1;
      break;
    case 2:
      drawMode = 2;
      break;
    case 3:
      drawMode = 3;
      break;
    case 4:
      drawMode = 4;
      break;
    case 7:
      clear();
      break;
  }

}

void test_create_menu()
{
  std::cout<<"Criando menu\n";
  GLint submenu_id = glutCreateMenu(menu_test);
  glutAddMenuEntry("Subitem 1", 4);
  glutAddMenuEntry("Subitem 2", 5);
  glutAddMenuEntry("Subitem 3", 6);  
  
  GLint menu_id = glutCreateMenu(menu_test);  
  glutAddMenuEntry("Line", 0);
  glutAddMenuEntry("Triangle", 1);
 // glutAddSubMenu("Square", submenu_id); 
  glutAddSubMenu("Square", 4);  
  glutAddMenuEntry("Circle", 2);
  glutAddMenuEntry("Ellipse", 3);
  glutAddMenuEntry("Clear", 7) ;

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}



