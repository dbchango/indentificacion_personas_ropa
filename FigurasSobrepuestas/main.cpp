#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <alloc.h>
#include <math.h>

typedef struct {
    int nv, *v;
    float z;
} ELEM;

float matt[4][4], matr[4][4], matp[4][4];

void mmat (float a[4][4], float b[4][4])
{
    int i, j, k;
    float c[4][4];
    for (i=0;i<4;i++)
        for (j=0;j<4;j++) {
            c[i][j]=0.;
            for (k=0;k<4;k++)
                c[i][j] +=a[i][k]*b[k][j];
        }
    for (i=0;i<4;i++)
        for (j=0;j<4;j++)
            b[i][j]=c[i][j];
}

void proyecta (float p[4], float q[4])
{
    int i, j;
    for (i=0;i<4;i++) {
        q[i]=0.;
        for (j=0;j<4;j++)
            q[i] +=p[j] * matp[j][i];
    }
    for (i=0;i<3;i++)
        q[i] /= q[3];
}

void rota (char eje, float a)
{
    float ang=a * M_PI / 180.;
    if (eje=='X') {
        matr[0][0]=1; matr[0][1]=0; matr[0][2]=0; matr[0][3]=0;
        matr[1][0]=0; matr[1][1]=cos(ang); matr[1][2]=sin(ang); matr[1][3]=0;
        matr[2][0]=0; matr[2][1]=-sin(ang); matr[2][2]=cos(ang); matr[2][3]=0;
        matr[3][0]=0; matr[3][1]=0; matr[3][2]=0; matr[3][3]=1;
    }
    if (eje=='Y') {
        matr[0][0]=cos(ang); matr[0][1]=0; matr[0][2]=-sin(ang); matr[0][3]=0;
        matr[1][0]=0; matr[1][1]=1; matr[1][2]=0; matr[1][3]=0;
        matr[2][0]=sin(ang); matr[2][1]=0; matr[2][2]=cos(ang); matr[2][3]=0;
        matr[3][0]=0; matr[3][1]=0; matr[3][2]=0; matr[3][3]=1;
    }
    if (eje=='Z') {
        matr[0][0]=cos(ang); matr[0][1]=sin(ang); matr[0][2]=0; matr[0][3]=0;
        matr[1][0]=-sin(ang); matr[1][1]=cos(ang); matr[1][2]=0; matr[1][3]=0;
        matr[2][0]=0; matr[2][1]=0; matr[2][2]=1; matr[2][3]=0;
        matr[3][0]=0; matr[3][1]=0; matr[3][2]=0; matr[3][3]=1;
    }
}

