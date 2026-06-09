#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWS 30
#define COLS 60
#define MAXOBJ 50

typedef enum {
    LINE = 1,
    RECTANGLE,
    TRIANGLE,
    CIRCLE
} ShapeType;

typedef struct {
    int id;
    ShapeType type;

    int x1, y1;
    int x2, y2;
    int x3, y3;

    int radius;
} Shape;

char canvas[ROWS][COLS];
Shape objects[MAXOBJ];
int objCount = 0;
int nextId = 1;

void clearCanvas() {
    for(int i=0;i<ROWS;i++)
        for(int j=0;j<COLS;j++)
            canvas[i][j] = '_';
}

void putPixel(int x,int y) {
    if(x>=0 && x<COLS && y>=0 && y<ROWS)
        canvas[y][x]='*';
}

void drawLineAlgo(int x1,int y1,int x2,int y2) {
    int dx = abs(x2-x1);
    int dy = abs(y2-y1);

    int sx = (x1<x2)?1:-1;
    int sy = (y1<y2)?1:-1;

    int err = dx-dy;

    while(1) {
        putPixel(x1,y1);

        if(x1==x2 && y1==y2)
            break;

        int e2 = 2*err;

        if(e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(Shape s) {
    drawLineAlgo(s.x1,s.y1,s.x2,s.y1);
    drawLineAlgo(s.x2,s.y1,s.x2,s.y2);
    drawLineAlgo(s.x2,s.y2,s.x1,s.y2);
    drawLineAlgo(s.x1,s.y2,s.x1,s.y1);
}

void drawTriangle(Shape s) {
    drawLineAlgo(s.x1,s.y1,s.x2,s.y2);
    drawLineAlgo(s.x2,s.y2,s.x3,s.y3);
    drawLineAlgo(s.x3,s.y3,s.x1,s.y1);
}

void drawCircle(Shape s) {
    int xc=s.x1;
    int yc=s.y1;

    for(int angle=0; angle<360; angle++) {
        double rad = angle * 3.14159 / 180.0;

        int x = xc + (int)(s.radius*cos(rad));
        int y = yc + (int)(s.radius*sin(rad));

        putPixel(x,y);
    }
}

void redraw() {
    clearCanvas();

    for(int i=0;i<objCount;i++) {

        switch(objects[i].type) {

            case LINE:
                drawLineAlgo(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2
                );
                break;

            case RECTANGLE:
                drawRectangle(objects[i]);
                break;

            case TRIANGLE:
                drawTriangle(objects[i]);
                break;

            case CIRCLE:
                drawCircle(objects[i]);
                break;
        }
    }
}

void display() {
    redraw();

    for(int i=0;i<ROWS;i++) {
        for(int j=0;j<COLS;j++)
            printf("%c ",canvas[i][j]);
        printf("\n");
    }
}

void addShape() {
    Shape s;

    printf("\n1.Line 2.Rectangle 3.Triangle 4.Circle\n");
    scanf("%d",(int*)&s.type);

    s.id = nextId++;

    if(s.type==LINE) {
        scanf("%d%d%d%d",&s.x1,&s.y1,&s.x2,&s.y2);
    }

    else if(s.type==RECTANGLE) {
        scanf("%d%d%d%d",&s.x1,&s.y1,&s.x2,&s.y2);
    }

    else if(s.type==TRIANGLE) {
        scanf("%d%d%d%d%d%d",
              &s.x1,&s.y1,
              &s.x2,&s.y2,
              &s.x3,&s.y3);
    }

    else if(s.type==CIRCLE) {
        scanf("%d%d%d",&s.x1,&s.y1,&s.radius);
    }

    objects[objCount++] = s;
}

void deleteShape() {
    int id;

    printf("Enter ID: ");
    scanf("%d",&id);

    for(int i=0;i<objCount;i++) {

        if(objects[i].id==id) {

            for(int j=i;j<objCount-1;j++)
                objects[j]=objects[j+1];

            objCount--;
            return;
        }
    }
}

void modifyShape() {
    int id;

    printf("Enter ID: ");
    scanf("%d",&id);

    for(int i=0;i<objCount;i++) {

        if(objects[i].id==id) {

            ShapeType t=objects[i].type;

            if(t==LINE || t==RECTANGLE)
                scanf("%d%d%d%d",
                     &objects[i].x1,
                     &objects[i].y1,
                     &objects[i].x2,
                     &objects[i].y2);

            else if(t==TRIANGLE)
                scanf("%d%d%d%d%d%d",
                     &objects[i].x1,
                     &objects[i].y1,
                     &objects[i].x2,
                     &objects[i].y2,
                     &objects[i].x3,
                     &objects[i].y3);

            else if(t==CIRCLE)
                scanf("%d%d%d",
                     &objects[i].x1,
                     &objects[i].y1,
                     &objects[i].radius);

            return;
        }
    }
}

int main() {

    int choice;

    clearCanvas();

    do {

        printf("\n===== GRAPHICS EDITOR =====\n");
        printf("1.Add Object\n");
        printf("2.Delete Object\n");
        printf("3.Modify Object\n");
        printf("4.Display Canvas\n");
        printf("5.Exit\n");

        scanf("%d",&choice);

        switch(choice) {

            case 1:
                addShape();
                break;

            case 2:
                deleteShape();
                break;

            case 3:
                modifyShape();
                break;

            case 4:
                display();
                break;
        }

    } while(choice!=5);

    return 0;
}