int main(int argc, char *argv[])
{
    int gdriver=DETECT, gmode, errorcode;
    int i, j, k, ni, nj, nf, np, ne, nv, pol[128], *id;
    char *pv, linea[256];
    FILE *fi;
    ELEM *el, *ve;
    float *x, *y, *z, p[4], q[4];
    float xmin, xmax, ymin, ymax, zmin, zmax, xc, yc, zc;
    float e, xm, ym, angx, angy, angz, *xp, *yp, *zp;

    if (argc !=2) {
        fprintf(stderr, "uso: obj fich.geo\n");
        exit(1);
    }

    matp[0][0]=1; matp[0][1]=0; matp[0][2]=0; matp[0][3]=0;
    matp[1][0]=0; matp[1][1]=1; matp[1][2]=0; matp[1][3]=0;
    matp[2][0]=0; matp[2][1]=0; matp[2][2]=1; matp[2][3]=0;
    matp[3][0]=0; matp[3][1]=0; matp[3][2]=0; matp[3][3]=1;

    if ((fi=fopen(argv[1], "r"))==NULL) {
        fprintf(stderr, "error: no puedo abrir %s\n", argv[1]);
        exit(2);
    }

    fscanf(fi, "%d %d %d", &np, &ne, &nv);
    clrscr();

    printf("Ang X: "); scanf("%f", &angx);
    printf("Ang Y: "); scanf("%f", &angy);
    printf("Ang Z: "); scanf("%f", &angz);

    /* coordenadas originales */
    x=(float *) calloc (np, sizeof (float));
    y=(float *) calloc (np, sizeof (float));
    z=(float *) calloc (np, sizeof (float));

    /* coordenadas proyectadas */
    xp=(float *) calloc (np, sizeof (float));
    yp=(float *) calloc (np, sizeof (float));
    zp=(float *) calloc (np, sizeof (float));

    /* vector de elementos */
    ve=(ELEM *) calloc (ne, sizeof (ELEM));
    id=(int *) calloc(ne, sizeof (int));

    rota('X', angx); mmat(matr, matp);
    rota('Y', angy); mmat(matr, matp);
    rota('Z', angz); mmat(matr, matp);

    xmin = ymin = zmin = 999999.;
    xmax = ymax = zmax = -xmin;

    for (i=0;i<np;i++) {
        fscanf (fi, "%f%f%f\n", &x[i], &y[i], &z[i]);

        p[0]=x[i]; p[1]=y[i]; p[2]=z[i]; p[3]=1.;
        proyecta (p, q);
        xp[i]=q[0]; yp[i]=q[1]; zp[i]=q[2];

        xmin = min(xmin, xp[i]);
        xmax = max(xmax, xp[i]);
        ymin = min(ymin, yp[i]);

        ymax = max(ymax, yp[i]);
        zmin = min(zmin, zp[i]);
        zmax = max(zmax, zp[i]);
    }
    xc = 0.5 * (xmin+xmax);
    yc = 0.5 * (ymin+ymax);
    zc = 0.5 * (zmin+zmax);

    /* initialize graphics and local variables */
    initgraph(&gdriver, &gmode, "");

    /* read result of initialization */
    errorcode=graphresult();
    /* an error occurred */
    if (errorcode !=grOk) {
        printf("Graphics error: %s\n", grapherrormsg(errorcode));
        printf("Press any key to halt:");
        getch();
        exit(1);
    }

    setcolor(getmaxcolor());
    xm = 0.5 * (getmaxx() + 1);
    ym = 0.5 * (getmaxy() + 1);
    e = 1.5 * min(xm/(xmax-xmin), ym/(ymax-ymin));

    for (i=0;i<ne;i++) {
        fgets(linea, 256, fi);
        pv=strtok(linea, " ");
        sscanf(pv, "%d", &nv);
        ve[i].nv = nv;
        id[i]=i;
        ve[i].v = (int *) calloc (nv, sizeof (int));
        for (j=0;j<nv;j++) {
            pv=strtok(NULL, " ");
            sscanf(pv, "%d", &ni); ni--;
            ve[i].v[j] = ni;
        }
    }
    fclose(fi);

    for (i=0;i<ne;i++) {
        el = &ve[i];
        ni = el->v[0];
        el->z = zp[ni];
        for (j=1;j<el->nv;j++) {
            nj = el->v[j];
            line (xm + (xp[ni]-xc)*e, ym - (yp[ni]-yc)*e,
                  xm + (xp[nj]-xc)*e, ym - (yp[nj]-yc)*e);
            ni = nj;
            el->z += zp[ni];
        }
        el->z /= el->nv;
        nj = el->v[0];
        line (xm + (xp[ni]-xc)*e, ym - (yp[ni]-yc)*e,
              xm + (xp[nj]-xc)*e, ym - (yp[nj]-yc)*e);
    }

    getch();

    for (i=0;i<ne;i++)
        for (j=i;j<ne;j++)
            if (ve[id[i]].z > ve[id[j]].z) {
                k = id[i];
                id[i] = id[j];
                id[j] = k;
            }

    for (i=0;i<ne;i++) {
        el = &ve[id[i]];
        for (j=0, k=0;j<el->nv;j++) {
            ni = el->v[j];
            pol[k++] = xm + (xp[ni]-xc)*e;
            pol[k++] = ym - (yp[ni]-yc)*e;
        }
        setfillstyle (SOLID_FILL, getbkcolor());
        fillpoly(el->nv, pol);
    }
    getch(); /* clean up */
    closegraph();
    return 0;
}